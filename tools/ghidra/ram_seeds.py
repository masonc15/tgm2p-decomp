#!/usr/bin/env python3
"""Write Ghidra seed addresses for the RAM-mapped project (build_project.sh).

Takes every function start tools/fn.py knows about (call targets, function
pointers, and the starts of each pool group's members) as ROM offsets and
writes them at the address the game runs them from: ROM 0x780-0x313fc is
copied to RAM 0x06000000, so those become ROM + 0x5fff880. Boot code below
0x780 stays at its ROM address. One hex address per line, the format
SeedFunctions.java reads.

usage: ram_seeds.py [-o build/ghidra/seeds_ram.txt]
"""
import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import fn  # noqa: E402

RAM_ROM, RAM_ROM_END, RAM_DELTA = 0x780, 0x313FC, 0x5FFF880


def run_addr(rom: int) -> int:
    return rom + RAM_DELTA if RAM_ROM <= rom < RAM_ROM_END else rom


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("-o", "--out", type=Path, default=ROOT / "build" / "ghidra" / "seeds_ram.txt")
    args = ap.parse_args()
    starts = set(fn.call_targets())
    for _, _, _, members in fn.units():
        starts.update(members)
    addrs = sorted(run_addr(a) for a in starts if a % 2 == 0)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text("".join(f"{a:08X}\n" for a in addrs))
    print(f"{args.out}: {len(addrs)} seeds")


if __name__ == "__main__":
    main()
