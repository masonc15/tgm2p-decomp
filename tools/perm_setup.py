#!/usr/bin/env python3
"""Turn a probe case into a decomp-permuter directory (runs on nuada).

The permuter reduces every function except the one being permuted to a bare
declaration. SHC shares literal pools across a run of functions, and at
-optimize=0 its register rotation depends on every earlier statement, so
compile.sh splices the other functions' bodies back in around the candidate:
the ones before it in head.c, the ones after it in tail.c.

target.o is the case compiled as-is with its .text bytes replaced by the ROM
bytes, except for the words the object relocates, so pool addresses compare
the same way probe.py compares them.

usage: perm_setup.py <case> [func] [--dir DIR]
then:  python /drive2/tgm2p/decomp-permuter/permuter.py DIR -j 6 --stop-on-zero
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))
from probe import CASES, HEADER, SHCC  # noqa: E402

FUNC_DEF = re.compile(r"^[A-Za-z_][^;{}]*?\b(\w+)\s*\([^;]*?\)\s*\{", re.M | re.S)
VERSION = "shc-v5.0r32"

COMPILE_SH = """#!/bin/bash
# $1 = candidate .c, $3 = output .o (the permuter calls: compile.sh in.c -o out.o)
D="$(dirname "$(realpath "$0")")"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
python3 "$D/splice.py" "$1" > "$T/in.c"
exec {shcc} {version} "$3" "$T/in.c" {flags}
"""

SPLICE_PY = """import re, sys
from pathlib import Path
d = Path(__file__).parent
src = Path(sys.argv[1]).read_text()
m = re.search({pat!r}, src, re.M | re.S)
sys.stdout.write(src[:m.start()] + (d / "head.c").read_text() + src[m.start():] + (d / "tail.c").read_text())
"""


def functions(text: str):
    """[(name, start, end)] for each top-level function definition."""
    out = []
    for m in FUNC_DEF.finditer(text):
        depth, i = 0, m.end() - 1
        while True:
            c = text[i]
            depth += c == "{"
            depth -= c == "}"
            i += 1
            if depth == 0:
                break
        if not out or m.start() >= out[-1][2]:
            out.append((m[1], m.start(), i))
    return out


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("case")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--dir", type=Path)
    args = ap.parse_args()
    raw_src = (CASES / f"{args.case}.c").read_text()
    hdr = HEADER.search(raw_src)
    # The permuter parses C with pycparser, which needs preprocessed input.
    src = subprocess.run(["cpp", "-P", "-nostdinc", "-undef"], input=raw_src,
                         capture_output=True, text=True, check=True).stdout
    func = args.func or hdr["func"] or args.case
    flags = (hdr["flags"] or "").strip()
    fns = functions(src)
    names = [n for n, _, _ in fns]
    if func not in names:
        sys.exit(f"{func} not in {names}")
    k = names.index(func)
    d = args.dir or Path("/drive2/tgm2p/perm") / func
    d.mkdir(parents=True, exist_ok=True)

    (d / "base.c").write_text(src)
    (d / "head.c").write_text("".join(src[a:b] + "\n\n" for _, a, b in fns[:k]))
    (d / "tail.c").write_text("".join("\n\n" + src[a:b] for _, a, b in fns[k + 1:]) + "\n")
    # Only the permuted function survives as a definition, so splice before it.
    pat = r"^[A-Za-z_][^;{}]*?\b" + func + r"\s*\([^;]*?\)\s*\{"
    (d / "splice.py").write_text(SPLICE_PY.format(pat=pat))
    (d / "compile.sh").write_text(COMPILE_SH.format(shcc=SHCC, version=VERSION, flags=flags))
    (d / "compile.sh").chmod(0o755)
    (d / "settings.toml").write_text(f'func_name = "{func}"\ncompiler_type = "base"\n')

    # Build target.o: compile the case unchanged, then swap in ROM bytes.
    base = d / "base.o"
    subprocess.run([str(SHCC), VERSION, str(base), str(CASES / f"{args.case}.c"), *flags.split()], check=True)
    raw = bytearray(base.read_bytes())
    with base.open("rb") as f:
        e = ELFFile(f)
        text = e.get_section_by_name(".text")
        off, size = text["sh_offset"], text["sh_size"]
        syms = e.get_section_by_name(".symtab")
        hdr_fn = hdr["func"] or args.case
        sym_off = next(s["st_value"] for s in syms.iter_symbols() if s.name == "_" + hdr_fn)
        rom_start = int(hdr["rom"], 16) - sym_off
        rom = bytearray((ROOT / "build" / "prog.bin").read_bytes()[rom_start:rom_start + size])
        for s in e.iter_sections():
            if isinstance(s, RelocationSection) and s.name.endswith(".text"):
                for r in s.iter_relocations():
                    o = r["r_offset"]
                    rom[o:o + 4] = raw[off + o:off + o + 4]
    raw[off:off + size] = rom
    (d / "target.o").write_bytes(raw)
    print(f"{d}: {func} ({len(fns[:k])} before, {len(fns) - k - 1} after), ROM {rom_start:#x}+{size:#x}, flags {flags}")


if __name__ == "__main__":
    main()
