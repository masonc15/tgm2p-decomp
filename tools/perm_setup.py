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

base.c wraps the function body in PERM_RANDOMIZE and its leading local
declarations in PERM_LINESWAP (see wrap_perm_macros).

usage: perm_setup.py <case> [func] [--dir DIR] [--cases CASEDIR] [--no-lineswap] [--lineswap-max N]
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
INLINE_ASM = re.compile(r"^[ \t]*#[ \t]*pragma[ \t]+inline_asm[ \t]*\(([^)]*)\)[^\n]*$", re.M)

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


LOCALS = re.compile(r"^(\s+)((?:(?:unsigned|signed|register|const|volatile|struct|union)\s+)*\w+)\s+"
                    r"(\**\s*\w+(?:\s*,\s*\**\s*\w+)+)\s*;\s*$", re.M)


def split_locals(src: str) -> str:
    """Give each local its own declaration. pycparser shares one type node
    between the names in `short x, y;`, and the permuter's randomizer then
    fails every candidate with "nodes should only appear once in AST". Any
    indented line is split, struct members included, which changes nothing
    the compiler emits."""
    def one(m):
        names = [n.strip() for n in m[3].split(",")]
        return "\n".join(f"{m[1]}{m[2]} {n};" for n in names)
    return LOCALS.sub(one, src)


TYPE_WORDS = ("static", "register", "const", "volatile", "unsigned", "signed", "short", "long",
              "int", "char", "void", "float", "double", "struct", "union", "enum")
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")


def _top_split(text: str, sep: str):
    """Split on sep outside (), [] and {}."""
    out, depth, cur = [], 0, ""
    for c in text:
        depth += c in "([{"
        depth -= c in ")]}"
        if c == sep and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += c
    return out + [cur]


def _decl_parts(line: str):
    """(names declared, identifiers read by the initializers) for one declaration line."""
    text = line.strip().rstrip(";")
    while re.search(r"\{[^{}]*\}", text):  # struct/union bodies
        text = re.sub(r"\{[^{}]*\}", " ", text)
    names, reads = set(), set()
    for d in _top_split(text, ","):
        lhs, *init = _top_split(d, "=")
        lhs = re.sub(r"\[[^\]]*\]", "", lhs)
        while re.search(r"\([^()]*\)\s*$", lhs):  # (*fp)(args)
            lhs = re.sub(r"\([^()]*\)\s*$", "", lhs)
        ids = IDENT.findall(lhs.replace("(", " ").replace(")", " "))
        if ids:
            names.add(ids[-1])
        reads.update(IDENT.findall("=".join(init)))
    return names, reads


