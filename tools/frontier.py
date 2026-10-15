#!/usr/bin/env python3
"""Generate a source/assembly frontier from actual splits; diagnostics, not acceptance.

Function names and dependency groups are discovery heuristics. A source definition
never earns credit. Only existing C splits count, and a cold replay must verify
them. Review CFG, pool ownership and entry addresses before trying a new split.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import build
import cuts
import fn

ROOT = Path(__file__).resolve().parents[1]
EXEMPT = [(0x30d40, 0x3127c), (0x312ba, 0x313e4)]
DEFINITION = re.compile(r"\bf_([0-9a-f]+)\s*\([^;{}]*\)\s*\{")


def overlap(a, b, c, d):
    return max(0, min(b, d) - max(a, c))


def generate():
    definitions = {}
    for path in sorted((ROOT / "src").glob("*.c")):
        text = re.sub(r"/\*.*?\*/|//[^\n]*", "", path.read_text(), flags=re.S)
        for address in DEFINITION.findall(text):
            definitions.setdefault(int(address, 16), []).append(str(path.relative_to(ROOT)))
    segments = build.read_splits()
    spans = cuts.spans()
    routes = json.loads((ROOT / "config/frontier_routes.json").read_text())
    queue = []
    total = done = remaining = exempt = 0
    for a, b, kind, source in segments:
        if kind not in ("asm", "c", "c-prefix", "c-range"):
            continue
        total += b - a
        if kind != "asm":
            done += b - a
            continue
        exception_bytes = sum(overlap(a, b, c, d) for c, d in EXEMPT)
        exempt += exception_bytes
        remaining += b - a - exception_bytes
        entries = [{"address": f"0x{c:06x}", "candidate_sources": paths}
                   for c, paths in sorted(definitions.items()) if a <= c < b]
        entry = {"start": f"0x{a:06x}", "end": f"0x{b:06x}", "asm_bytes": b - a,
                 "exception_bytes": exception_bytes, "unresolved_bytes": b - a - exception_bytes,
                 "source_candidates": entries,
                 "heuristic_dependency_groups": [[f"0x{c:06x}", f"0x{d:06x}"]
                                                  for c, d, _ in spans if overlap(a, b, c, d)],
                 "route": "review-boundaries-and-real-context"}
        entry["hypotheses"] = [r for r in routes["targets"] if a <= int(r["address"], 0) < b]
        queue.append(entry)
    if total != 200698 or exempt != 1638 or done + remaining + exempt != total:
        raise ValueError("coverage partition or approved exceptions drifted")
    return {"schema": 1, "scope": "source-map routing only; not a fresh exactness verdict",
            "program_sha256": hashlib.sha256(fn.ROM).hexdigest(),
            "splits_sha256": hashlib.sha256((ROOT / "splits.txt").read_bytes()).hexdigest(),
            "c_bytes": done, "total_bytes": total, "percent": 100 * done / total,
            "unresolved_nonexempt_bytes": remaining, "approved_asm_bytes": exempt,
            "completion_c_bytes": total - exempt,
            "experiment_policy": routes["experiment_policy"], "queue": queue}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--output", type=Path, default=ROOT / "build/frontier.json")
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()
    report = generate()
    text = json.dumps(report, indent=2) + "\n"
    if args.check:
        if args.output.read_text() != text:
            raise SystemExit("frontier is stale; regenerate it")
    else:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text)
    print(f"{report['c_bytes']}/{report['total_bytes']} C bytes; "
          f"{report['unresolved_nonexempt_bytes']} unresolved + {report['approved_asm_bytes']} exempt; "
          f"{len(report['queue'])} assembly ranges")
    for r in report["queue"]:
        if r["unresolved_bytes"]:
            print(f"{r['start']}-{r['end']}: {r['unresolved_bytes']:5d} bytes; "
                  f"{len(r['source_candidates'])} source-defined entry candidates; "
                  f"{len(r['hypotheses'])} focused hypotheses")


if __name__ == "__main__":
    main()
