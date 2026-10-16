#!/usr/bin/env python3
"""Difftest every function of the non-matching units (splits_nonmatching.txt).

Each c-nonmatching unit replaces an unresolved range with functionally
equivalent C for the shiftable build only. A unit is accepted only when every
function it defines reports MATCHED_TRIALS from tools/difftest over 500
complete trials, and the shiftable build with it passes tools/replay_check.sh.
This script runs the first half: it compiles each unit through the shiftable
build's checks (no movable-address literals, no writable data), finds the
functions it defines, and runs difftest on each one with an isolated HOME
(difftest caches callee arities there).

A source may give a verified ROM end for difftest on its second header line
or later, as `difftest-rom-end: f_cac8=0xcf68`, for a function whose end
fn.py's heuristic misjudges.

usage: nonmatching_check.py [--file LIST] [--only f_xxx ...] [-j N] [--trials N]
Logs and results.json go to build/nonmatching-check/.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
OUT = ROOT / "build" / "nonmatching-check"


def rom_ends(src: Path) -> dict:
    return {m[1]: int(m[2], 16) for m in re.finditer(r"difftest-rom-end:\s*(f_[0-9a-f]+)=(0x[0-9a-f]+)",
                                                     src.read_text())}


def defined_functions(obj: Path, start: int, end: int) -> list:
    r = subprocess.run(["sh-elf-nm", "--defined-only", str(obj)], capture_output=True, text=True, check=True)
    out = []
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) == 3 and p[1] in "Tt":
            m = re.fullmatch(r"_(f_([0-9a-f]+))", p[2])
            if m and start <= int(m[2], 16) < end:
                out.append(m[1])
    return sorted(out, key=lambda n: int(n[2:], 16))


def verdict(log: str, trials: int):
    """(ok, verdict line, trials line) from a difftest log."""
    v = next((l for l in log.splitlines() if l.startswith(("MATCHED_TRIALS", "DIFFERENT", "INCONCLUSIVE"))), "")
    t = next((l.strip() for l in log.splitlines() if l.strip().startswith("trials:")), "")
    m = re.match(r"trials: (\d+) complete, (\d+) hit the step limit", t)
    ok = v.startswith("MATCHED_TRIALS") and m is not None and int(m[1]) >= trials and m[2] == "0"
    return ok, v or (log.strip().splitlines() or ["no output"])[-1], t


def run(job, args):
    unit, fn, src, rom_end = job
    home = OUT / "home" / fn
    home.mkdir(parents=True, exist_ok=True)
    cmd = [sys.executable, str(ROOT / "tools" / "difftest" / "difftest.py"), str(src), fn,
           "--trials", str(args.trials), "--time", str(args.time), "--steps", str(args.steps)]
    if rom_end:
        cmd += ["--rom-end", f"0x{rom_end:x}"]
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, env={**os.environ, "HOME": str(home)})
    log = r.stdout + r.stderr
    (OUT / f"{fn}.log").write_text(" ".join(cmd[1:]) + "\n" + log)
    ok, v, t = verdict(log, args.trials)
    return {"unit": unit, "function": fn, "ok": ok, "verdict": v, "trials": t,
            "rom_end": f"0x{rom_end:x}" if rom_end else None}


def main():
    import shiftable
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--file", type=Path, help="unit list (default splits_nonmatching.txt)")
    ap.add_argument("--only", nargs="*", default=[], help="function names to run")
    ap.add_argument("-j", type=int, default=2, help="parallel difftest runs")
    ap.add_argument("--trials", type=int, default=500)
    ap.add_argument("--steps", type=int, default=2000000)
    ap.add_argument("--time", type=float, default=6000)
    args = ap.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    jobs = []
    for start, end, src in shiftable.read_nonmatching(args.file):
        u = shiftable.Unit(start, end, "c-nonmatching", ".text", src)
        obj = shiftable.compile_nonmatching(u, OUT)
        ends = rom_ends(ROOT / src)
        for fn in defined_functions(obj, start, end):
            if not args.only or fn in args.only:
                jobs.append((src, fn, ROOT / src, ends.get(fn)))
    with ThreadPoolExecutor(args.j) as ex:
        results = list(ex.map(lambda j: run(j, args), jobs))
    for r in results:
        print(f"{'ok  ' if r['ok'] else 'FAIL'} {r['function']:8s} {r['unit']}: {r['verdict']}")
        print(f"     {r['trials']}" + (f" (--rom-end {r['rom_end']})" if r["rom_end"] else ""))
    (OUT / "results.json").write_text(json.dumps(results, indent=1) + "\n")
    bad = sum(not r["ok"] for r in results)
    print(f"{len(results) - bad}/{len(results)} functions MATCHED_TRIALS over {args.trials} complete trials")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
