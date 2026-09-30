#!/usr/bin/env python3
"""Run rof2elf_fillff.py with rof2elf-expr.diff applied, without patching it on disk.

asmsh writes a "symbol + constant" pool word as an RPN relocation expression,
which stock rof2elf rejects ("Relocation expression too big"). The diff folds
the constant into the data word the way shc's own objects carry it. Applying
it in memory keeps the toolchain on nuada and in CI untouched and needs no
`patch` binary. Each hunk must match the stock script exactly once.

usage: rof2elf_expr.py <rof2elf_fillff.py> <in.obj> <out.o> [rof2elf args...]
"""
import sys
from pathlib import Path

DIFF = Path(__file__).with_name("rof2elf-expr.diff")


def hunks(diff: str):
    """(old, new) text for each hunk of a unified diff."""
    old = new = None
    for line in diff.splitlines(keepends=True):
        if line.startswith("@@"):
            if old is not None:
                yield "".join(old), "".join(new)
            old, new = [], []
        elif old is None or line.startswith(("---", "+++")):
            continue
        elif line.startswith("-"):
            old.append(line[1:])
        elif line.startswith("+"):
            new.append(line[1:])
        else:  # context; an empty line in the diff is an empty context line
            old.append(line[1:] if line.startswith(" ") else line)
            new.append(line[1:] if line.startswith(" ") else line)
    if old is not None:
        yield "".join(old), "".join(new)


def main():
    script = Path(sys.argv[1])
    src = script.read_text()
    for i, (old, new) in enumerate(hunks(DIFF.read_text()), 1):
        n = src.count(old)
        if n != 1:
            sys.exit(f"rof2elf_expr: hunk {i} matches {n} times in {script}")
        src = src.replace(old, new)
    sys.argv = [str(script), *sys.argv[2:]]
    exec(compile(src, str(script), "exec"), {"__name__": "__main__", "__file__": str(script)})


if __name__ == "__main__":
    main()
