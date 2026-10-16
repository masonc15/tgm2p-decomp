#!/usr/bin/env python3
"""Recompile near-miss drafts under every documented SHC optimization option set.

SHC v5.0r32's own help (`shc.exe` with no arguments) documents these
optimization controls: OPtimize=0|1, SPeed|NOSPeed|SIze, INLine[=<node-count>]
|NOINLine, NEstinline=<number> and LOop|NOLOop. This tool builds their cross
product (keeping every other flag of the draft, such as -macsave=1), compiles
each draft once per combination with the pinned compiler, and scores every
function the draft defines with funcscore's per-function comparison (pool
loads compared by value, outside calls excused). The residual of a function is
the number of ROM halfwords that differ.

Drafts are the research cases (research/**/*.c with a first-line header), the
historical near-miss drafts in tools/shc_probe/cases/wip/, the genuine context
drafts in src/ behind c-prefix/c-range entries (whose unretained functions are
near-misses of unresolved ranges), and the four probe positive controls in
tools/shc_probe/cases/. Combinations that give identical .text, relocations
and symbols are compiled once and reported together.

This is diagnostic research: a lower residual grants nothing, and no flag set
outside the build's existing ones may be integrated without the user's
approval.

usage: flag_sweep.py [--jobs N] [--json build/flag_sweep.json] [--only SUBSTR]
                     [--grid full|quick] [draft.c ...]
"""
from __future__ import annotations

import argparse
import hashlib
import itertools
import json
import os
import re
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))
import build  # noqa: E402
import fn  # noqa: E402
import funcscore  # noqa: E402
from probe import HEADER, SHCC  # noqa: E402

VERSION = "shc-v5.0r32"
TIMEOUT = 120
OPT_FLAG = re.compile(r"^-(optimize|speed|nospeed|size|inline|noinline|nestinline|loop|noloop)\b", re.I)
BUILD_SETS = {  # flag sets src/ already uses (first-line headers)
    "-macsave=1 -optimize=1 -speed", "-macsave=1 -optimize=0 -speed", "-macsave=1 -optimize=0"}
DEFAULT_FLAGS = "-macsave=1 -optimize=1 -speed"


def grid(kind="full"):
    optimize = ["-optimize=0", "-optimize=1"]
    speed = ["", "-speed", "-nospeed", "-size"]
    inline = ["", "-noinline", "-inline", "-inline=10", "-inline=20", "-inline=40", "-inline=80"]
    loop = ["", "-loop", "-noloop"]
    nest = ["", "-nestinline=3"]
    if kind == "quick":
        inline = ["", "-noinline", "-inline=40"]
        nest = [""]
    out = []
    for combo in itertools.product(optimize, speed, inline, loop, nest):
        flags = [f for f in combo if f]
        if "-nestinline=3" in flags and ("-noinline" in flags):
            continue  # nesting depth is meaningless without inlining
        out.append(" ".join(flags))
    return out


# ---------------------------------------------------------------- drafts

def header_flags(path: Path) -> str:
    first = path.read_text(errors="replace").split("\n", 1)[0]
    m = re.search(r"flags:\s*([^*\n]+?)\s*\*/", first)
    return m[1].strip() if m else DEFAULT_FLAGS


def keep_flags(flags: str) -> list:
    return [f for f in flags.split() if not OPT_FLAG.match(f)]


def discover():
    drafts = []
    for p in sorted((ROOT / "research").glob("**/*.c")):
        first = p.read_text(errors="replace").split("\n", 1)[0]
        if "flags:" in first or "rom:" in first:
            drafts.append(("research", p))
    for p in sorted((ROOT / "tools/shc_probe/cases/wip").glob("*.c")):
        drafts.append(("wip", p))
    seen = set()
    for a, b, kind, arg in build.read_splits():
        if kind in ("c-prefix", "c-range") and arg not in seen:
            seen.add(arg)
            drafts.append(("context", ROOT / arg))
    for p in sorted((ROOT / "tools/shc_probe/cases").glob("*.c")):
        drafts.append(("control", p))
    return drafts


