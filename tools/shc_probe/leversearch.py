#!/usr/bin/env python3
"""Coordinate search over hand-named levers for one function.

A spec file (Python) defines FUNC, the draft-relative path DRAFT, and LEVERS:
an ordered dict of lever name -> {choice name -> transform}. A transform takes
the whole unit source and returns the changed source, or None when its anchor
text is missing. The first choice of each lever is the starting point. Each
round tries every other choice of every lever with the rest held fixed, keeps
the best single change, and stops when none helps. Scores come from
fdiff.analyse, as in formsearch.py; a higher score grants nothing.

usage: leversearch.py SPEC.py [--out DIR] [--jobs N] [--start lever=choice,...] [--all]
  --all   try the full product instead (small specs only)
"""
import argparse
import importlib.util
import itertools
import json
import sys
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))


def load_spec(path):
    spec = importlib.util.spec_from_file_location("lever_spec", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def build(mod, base, state):
    src = base
    for lever, choices in mod.LEVERS.items():
        src = choices[state[lever]](src)
        if src is None:
            return None
    return src


def evaluate(job):
    spec_path, base_path, state, workdir = job
    import fdiff
    mod = load_spec(spec_path)
    src = build(mod, Path(base_path).read_text(), state)
    if src is None:
        return state, None
    name = "_".join(f"{k}-{v}" for k, v in state.items())
    path = Path(workdir) / (name[:180] + ".c")
    path.write_text(src)
    try:
        r = fdiff.analyse(path, mod.FUNC)
    except Exception as e:  # does not compile: not a candidate
        return state, str(e)[-120:]
    ours, rom = len(r["ours"]), len(r["theirs"])
    return state, {"ours": ours, "rom": rom, "eq": r["eq"], "eq_noreg": r["eq_noreg"],
                   "score": r["eq"] + r["eq_noreg"] - abs(ours - rom)}


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("spec", type=Path)
    ap.add_argument("--out", type=Path)
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--start", default="")
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args()
    mod = load_spec(args.spec)
    base = ROOT / mod.DRAFT
    out = args.out or ROOT / "build" / "leversearch" / mod.FUNC
    out.mkdir(parents=True, exist_ok=True)
    state = {k: next(iter(v)) for k, v in mod.LEVERS.items()}
    for item in filter(None, args.start.split(",")):
        k, v = item.split("=")
        state[k] = v
    show = lambda r: f"ours {r['ours']}/{r['rom']} eq {r['eq']} noreg {r['eq_noreg']} score {r['score']}"
    job = lambda st: (str(args.spec), str(base), dict(st), str(out))
    log = []
    with ProcessPoolExecutor(args.jobs) as ex:
        if args.all:
            keys = list(mod.LEVERS)
            states = [dict(zip(keys, c)) for c in itertools.product(*(mod.LEVERS[k] for k in keys))]
            res = [(s, r) for s, r in ex.map(evaluate, map(job, states)) if isinstance(r, dict)]
            res.sort(key=lambda x: -x[1]["score"])
            for s, r in res[:15]:
                print(" ".join(f"{k}={v}" for k, v in s.items()), "|", show(r))
            best_state, best = res[0]
        else:
            _, best = evaluate(job(state))
            if not isinstance(best, dict):
                sys.exit(f"start state does not build: {best}")
            best_state = dict(state)
            print("start:", show(best), flush=True)
            while True:
                trials = [dict(best_state, **{k: c}) for k, cs in mod.LEVERS.items() for c in cs if c != best_state[k]]
                res = [(s, r) for s, r in ex.map(evaluate, map(job, trials)) if isinstance(r, dict)]
                log.append([{"state": s, **r} for s, r in res])
                res.sort(key=lambda x: -x[1]["score"])
                if not res or res[0][1]["score"] <= best["score"]:
                    top = "; ".join(f"{next(f'{k}={v}' for k, v in s.items() if best_state[k] != v)} {r['score']}" for s, r in res[:6])
                    print("no further gain. next best:", top)
                    break
                best_state, best = res[0]
                print("take", " ".join(f"{k}={v}" for k, v in best_state.items()), "|", show(best), flush=True)
    final = out / f"{mod.FUNC}.best.c"
    final.write_text(build(mod, base.read_text(), best_state))
    (out / "log.json").write_text(json.dumps({"best_state": best_state, "best": best, "rounds": log}, indent=1) + "\n")
    print("best:", " ".join(f"{k}={v}" for k, v in best_state.items()), "->", final)


if __name__ == "__main__":
    main()
