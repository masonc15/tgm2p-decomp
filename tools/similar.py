#!/usr/bin/env python3
"""Find the matched C functions whose code looks most like a ROM function.

Every function in the code is reduced to its instruction sequence, with
literal-pool words and padding left out, in three spellings:

    sim    mnemonic plus operand class (register, sp, immediate, pool load,
           branch target, @rN, @(d,rN), @(r0,rN), ...), registers anonymous
    mnem   mnemonic only
    exact  mnemonic plus operands as written, registers and immediates kept,
           pool and branch addresses still abstracted

and two functions score 1 - levenshtein(a, b) / max(len a, len b) on each
spelling (the ethteck/coddog idea). Candidates are the functions inside `c`
segments of splits.txt; results are ranked by sim, then exact. The distance
comes from rapidfuzz when it's installed, else from Myers' bit-parallel
algorithm on Python ints (SIMILAR_PURE=1 forces that; both give the same
numbers). Candidates are visited in order of a token-multiset bound on sim,
so most never need the full distance.

C function names come from the sources: an f_<hex> name is its address, and
a real name sits after the previous function's code and pool. Asm function
starts are fn.call_targets() (minus literal words that decode as bsr), plus
frame-opening prologues right after another function or at a segment start.
Sizes are code bytes, without the pool.

The index (instruction sequences, names, extents) is cached as JSON under
~/.cache/tgm2p-decomp/ (or $SIMILAR_CACHE), keyed on build/prog.bin,
splits.txt, the C sources the `c` segments name, fn.py and this script.

usage: similar.py 0x1050e [0x2f852 ...]   top matches for each function
       similar.py --all-asm              best C neighbour of every asm function
options: -n N (matches per target, default 5), --json, --min-insns N
         (--all-asm: skip shorter functions, default 6), --rebuild
"""
import argparse
import hashlib
import json
import os
import re
import sys
import time
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import build  # noqa: E402  (read_splits)
import fn  # noqa: E402

try:  # the same distance in C++, if it's installed; else Myers in Python below
    if os.environ.get("SIMILAR_PURE"):
        raise ImportError
    from rapidfuzz.distance import Levenshtein as _rf
except ImportError:
    _rf = None

TEXT_END = 0x313fc
PAD = (0x0009, 0x0000, 0xFFFF)
CACHE_DIR = Path(os.environ.get("SIMILAR_CACHE", Path.home() / ".cache" / "tgm2p-decomp"))
FORMAT = 4  # bump when the index layout or tokenization changes


# ---------------------------------------------------------------- C sources

def c_defs(text: str):
    """[(name, line)] of the functions a C file emits, in source order:
    top-level definitions, minus #pragma inline/inline_asm helpers and static
    functions without an f_<hex> name (SHC inlines those)."""
    inline = set()
    for m in re.finditer(r"#pragma\s+inline(?:_asm)?\s*\(([^)]*)\)", text):
        inline |= {x.strip() for x in m[1].split(",")}

    def blank(m):  # keep newlines so line numbers survive
        return re.sub(r"[^\n]", " ", m[0])
    text = re.sub(r"/\*.*?\*/", blank, text, flags=re.S)
    text = re.sub(r"//[^\n]*", blank, text)
    text = re.sub(r'"(\\.|[^"\\\n])*"', blank, text)
    text = re.sub(r"'(\\.|[^'\\\n])*'", blank, text)
    text = re.sub(r"^[ \t]*#[^\n]*", blank, text, flags=re.M)
    out, depth, last = [], 0, 0
    for m in re.finditer(r"[{};]", text):
        ch = m[0]
        if depth:
            depth += {"{": 1, "}": -1}.get(ch, 0)
            if not depth:
                last = m.end()
            continue
        if ch != "{":
            last = m.end()
            continue
        decl = text[last:m.start()]
        d = re.search(r"(\w+)\s*\((?:[^()]|\([^()]*\))*\)\s*$", decl)
        if d and "=" not in decl:
            static = re.search(r"\bstatic\b", decl)
            if d[1] not in inline and not (static and not re.fullmatch(r"f_[0-9a-f]+", d[1])):
                out.append((d[1], text.count("\n", 0, last + d.start(1)) + 1))
        depth = 1
    return out


# ---------------------------------------------------------------- extents

