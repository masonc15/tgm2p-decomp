#!/usr/bin/env python3
"""Side-by-side, alignment-based diff of one function: ROM vs a compiled draft.

Pool loads show the value loaded (ROM: the literal; draft: the literal or the
relocated symbol). Branch targets are shown as function-relative offsets in
the listing but ignored when aligning.
usage: fdiff.py DRAFT.c FUNC [--version v5.0r32] [--all] [--summary] [--flags "..."] [--obj OBJ.o] [--end 0xADDR]"""
import argparse, difflib, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools")); sys.path.insert(0, str(ROOT / "tools/shc_probe"))
import flag_sweep as fs, funcscore, fn
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

REG = re.compile(r"\br(1[0-5]|[0-9])\b")

def load(obj):
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        idx = next(i for i, s in enumerate(elf.iter_sections()) if s.name == ".text")
        data = elf.get_section(idx).data()
        symtab = elf.get_section_by_name(".symtab")
        funcs = sorted((s["st_value"], s.name) for s in symtab.iter_symbols()
                       if s["st_shndx"] == idx and s["st_info"]["type"] == "STT_FUNC")
        rel = {}
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] == idx:
                for r in sec.iter_relocations():
                    sym = symtab.get_symbol(r["r_info_sym"])
                    name = sym.name or elf.get_section(sym["st_shndx"]).name
                    add = r["r_addend"] if "r_addend" in r.entry else 0
                    rel[r["r_offset"]] = name + (f"+{add:#x}" if add else "")
    return data, funcs, rel

def _symbols():
    """symbols.txt: C name -> address, for names that do not carry their address."""
    out = {}
    for line in (ROOT / "symbols.txt").read_text().splitlines():
        parts = line.split("#")[0].split()
        if len(parts) == 2:
            out[parts[0]] = int(parts[1], 16)
    return out


SYMBOLS = _symbols()


def symval(name):
    """Best-effort numeric address a symbol name stands for (f_<rom>, g_<ram>, d_<rom>, symbols.txt)."""
    if name[1:] in SYMBOLS:
        return SYMBOLS[name[1:]]
    m = re.match(r"_?(f|g|d)_([0-9a-f]+)(?:\+(0x[0-9a-f]+))?$", name)
    if not m: return None
    v = int(m[2], 16) + (int(m[3], 16) if m[3] else 0)
    if m[1] == "f": v += 0x5fff880
    return v

def listing(get, start, end, pool, value_of):
    """[(addr, text, key)]: text for display, key for alignment."""
    out = []
    a = start
    while a < end:
        if a in pool: a += 2; continue
        w = get(a)
        p = funcscore.pcrel(w, a); b = funcscore.branch(w, a)
        if p:
            kind = "mov.l" if w >> 12 == 0xD else ("mova" if w & 0xFF00 == 0xC700 else "mov.w")
            r = (w >> 8) & 15 if kind != "mova" else 0
            v = value_of(p[0], p[1])
            out.append((a, f"{kind} #{v},r{r}", f"{kind} #{v},r{r}"))
        elif b is not None:
            op = w >> 12
            mn = {0x9: "bt", 0xB: "bf", 0xD: "bt.s", 0xF: "bf.s"}[(w >> 8) & 15] if op == 8 else ("bra" if op == 0xA else "bsr")
            inside = start <= b < end
            out.append((a, f"{mn} {'+%x' % (b - start) if inside else 'OUT'}", f"{mn} {'L' if inside else 'OUT'}"))
        else:
            i = next(fn.MD.disasm(w.to_bytes(2, "big"), a), None)
            t = f"{i.mnemonic} {i.op_str}".strip() if i else f".word {w:04x}"
            out.append((a, t, t))
        a += 2
    return out