def wrap_perm_macros(src: str, a: int, b: int, max_run: int = 0) -> str:
    """Wrap the body of the function at src[a:b] in PERM_RANDOMIZE, with each run
    of two or more swappable leading declarations in a PERM_LINESWAP.

    Any PERM macro that has more than one expansion turns the permuter's random
    mode off unless PERM_RANDOMIZE is present, so it is always added. Seed 0 of
    PERM_LINESWAP is the original order, so the base source and score are
    unchanged. A declaration whose initializer reads another local, or whose
    name another initializer reads, stays where it is; reordering it would only
    produce candidates that fail to compile. max_run > 0 leaves runs longer than
    that unwrapped: each fresh candidate starts from a uniformly random order, so
    with n declarations the original order is only 1/n! of the starts."""
    m = FUNC_DEF.match(src, a)
    open_, close = m.end(), b - 1
    body = src[open_:close]
    literals = re.findall(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', body)
    if src[close] != "}" or any(c in s for s in literals for c in "()"):
        print("perm_setup: body has parens inside literals; no PERM macros", file=sys.stderr)
        return src
    flat, n = src, 1
    while n:  # drop struct bodies so their members don't look like typedef names
        flat, n = re.subn(r"\{[^{}]*\}", " ", flat)
    typedefs = set(re.findall(r"\btypedef\b[^;]*?(\w+)\s*;", flat))
    first = re.compile(r"^\s+(?:" + "|".join(TYPE_WORDS + tuple(sorted(typedefs))) + r")\b")
    lines = body.split("\n")
    decls = []  # indices into lines
    for i, line in enumerate(lines[1:], 1):
        if not line.strip():
            continue
        s = line.rstrip()
        balanced = all(s.count(o) == s.count(c) for o, c in ("()", "[]", "{}"))
        if not (first.match(line) and s.endswith(";") and balanced):
            break
        decls.append(i)
    parts = {i: _decl_parts(lines[i]) for i in decls}
    local = set().union(*(n for n, _ in parts.values())) if parts else set()
    free = []
    for i in decls:
        names, reads = parts[i]
        others_read = set().union(*(r for j, (_, r) in parts.items() if j != i))
        free.append(not (reads & (local - names)) and not (names & others_read))
    runs, cur = [], []
    for i, ok in zip(decls, free):
        if ok and (not cur or i == cur[-1] + 1):
            cur.append(i)
        else:
            runs.append(cur)
            cur = [i] if ok else []
    runs.append(cur)
    runs = [r for r in runs if len(r) >= 2 and (max_run <= 0 or len(r) <= max_run)]
    for r in reversed(runs):
        lines[r[0]:r[-1] + 1] = ["PERM_LINESWAP("] + lines[r[0]:r[-1] + 1] + [")"]
    swapped = sum(len(r) for r in runs)
    print(f"perm_setup: {len(decls)} declarations, {swapped} in {len(runs)} PERM_LINESWAP block(s)")
    return src[:open_] + "\nPERM_RANDOMIZE(" + "\n".join(lines) + "\n)\n" + src[close:]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("case")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--dir", type=Path)
    ap.add_argument("--cases", type=Path, default=CASES, help="directory holding the case")
    ap.add_argument("--no-lineswap", action="store_true",
                    help="leave base.c without PERM_RANDOMIZE/PERM_LINESWAP")
    ap.add_argument("--lineswap-max", type=int, default=0, metavar="N",
                    help="only PERM_LINESWAP runs of at most N declarations (0 = any length)")
    args = ap.parse_args()
    case_src = args.cases / f"{args.case}.c"
    raw_src = case_src.read_text()
    hdr = HEADER.search(raw_src)
    # The permuter parses C with pycparser, which needs preprocessed input.
    src = subprocess.run(["cpp", "-P", "-nostdinc", "-undef"], input=raw_src,
                         capture_output=True, text=True, check=True).stdout
    src = split_locals(src)
    func = args.func or hdr["func"] or args.case
    flags = (hdr["flags"] or "").strip()
    fns = functions(src)
    names = [n for n, _, _ in fns]
    if func not in names:
        sys.exit(f"{func} not in {names}")
    k = names.index(func)
    d = args.dir or Path("/drive2/tgm2p/perm") / func
    d.mkdir(parents=True, exist_ok=True)

    # pycparser can't read #pragma inline_asm bodies (SH assembly), so base.c
    # keeps only a parser-only prototype for those functions; head.c/tail.c carry the pragma
    # and the body, which compile.sh splices back in, and shcc.sh then sees the
    # pragma and builds through asmsh as it does for the case itself.
    asm = set()
    for m in INLINE_ASM.finditer(src):
        asm.update(re.findall(r"\w+", m[1]))
    if func in asm:
        sys.exit(f"{func} is a #pragma inline_asm function")

    def whole(name, a, b):
        return (f"#pragma inline_asm({name})\n" if name in asm else "") + src[a:b]

    edits = []  # (start, end, replacement) in src
    for name, a, b in fns:
        if name in asm:
            # PERM_PRETEND: the parser sees the prototype, the compiler doesn't.
            # (The permuter drops "static" from function prototypes, and a
            # non-static one makes SHC emit the helper as a real function.)
            proto = src[a:FUNC_DEF.match(src, a).end() - 1].rstrip() + ";"
            edits.append((a, b, f"PERM_PRETEND({proto})"))
    _, fa, fb = fns[k]
    if not args.no_lineswap:
        wrapped = wrap_perm_macros(src, fa, fb, args.lineswap_max)
        edits.append((fa, fb, wrapped[fa:len(wrapped) - (len(src) - fb)]))
    base_c = src
    for a, b, rep in sorted(edits, reverse=True):
        base_c = base_c[:a] + rep + base_c[b:]
    base_c = INLINE_ASM.sub("", base_c)
    (d / "base.c").write_text(base_c)
    (d / "head.c").write_text("".join(whole(n, a, b) + "\n\n" for n, a, b in fns[:k]))
    (d / "tail.c").write_text("".join("\n\n" + whole(n, a, b) for n, a, b in fns[k + 1:]) + "\n")
    # Only the permuted function survives as a definition, so splice before it.
    pat = r"^[A-Za-z_][^;{}]*?\b" + func + r"\s*\([^;]*?\)\s*\{"
    (d / "splice.py").write_text(SPLICE_PY.format(pat=pat))
    (d / "compile.sh").write_text(COMPILE_SH.format(shcc=SHCC, version=VERSION, flags=flags))
    (d / "compile.sh").chmod(0o755)
    (d / "settings.toml").write_text(f'func_name = "{func}"\ncompiler_type = "base"\n')

    # Build target.o: compile the case unchanged, then swap in ROM bytes.
    base = d / "base.o"
    subprocess.run([str(SHCC), VERSION, str(base), str(case_src), *flags.split()], check=True)
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
