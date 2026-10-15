#!/usr/bin/env python3
"""One-screen project status for the start of a session.

usage: make status          (refreshes build/frontier.json, then runs this)
       tools/status.py [--top N]

Prints this checkout's git state, the verified coverage from build/report.json
(run `make report` first if it is stale), and the largest unresolved ranges
from build/frontier.json with their recorded hypotheses. It reads files only;
it grants no credit and changes nothing.
"""
import argparse
import json
import os
from pathlib import Path
import socket
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import toolchain  # noqa: E402

TOTAL = 200698
TARGET = 199060  # every non-exempt byte; see RUNTIME_EXCEPTIONS.md


def git(*args):
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True).stdout.strip()


def newest(paths):
    return max((p.stat().st_mtime for p in paths if p.exists()), default=0)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--top", type=int, default=12)
    args = ap.parse_args()

    print(f"host {socket.gethostname()}   toolchain {toolchain.ROOT}")
    branch = git("status", "-sb").splitlines()[:1]
    dirty = [l for l in git("status", "--porcelain").splitlines() if l]
    print(f"git  {branch[0] if branch else '?'}   {len(dirty)} changed paths   HEAD {git('log', '-1', '--format=%h %s')}")

    report = ROOT / "build/report.json"
    if report.exists():
        m = json.loads(report.read_text())["measures"]
        c = int(m["matched_code"])
        stale = newest([ROOT / "splits.txt", *(ROOT / "src").glob("*.c")]) > report.stat().st_mtime
        print(f"\ncoverage  {c:,} / {TOTAL:,} C bytes = {100 * c / TOTAL:.4f}%"
              f"{'   (report older than src/ or splits.txt: run make report)' if stale else ''}")
        print(f"remaining {TARGET - c:,} non-exempt bytes to the {TARGET:,}-byte goal ({100 * TARGET / TOTAL:.4f}%)")
        print(f"functions {m.get('matched_functions')}/{m.get('total_functions')}   units {m.get('complete_units')}/{m.get('total_units')} complete (objdiff report counts)")
    else:
        print("\nno build/report.json yet: run make split check report")

    frontier = ROOT / "build/frontier.json"
    if not frontier.exists():
        print("no build/frontier.json yet: run make status")
        return
    queue = json.loads(frontier.read_text())["queue"]
    queue.sort(key=lambda r: -r["unresolved_bytes"])
    print(f"\n{len(queue)} unresolved ranges; largest {min(args.top, len(queue))}"
          " (hypotheses: config/frontier_routes.json, evidence paths below):")
    for r in queue[: args.top]:
        hyps = r.get("hypotheses", [])
        states = ", ".join(sorted({h.get("state", "?") for h in hyps})) or "no recorded hypothesis"
        print(f"  {r['start']}-{r['end']} {r['unresolved_bytes']:6,} B  {states}")
        for h in hyps[:1]:
            if h.get("evidence"):
                print(f"      evidence: {h['evidence']}")
            if h.get("next_question"):
                q = h["next_question"]
                print(f"      next: {q[:150]}{'...' if len(q) > 150 else ''}")
    print("\nLatest GOAL.md entry: " + next((l[3:] for l in (ROOT / "GOAL.md").read_text().splitlines()
                                            if l.startswith("## ")), "?"))


if __name__ == "__main__":
    main()
