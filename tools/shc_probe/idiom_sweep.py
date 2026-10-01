#!/usr/bin/env python3
"""Insert behavior-neutral SHC idioms at every statement boundary of one
function and score each variant with funcscore.py.

SHC's register and stack-slot allocation can be steered by statements that
emit no code: empty conditionals, dead stores under `if (0)`, unused block
locals. These idioms come from this project and from the Dreamcast SHC
decomps (sa2dc, recv-dc-decomp). This script tries each one at each place
and reports the variants that score best.

usage: idiom_sweep.py CASE.c FUNC [--idioms FILE] [-j N] [--top N] [--keep DIR] [--out FILE]
       idiom_sweep.py --rescore DIR FUNC [-j N] [--top N] [--out FILE]

Idioms are C statements, one per line in --idioms (or the built-in list);
`$V` in an idiom is replaced by each local variable name the function
declares and `$F` by each pointer parameter. Run on nuada (SHC lives there).

Each variant's score is appended to --out as soon as it's known, so a long
sweep survives a dropped ssh session; --rescore scores a kept directory again.
SHC under wibo occasionally never returns, so every compile has a timeout.
"""
import argparse
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PY = sys.executable
FUNCSCORE = ROOT / "tools" / "shc_probe" / "funcscore.py"

DEFAULT_IDIOMS = [
    "if (0);",
    "if (0) {}",
    "if ($V) {} else {}",
    "if (!$V) {} else {}",
    "if (1) ; else $V = 0;",
    "if (0) $V = 0;",
    "if (0) $V += 1;",
    "{ int t_ = $V; }",
    "$V = $V;",
]


def find_function(src: str, func: str):
    """(body_start, body_end) offsets of FUNC's braces in src."""
    m = re.search(r"^[A-Za-z_][^;{}()]*\b%s\s*\([^;{}]*\)\s*\{" % re.escape(func), src, re.M)
    if not m:
        sys.exit(f"{func} not found")
    start = m.end() - 1
    depth = 0
    for i in range(start, len(src)):
        if src[i] == "{":
            depth += 1
        elif src[i] == "}":
            depth -= 1
            if depth == 0:
                return m.start(), start, i
    sys.exit("unbalanced braces")


def locals_and_params(src: str, head: int, body: int, end: int):
    sig = src[head:body]
    params = re.findall(r"\*\s*(\w+)\s*[,)]", sig)
    scalars = re.findall(r"\b(?:char|short|int|long)\s+(\w+)\s*[,)]", sig)
    decls = re.findall(r"^\s+(?:register\s+)?(?:unsigned\s+|signed\s+)?(?:char|short|int|long)\s+(\w+)\s*;",
                       src[body:end], re.M)
    return params, scalars + decls


def boundaries(src: str, body: int, end: int):
    """Offsets just after lines inside the body that end a statement or open a block.

    A statement inserted before an `else` would take the else over and change
    what the code does, so those positions are skipped, as are positions that
    split an unbraced if/else arm from its condition."""
    out = []
    pos = src.index("\n", body) + 1
    while pos < end:
        nl = src.index("\n", pos)
        line = src[pos:nl].rstrip()
        rest = src[nl + 1:end].lstrip()
        braceless_head = re.match(r"\s*(?:else\s+)?(?:if|for|while)\s*\(.*\)\s*$|\s*else\s*$", line)
        if (line.endswith((";", "{", "}")) and not line.lstrip().startswith(("case", "default"))
                and not rest.startswith("else") and not braceless_head):
            out.append(nl + 1)
        pos = nl + 1
    return out


TIMEOUT = 120


def score(path: Path, func: str):
    try:
        p = subprocess.run([PY, str(FUNCSCORE), str(path)], cwd=ROOT, capture_output=True, text=True,
                           timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        return None
    m = re.search(r"\b%s\s+(\d+)/(\d+)" % re.escape(func), p.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("case", nargs="?")
    ap.add_argument("func")
    ap.add_argument("--rescore")
    ap.add_argument("--out")
    ap.add_argument("--idioms")
    ap.add_argument("-j", type=int, default=6)
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("--keep")
    a = ap.parse_args()
    if a.rescore:
        jobs = [(p, 0, p.name) for p in sorted(Path(a.rescore).glob("v*.c"))]
        print(f"rescoring {len(jobs)} variants in {a.rescore}", flush=True)
        report(jobs, a)
        return
    if not a.case:
        ap.error("CASE is required without --rescore")

    src = Path(a.case).read_text()
    head, body, end = find_function(src, a.func)
    params, vars_ = locals_and_params(src, head, body, end)
    idioms = Path(a.idioms).read_text().splitlines() if a.idioms else DEFAULT_IDIOMS
    expanded = []
    for idiom in (i.strip() for i in idioms):
        if not idiom:
            continue
        if "$V" in idiom:
            expanded += [idiom.replace("$V", v) for v in vars_]
        elif "$F" in idiom:
            expanded += [idiom.replace("$F", p) for p in params]
        else:
            expanded.append(idiom)

    tmp = Path(a.keep) if a.keep else Path(tempfile.mkdtemp(prefix="sweep_"))
    tmp.mkdir(parents=True, exist_ok=True)
    jobs = []
    for bi, off in enumerate(boundaries(src, body, end)):
        line_no = src.count("\n", 0, off)
        for ii, idiom in enumerate(expanded):
            path = tmp / f"v{bi:03d}_{ii:03d}.c"
            path.write_text(src[:off] + "  " + idiom + "\n" + src[off:])
            jobs.append((path, line_no, idiom))
    base = score(Path(a.case), a.func)
    print(f"base {base}; {len(jobs)} variants ({len(expanded)} idioms x {len(boundaries(src, body, end))} places)",
          flush=True)
    report(jobs, a)


def report(jobs, a):
    out = open(a.out, "a") if a.out else None

    def run(job):
        s = score(job[0], a.func)
        if out:
            out.write(f"{s[0] if s else 'ERR'}\t{job[0].name}\t{job[1]}\t{job[2]}\n")
            out.flush()
        return s, job

    with ThreadPoolExecutor(a.j) as ex:
        results = list(ex.map(run, jobs))
    ok = [(s, j) for s, j in results if s]
    ok.sort(key=lambda r: -r[0][0])
    for s, (path, line_no, idiom) in ok[: a.top]:
        print(f"{s[0]}/{s[1]}  after line {line_no}: {idiom}  [{path.name}]")
    print(f"{len(results) - len(ok)} variants failed to compile or timed out")


if __name__ == "__main__":
    main()
