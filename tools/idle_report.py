#!/usr/bin/env python3
"""Per-unit frame-time margins for the non-matching C in a pinned replay.

With --pin-idle, tools/replay_check.sh records the real number of idle loops
the frame wait in f_2b4e8 runs in every frame (the time left before the next
vertical blank) for both images, and the frames in which code in each
REPLAY_MARKS range was fetched. This tool supplies those ranges for the
c-nonmatching units of a shiftable build and summarises, per unit, the
candidate's smallest idle count over the frames it ran in and the original's
over the same frames. A frame with no idle loops where the original had some
means the replacement overran the frame time and the game missed a frame.

usage: idle_report.py marks BUILD_DIR             REPLAY_MARKS value for that build
       idle_report.py report BUILD_DIR REPLAY_DIR  table; exit 1 if any frame was missed

BUILD_DIR is a shiftable build's output (layout.json), REPLAY_DIR the
replay_check.sh --out directory of a --pin-idle run with those marks.
"""
from __future__ import annotations

import json
import sys
from pathlib import Path


def nonmatching_ranges(build: Path):
    """[(name, lo, hi)] run-time code ranges of the c-nonmatching units: from
    the unit's first function (after any literal island, which the kept code
    around it reads) to the next unit."""
    units = json.loads((build / "layout.json").read_text())["units"]
    syms = function_symbols(build / "tgm2p.elf")
    out = []
    for i, u in enumerate(units):
        if u["kind"] != "c-nonmatching":
            continue
        nxt = next((v["vma"] for v in units[i + 1:] if v["vma"] is not None), None)
        entries = [a for rom, a in syms.items() if u["start"] <= rom < u["end"]]
        out.append((Path(u["arg"]).stem, min(entries) if entries else u["vma"], nxt))
    return out


def function_symbols(elf: Path) -> dict:
    """{rom offset: run-time address} of the f_<rom> symbols in a linked image."""
    if not elf.exists():
        return {}
    import re
    import subprocess
    r = subprocess.run(["sh-elf-nm", str(elf)], capture_output=True, text=True, check=True)
    out = {}
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) == 3 and p[1] in "Tt":
            m = re.fullmatch(r"_f_([0-9a-f]+)", p[2])
            if m:
                out[int(m[1], 16)] = int(p[0], 16)
    return out


def read_idle(path: Path):
    counts, marks = {}, {}
    for line in path.read_text().splitlines():
        p = line.split()
        if p[0] == "mark":
            marks.setdefault(int(p[2], 16), set()).add(int(p[1]))
        else:
            counts[int(p[0])] = int(p[1])
    return counts, marks


def summarise(ranges, runs):
    """Rows (name, frames run, candidate min, original min, missed) over the
    frames each unit ran in and the original waited in; runs is a list of
    (reference counts, candidate counts, candidate marks)."""
    rows = []
    for name, lo, _hi in ranges:
        n = missed = 0
        cmin = rmin = None
        for ref, cand, marks in runs:
            for f in marks.get(lo, ()):
                if ref.get(f, 0) <= 0 or f not in cand:
                    continue
                n += 1
                c = cand[f]
                cmin = c if cmin is None else min(cmin, c)
                rmin = ref[f] if rmin is None else min(rmin, ref[f])
                missed += c == 0
        rows.append((name, n, cmin, rmin, missed))
    return rows


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("marks", "report"):
        sys.exit(__doc__)
    build = Path(sys.argv[2])
    ranges = nonmatching_ranges(build)
    if sys.argv[1] == "marks":
        print(",".join(f"{lo:x}-{hi:x}" for _, lo, hi in ranges))
        return
    replay = Path(sys.argv[3])
    runs = []
    for mode in ("attract", "gameplay"):
        if (replay / f"cand-{mode}" / "idle.txt").exists():
            ref, _ = read_idle(replay / f"ref-{mode}" / "idle.txt")
            cand, marks = read_idle(replay / f"cand-{mode}" / "idle.txt")
            runs.append((ref, cand, marks))
    rows = summarise(ranges, runs)
    print(f"{'unit':24s} {'frames':>7s} {'min idle':>9s} {'original':>9s} {'missed':>7s}")
    for name, n, cmin, rmin, missed in rows:
        print(f"{name:24s} {n:7d} {'-' if cmin is None else cmin:>9} {'-' if rmin is None else rmin:>9} {missed:7d}")
    sys.exit(1 if any(r[4] for r in rows) else 0)


if __name__ == "__main__":
    main()