def walk(start: int, stop: int):
    """fn.extent's walk, also ending at rte, never past stop. Returns
    (instruction addresses, code end, own pool addresses)."""
    pc, far, resume, after_jump = start, start, None, False
    pool, insns = set(), []
    allp = fn.all_pool()
    while pc < stop:
        if pc == resume:
            after_jump = True
        if pc in pool or (after_jump and (pc in allp or (fn.hw(pc) in PAD and pc + 2 in allp))):
            pc += 2
            continue
        after_jump = False
        w = fn.hw(pc)
        op = w >> 12
        insns.append(pc)
        end_here = False
        if op == 0xD:
            a = ((pc + 4) & ~3) + (w & 0xFF) * 4
            pool.update((a, a + 2))
        elif op == 0x9:
            pool.add(pc + 4 + (w & 0xFF) * 2)
        elif (w & 0xFF00) == 0xC700:
            pool.add(((pc + 4) & ~3) + (w & 0xFF) * 4)
        elif op == 0x8 and (w >> 8) & 0xF in (0x9, 0xB, 0xD, 0xF):
            far = max(far, pc + 4 + fn.sdisp(w & 0xFF, 8) * 2)
        elif op == 0xA:
            far = max(far, pc + 4 + fn.sdisp(w & 0xFFF, 12) * 2)
            end_here = True
        elif w in (0x000B, 0x002B) or (op == 0x4 and (w & 0xFF) == 0x2B):  # rts / rte / jmp
            end_here = True
        if end_here:
            if pc + 2 >= far:
                if pc + 2 < stop:
                    insns.append(pc + 2)  # delay slot
                pc += 4
                break
            resume = pc + 4
        pc += 2
    while True:  # unoptimized code can leave a second, unreachable epilogue
        tail = dead_epilogue(pc, stop)
        if not tail:
            break
        insns += tail
        pc = tail[-1] + 2
    return insns, min(pc, stop), pool


def dead_epilogue(pc: int, stop: int):
    """Addresses of an epilogue at pc that nothing branches to: stack pops,
    lds.l @r15+ and add #imm,r15, then rts and its slot."""
    out = []
    while pc + 2 < stop and len(out) < 12:
        w = fn.hw(pc)
        if w == 0x000B:
            return out + [pc, pc + 2] if out else []
        if not (w >> 8 == 0x7F or w in (0x4F26, 0x4F16, 0x4F06) or (w & 0xF0FF) == 0x60F6):
            return []
        out.append(pc)
        pc += 2
    return []


def after_code(start: int, stop: int) -> int:
    """Where the function after the one at start begins: past its code, its
    pool, other units' pool words and padding."""
    _, q, pool = walk(start, stop)
    allp = fn.all_pool()
    while q < stop and (q in pool or q in allp or (fn.hw(q) in PAD and (q % 4 or q + 2 in allp or q + 2 in pool))):
        q += 2
    return q


def prologue(lo: int, hi: int):
    """The first address in [lo, hi) that opens a stack frame (mov.l rN,@-r15,
    sts.l pr/macl,@-r15 or add #-n,r15) and isn't pool data, or None."""
    allp = fn.all_pool()
    for a in range(lo, hi, 2):
        w = fn.hw(a)
        if a in allp:
            continue
        if (w & 0xFF0F) == 0x2F06 or w in (0x4F22, 0x4F12) or (w >> 8 == 0x7F and w & 0x80):
            return a
        if w not in PAD:
            break  # code or data that isn't a frame opening: not a start
    return None


def asm_starts(segs):
    """fn.call_targets() inside asm segments, minus bsr targets that only
    literal words decoded as bsr produce."""
    allp = fn.all_pool()
    good, bad = set(), set()
    for pc in range(0x400, fn.TEXT_END, 2):
        w = fn.hw(pc)
        if w >> 12 == 0xB:
            (bad if pc in allp else good).add(pc + 4 + fn.sdisp(w & 0xFFF, 12) * 2)
    asm = [(a, b) for a, b, k, _ in segs if k == "asm"]
    out = []
    for t in fn.call_targets():
        if not any(a <= t < b for a, b in asm) or t in allp:
            continue
        if t in bad and t not in good and not fn.after_return(t):
            continue
        out.append(t)
    return out


