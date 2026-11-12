#!/usr/bin/env python3
"""Check that SHC gives the same code on another host (Wine, native Windows).

`make OUTDIR [extra.c ...]` copies the pinned compiler into OUTDIR with every
src/ unit and wip/ draft as uNNN.c (CRLF) and writes run.sh (wibo or Wine:
RUN=wine ./run.sh) and run.cmd (native Windows). Each runner compiles every
unit in that directory, as shcc.sh does (cwd = compiler directory, SHC_LIB=.
SHC_TMP=.), and leaves uNNN.obj beside the source. Files that need the asmsh
path (#pragma inline_asm) are left out.

`compare NAME=DIR NAME=DIR ...` converts each host's objects with
rof2elf_fillff.py and compares .text, relocations and symbols with the first
directory. The objects themselves differ in a header timestamp.

2026-11-12: wibo 1.2.0 on macOS and native Windows 11 give identical code for
all 204 units (docs/research/2026-11-12-first-principles-review.md).
"""
import hashlib, shutil, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import toolchain  # noqa: E402
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))
from flag_sweep import header_flags  # noqa: E402

VERSION = "shc-v5.0r32"
BASE = "-comment=nonest -cpu=sh2 -endian=big -sjis -string=const"


def make(out: Path, extra):
    if out.exists():
        shutil.rmtree(out)
    shutil.copytree(toolchain.COMPILERS / VERSION / "bin", out)
    srcs = sorted((ROOT / "src").glob("*.c")) + sorted((ROOT / "tools/shc_probe/cases/wip").glob("*.c"))
    jobs = []
    for src in srcs + [Path(e).resolve() for e in extra]:
        text = src.read_text(errors="surrogateescape")
        if "inline_asm" in text:
            continue
        uid = f"u{len(jobs):03d}"
        data = text.replace("\r\n", "\n").replace("\n", "\r\n").encode(errors="surrogateescape")
        (out / f"{uid}.c").write_bytes(data)
        rel = str(src.relative_to(ROOT)) if src.is_relative_to(ROOT) else str(src)
        jobs.append((uid, rel, header_flags(src)))
    (out / "jobs.tsv").write_text("".join(f"{u}\t{r}\t{f}\n" for u, r, f in jobs))
    cmd = ["@echo off", "set SHC_LIB=.", "set SHC_TMP=."]
    sh = ["#!/bin/sh", 'cd "$(dirname "$0")"', "export SHC_LIB=. SHC_TMP=.", 'RUN="${RUN:-wine}"']
    for u, _, f in jobs:
        cmd.append(f"shc.exe {u}.c {BASE} {f} -object={u}.obj > {u}.log 2>&1")
        sh.append(f"$RUN ./shc.exe {u}.c {BASE} {f} -object={u}.obj > {u}.log 2>&1")
    (out / "run.cmd").write_bytes(("\r\n".join(cmd) + "\r\n").encode())
    (out / "run.sh").write_text("\n".join(sh) + "\n")
    (out / "run.sh").chmod(0o755)
    print(f"{len(jobs)} units in {out}")


def fingerprint(obj: Path):
    if not obj.exists() or obj.stat().st_size == 0:
        return None
    with tempfile.TemporaryDirectory() as td:
        elf, raw = Path(td) / "o.o", Path(td) / "t.bin"
        r = subprocess.run([sys.executable, str(toolchain.COMPILERS / "rof2elf_fillff.py"), str(obj), str(elf),
                            "--isa=sh2"], capture_output=True)
        if r.returncode or not elf.exists():
            return "rof2elf failed"
        subprocess.run(["sh-elf-objcopy", "-O", "binary", "-j", ".text", str(elf), str(raw)], check=True,
                       capture_output=True)
        text = raw.read_bytes() if raw.exists() else b""
        rel = subprocess.run(["sh-elf-objdump", "-r", str(elf)], capture_output=True, text=True).stdout
        rel = "\n".join(l for l in rel.splitlines() if str(elf) not in l)
        nm = subprocess.run(["sh-elf-nm", str(elf)], capture_output=True, text=True).stdout
    return hashlib.sha256(text + rel.encode() + nm.encode()).hexdigest()


def compare(hosts):
    (ref_name, ref_dir), others = hosts[0], hosts[1:]
    jobs = [l.split("\t") for l in (ref_dir / "jobs.tsv").read_text().splitlines()]
    tot = {n: [0, 0, 0] for n, _ in others}
    for uid, rel, _ in jobs:
        r = fingerprint(ref_dir / f"{uid}.obj")
        for n, d in others:
            h = fingerprint(d / f"{uid}.obj")
            if h is None or r is None:
                tot[n][2] += 1
                print(f"MISSING {n} {uid} {rel}")
            elif h == r:
                tot[n][0] += 1
            else:
                tot[n][1] += 1
                print(f"DIFF    {n} {uid} {rel}")
    for n, (same, diff, miss) in tot.items():
        print(f"{n} vs {ref_name}: {same} identical, {diff} different, {miss} missing")
    return all(v[1] == 0 and v[2] == 0 for v in tot.values())


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == "make":
        make(Path(sys.argv[2]), sys.argv[3:])
    elif len(sys.argv) >= 4 and sys.argv[1] == "compare":
        ok = compare([(a.split("=", 1)[0], Path(a.split("=", 1)[1])) for a in sys.argv[2:]])
        sys.exit(0 if ok else 1)
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
