#!/usr/bin/env python3
"""Alignment-based triage of every unresolved function that has a draft.

For each draft (wip + context sources) compiled with the pinned compiler, each
f_<addr> function that lies in an unresolved ROM range is compared with the
ROM by instruction alignment (difflib), once with registers and once with
register numbers masked. Branch targets and pool displacements are masked;
pool words are skipped. The best draft per function is reported.
usage: triage.py OUT.json [--jobs N] [--kinds wip,context]"""
import argparse, difflib, json, re, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools")); sys.path.insert(0, str(ROOT / "tools/shc_probe"))
import flag_sweep as fs, funcscore, fn

REG = re.compile(r"\br(1[0-5]|[0-9])\b")

def tok(w, pc):
    """Normalised text of one instruction word."""
    if funcscore.pcrel(w, pc):
        kind = "mov.l" if w >> 12 == 0xD else ("mova" if w & 0xFF00 == 0xC700 else "mov.w")
        return f"{kind} POOL,r{(w >> 8) & 15}" if kind != "mova" else "mova POOL,r0"
    if funcscore.branch(w, pc) is not None:
        op = w >> 12
        if op == 8: return {0x9: "bt", 0xB: "bf", 0xD: "bt.s", 0xF: "bf.s"}[(w >> 8) & 15] + " L"
        return ("bra" if op == 0xA else "bsr") + " L"
    i = next(fn.MD.disasm(w.to_bytes(2, "big"), pc), None)
    return f"{i.mnemonic} {i.op_str}" if i else f".word {w:04x}"

def stream(get, start, end, is_pool):
    out = []
    a = start
    while a < end:
        if not is_pool(a):
            out.append(tok(get(a), a))
        a += 2
    return out

def pool_addrs(get, start, end, relocs=()):
    """Addresses of literal words inside [start,end): pc-relative load targets and relocations."""
    pool = set()
    for a in range(start, end, 2):
        if a in pool: continue
        p = funcscore.pcrel(get(a), a)
        if p and start <= p[0] < end:
            pool.update(range(p[0], p[0] + p[1], 2))
    for r in relocs:
        if start <= r < end: pool.update((r, r + 2))
    return pool

_STARTS = []
def next_start(rom):
    """The next function start after rom that some draft defines."""
    import bisect
    i = bisect.bisect_right(_STARTS, rom)
    return _STARTS[i] if i < len(_STARTS) else fn.TEXT_END

def compare(data, funcs, relocs, i, rom):
    ostart = funcs[i][0]
    oend = funcs[i + 1][0] if i + 1 < len(funcs) else len(data)
    rend = min(fn.extent(rom)[0], next_start(rom))
    og = lambda a: data[a] << 8 | data[a + 1]
    # our pools: iterate to a fixed point so words reached only via the pool are not decoded as code
    op = pool_addrs(og, ostart, oend, relocs)
    rp = fn.all_pool()
    # trailing alignment/padding after the last instruction of ours: drop 0xffff/0x0009 filler
    ours = stream(og, ostart, oend, lambda a: a in op)
    while ours and ours[-1] in (".word ffff", "nop "): ours.pop()
    theirs = stream(fn.hw, rom, rend, lambda a: a in rp)
    m = lambda x, y: difflib.SequenceMatcher(None, x, y, autojunk=False)
    full = m(theirs, ours)
    mask = lambda s: [REG.sub("R", t) for t in s]
    nr = m(mask(theirs), mask(ours))
    eq = sum(b.size for b in full.get_matching_blocks())
    eqr = sum(b.size for b in nr.get_matching_blocks())
    return {"rom_insns": len(theirs), "our_insns": len(ours), "eq": eq, "eq_noreg": eqr,
            "ratio": round(full.ratio(), 4), "ratio_noreg": round(nr.ratio(), 4)}

def build(args):
    kind, path = args
    flags = fs.header_flags(path).split()
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp) / "case.o"
        try:
            r = subprocess.run([str(fs.SHCC), "shc-v5.0r32", str(out), str(path), *flags], capture_output=True, text=True, timeout=180)
        except subprocess.TimeoutExpired:
            return None
        if r.returncode or not out.exists(): return None
        return (path, *funcscore.load(out))

def addr_of(path, sym, where):
    name = sym[1:]; m = funcscore.AUTO.match(sym)
    return name, where.get(name, int(m[1], 16) if m else None)

def work(obj):
    path, data, funcs, relocs = obj
    where = fs.function_map(path); rows = []
    for i, (ostart, sym) in enumerate(funcs):
        name, rom = addr_of(path, sym, where)
        if rom is None or not (0x400 <= rom < fn.TEXT_END) or fs.segment_kind(rom) != "unresolved": continue
        try:
            c = compare(data, funcs, relocs, i, rom)
        except Exception as e:
            continue
        hit, n, _ = funcscore.score(data, relocs, ostart, rom)
        rows.append({"draft": str(path.relative_to(ROOT)), "function": name, "rom": rom, "bytes": min(fn.extent(rom)[0], next_start(rom)) - rom,
                     "positional_residual": n - hit, **c})
    return rows

if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("out"); ap.add_argument("--jobs", type=int, default=9)
    ap.add_argument("--kinds", default="wip,context"); a = ap.parse_args()
    drafts = [(k, p) for k, p in fs.discover() if k in a.kinds.split(",")]
    rows = []
    with ThreadPoolExecutor(a.jobs) as ex:
        objs = [o for o in ex.map(build, drafts) if o]
    st = set()
    for path, data, funcs, relocs in objs:
        where = fs.function_map(path)
        for _, sym in funcs:
            _, rom = addr_of(path, sym, where)
            if rom: st.add(rom)
    _STARTS[:] = sorted(st)
    for o in objs: rows += work(o)
    best = {}
    for r in rows:
        k = r["rom"]
        key = (r["eq"] / max(r["rom_insns"], 1), r["ratio"])
        if k not in best or key > best[k][0]: best[k] = (key, r)
    out = [v[1] for _, v in sorted(best.items())]
    json.dump({"all": rows, "best": out}, open(a.out, "w"), indent=1)
    tot = sum(r["bytes"] for r in out)
    print(f"{len(out)} unresolved functions with drafts, {tot} code bytes")
    cls = lambda r: ("exact-insns" if r["eq"] == r["rom_insns"] == r["our_insns"] else
                     "regs-only" if r["eq_noreg"] == r["rom_insns"] == r["our_insns"] else
                     "near (>=95% shape)" if r["ratio_noreg"] >= 0.95 else
                     "mid (80-95%)" if r["ratio_noreg"] >= 0.80 else "far (<80%)")
    agg = {}
    for r in out:
        c = cls(r); agg.setdefault(c, [0, 0]); agg[c][0] += 1; agg[c][1] += r["bytes"]
    for c, (n, b) in sorted(agg.items(), key=lambda x: -x[1][1]): print(f"  {c:<22} {n:3d} functions {b:6d} bytes")
    print(f"{'rom':>7} {'func':<10} {'bytes':>5} {'insns':>5} {'ours':>5} {'eq':>5} {'eq-noreg':>8} {'ratio':>6} {'noreg':>6}  class / draft")
    for r in out:
        print(f"{r['rom']:#07x} {r['function']:<10} {r['bytes']:5d} {r['rom_insns']:5d} {r['our_insns']:5d} {r['eq']:5d} {r['eq_noreg']:8d} {r['ratio']:6.3f} {r['ratio_noreg']:6.3f}  {cls(r)} / {r['draft'].split('/')[-1]}")