def functions():
    """[{addr, end, kind, src, name, line}] for every function in the code
    segments, ordered by address. C functions come from their sources
    (f_<hex> names carry their address; real names are placed after the
    previous function's code and pool), asm ones from asm_starts."""
    segs = build.read_splits()
    funcs = {}
    for a, b, kind, arg in segs:
        if kind != "c":
            continue
        defs = c_defs((ROOT / arg).read_text())
        prev = None
        for name, line in defs:
            m = re.fullmatch(r"f_([0-9a-f]+)", name)
            if m and a <= int(m[1], 16) < b:
                addr = int(m[1], 16)
            elif prev is None:
                addr = a
            else:
                addr = after_code(prev, b)
            if addr >= b:
                break
            funcs[addr] = {"addr": addr, "kind": "c", "src": arg, "name": name, "line": line}
            prev = addr
    for t in asm_starts(segs):
        funcs.setdefault(t, {"addr": t, "kind": "asm", "src": None, "name": f"f_{t:x}", "line": None})
    # Functions nothing calls directly (reached through tables the scan
    # misses): a prologue at an asm segment's start or soon after the code and
    # pool of the function before.
    for a, b, kind, _ in segs:
        if kind != "asm" or a >= TEXT_END:
            continue
        b = min(b, TEXT_END)
        p = prologue(a, min(a + 0x40, b)) if a not in funcs else a
        if p is not None and p not in funcs:
            funcs[p] = {"addr": p, "kind": "asm", "src": None, "name": f"f_{p:x}", "line": None}
        here = sorted(f for f in funcs if a <= f < b)
        while here:
            f = here.pop(0)
            nxt = here[0] if here else b
            p = prologue(after_code(f, nxt), min(after_code(f, nxt) + 0x40, nxt))
            if p is not None and p not in funcs:
                funcs[p] = {"addr": p, "kind": "asm", "src": None, "name": f"f_{p:x}", "line": None}
                here.insert(0, p)
    order = sorted(funcs)
    seg_end = {}
    for a, b, _, _ in segs:
        for f in order:
            if a <= f < b:
                seg_end[f] = b
    out = []
    for i, f in enumerate(order):
        stop = min(order[i + 1] if i + 1 < len(order) else TEXT_END, seg_end.get(f, TEXT_END))
        insns, code_end, _ = walk(f, stop)
        d = funcs[f]
        d["end"] = code_end
        d["insns"] = insns
        out.append(d)
    return out


# ---------------------------------------------------------------- tokens

BRANCH = {"bt", "bf", "bt/s", "bf/s", "bra", "bsr"}
SPECIAL = {"pr", "macl", "mach", "sr", "gbr", "vbr"}
SPLIT_OPS = re.compile(r",(?![^()]*\))")


def op_class(mn: str, op: str) -> str:
    if re.fullmatch(r"r\d+", op):
        return "sp" if op == "r15" else "r"
    if op in SPECIAL:
        return op
    if op.startswith("#"):
        return "i"
    if op.startswith("0x"):
        return "L" if mn in BRANCH else "pool"
    m = re.fullmatch(r"@(-?)r(\d+)(\+?)", op)
    if m:
        return f"@{m[1]}{'sp' if m[2] == '15' else 'r'}{m[3]}"
    m = re.fullmatch(r"@\(([^,]+),([^)]+)\)", op)
    if m:
        base = "gbr" if m[2] == "gbr" else "sp" if m[2] == "r15" else "r"
        return f"@({'r0' if m[1] == 'r0' else 'd'},{base})"
    return op


_TOKENS = {}


def token(w: int):
    """(sim, mnem, exact) tokens of halfword w. Pool and branch operands are
    abstracted, so the tokens depend on the halfword alone."""
    t = _TOKENS.get(w)
    if t is None:
        pc = 0x1000  # any address; pc-relative operands are abstracted
        ins = next(fn.MD.disasm(w.to_bytes(2, "big"), pc), None)
        if ins is None or ins.mnemonic == ".byte":
            t = (".data",) * 3
        else:
            mn = ins.mnemonic
            ops = SPLIT_OPS.split(ins.op_str) if ins.op_str else []
            cls = [op_class(mn, o) for o in ops]
            ex = [c if c in ("L", "pool") else o for o, c in zip(ops, cls)]
            t = (f"{mn} {','.join(cls)}", mn, f"{mn} {','.join(ex)}")
        _TOKENS[w] = t
    return t


def tokens(addrs):
    """The (sim, mnem, exact) token strings of the instructions at addrs."""
    ts = [token(fn.hw(pc)) for pc in addrs]
    return [t[0] for t in ts], [t[1] for t in ts], [t[2] for t in ts]


# ---------------------------------------------------------------- index

def cache_key() -> str:
    h = hashlib.sha1(str(FORMAT).encode())
    for p in [ROOT / "build" / "prog.bin", ROOT / "splits.txt", Path(__file__), ROOT / "tools" / "fn.py"]:
        h.update(p.read_bytes())
    for _, _, kind, arg in build.read_splits():
        if kind == "c":
            h.update(arg.encode())
            h.update((ROOT / arg).read_bytes())
    return h.hexdigest()[:16]


