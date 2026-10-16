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

A source may give a verified ROM end for difftest, as `difftest-rom-end:
f_cac8=0xcf68`, for a function whose end fn.py's heuristic misjudges, and a
larger step limit, as `difftest-steps: f_16938=30000000`, for a function
whose original itself runs longer than the default on large random inputs.

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


def annotations(src: Path, key: str) -> dict:
    """{function: value} from `key: f_a=V f_b=V ...` annotations in src."""
    out = {}
    for m in re.finditer(re.escape(key) + r":([^\n]*)", src.read_text()):
        for f, v in re.findall(r"\b(f_[0-9a-f]+)=(0x[0-9a-f]+|\d+)", m[1]):
            out[f] = int(v, 0)
    return out


def rom_ends(src: Path) -> dict:
    return annotations(src, "difftest-rom-end")


def step_limits(src: Path) -> dict:
    return annotations(src, "difftest-steps")


def nonreturning_functions(src: Path) -> set:
    """Functions marked `difftest-nonreturning: f_a f_b (why)`."""
    out = set()
    for m in re.finditer(r"difftest-nonreturning:([^\n]*)", src.read_text()):
        out |= set(re.findall(r"\bf_[0-9a-f]+\b", m[1].split("(")[0]))
    return out


def returning_inputs(src: Path) -> dict:
    """`difftest-inputs f_a: SPEC SPEC ...`: inputs on which the original
    returns, to be run as trials (difftest --input)."""
    out = {}
    for m in re.finditer(r"difftest-inputs (f_[0-9a-f]+):([^\n]*)", src.read_text()):
        out.setdefault(m[1], []).extend(x for x in m[2].replace("*/", " ").split() if re.match(r"\d", x))
    return out


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


def difftest(src, fn, home, steps, rom_end, extra):
    cmd = [sys.executable, str(ROOT / "tools" / "difftest" / "difftest.py"), str(src), fn, "--steps", str(steps)]
    if rom_end:
        cmd += ["--rom-end", f"0x{rom_end:x}"]
    cmd += extra
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, env={**os.environ, "HOME": str(home)})
    return " ".join(cmd[1:]) + "\n" + r.stdout + r.stderr


TRIAL = re.compile(r"^\s+input (\S+): ok \((\d+)/(\d+) steps(, truncated)?\)", re.M)


def nonreturning(log, steps, trials):
    """For a function whose original does not return on some inputs: (ok,
    why, hanging inputs). Every trial must be compared without a difference,
    and each one that hit the step limit must have hit it on both sides, so
    the C hangs on exactly the inputs where the original hangs."""
    v = next((l for l in log.splitlines() if l.startswith(("MATCHED_TRIALS", "DIFFERENT", "INCONCLUSIVE"))), "")
    m = re.search(r"trials: (\d+) complete, (\d+) hit the step limit \(compared up to it\), (\d+) unsupported", log)
    if not m or v.startswith("DIFFERENT") or m[3] != "0" or int(m[1]) + int(m[2]) < trials:
        return False, v or "no verdict", []
    if "only" in v and "step limit" not in v:
        return False, v, []
    hang = []
    for spec, rs, cs, trunc in TRIAL.findall(log):
        if trunc:
            if int(rs) <= steps or int(cs) <= steps:
                side = "original" if int(rs) > steps else "candidate"
                return False, f"input {spec}: only the {side} hit the step limit", []
            hang.append(spec)
    if len(hang) != int(m[2]):
        return False, "could not read every step-limited trial", []
    return True, f"{m[1]} trials returned and matched; {m[2]} hung on both sides, compared up to the limit", hang


def trace_status(log):
    """{'ROM': status, 'candidate': status} from a --status run."""
    return dict(re.findall(r"^--- (ROM|candidate): \d+ steps, (\w+)", log, re.M))