def function_map(path: Path) -> dict:
    """Object symbol name -> ROM address, as funcscore locates functions."""
    hdr = HEADER.search(path.read_text(errors="replace").split("\n", 1)[0] + "\n")
    where = {}
    if hdr and hdr["func"] and hdr["rom"]:
        where[hdr["func"]] = int(hdr["rom"], 16)
    return where


def segment_kind(rom: int) -> str:
    for a, b, kind, _ in build.read_splits():
        if a <= rom < b:
            exempt = any(x <= rom < y for x, y in ((0x30D40, 0x3127C), (0x312BA, 0x313E4)))
            return "exempt" if exempt else ("unresolved" if kind == "asm" else "c")
    return "?"


# ---------------------------------------------------------------- compile + score

def compile_one(src: Path, flags: list):
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp) / "case.o"
        try:
            r = subprocess.run([str(SHCC), VERSION, str(out), str(src), *flags],
                               capture_output=True, text=True, timeout=TIMEOUT)
        except subprocess.TimeoutExpired:
            return None, "timeout"
        if r.returncode or not out.exists():
            return None, (r.stdout + r.stderr).strip().splitlines()[-1:] or ["failed"]
        data, funcs, relocs = funcscore.load(out)
        rel = subprocess.run(["sh-elf-objdump", "-r", "-j", ".text", str(out)], capture_output=True,
                             text=True).stdout
        rel = "\n".join(l for l in rel.splitlines() if str(out) not in l)  # drop the temp path
        h = hashlib.sha256(data + rel.encode() + repr(funcs).encode()).hexdigest()
        return (data, funcs, relocs, h), None


def score_all(obj, where):
    data, funcs, relocs, _ = obj
    out = {}
    for ostart, sym in funcs:
        name = sym[1:]
        m = funcscore.AUTO.match(sym)
        rom = where.get(name, int(m[1], 16) if m else None)
        if rom is None or not (0x400 <= rom < fn.TEXT_END):
            continue
        hit, n, _ = funcscore.score(data, relocs, ostart, rom)
        out[name] = {"rom": rom, "residual": n - hit, "halfwords": n}
    return out


def sweep_draft(kind, path, combos, jobs):
    base = header_flags(path)
    keep = keep_flags(base)
    where = function_map(path)
    sets = {"baseline": base.split()}
    for c in combos:
        sets[c or "(defaults)"] = keep + c.split()
    results = {}
    errors = {}
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        futs = {ex.submit(compile_one, path, flags): label for label, flags in sets.items()}
        for f in as_completed(futs):
            obj, err = f.result()
            label = futs[f]
            if err:
                errors[label] = err if isinstance(err, str) else " ".join(err)
            else:
                results[label] = obj
    if "baseline" not in results:
        return {"draft": str(path.relative_to(ROOT)), "kind": kind, "baseline_flags": base,
                "error": errors.get("baseline", "baseline failed")}
    groups = {}
    for label, obj in results.items():
        groups.setdefault(obj[3], []).append(label)
    scores = {h: score_all(next(results[l] for l in labels), where) for h, labels in groups.items()}
    bh = results["baseline"][3]
    baseline = scores[bh]
    funcs = {}
    for name, b in baseline.items():
        entry = {"rom": f"0x{b['rom']:05x}", "segment": segment_kind(b["rom"]),
                 "halfwords": b["halfwords"], "baseline": b["residual"], "outcomes": []}
        for h, labels in groups.items():
            s = scores[h].get(name)
            r = None if s is None else s["residual"]
            entry["outcomes"].append({"residual": r, "flag_sets": sorted(labels)})
        vals = [o["residual"] for o in entry["outcomes"] if o["residual"] is not None]
        entry["best"] = min(vals) if vals else None
        entry["best_flag_sets"] = sorted({l for o in entry["outcomes"] if o["residual"] == entry["best"]
                                          for l in o["flag_sets"]})
        entry["improving_flag_sets"] = sorted({l for o in entry["outcomes"]
                                               if o["residual"] is not None and o["residual"] < b["residual"]
                                               for l in o["flag_sets"]})
        funcs[name] = entry
    return {"draft": str(path.relative_to(ROOT)), "kind": kind, "baseline_flags": base,
            "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
            "combinations": len(sets), "distinct_outputs": len(groups),
            "baseline_group": sorted(groups[bh]), "errors": errors, "functions": funcs}