def build_index():
    vocab = {}

    def enc(seq):
        return [vocab.setdefault(t, len(vocab)) for t in seq]

    out = []
    for f in functions():
        sim, mnem, exact = tokens(f.pop("insns"))
        f.update(n=len(sim), sim=enc(sim), mnem=enc(mnem), exact=enc(exact))
        out.append(f)
    return {"format": FORMAT, "functions": out, "vocab": list(vocab)}


def load_index(rebuild=False):
    key = cache_key()
    path = CACHE_DIR / f"similar-{key}.json"
    if not rebuild and path.exists():
        try:
            return json.loads(path.read_text()), path, False
        except ValueError:
            pass
    idx = build_index()
    CACHE_DIR.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(f".{os.getpid()}.tmp")
    tmp.write_text(json.dumps(idx, separators=(",", ":")))
    os.replace(tmp, path)
    for old in CACHE_DIR.glob("similar-*.json"):
        if old != path:
            old.unlink(missing_ok=True)
    return idx, path, True


# ---------------------------------------------------------------- distance

def peq_of(seq):
    peq = {}
    for i, c in enumerate(seq):
        peq[c] = peq.get(c, 0) | (1 << i)
    return peq


def levenshtein(a, b, peq_a=None) -> int:
    """Myers/Hyyrö bit-parallel edit distance; a is the bit-vector side."""
    m = len(a)
    if not m:
        return len(b)
    if not b:
        return m
    peq = peq_a if peq_a is not None else peq_of(a)
    full = (1 << m) - 1
    hb = 1 << (m - 1)
    pv, mv, score = full, 0, m
    for c in b:
        eq = peq.get(c, 0)
        xv = eq | mv
        xh = (((eq & pv) + pv) ^ pv) | eq
        ph = mv | (~(xh | pv) & full)
        mh = pv & xh
        if ph & hb:
            score += 1
        elif mh & hb:
            score -= 1
        ph = ((ph << 1) | 1) & full
        mh = (mh << 1) & full
        pv = mh | (~(xv | ph) & full)
        mv = ph & xv
    return score


_PEQ, _BAG = {}, {}


def _peq(f, key):
    k = (f["addr"], f["kind"], key)
    if k not in _PEQ:
        _PEQ[k] = peq_of(f[key])
    return _PEQ[k]


def _bag(f):
    k = (f["addr"], f["kind"])
    if k not in _BAG:
        _BAG[k] = Counter(f["sim"])
    return _BAG[k]


def common(a: dict, b: dict) -> int:
    """Size of the intersection of two token multisets."""
    if len(a) > len(b):
        a, b = b, a
    return sum(min(v, b.get(t, 0)) for t, v in a.items())


def score(q, c, key) -> float:
    """Similarity of q and c on one spelling, iterating over the shorter."""
    a, b = q[key], c[key]
    if len(a) < len(b):
        q, c, a, b = c, q, b, a
    n = len(a)
    if n == 0:
        return 1.0
    return 1 - (_rf.distance(a, b) if _rf else levenshtein(a, b, _peq(q, key))) / n


def top_matches(q, cands, k):
    """The k candidates most similar to q, as (sim, exact, mnem, cand),
    ranked by sim, then exact, then mnem. The edit distance is at least
    max(len) minus the size of the two token multisets' intersection, so
    candidates are visited in order of that bound and the search stops once
    the bound falls below the k-th best sim."""
    nq, bq = q["n"], _bag(q)
    bounds = sorted(((common(bq, _bag(c)) / max(nq, c["n"], 1), i) for i, c in enumerate(cands)),
                    reverse=True)
    sims = []  # (sim, cand index), everything that may still make the top k
    kth = -1.0
    for bound, i in bounds:
        if bound < kth:
            break
        s = score(q, cands[i], "sim")
        if s >= kth:
            sims.append((s, i))
            if len(sims) >= k:
                kth = sorted((x for x, _ in sims), reverse=True)[k - 1]
    best = [(s, score(q, cands[i], "exact"), score(q, cands[i], "mnem"), cands[i]) for s, i in sims if s >= kth]
    best.sort(key=lambda x: (-x[0], -x[1], -x[2], x[3]["addr"] != q["addr"], x[3]["addr"]))
    return best[:k]


# ---------------------------------------------------------------- output

