#!/usr/bin/env python3
"""Compiler-identification harness.

Each probe case is tools/shc_probe/cases/<name>.c with a header comment:

    /* rom: 0x2f852 len: 0x88 func: f_2f852 flags: -optimize=0 */

`rom`/`len` give the function's extent in build/prog.bin (code plus its
literal pool), `func` the C function to compare (default: the file stem),
and `flags` optional per-case SHC flags that replace the default grid.

The case is compiled with every requested SHC version x flag set and compared
halfword by halfword against the ROM. The only bytes excused are:
  - the 4 bytes of every R_SH_DIR32 relocation in the compiled object
    (literal-pool words holding addresses of external symbols), and
  - the 12-bit displacement of `bsr`/`bra` instructions whose target lies
    outside the function (calls and tail calls into other functions of the
    same translation unit).
Everything else, including pool constants, pool placement and intra-function
branches, has to match exactly. A length difference counts as mismatches.

usage: probe.py [--versions v5.0r10,...] [--flags "..."]... [--cases a,b] [-j N] [-v]
"""
import argparse
import concurrent.futures as cf
import itertools
import re
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[2]
CASES = ROOT / "tools" / "shc_probe" / "cases"
SHCC = ROOT / "tools" / "shc_probe" / "shcc.sh"
VERSIONS = ["v5.0r10", "v5.0r26", "v5.0r28", "v5.0r31", "v5.0r32",
            "v5.1r01", "v5.1r03", "v5.1r04", "v5.1r08", "v5.1r11", "v5.1r13"]
FLAG_SETS = [
    "-macsave=1 -optimize=0",
    "-macsave=1 -optimize=1",
    "-macsave=1 -optimize=1 -speed",
    "-macsave=1 -optimize=1 -size",
]
SPAN = True  # compare from the named function to the end of .text (multi-function cases)
HEADER = re.compile(r"rom:\s*0x(?P<rom>[0-9a-fA-F]+)"
                    r"(?:\s+len:\s*0x(?P<len>[0-9a-fA-F]+))?"
                    r"(?:\s+func:\s*(?P<func>\w+))?"
                    r"(?:\s+flags:\s*(?P<flags>[^*\n]+?))?\s*\*/")


def load_function(obj: Path, name: str, span: bool = False):
    """Return (bytes, masked byte offsets, external bsr/bra offsets, [(func, start, end)]).

    Offsets are relative to the named function; the function list covers every
    function from the named one onward, so multi-function cases can be scored
    per function."""
    with obj.open("rb") as f:
        elf = ELFFile(f)
        text_idx = next(i for i, s in enumerate(elf.iter_sections()) if s.name == ".text")
        data = elf.get_section(text_idx).data()
        syms = elf.get_section_by_name(".symtab")
        funcs = sorted((s["st_value"], s.name) for s in syms.iter_symbols()
                       if s["st_shndx"] == text_idx and s["st_info"]["type"] == "STT_FUNC")
        starts = [a for a, _ in funcs]
        try:
            start = next(a for a, n in funcs if n == "_" + name)
        except StopIteration:
            sys.exit(f"{obj}: no function _{name} (have {[n for _, n in funcs]})")
        later = [a for a in starts if a > start]
        end = len(data) if span else (min(later) if later else len(data))
        masked = set()
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] == text_idx:
                for r in sec.iter_relocations():
                    masked.update(range(r["r_offset"], r["r_offset"] + 4))
    body = data[start:end]
    masked = {o - start for o in masked if start <= o < end}
    members = [a for a, _ in funcs if start <= a < end]
    ranges = []
    for i, (a, n) in enumerate(f for f in funcs if start <= f[0] < end):
        b = members[i + 1] if i + 1 < len(members) else end
        ranges.append((n.lstrip("_"), a - start, b - start))
    ext_bsr = set()
    for off in range(0, len(body) - 1, 2):
        w = body[off] << 8 | body[off + 1]
        if w >> 12 in (0xA, 0xB):  # bra / bsr
            disp = w & 0xFFF
            disp -= 0x1000 if disp & 0x800 else 0
            target = off + 4 + disp * 2
            if not 0 <= target < len(body):
                ext_bsr.add(off)
    return body, masked, ext_bsr, ranges


