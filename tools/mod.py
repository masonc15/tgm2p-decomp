#!/usr/bin/env python3
"""Build a changed game: the program linked from source, as a MAME ROM set.

    tools/mod.py build [--mod DIR] [--out build/mod]
    tools/mod.py check [--out build/mod-check]

`build` links every C source as it compiles now (tools/shiftable.py build
--source --nonmatching): the matching C of src/, the functionally equivalent C
of src/nonmatching/ for the ranges that are still unresolved (the units of
splits_nonmatching.txt), and the files of the mod directory. A C file in the
mod directory replaces the file of the same path in the tree; a file with no counterpart is new code, and may define
variables. Sources edited in place are picked up the same way. Code may grow:
every address is a symbol, and the image keeps its 1 MB by giving up filler.

The result is build/mod/tgm2p.bin, the two program ROM halves in
build/mod/roms/tgm2p/ next to links to the other ROMs, and
build/mod/functions.txt, which says where each function ended up and which
file to edit to change it.

`check` builds the tree with no mod directory and runs the recorded replays
against the original ROM (tools/replay_check.sh --pin-idle). With unedited
sources that is the proof that the source-linked program plays the same; after
an edit that should not change behaviour, it tests that.

None of this touches the matching build, splits.txt or the coverage figures.
"""
from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import shiftable  # noqa: E402
import toolchain  # noqa: E402

PROGRAM = ("2b.u21", "1b.u22")
# The frame wait counts idle loops into the random seed, and the boot copy and
# checksum take longer with more code. Padding the code to a whole number of
# these steps past the original size puts the game back in the original's
# phase, which the unpinned replay needs (docs/research/2026-10-16-shiftable-build.md).
PHASE_STEP = 1024


def romset(image: Path, out: Path) -> Path:
    """out/roms/tgm2p: the image as the two program ROMs, the rest linked."""
    src = toolchain.ROOT / "mame" / "tgm2p"
    dst = out / "roms" / "tgm2p"
    dst.mkdir(parents=True, exist_ok=True)
    for f in dst.iterdir():
        f.unlink()
    for f in sorted(src.iterdir()):
        if f.name not in PROGRAM:
            (dst / f.name).symlink_to(f.resolve())
    subprocess.run([sys.executable, str(ROOT / "tools" / "interleave.py"), "split", str(image), "-o", str(dst)],
                   check=True, stdout=subprocess.DEVNULL)
    return dst


def report(layout, out: Path):
    sec = layout["sections"]
    text, data = sec[".text"], sec[".data"]
    end = data[0] + data[2]
    grown = text[2] - shiftable.MAP.text_size
    lines = [f"{name:<24}0x{rom:06x}  0x{vma:08x}  {src}" for name, rom, vma, src in layout["functions"]]
    (out / "functions.txt").write_text(
        "# function, original ROM offset, address in this build, source that defines it\n" + "\n".join(lines) + "\n")
    bss = sec.get(".bss", [0, 0, 0])[2]
    print(f"code {text[2]:#x} bytes ({grown:+d} against the original), "
          f"{shiftable.MAP.bss_lo - end:#x} bytes of RAM left for code and initialized data, "
          f"new variables {bss:#x} bytes")
    print(f"{len(layout['functions'])} functions -> {out / 'functions.txt'}")
    for src, v in layout["numbers"]:
        print(f"note: {src}: the constant {v} could be an address but is not one the game uses, so it stays "
              "a number (for an address, name it: g_/d_/f_<address>)")


def build(mod, out: Path, text_size=None):
    data, layout = shiftable.build_shiftable([], out, nonmatching=True, text_size=text_size, source=True, mod=mod)
    if len(data) != shiftable.ROM_SIZE:
        sys.exit(f"image is {len(data):#x} bytes, not {shiftable.ROM_SIZE:#x}")
    return layout


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    b = sub.add_parser("build")
    b.add_argument("--mod", type=Path, help="directory of replacement and new C files")
    b.add_argument("--out", type=Path, default=ROOT / "build" / "mod")
    c = sub.add_parser("check")
    c.add_argument("--out", type=Path, default=ROOT / "build" / "mod-check")
    args = ap.parse_args()
    out = args.out.resolve()
    if args.cmd == "build":
        layout = build(args.mod, out)
        report(layout, out)
        roms = romset(out / "tgm2p.bin", out)
        print(f"image {out / 'tgm2p.bin'} sha1 {layout['sha1']}")
        print(f"run it: mame tgm2p -rompath {roms.parent}")
        return
    # check: pad the code to the original's phase, then replay against the ROM
    layout = build(None, out)
    need = layout["sections"][".text"][2] - shiftable.MAP.text_size
    size = shiftable.MAP.text_size + max(0, -(-need // PHASE_STEP)) * PHASE_STEP
    layout = build(None, out, size)
    report(layout, out)
    env = dict(os.environ)
    env["REPLAY_MARKS"] = subprocess.run(
        [sys.executable, str(ROOT / "tools" / "idle_report.py"), "marks", str(out)],
        capture_output=True, text=True, check=True).stdout.strip()
    r = subprocess.run([str(ROOT / "tools" / "replay_check.sh"), "--pin-idle", "--out", str(out / "replay"),
                        str(out / "tgm2p.bin")], env=env)
    if r.returncode:
        sys.exit("the source-linked build does not play like the original ROM")
    subprocess.run([sys.executable, str(ROOT / "tools" / "idle_report.py"), "report", str(out),
                    str(out / "replay")], env=env)
    print("the source-linked build plays the recorded replays like the original ROM")


if __name__ == "__main__":
    main()
