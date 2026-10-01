#!/usr/bin/env python3
"""Diff one probe case against the ROM with objdiff-cli (runs on nuada).

The case is compiled as usual (shcc.sh, so inline-asm cases go through asmsh)
into base.o. target.o is rendered from the ROM over the same stretch: every
halfword as raw data, a global function label at the ROM address of each of
the case's functions (f_<hex>, the header's func, or --at name=0x...), and
each literal-pool word that holds the address one of base.o's relocations
resolves to written as that same relocation (symbol plus in-place offset, the
way SHC and rof2elf carry it). objdiff then pairs functions by name and
compares relocations by symbol, as in the project report.

Symbol addresses come from the same conventions the build uses: f_<rom>
(at its RAM address inside the copied block), d_<addr>, g_<addr>, plus
symbols.txt and the names the linked C files define. A pool word in base.o
relocated against .text resolves through the function it points into.

Everything runs in a fresh temp directory; nothing under build/ is written.

usage: objdiff_case.py <case.c> [func] [--at name=0xaddr]... [--flags "..."]
                       [--version v5.0r32] [--json] [--raw-json FILE] [--keep DIR]
                       [-c key=value]...

With func, prints that function's objdiff listing (target left, base right,
`*` marks a differing row) and its match %; without, a table of every
function's objdiff match %. --json prints a machine-readable summary.

objdiff's percentage is not funcscore's: it aligns instructions (a moved
instruction is one delete plus one insert, not a run of mismatches), charges
100 per inserted/deleted row, 60 per different opcode, 5 per different
register or relocation symbol and 1 per different immediate (a pc-relative
displacement included), and counts the pool words that follow a function in
that function. So a register swap costs 5% of one instruction, and a pool
word that differs is charged to whichever function the pool sits in. It does
compare relocation symbols, which funcscore doesn't (any relocated pool word
passes there). The `=` notes on pc-relative loads are computed here, since
objdiff's own pool comments are often missing or wrong for SHC's shared pools.
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))
import fn  # noqa: E402
from probe import HEADER, SHCC  # noqa: E402

OBJDIFF = os.environ.get("OBJDIFF", "/drive2/tgm2p/bin/objdiff-cli")
AS = ["sh-elf-as", "--isa=sh2", "--big", "-no-pad-sections"]
RAM_ROM_END = 0x313fc
AUTO = re.compile(r"^_?(f|d|g)_([0-9a-f]+)$")


def ram(a: int) -> int:
    """Address a pool word holds for ROM address a (the copied block runs from RAM)."""
    return a + fn.RAM_BASE - fn.RAM_ROM if fn.RAM_ROM <= a < RAM_ROM_END else a


def known_symbols() -> dict:
    """name (no underscore) -> address, from symbols.txt and the linked C objects."""
    syms = {}
    for line in (ROOT / "symbols.txt").read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            name, addr = line.split()[:2]
            syms[name] = int(addr, 16)
    # Functions the linked C files define under real names (field_clear_flag, ...).
    obj = ROOT / "build" / "obj"
    for start, end, kind, arg in read_splits():
        o = obj / (Path(arg).stem + ".o") if kind == "c" else None
        if o is None or not o.exists():
            continue
        nm = subprocess.run(["sh-elf-nm", "--defined-only", str(o)], capture_output=True, text=True).stdout
        for l in nm.splitlines():
            p = l.split()
            if len(p) == 3 and p[1] in "Tt":
                syms.setdefault(p[2].removeprefix("_"), ram(start + int(p[0], 16)))
    return syms


def read_splits():
    segs = []
    for line in (ROOT / "splits.txt").read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            p = line.split()
            segs.append((int(p[0], 16), p[1], p[2] if len(p) > 2 else None))
    segs.sort()
    return [(a, segs[i + 1][0] if i + 1 < len(segs) else 0x100000, k, arg) for i, (a, k, arg) in enumerate(segs)]


def symbol_addr(name: str, syms: dict, local: dict):
    n = name.removeprefix("_")
    if n in local:
        return ram(local[n])
    if n in syms:
        return syms[n]
    m = AUTO.match(n)
    if m:
        a = int(m[2], 16)
        return ram(a) if m[1] == "f" else a
    return None


def load_base(path: Path):
    with path.open("rb") as f:
        e = ELFFile(f)
        idx = next(i for i, s in enumerate(e.iter_sections()) if s.name == ".text")
        data = e.get_section(idx).data()
        symtab = e.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        funcs = sorted((s["st_value"], s.name) for s in syms
                       if s["st_shndx"] == idx and s["st_info"]["type"] in ("STT_FUNC", "STT_NOTYPE")
                       and s["st_info"]["bind"] == "STB_GLOBAL")
        relocs = []  # (offset, symbol name or None for .text, in-place value)
        for sec in e.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] == idx:
                for r in sec.iter_relocations():
                    s = syms[r["r_info_sym"]]
                    o = r["r_offset"]
                    k = int.from_bytes(data[o:o + 4], "big") + (r["r_addend"] if r.is_RELA() else 0)
                    if s["st_shndx"] == idx:
                        # .text itself, or a function this file defines:
                        # resolve it through the object's own layout.
                        relocs.append((o, None, k + s["st_value"]))
                    else:
                        relocs.append((o, s.name, k))
    return data, funcs, relocs


def build_target(base: Path, out: Path, hdr, where: dict):
    data, funcs, relocs = load_base(base)
    named = {}  # object offset -> (name, rom)
    for off, sym in funcs:
        n = sym.removeprefix("_")
        m = AUTO.match(n)
        rom = where.get(n, int(m[2], 16) if m and m[1] == "f" else None)
        if rom is not None:
            named[off] = (sym, rom)
    if not named:
        sys.exit(f"{base}: no function with a ROM address (have {[s for _, s in funcs]})")
    # The ROM address of object offset 0, from the first mapped function.
    off0, (_, rom0) = min(named.items())
    r0 = rom0 - off0
    if r0 % 4:
        sys.exit(f"{base}: .text would start at ROM {r0:#x}, not 4-aligned; check the f_ names and --at")
    r1 = r0 + len(data)
    if hdr and hdr["len"]:
        r1 = max(r1, int(hdr["rom"], 16) + int(hdr["len"], 16))
    for off, (_, rom) in named.items():
        r1 = max(r1, fn.extent(rom)[1])
    r1 = min(r1, RAM_ROM_END)
    local = {s.removeprefix("_"): rom for _, (s, rom) in named.items()}
    syms = known_symbols()

    def rom_of_offset(o: int):
        """ROM address of base .text offset o, through the function containing it."""
        cands = [(fo, rom) for fo, (_, rom) in named.items() if fo <= o]
        if not cands:
            return None
        fo, rom = max(cands)
        return rom + (o - fo)

    # value a pool word holds in the ROM -> how target.o should spell it.
    # Several names can share an address (eeprom_shadow and g_6060034), so
    # the spelling base.o uses at the same offset wins over the first one.
    by_value, at_offset = {}, {}
    unresolved = []
    for o, name, k in relocs:
        if name is None:
            rom = rom_of_offset(k)
            if rom is None:
                continue
            v, spell = ram(rom), f".Lsec+{rom - r0}"
        else:
            a = symbol_addr(name, syms, local)
            if a is None:
                unresolved.append(name)
                continue
            v, spell = (a + k) & 0xFFFFFFFF, f"{name}+{k}" if k else name
        by_value.setdefault(v, spell)
        at_offset[o] = (v, spell)
    labels = {}
    for off, (sym, rom) in named.items():
        labels.setdefault(rom, []).append(sym)
    pool = fn.all_pool()
    lines = ["\t.text", "\t.align 2", ".Lsec:"]
    a = r0
    nreloc = 0
    while a < r1:
        for sym in labels.get(a, []):
            lines += [f"\t.global {sym}", f"\t.type {sym},@function", f"{sym}:"]
        if a % 4 == 0 and a + 4 <= r1 and a in pool and a + 2 in pool and not labels.get(a + 2):
            v = int.from_bytes(fn.ROM[a:a + 4], "big")
            here = at_offset.get(a - r0)
            spell = here[1] if here and here[0] == v else by_value.get(v)
            if spell:
                lines.append(f"\t.long {spell}\t/* {a:06x} = {v:#010x} */")
                nreloc += 1
                a += 4
                continue
        lines.append(f"\t.short 0x{fn.hw(a):04x}\t/* {a:06x} */")
        a += 2
    s = out.with_suffix(".s")
    s.write_text("\n".join(lines) + "\n")
    subprocess.run([*AS, "-o", str(out), str(s)], check=True)
    return r0, r1, named, nreloc, len(relocs), sorted(set(unresolved))


def run_objdiff(target: Path, base: Path, out: Path, config):
    cmd = [OBJDIFF, "--no-color", "-L", "error", "diff", "-1", str(target), "-2", str(base), "-o", str(out), "--format", "json"]
    for c in config:
        cmd += ["-c", c]
    subprocess.run(cmd, check=True)
    return json.loads(out.read_text())


def rows(sym):
    return sym.get("instructions", [])


class Pools:
    """An object's .text bytes and relocations, for annotating pc-relative
    loads. objdiff's SuperH view only shows a pool value when the pool lies
    inside the same symbol, computes mov.l targets as if every symbol were
    4-aligned (wrong for functions at 2 mod 4), and shows a relocated word as
    its in-place bytes; this resolves every load from the whole section."""

    def __init__(self, path: Path, origin: int = 0):
        self.origin = origin  # added to displayed addresses (the ROM address of offset 0)
        with path.open("rb") as f:
            e = ELFFile(f)
            idx = next(i for i, s in enumerate(e.iter_sections()) if s.name == ".text")
            self.data = e.get_section(idx).data()
            syms = list(e.get_section_by_name(".symtab").iter_symbols())
            self.relocs = {}
            for sec in e.iter_sections():
                if isinstance(sec, RelocationSection) and sec["sh_info"] == idx:
                    for r in sec.iter_relocations():
                        s, o = syms[r["r_info_sym"]], r["r_offset"]
                        k = int.from_bytes(self.data[o:o + 4], "big") + (r["r_addend"] if r.is_RELA() else 0)
                        name = ".text" if s["st_info"]["type"] == "STT_SECTION" else s.name.removeprefix("_")
                        self.relocs[o] = f"{name}+{k:#x}" if k else name

    def note(self, a: int) -> str:
        w = int.from_bytes(self.data[a:a + 2], "big")
        if w >> 12 == 0xD or w & 0xFF00 == 0xC700:
            t, n = ((a + 4) & ~3) + (w & 0xFF) * 4, 4
        elif w >> 12 == 0x9:
            t, n = a + 4 + (w & 0xFF) * 2, 2
        else:
            return f"[{self.relocs[a]}]" if a in self.relocs else ""
        if w & 0xFF00 == 0xC700:
            return f"={t:#x}"
        if t in self.relocs:
            return f"={self.relocs[t]}"
        return f"={int.from_bytes(self.data[t:t + n], 'big'):#0{2 + 2 * n}x}" if t + n <= len(self.data) else ""


COMMENT = re.compile(r"\s*/\*.*?\*/")


def listing(left, right, lp: Pools, rp: Pools) -> str:
    out = []
    for lr, rr in zip(rows(left), rows(right)):
        kind = lr.get("diff_kind", "DIFF_NONE")
        li, ri = lr.get("instruction"), rr.get("instruction")

        def fmt(ins, p):
            if not ins:
                return ""
            a = int(ins.get("address", 0))
            return f"{a + p.origin:5x} {COMMENT.sub('', ins['formatted'])} {p.note(a)}".rstrip()

        mark = " " if kind == "DIFF_NONE" else "*"
        tag = {"DIFF_REPLACE": "repl", "DIFF_DELETE": "del", "DIFF_INSERT": "ins", "DIFF_OP_MISMATCH": "op",
               "DIFF_ARG_MISMATCH": "arg"}.get(kind, "")
        out.append(f"{fmt(li, lp):48.48s} {mark}{tag:4s}| {fmt(ri, rp)}")
    return "\n".join(out)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("case", type=Path)
    ap.add_argument("func", nargs="?")
    ap.add_argument("--at", action="append", default=[], help="name=0xaddr for a function without an f_ name")
    ap.add_argument("--flags")
    ap.add_argument("--version", default="v5.0r32")
    ap.add_argument("--json", action="store_true", help="print a JSON summary instead of text")
    ap.add_argument("--raw-json", type=Path, help="also copy objdiff's full JSON diff here")
    ap.add_argument("--keep", type=Path, help="copy base.o, target.o and target.s into this directory")
    ap.add_argument("-c", "--config", action="append", default=[],
                    help="objdiff diff option, e.g. functionRelocDiffs=none")
    args = ap.parse_args()
    text = args.case.read_text()
    hdr = HEADER.search(text)
    flags = args.flags or (hdr["flags"] if hdr and hdr["flags"] else "-macsave=1 -optimize=1 -speed")
    where = {}
    if hdr and hdr["func"]:
        where[hdr["func"]] = int(hdr["rom"], 16)
    for a in args.at:
        n, v = a.split("=")
        where[n] = int(v, 0)

    work = Path(tempfile.mkdtemp(prefix="objdiff_case."))
    try:
        base, target = work / "base.o", work / "target.o"
        subprocess.run([str(SHCC), f"shc-{args.version}", str(base), str(args.case.resolve()), *flags.split()],
                       check=True)
        r0, r1, named, nreloc, nbase, unresolved = build_target(base, target, hdr, where)
        res = run_objdiff(target, base, work / "diff.json", args.config)
        tpools, bpools = Pools(target, r0), Pools(base, r0)
        if args.raw_json:
            shutil.copy(work / "diff.json", args.raw_json)
        if args.keep:
            args.keep.mkdir(parents=True, exist_ok=True)
            for f in ("base.o", "target.o", "target.s"):
                shutil.copy(work / f, args.keep / f)
    finally:
        shutil.rmtree(work, ignore_errors=True)

    left = res.get("left", {}).get("symbols", [])
    right = res.get("right", {}).get("symbols", [])
    roms = {sym.removeprefix("_"): rom for sym, rom in named.values()}
    table = []
    for i, s in enumerate(right):
        if s.get("kind") != "SYMBOL_FUNCTION" or s["name"].startswith("."):
            continue
        n = s["name"].removeprefix("_")
        t = s.get("target_symbol")
        table.append({"name": n, "rom": roms.get(n), "size": int(s.get("size", 0)),
                      "match": s.get("match_percent") if t is not None else None, "index": i,
                      "target": t})
    table.sort(key=lambda r: (r["rom"] is None, r["rom"] or 0))
    if args.json:
        out = {"case": str(args.case), "rom": [r0, r1], "pool_relocs": [nreloc, nbase],
               "unresolved": unresolved,
               "functions": [{k: r[k] for k in ("name", "rom", "size", "match")} for r in table]}
        if args.func:
            r = next((r for r in table if r["name"] == args.func), None)
            if r and r["target"] is not None:
                def one(ins, p):
                    if not ins:
                        return None
                    a = int(ins.get("address", 0))
                    return f"{COMMENT.sub('', ins['formatted'])} {p.note(a)}".rstrip()
                out["rows"] = [
                    {"kind": lr.get("diff_kind", "DIFF_NONE"),
                     "target": one(lr.get("instruction"), tpools),
                     "base": one(rr.get("instruction"), bpools)}
                    for lr, rr in zip(rows(left[r["target"]]), rows(right[r["index"]]))]
        print(json.dumps(out, indent=1))
        return
    print(f"ROM {r0:#x}-{r1:#x}; target relocs {nreloc} pool words (base.o has {nbase} relocs)"
          + (f"; unresolved symbols: {' '.join(unresolved)}" if unresolved else ""))
    if args.func:
        r = next((r for r in table if r["name"] == args.func), None)
        if r is None:
            sys.exit(f"{args.func} not in {[r['name'] for r in table]}")
        if r["target"] is None:
            sys.exit(f"{args.func}: no ROM address, so no target symbol (use --at)")
        print(f"target (ROM) {'':42s}| base (compiled; addresses are .text offset + {r0:#x})")
        print(listing(left[r["target"]], right[r["index"]], tpools, bpools))
        print(f"{r['name']}: {r['match']:.2f}%")
        return
    for r in table:
        m = f"{r['match']:.2f}%" if r["match"] is not None else "(no target)"
        rom = f"{r['rom']:06x}" if r["rom"] is not None else "------"
        print(f"  {rom} {r['name']:24s} {r['size']:#6x}  {m}")


if __name__ == "__main__":
    main()
