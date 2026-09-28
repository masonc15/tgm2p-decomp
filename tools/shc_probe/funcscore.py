#!/usr/bin/env python3
"""Score the functions of one compiled C file against the ROM, one by one.

probe.py compares a case byte for byte from its start, so anything that moves
a function (code in front of it, a pool that grew) wrecks the score even when
the function itself compiles perfectly. This compares each function on its
own: halfword by halfword over the ROM function's code, with every pc-relative
load compared by the value it loads rather than its displacement (a relocated
pool word counts as equal), and bsr/bra to outside the function excused.

Functions are located by name: f_<hex> is the function at ROM 0x<hex>, the
case header's func is at its rom address, and --at name=0x... adds others.
Run on nuada (SHC lives there).

usage: funcscore.py <case.c> [--at name=0xaddr]... [--flags "..."] [--version v5.0r32] [--diff NAME]
"""
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))
import fn  # noqa: E402
from probe import HEADER, SHCC  # noqa: E402

AUTO = re.compile(r"^_f_([0-9a-f]+)$")


def pcrel(w: int, pc: int):
    """(target, size) of a pc-relative load, or None."""
    if w >> 12 == 0xD or w & 0xFF00 == 0xC700:
        return ((pc + 4) & ~3) + (w & 0xFF) * 4, 4
    if w >> 12 == 0x9:
        return pc + 4 + (w & 0xFF) * 2, 2
    return None


def branch(w: int, pc: int):
    op = w >> 12
    if op == 0x8 and (w >> 8) & 0xF in (0x9, 0xB, 0xD, 0xF):
        return pc + 4 + fn.sdisp(w & 0xFF, 8) * 2
    if op in (0xA, 0xB):
        return pc + 4 + fn.sdisp(w & 0xFFF, 12) * 2
    return None


def compile_case(src: Path, flags: str, version: str) -> Path:
    out = Path(tempfile.mkdtemp()) / "case.o"
    subprocess.run([str(SHCC), f"shc-{version}", str(out), str(src), *flags.split()], check=True)
    return out


def load(obj: Path):
    with obj.open("rb") as f:
        elf = ELFFile(f)
        idx = next(i for i, s in enumerate(elf.iter_sections()) if s.name == ".text")
        data = elf.get_section(idx).data()
        syms = elf.get_section_by_name(".symtab")
        funcs = sorted((s["st_value"], s.name) for s in syms.iter_symbols()
                       if s["st_shndx"] == idx and s["st_info"]["type"] == "STT_FUNC")
        relocs = set()
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] == idx:
                relocs.update(r["r_offset"] for r in sec.iter_relocations())
    return data, funcs, relocs


def score(data, relocs, ostart, rom):
    """(matched, total, rows) for the object function at ostart against ROM rom."""
    code_end = fn.extent(rom)[0]
    ohw = lambda a: data[a] << 8 | data[a + 1] if a + 1 < len(data) else -1  # noqa: E731
    rows, hit = [], 0
    for i in range(0, code_end - rom, 2):
        r, o = fn.hw(rom + i), ohw(ostart + i)
        ok = r == o
        rp, op = pcrel(r, rom + i), pcrel(o, ostart + i) if o >= 0 else None
        if rp and op and (r & 0xFF00) == (o & 0xFF00) and rp[1] == op[1]:
            rv = fn.ROM[rp[0]:rp[0] + rp[1]]
            ov = data[op[0]:op[0] + op[1]]
            ok = op[0] in relocs or rv == ov
        elif not ok and o >= 0 and r >> 12 == o >> 12 and r >> 12 in (0xA, 0xB):
            t = branch(r, rom + i)
            ok = not (rom <= t < code_end)  # a call or jump out of the function
        hit += ok
        rows.append((i, r, o, ok))
    return hit, len(rows), rows


def show(rows, rom, ostart):
    for i, r, o, ok in rows:
        ra = next(fn.MD.disasm(r.to_bytes(2, "big"), rom + i), None)
        oa = next(fn.MD.disasm(o.to_bytes(2, "big"), ostart + i), None) if o >= 0 else None
        rt = f"{ra.mnemonic} {ra.op_str}" if ra else f".word {r:04x}"
        ot = f"{oa.mnemonic} {oa.op_str}" if oa else (f".word {o:04x}" if o >= 0 else "")
        print(f"{i:4x} {r:04x} {rt:28s} {' ' if ok else '*'}| {o:04x} {ot}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("case", type=Path)
    ap.add_argument("--at", action="append", default=[], help="name=0xaddr for a function without an f_ name")
    ap.add_argument("--flags")
    ap.add_argument("--version", default="v5.0r32")
    ap.add_argument("--diff", help="print a listing for this function")
    args = ap.parse_args()
    hdr = HEADER.search(args.case.read_text())
    flags = args.flags or (hdr["flags"] if hdr and hdr["flags"] else "-macsave=1 -optimize=1 -speed")
    where = {}
    if hdr and hdr["func"]:
        where[hdr["func"]] = int(hdr["rom"], 16)
    for a in args.at:
        n, v = a.split("=")
        where[n] = int(v, 0)
    data, funcs, relocs = load(compile_case(args.case, flags, args.version))
    tot_hit = tot = 0
    for ostart, sym in funcs:
        name = sym[1:]
        m = AUTO.match(sym)
        rom = where.get(name, int(m[1], 16) if m else None)
        if rom is None:
            print(f"  {name:24s} (no ROM address)")
            continue
        hit, n, rows = score(data, relocs, ostart, rom)
        tot_hit += hit
        tot += n
        print(f"  {rom:06x} {name:24s} {hit}/{n} ({100.0 * hit / max(1, n):.1f}%)")
        if args.diff == name:
            show(rows, rom, ostart)
    print(f"total {tot_hit}/{tot} ({100.0 * tot_hit / max(1, tot):.1f}%)")


if __name__ == "__main__":
    main()