def run(job, args):
    unit, fn, src, rom_end, steps, nonret, inputs = job
    steps = steps or args.steps
    home = OUT / "home" / fn
    home.mkdir(parents=True, exist_ok=True)
    log = difftest(src, fn, home, steps, rom_end, ["--trials", str(args.trials), "--time", str(args.time), "-v"])
    ok, v, t = verdict(log, args.trials)
    notes = []
    if not ok and nonret:
        ok, why, hang = nonreturning(log, steps, args.trials)
        if ok:
            # the original still hangs, and the C with it, at ten times the limit
            for spec in hang[:args.hang_samples]:
                tl = difftest(src, fn, home, 10 * steps, rom_end, ["--status", spec])
                log += "\n" + tl
                st = trace_status(tl)
                if st.get("ROM") != "limit" or st.get("candidate") != "limit":
                    ok, why = False, f"input {spec} at {10 * steps} steps: {st}"
                    break
            else:
                notes.append(f"{min(len(hang), args.hang_samples)} hanging inputs still hang on both sides "
                             f"at {10 * steps} steps")
        if ok and inputs:
            il = difftest(src, fn, home, steps, rom_end, [x for spec in inputs for x in ("--input", spec)])
            log += "\n" + il
            iok, iv, it = verdict(il, len(inputs))
            if not iok:
                ok, why = False, f"listed returning inputs: {iv} {it}"
            else:
                notes.append(f"{len(inputs)} listed inputs that return all matched")
        if ok and not re.search(r"trials: [1-9]", log) and not inputs:
            ok, why = False, "no input returned"
        v = ("non-returning: " if ok else "") + why
    (OUT / f"{fn}.log").write_text(log)
    return {"unit": unit, "function": fn, "ok": ok, "verdict": v, "trials": t,
            "rom_end": f"0x{rom_end:x}" if rom_end else None, "steps": steps, "notes": notes}


def main():
    import shiftable
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--file", type=Path, help="unit list (default splits_nonmatching.txt)")
    ap.add_argument("--only", nargs="*", default=[], help="function names to run")
    ap.add_argument("-j", type=int, default=2, help="parallel difftest runs")
    ap.add_argument("--trials", type=int, default=500)
    ap.add_argument("--steps", type=int, default=2000000)
    ap.add_argument("--time", type=float, default=6000)
    ap.add_argument("--hang-samples", type=int, default=5,
                    help="hanging inputs of a non-returning function re-run at ten times the step limit")
    args = ap.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    jobs = []
    for start, end, src in shiftable.read_nonmatching(args.file):
        u = shiftable.Unit(start, end, "c-nonmatching", ".text", src)
        obj = shiftable.compile_nonmatching(u, OUT)
        ends, steps = rom_ends(ROOT / src), step_limits(ROOT / src)
        nonret, inputs = nonreturning_functions(ROOT / src), returning_inputs(ROOT / src)
        for fn in defined_functions(obj, start, end):
            if not args.only or fn in args.only:
                jobs.append((src, fn, ROOT / src, ends.get(fn), steps.get(fn), fn in nonret, inputs.get(fn, [])))
    with ThreadPoolExecutor(args.j) as ex:
        results = list(ex.map(lambda j: run(j, args), jobs))
    for r in results:
        print(f"{'ok  ' if r['ok'] else 'FAIL'} {r['function']:8s} {r['unit']}: {r['verdict']}")
        extra = (f" --rom-end {r['rom_end']}" if r["rom_end"] else "") + \
            (f" --steps {r['steps']}" if r["steps"] != args.steps else "")
        print(f"     {r['trials']}" + (f" ({extra.strip()})" if extra else ""))
        for n in r.get("notes", []):
            print(f"     {n}")
    (OUT / "results.json").write_text(json.dumps(results, indent=1) + "\n")
    bad = sum(not r["ok"] for r in results)
    print(f"{len(results) - bad}/{len(results)} functions MATCHED_TRIALS over {args.trials} complete trials")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