def target(idx, addr):
    """The index entry for addr, or one built on the spot for an address the
    index doesn't list as a function start."""
    addr = fn.rom_addr(addr)
    for f in idx["functions"]:
        if f["addr"] == addr:
            return f
    vocab = {t: i for i, t in enumerate(idx["vocab"])}
    later = [f["addr"] for f in idx["functions"] if f["addr"] > addr]
    insns, end, _ = walk(addr, min(later) if later else TEXT_END)
    sim, mnem, exact = tokens(insns)

    def enc(seq):
        return [vocab.setdefault(t, len(vocab)) for t in seq]
    return {"addr": addr, "kind": "?", "src": None, "name": f"f_{addr:x}", "line": None, "end": end,
            "n": len(sim), "sim": enc(sim), "mnem": enc(mnem), "exact": enc(exact)}


def where(f):
    if f["kind"] == "c":
        return f"{f['src']}:{f['line']} {f['name']}"
    return f["name"]


def row(s, e, mn, c):
    return {"addr": f"0x{c['addr']:x}", "size": c["end"] - c["addr"], "insns": c["n"], "sim": round(s, 3),
            "exact": round(e, 3), "mnem": round(mn, 3), "src": c["src"], "name": c["name"], "line": c["line"]}


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("addrs", nargs="*", help="ROM (or RAM) function addresses")
    ap.add_argument("-n", type=int, default=5, help="matches per target (default 5)")
    ap.add_argument("--all-asm", action="store_true", help="best matched neighbour of every asm function")
    ap.add_argument("--min-insns", type=int, default=6, help="--all-asm: skip functions shorter than this")
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--rebuild", action="store_true", help="rebuild the cached index")
    args = ap.parse_args()
    if not args.addrs and not args.all_asm:
        ap.error("give function addresses or --all-asm")
    t0 = time.time()
    idx, path, built = load_index(args.rebuild)
    t1 = time.time()
    matched = [f for f in idx["functions"] if f["kind"] == "c" and f["n"]]
    note = f"index {path} ({'built' if built else 'cached'} in {t1 - t0:.2f}s): " \
           f"{len(idx['functions'])} functions, {len(matched)} in C"
    print(note, file=sys.stderr)
    results = []
    if args.all_asm:
        for f in idx["functions"]:
            if f["kind"] != "asm" or f["n"] < args.min_insns:
                continue
            best = top_matches(f, matched, 1)
            if best:
                results.append((f, best[0]))
        results.sort(key=lambda x: (-x[1][0], -x[1][1], x[0]["addr"]))
        if args.json:
            print(json.dumps([{"addr": f"0x{f['addr']:x}", "size": f["end"] - f["addr"], "insns": f["n"],
                               "best": row(*b)} for f, b in results], indent=1))
        else:
            print(f"{'asm func':>8} {'size':>5} {'ins':>4}   {'sim':>5} {'exact':>5} {'mnem':>5}  "
                  f"{'match':>7} {'size':>5}  where")
            for f, (s, e, mn, c) in results:
                print(f"{f['addr']:8x} {f['end'] - f['addr']:5x} {f['n']:4d}   {s:5.3f} {e:5.3f} {mn:5.3f}  "
                      f"{c['addr']:7x} {c['end'] - c['addr']:5x}  {where(c)}")
        print(f"{len(results)} asm functions in {time.time() - t1:.2f}s", file=sys.stderr)
        return
    out = []
    for a in args.addrs:
        q = target(idx, int(a, 16))
        best = top_matches(q, matched, args.n)
        out.append((q, best))
    if args.json:
        print(json.dumps([{"addr": f"0x{q['addr']:x}", "size": q["end"] - q["addr"], "insns": q["n"],
                           "kind": q["kind"], "matches": [row(*b) for b in best]} for q, best in out], indent=1))
    else:
        for q, best in out:
            kind = {"c": f"in C: {where(q)}", "asm": "asm", "?": "not a known function start"}[q["kind"]]
            print(f"0x{q['addr']:x}  size 0x{q['end'] - q['addr']:x}, {q['n']} insns, {kind}")
            print(f"  {'sim':>5} {'exact':>5} {'mnem':>5}  {'addr':>7} {'size':>5} {'ins':>4}  where")
            for s, e, mn, c in best:
                self_ = "  (self)" if c["addr"] == q["addr"] else ""
                print(f"  {s:5.3f} {e:5.3f} {mn:5.3f}  {c['addr']:7x} {c['end'] - c['addr']:5x} {c['n']:4d}  "
                      f"{where(c)}{self_}")
    print(f"queried in {time.time() - t1:.3f}s", file=sys.stderr)


if __name__ == "__main__":
    main()
