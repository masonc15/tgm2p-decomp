#!/usr/bin/env python3
"""Per-unit frame-time margins for the non-matching C in a pinned replay.

With --pin-idle, tools/replay_check.sh records the real number of idle loops
the frame wait in f_2b4e8 runs in every frame (the time left before the next
vertical blank) for both images, and the frames in which code in each
REPLAY_MARKS range was fetched. This tool supplies those ranges for the
c-nonmatching units of a shiftable build and summarises, per unit, the
candidate's smallest idle count over the frames it ran in and the original's
over the same frames.

A missed frame is read from the game's own frame counter, g_6065644, which the
frame wait increments each time it returns and the replay records as every
MAME frame ends. Code that overruns the frame time makes the wait skip a
vertical blank, so the candidate's counter falls behind the original's in that
frame and stays behind. An idle count of zero is not that test: the wait also
counts loops while the interrupt acknowledge is pending, and in the frames
where the original starts several frames of blocking work it logs one such
loop, which lands on either side of the MAME frame boundary (a build of the
original's own instructions, only moved by padding, logs none in one of them).

usage: idle_report.py marks BUILD_DIR             REPLAY_MARKS value for that build
       idle_report.py report BUILD_DIR REPLAY_DIR  table; exit 1 if any frame was missed
       idle_report.py frames REF_IDLE CAND_IDLE    frame counter comparison; exit 1 on a miss

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


def read_game_frames(path: Path) -> dict:
    """{frame: the game's frame counter as that frame ended}."""
    out = {}
    for line in path.read_text().splitlines():
        p = line.split()
        if p[0] != "mark" and len(p) > 2:
            out[int(p[0])] = int(p[2])
    return out


def frame_slips(ref_idle, ref_game, cand_game):
    """(frames compared, frames in which the candidate fell behind the
    original's game frame counter, frames in which it got ahead, final lag).
    Compared from the first frame the original waits in: before it the boot
    code is still testing and clearing the RAM the counter lives in."""
    waits = [f for f, n in ref_idle.items() if n > 0]
    if not waits:
        return 0, [], [], 0
    start = min(waits)
    behind, ahead = [], []
    lag = n = 0
    for f in sorted(ref_game):
        if f < start or f not in cand_game:
            continue
        n += 1
        d = (ref_game[f] - cand_game[f] + 0x80000000) % 0x100000000 - 0x80000000
        if d > lag:
            behind.append(f)
        elif d < lag:
            ahead.append(f)
        lag = d
    return n, behind, ahead, lag


def summarise(ranges, runs):
    """Rows (name, frames run, candidate min, original min, missed) over the
    frames each unit ran in and the original waited in; runs is a list of
    (reference counts, candidate counts, candidate marks, frames in which the
    candidate's game frame counter fell behind the original's)."""
    rows = []
    for name, lo, _hi in ranges:
        n = missed = 0
        cmin = rmin = None
        for ref, cand, marks, behind in runs:
            ran = marks.get(lo, ())
            missed += len(set(behind) & set(ran))
            for f in ran:
                if ref.get(f, 0) <= 0 or f not in cand:
                    continue
                n += 1
                c = cand[f]
                cmin = c if cmin is None else min(cmin, c)
                rmin = ref[f] if rmin is None else min(rmin, ref[f])
        rows.append((name, n, cmin, rmin, missed))
    return rows


def frames_line(ref_path: Path, cand_path: Path):
    """(ok, one-line summary) of the frame counter comparison."""
    ref_idle, _ = read_idle(ref_path)
    ref_game, cand_game = read_game_frames(ref_path), read_game_frames(cand_path)
    if not ref_game or not cand_game:
        return False, "game frame counter: not recorded (rerun both images with this replay.lua)"
    n, behind, ahead, lag = frame_slips(ref_idle, ref_game, cand_game)
    text = f"game frame counter over {n} frames: "
    if not behind and not ahead and n:
        return True, text + "identical; frames missed: 0"
    parts = []
    if behind:
        parts.append(f"falls behind in {len(behind)} (first at frame {behind[0]})")
    if ahead:
        parts.append(f"gets ahead in {len(ahead)} (first at frame {ahead[0]})")
    if not n:
        parts.append("the original never waits")
    return False, text + ", ".join(parts) + f"; frames missed: {len(behind)}, final lag {lag}"


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("marks", "report", "frames"):
        sys.exit(__doc__)
    if sys.argv[1] == "frames":
        ok, line = frames_line(Path(sys.argv[2]), Path(sys.argv[3]))
        print(line)
        sys.exit(0 if ok else 1)
    build = Path(sys.argv[2])
    ranges = nonmatching_ranges(build)
    if sys.argv[1] == "marks":
        print(",".join(f"{lo:x}-{hi:x}" for _, lo, hi in ranges))
        return
    replay = Path(sys.argv[3])
    runs = []
    slipped = False
    for mode in ("attract", "gameplay"):
        if (replay / f"cand-{mode}" / "idle.txt").exists():
            rp, cp = replay / f"ref-{mode}" / "idle.txt", replay / f"cand-{mode}" / "idle.txt"
            ref, _ = read_idle(rp)
            cand, marks = read_idle(cp)
            ok, line = frames_line(rp, cp)
            print(f"{mode}: {line}")
            slipped |= not ok
            runs.append((ref, cand, marks, frame_slips(ref, read_game_frames(rp), read_game_frames(cp))[1]))
    rows = summarise(ranges, runs)
    print(f"{'unit':24s} {'frames':>7s} {'min idle':>9s} {'original':>9s} {'missed':>7s}")
    for name, n, cmin, rmin, missed in rows:
        print(f"{name:24s} {n:7d} {'-' if cmin is None else cmin:>9} {'-' if rmin is None else rmin:>9} {missed:7d}")
    sys.exit(1 if slipped or any(r[4] for r in rows) else 0)


if __name__ == "__main__":
    main()
