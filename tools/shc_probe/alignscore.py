#!/usr/bin/env python3
"""Score one function by instruction alignment instead of position.

funcscore.py compares halfword by halfword, so one extra or missing
instruction early on makes everything after it count as wrong. This takes
funcscore's --diff listing and aligns the two instruction sequences with
difflib, which tells "a few instructions differ" apart from "everything
shifted". Branch targets and pool displacements are ignored; with -r,
register numbers are too, which separates a register-allocation miss from a
real structural difference.

Run on nuada (it calls funcscore.py, which needs SHC).

usage: alignscore.py <case.c> <func> [-r] [-v]
"""
import argparse
import difflib
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROM_LINE = re.compile(r"\s*[0-9a-f]+ [0-9a-f]{4} (.*?)\s*\*?$")
OUR_LINE = re.compile(r"\s*[0-9a-f]{4} (.*)$")
ADDR_OPS = ("mov.l 0x", "mov.w 0x", "bt", "bf", "bra", "bsr")


def norm(insn: str, regs: bool) -> str:
    insn = insn.strip()
    if insn.startswith(ADDR_OPS):
        insn = re.sub(r"0x[0-9a-f]+", "X", insn)
    if regs:
        insn = re.sub(r"\br\d+\b", "R", insn)
    return insn


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("case")
    ap.add_argument("func")
    ap.add_argument("-r", action="store_true", help="ignore register numbers")
    ap.add_argument("-v", action="store_true", help="print the differing runs")
    args = ap.parse_args()

    out = subprocess.run([sys.executable, str(HERE / "funcscore.py"), args.case, "--diff", args.func],
                         capture_output=True, text=True).stdout
    rom, ours = [], []
    for line in out.splitlines():
        if "|" not in line:
            continue
        a, b = line.split("|", 1)
        if m := ROM_LINE.match(a):
            rom.append(norm(m.group(1), args.r))
        if m := OUR_LINE.match(b):
            ours.append(norm(m.group(1), args.r))
    if not rom:
        sys.exit(f"no listing for {args.func}; check funcscore.py {args.case} --diff {args.func}")

    sm = difflib.SequenceMatcher(None, rom, ours, autojunk=False)
    print(f"{args.func} ratio={sm.ratio():.3f} rom={len(rom)} ours={len(ours)}")
    if args.v:
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            if op != "equal":
                print(op, i1, rom[i1:i2][:12], "=>", ours[j1:j2][:12])


if __name__ == "__main__":
    main()