def in_build(label: str, base: str) -> bool:
    if label == "baseline":
        return True
    flags = set(keep_flags(base)) | set(label.split())
    return any(flags == set(s.split()) for s in BUILD_SETS)


def summarize(report):
    rows = []
    for d in report["drafts"]:
        if "error" in d:
            continue
        for name, f in d["functions"].items():
            if f["baseline"] == 0 and f["best"] == 0:
                continue
            new = [l for l in f["improving_flag_sets"] if not in_build(l, d["baseline_flags"])]
            rows.append({"draft": d["draft"], "kind": d["kind"], "function": name, "rom": f["rom"],
                         "segment": f["segment"], "halfwords": f["halfwords"], "baseline": f["baseline"],
                         "best": f["best"], "exact_flag_sets": f["best_flag_sets"] if f["best"] == 0 else [],
                         "improving_new_flag_sets": new,
                         "best_flag_sets": f["best_flag_sets"][:6]})
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("drafts", nargs="*", type=Path)
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 2)
    ap.add_argument("--json", type=Path, default=ROOT / "build" / "flag_sweep.json")
    ap.add_argument("--only", help="sweep drafts whose path contains this text")
    ap.add_argument("--grid", choices=["full", "quick"], default="full")
    args = ap.parse_args()
    combos = grid(args.grid)
    drafts = [("given", p.resolve()) for p in args.drafts] if args.drafts else discover()
    if args.only:
        drafts = [(k, p) for k, p in drafts if args.only in str(p)]
    t0 = time.time()
    report = {"schema": 1, "compiler": VERSION, "grid": args.grid, "combinations": len(combos) + 1,
              "tool_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "program_sha256": hashlib.sha256(fn.ROM).hexdigest(), "drafts": []}
    for i, (kind, path) in enumerate(drafts, 1):
        d = sweep_draft(kind, path, combos, args.jobs)
        report["drafts"].append(d)
        if "error" in d:
            print(f"[{i}/{len(drafts)}] {d['draft']}: baseline failed: {d['error']}", flush=True)
            continue
        near = {n: f for n, f in d["functions"].items() if f["baseline"] or f["best"]}
        better = {n: f for n, f in near.items() if f["best"] is not None and f["best"] < f["baseline"]}
        print(f"[{i}/{len(drafts)}] {d['draft']}: {d['distinct_outputs']} distinct outputs from "
              f"{d['combinations']} flag sets; {len(near)} near-miss functions, {len(better)} improved",
              flush=True)
        for n, f in better.items():
            print(f"    {n} {f['rom']} {f['segment']}: {f['baseline']} -> {f['best']} "
                  f"({', '.join(f['best_flag_sets'][:3])}{', ...' if len(f['best_flag_sets']) > 3 else ''})")
    report["summary"] = summarize(report)
    report["seconds"] = round(time.time() - t0)
    args.json.parent.mkdir(parents=True, exist_ok=True)
    args.json.write_text(json.dumps(report, indent=1) + "\n")
    rows = report["summary"]
    exact = [r for r in rows if r["exact_flag_sets"] and r["baseline"]]
    newgain = [r for r in rows if r["improving_new_flag_sets"]]
    print(f"\n{len(report['drafts'])} drafts, {len(rows)} near-miss functions; "
          f"{len(exact)} become exact under some flag set; {len(newgain)} improve only under flag sets "
          f"the build does not use; {report['seconds']} s")


if __name__ == "__main__":
    main()