def analyse(draft, func, version="v5.0r32", flags=None, obj=None, end=None):
    """Compile draft (unless obj is given) and align func with the ROM.

    Returns a dict: rom, rend, ostart, oend, theirs, ours ([(addr, text, key)]),
    eq, eq_noreg and matcher (difflib.SequenceMatcher over the keys).
    Raises RuntimeError with the compiler output when the compile fails."""
    path = Path(draft).resolve()
    with tempfile.TemporaryDirectory() as tmp:
        o = obj or str(Path(tmp) / "case.o")
        if not obj:
            fl = (flags or fs.header_flags(path)).split()
            r = subprocess.run([str(fs.SHCC), "shc-" + version, o, str(path), *fl], capture_output=True, text=True)
            if r.returncode or not Path(o).exists():
                raise RuntimeError((r.stdout + r.stderr).strip()[-400:])
        data, funcs, rel = load(o)
    where = fs.function_map(path)
    def rom_of(sym):
        m = funcscore.AUTO.match(sym); return where.get(sym[1:], int(m[1], 16) if m else None)
    i = next((k for k, (_, s) in enumerate(funcs) if s[1:] == func), None)
    if i is None:
        raise RuntimeError(f"{func} is not defined in {path.name}")
    ostart = funcs[i][0]; oend = funcs[i + 1][0] if i + 1 < len(funcs) else len(data)
    rom = rom_of(funcs[i][1])
    starts = sorted(x for x in (rom_of(s) for _, s in funcs) if x)
    nxt = next((x for x in starts if x > rom), fn.TEXT_END)
    rend = int(end, 16) if end else min(fn.extent(rom)[0], nxt)
    og = lambda x: data[x] << 8 | data[x + 1]
    opool = set()
    for x in range(ostart, oend, 2):
        if x in opool: continue
        p = funcscore.pcrel(og(x), x)
        if p and ostart <= p[0] < oend: opool.update(range(p[0], p[0] + p[1], 2))
    for r in rel:
        if ostart <= r < oend: opool.update((r, r + 2))
    def oval(addr, size):
        word = int.from_bytes(data[addr:addr + size], "big")
        if addr in rel:  # rof2elf leaves a symbol's offset in the word itself
            v = symval(rel[addr])
            return f"{(v + word) & 0xffffffff:#x}" if v is not None else rel[addr] + (f"+{word:#x}" if word else "")
        return f"{word:#x}"
    def rval(addr, size):
        return f"{int.from_bytes(fn.ROM[addr:addr + size], 'big'):#x}"
    ours = listing(og, ostart, oend, opool, oval)
    while ours and ours[-1][1] in (".word ffff", "nop"): ours.pop()
    theirs = listing(fn.hw, rom, rend, fn.all_pool(), rval)
    sm = difflib.SequenceMatcher(None, [t[2] for t in theirs], [t[2] for t in ours], autojunk=False)
    nr = difflib.SequenceMatcher(None, [REG.sub("R", t[2]) for t in theirs], [REG.sub("R", t[2]) for t in ours], autojunk=False)
    return {"rom": rom, "rend": rend, "ostart": ostart, "oend": oend, "theirs": theirs, "ours": ours, "matcher": sm,
            "eq": sum(b.size for b in sm.get_matching_blocks()), "eq_noreg": sum(b.size for b in nr.get_matching_blocks())}


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0]); ap.add_argument("draft"); ap.add_argument("func")
    ap.add_argument("--version", default="v5.0r32"); ap.add_argument("--all", action="store_true")
    ap.add_argument("--flags"); ap.add_argument("--obj"); ap.add_argument("--end", help="ROM end address override")
    ap.add_argument("--summary", action="store_true")
    a = ap.parse_args()
    try:
        r = analyse(a.draft, a.func, a.version, a.flags, a.obj, a.end)
    except RuntimeError as e:
        sys.exit(str(e))
    rom, rend, ostart, oend, theirs, ours, sm = (r[k] for k in ("rom", "rend", "ostart", "oend", "theirs", "ours", "matcher"))
    print(f"{a.func}: ROM {rom:#x}-{rend:#x} {len(theirs)} insns ({rend-rom} bytes) | ours {len(ours)} insns ({oend-ostart} bytes incl. pool) | equal {r['eq']} | equal ignoring regs {r['eq_noreg']}")
    if a.summary: return
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            if a.all:
                for k in range(i2 - i1):
                    print(f"  {theirs[i1+k][0]-rom:4x} {theirs[i1+k][1]:<30} | {ours[j1+k][0]-ostart:4x} {ours[j1+k][1]}")
            else:
                print(f"  ... {i2-i1} equal (rom +{theirs[i1][0]-rom:x}..)")
            continue
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            l = theirs[i1 + k] if i1 + k < i2 else None
            r_ = ours[j1 + k] if j1 + k < j2 else None
            ls = f"{l[0]-rom:4x} {l[1]:<30}" if l else " " * 35
            rs = f"{r_[0]-ostart:4x} {r_[1]}" if r_ else ""
            print(f"* {ls} | {rs}")


if __name__ == "__main__":
    main()
