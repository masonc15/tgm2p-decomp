#!/usr/bin/env python3
"""Score every near-miss draft's functions against the ROM under each SHC
release (flags as in the draft header). Reports unresolved functions that are
closer to the ROM under a release other than the pinned v5.0r32.
usage: versweep.py OUT.json [--kinds wip,context,research] [--jobs N] [--versions ...]"""
import argparse, json, subprocess, sys, tempfile, time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools")); sys.path.insert(0, str(ROOT / "tools/shc_probe"))
import flag_sweep as fs, funcscore

ap = argparse.ArgumentParser(); ap.add_argument("out"); ap.add_argument("--kinds", default="wip,context")
ap.add_argument("--jobs", type=int, default=9)
ap.add_argument("--versions", default="v5.0r32,v5.0r28,v5.0r10,v5.1r01,v5.1r03,v5.1r13")
ap.add_argument("--only"); a = ap.parse_args()
vers = a.versions.split(","); kinds = set(a.kinds.split(","))
drafts = [(k, p) for k, p in fs.discover() if k in kinds and (not a.only or a.only in str(p))]
print(len(drafts), "drafts", flush=True)

def comp(args):
    kind, path, v = args
    flags = fs.header_flags(path).split()
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp) / "case.o"
        try:
            r = subprocess.run([str(fs.SHCC), "shc-" + v, str(out), str(path), *flags],
                               capture_output=True, text=True, timeout=180)
        except subprocess.TimeoutExpired:
            return kind, path, v, None
        if r.returncode or not out.exists():
            return kind, path, v, None
        data, funcs, relocs = funcscore.load(out)
        return kind, path, v, fs.score_all((data, funcs, relocs, None), fs.function_map(path))

t0 = time.time(); res = {}
with ThreadPoolExecutor(a.jobs) as ex:
    for kind, path, v, sc in ex.map(comp, [(k, p, v) for k, p in drafts for v in vers]):
        res.setdefault(str(path.relative_to(ROOT)), {"kind": kind, "v": {}})["v"][v] = sc
rows = []
for d, e in res.items():
    base = e["v"].get("v5.0r32")
    if not base: continue
    for name, b in base.items():
        seg = fs.segment_kind(b["rom"])
        per = {v: (e["v"][v] or {}).get(name, {}).get("residual") if e["v"].get(v) else None for v in vers}
        rows.append({"draft": d, "kind": e["kind"], "function": name, "rom": f"0x{b['rom']:05x}", "segment": seg,
                     "halfwords": b["halfwords"], "residual": per})
json.dump({"versions": vers, "rows": rows, "seconds": round(time.time() - t0)}, open(a.out, "w"), indent=1)
unres = [r for r in rows if r["segment"] == "unresolved"]
print(f"{len(rows)} function scores, {len(unres)} in unresolved ranges, {round(time.time()-t0)} s")
for v in vers[1:]:
    better = [r for r in unres if r["residual"][v] is not None and r["residual"][v] < r["residual"]["v5.0r32"]]
    exact = [r for r in better if r["residual"][v] == 0]
    worse = [r for r in unres if r["residual"][v] is not None and r["residual"][v] > r["residual"]["v5.0r32"]]
    same = [r for r in unres if r["residual"][v] == r["residual"]["v5.0r32"]]
    print(f"{v}: unresolved functions better {len(better)} (exact {len(exact)}), worse {len(worse)}, same {len(same)}")
    for r in sorted(better, key=lambda r: r["residual"][v])[:25]:
        print(f"    {r['function']:<14} {r['rom']} {r['halfwords']:5d} hw  r32 {r['residual']['v5.0r32']:4d} -> {r['residual'][v]:4d}   {r['draft']}")
