#!/usr/bin/env python3
"""Linear-sweep the ROM text for call targets to seed Ghidra's function list.

Collects: exception vectors, bsr targets, and `mov.l @(disp,pc),Rn` literals
that feed a `jsr @Rn` within the next few instructions. Writes one hex
address per line to build/seeds.txt.
"""
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
rom = (ROOT / "build" / "prog.bin").read_bytes()
TEXT_END = 0x31000
seeds = set()

for i in range(0, 0x200, 4):
    v = struct.unpack(">I", rom[i:i + 4])[0]
    if v != 0xFFFFFFFF and v < TEXT_END:
        seeds.add(v)

lit = {}  # reg -> literal value loaded, expires after 8 instructions
for pc in range(0x400, TEXT_END, 2):
    w = rom[pc] << 8 | rom[pc + 1]
    op = w >> 12
    if op == 0xB:  # bsr disp12
        disp = w & 0xFFF
        if disp & 0x800:
            disp -= 0x1000
        seeds.add(pc + 4 + disp * 2)
    elif op == 0xD:  # mov.l @(disp,pc),Rn
        n = (w >> 8) & 0xF
        addr = ((pc + 4) & ~3) + (w & 0xFF) * 4
        if addr + 4 <= len(rom):
            lit[n] = (struct.unpack(">I", rom[addr:addr + 4])[0], pc)
    elif op == 0x4 and (w & 0xFF) == 0x0B:  # jsr @Rn
        n = (w >> 8) & 0xF
        if n in lit and pc - lit[n][1] <= 16:
            t = lit[n][0]
            if t < TEXT_END and t >= 0x400 and t % 2 == 0:
                seeds.add(t)

out = ROOT / "build" / "seeds.txt"
out.write_text("".join(f"{a:08X}\n" for a in sorted(seeds)))
print(f"{len(seeds)} seeds -> {out}", file=sys.stderr)