def score(rom: bytes, got: bytes, masked: set, ext_bsr: set, lo: int = 0, hi: int = None):
    # The ROM region named by the case header is what must be reproduced;
    # compiled bytes past it (later functions kept for pool placement) are ignored.
    hi = len(rom) if hi is None else min(hi, len(rom))
    n = max(0, hi - lo) // 2
    hits = 0
    first_bad = None
    for i in range(lo // 2, lo // 2 + n):
        o = 2 * i
        a, b = rom[o:o + 2], got[o:o + 2]
        if len(a) == 2 and len(b) == 2:
            if a == b or (o in masked and o + 1 in masked):
                hits += 1
                continue
            if o in ext_bsr and a[0] >> 4 == b[0] >> 4:
                hits += 1
                continue
        if first_bad is None:
            first_bad = o
    return hits, n, first_bad


def run_one(src: Path, ver: str, flags: str, rom_bytes: bytes, func: str):
    with tempfile.TemporaryDirectory() as td:
        obj = Path(td) / "out.o"
        r = subprocess.run([str(SHCC), f"shc-{ver}", str(obj), str(src), *flags.split()],
                           capture_output=True, text=True)
        if r.returncode:
            return None, r.stdout + r.stderr
        body, masked, ext_bsr, ranges = load_function(obj, func, span=len(rom_bytes) > 0 and SPAN)
        per = []
        for name, a, b in ranges:
            if a >= len(rom_bytes):
                break
            h, n, _ = score(rom_bytes, body, masked, ext_bsr, a, b)
            per.append((name, a, min(b, len(rom_bytes)), h, n, b <= len(rom_bytes)))
        return (score(rom_bytes, body, masked, ext_bsr), per), ""


def show_diff(src: Path, ver: str, flags: str, rom_bytes: bytes, func: str) -> None:
    import capstone as cs
    md = cs.Cs(cs.CS_ARCH_SH, cs.CS_MODE_SH2 | cs.CS_MODE_BIG_ENDIAN)
    md.skipdata = True
    with tempfile.TemporaryDirectory() as td:
        obj = Path(td) / "out.o"
        r = subprocess.run([str(SHCC), f"shc-{ver}", str(obj), str(src), *flags.split()],
                           capture_output=True, text=True)
        if r.returncode:
            print(r.stdout + r.stderr)
            return
        body, masked, ext_bsr, _ = load_function(obj, func, span=SPAN)

    def lines(b):
        out = []
        for o in range(0, len(b) - 1, 2):
            ins = next(md.disasm(b[o:o + 2], o), None)
            txt = f"{ins.mnemonic} {ins.op_str}" if ins else ".word"
            out.append(f"{b[o]:02x}{b[o+1]:02x} {txt}")
        return out
    a, b = lines(rom_bytes), lines(body)
    print(f"{'ROM':38s} | compiled ({ver} {flags})")
    for i in range(len(a)):
        o = 2 * i
        x = a[i] if i < len(a) else ""
        y = b[i] if i < len(b) else ""
        same = x[:4] == y[:4] or (o in masked and o + 1 in masked) or (o in ext_bsr and x[:1] == y[:1])
        print(f"{o:4x} {x:33s} {' ' if same else '*'}| {y}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--diff", metavar="VER", help="show a side-by-side listing for one version (use with --cases and --flags)")
    ap.add_argument("--rom", type=Path, default=ROOT / "build" / "prog.bin")
    ap.add_argument("--versions", default=",".join(VERSIONS))
    ap.add_argument("--flags", action="append", help="flag set (repeatable); overrides per-case flags")
    ap.add_argument("--cases", default="", help="comma-separated case names; default all")
    ap.add_argument("-j", "--jobs", type=int, default=10)
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    rom = args.rom.read_bytes()
    versions = args.versions.split(",")
    cases = sorted(CASES.glob("*.c"))
    if args.cases:
        wanted = set(args.cases.split(","))
        cases = [c for c in cases if c.stem in wanted]

    jobs = {}
    with cf.ThreadPoolExecutor(args.jobs) as pool:
        for src in cases:
            m = HEADER.search(src.read_text())
            if not m:
                sys.exit(f"{src}: missing 'rom: 0x... len: 0x...' header")
            addr = int(m["rom"], 16)
            if not m["len"]:
                sys.exit(f"{src}: header needs len:")
            rom_bytes = rom[addr:addr + int(m["len"], 16)]
            func = m["func"] or src.stem
            flag_sets = args.flags or ([m["flags"].strip()] if m["flags"] else FLAG_SETS)
            if args.diff:
                show_diff(src, args.diff, flag_sets[0], rom_bytes, func)
                continue
            for ver, flags in itertools.product(versions, flag_sets):
                jobs[(src.stem, ver, flags)] = pool.submit(run_one, src, ver, flags, rom_bytes, func)

    results = {k: f.result() for k, f in jobs.items()}
    print(f"{'case':12s} {'version':8s} {'flags':32s} match")
    best = {}
    per_best = {}
    for (case, ver, flags), (res, err) in sorted(results.items()):
        s = per = None
        if res is not None:
            s, per = res
        if s is None:
            print(f"{case:12s} {ver:8s} {flags:32s} FAIL")
            if args.verbose:
                print("   " + "\n   ".join(err.strip().splitlines()[-6:]))
            continue
        hits, n, bad = s
        pct = 100.0 * hits / n
        where = "" if bad is None else f"  first diff +0x{bad:x}"
        print(f"{case:12s} {ver:8s} {flags:32s} {hits}/{n} ({pct:.1f}%){where}")
        best.setdefault(case, []).append((pct, ver, flags))
        if case not in per_best or pct > per_best[case][0]:
            per_best[case] = (pct, ver, flags, per)
    print("\nbest per case:")
    for case, lst in sorted(best.items()):
        lst.sort(key=lambda t: -t[0])
        top = lst[0][0]
        winners = sorted({f"{v}" for p, v, f in lst if p == top})
        flags = sorted({f for p, v, f in lst if p == top})
        print(f"  {case}: {top:.1f}%  versions={','.join(winners)}  flags={' | '.join(flags)}")
        _, ver, fl, per = per_best[case]
        base = int(HEADER.search((CASES / f"{case}.c").read_text())["rom"], 16)
        for name, a, b, h, n, whole in per:
            tag = "" if whole else "  (extends past region; partial)"
            print(f"      {base + a:06x} {name:28s} {h}/{n} ({100.0 * h / max(1, n):.1f}%){tag}")


if __name__ == "__main__":
    main()
