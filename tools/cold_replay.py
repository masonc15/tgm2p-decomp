#!/usr/bin/env python3
"""Two isolated, cache-free ROM replays with frozen inputs and a JSON receipt.

Run on Nuada: python tools/cold_replay.py --out research/cold-UNIQUE
Never changes production build outputs, splits, coverage, or the dashboard.
"""
import argparse
import datetime
import fcntl
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
ROM_SHA1 = {"2b.u21": "783e8413b11f1fa08d331b09ef4ed63f62b87ead",
            "1b.u22": "3f7e81756470c173cc17a7e7dee91437571fd0c3"}
PROGRAM_SHA256 = "d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7"


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def source_paths(root):
    paths = [root / p for p in ("Makefile", "splits.txt", "symbols.txt", "tail_targets.json",
                                "RUNTIME_EXCEPTIONS.md")]
    for name in ("src", "include", "data"):
        paths += [p for p in (root / name).rglob("*") if p.is_file()]
    # Build and verification code, not gigabytes of historical probe products.
    paths += list((root / "tools").glob("*.py"))
    paths += list((root / "tools").glob("*.sh"))
    paths += [root / "tools/shc_probe/shcc.sh"]
    paths += list((root / "tools/shc_probe/inline_asm").glob("*.py"))
    paths += list((root / "tools/shc_probe/inline_asm").glob("*.diff"))
    paths += [root / "roms" / name for name in ROM_SHA1]
    return sorted(set(paths))


def manifest(paths, root):
    return {str(p.relative_to(root)): {"size": p.stat().st_size, "sha256": digest(p)}
            for p in paths}


def verify_roms(tree):
    for name, expected in ROM_SHA1.items():
        actual = hashlib.sha1((tree / "roms" / name).read_bytes()).hexdigest()
        if actual != expected:
            raise ValueError(f"pinned ROM mismatch: {name}: {actual}")


def coverage(tree):
    rows = []
    for line in (tree / "splits.txt").read_text().splitlines():
        fields = line.split("#", 1)[0].split()
        if fields:
            rows.append((int(fields[0], 0), fields[1]))
    if rows != sorted(rows) or len({a for a, _ in rows}) != len(rows):
        raise ValueError("splits must be ordered and unique")
    cbytes = total = 0
    for (a, kind), (b, _) in zip(rows, rows[1:] + [(0x100000, "end")]):
        if kind in ("asm", "c", "c-prefix", "c-range"):
            total += b - a
        if kind in ("c", "c-prefix", "c-range"):
            cbytes += b - a
    if total != 200698:
        raise ValueError(f"coverage denominator drift: {total}")
    return {"c_bytes": cbytes, "total_bytes": total, "percent": 100 * cbytes / total}


def check_outputs(tree):
    prog = tree / "build/tgm2p.bin"
    if digest(prog) != PROGRAM_SHA256 or prog.read_bytes() != (tree / "build/prog.bin").read_bytes():
        raise ValueError("rebuilt program is not the pinned original")
    for name, expected in ROM_SHA1.items():
        if hashlib.sha1((tree / "build/roms/tgm2p" / name).read_bytes()).hexdigest() != expected:
            raise ValueError(f"rebuilt EPROM mismatch: {name}")
    return {"program_sha256": digest(prog), "coverage": coverage(tree),
            "report_sha256": digest(tree / "build/report.json")}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", required=True, type=Path)
    args = ap.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    receipt = {"schema": 1, "started_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
               "passed": False, "scope": "two cold whole-ROM replays; no new source credit", "runs": []}
    try:
        lock = Path("/home/colin/services/decomp-dev/import.lock")
        with (lock if lock.exists() else out / "snapshot.lock").open("a") as stream:
            fcntl.flock(stream, fcntl.LOCK_EX)
            paths = source_paths(ROOT)
            before = manifest(paths, ROOT)
            frozen = out / "frozen"
            for p in paths:
                target = frozen / p.relative_to(ROOT)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(p, target)
            if before != manifest(source_paths(ROOT), ROOT) or before != manifest(source_paths(frozen), frozen):
                raise ValueError("source drift during snapshot")
        verify_roms(frozen)
        receipt["inputs"] = before
        compilers = Path(os.environ.get("SHC_COMPILERS", "/drive2/tgm2p/compilers"))
        tool_paths = sorted((compilers / "shc-v5.0r32/bin").glob("*"))
        tool_paths += [compilers / "wibo", compilers / "rof2elf_fillff.py"]
        tool_before = manifest(tool_paths, compilers)
        pinned_tools = out / "compilers"
        for p in tool_paths:
            q = pinned_tools / p.relative_to(compilers)
            q.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(p, q)
        if tool_before != manifest(tool_paths, compilers) or tool_before != manifest(
                [pinned_tools / p for p in tool_before], pinned_tools):
            raise ValueError("compiler drift during snapshot")
        receipt["compiler_inputs"] = tool_before
        objdiff = Path("/drive2/tgm2p/bin/objdiff-cli")
        external = [Path(shutil.which(x)) for x in ("sh-elf-as", "sh-elf-ld", "sh-elf-nm", "sh-elf-objcopy", "sh-elf-objdump", "make", "python3")]
        external += [Path(sys.executable), objdiff]
        ext_before = {str(p): digest(p) for p in external}
        receipt["host_tools"] = ext_before
        versions = subprocess.run([sys.executable, "-m", "pip", "freeze"], text=True, capture_output=True, check=True)
        receipt["python_packages"] = versions.stdout.splitlines()
        env = os.environ.copy()
        env.pop("ROF2ELF", None)
        env["SHC_COMPILERS"] = str(pinned_tools)
        receipt["environment"] = {k: env.get(k) for k in ("PATH", "LANG", "LC_ALL", "SHC_COMPILERS")}
        for n in (1, 2):
            tree = out / f"run{n}"
            shutil.copytree(frozen, tree)
            command = ["make", "split", "check", "report", f"PY={sys.executable}", f"OBJDIFF={objdiff}"]
            run = {"command": command, "cwd": str(tree), "cold": True}
            receipt["runs"].append(run)
            print(f"cold replay {n}/2: {tree}", flush=True)
            with (out / f"run{n}.log").open("w") as log:
                result = subprocess.run(command, cwd=tree, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=1800)
            run["returncode"] = result.returncode
            if result.returncode:
                raise ValueError(f"cold replay {n} failed; see run{n}.log")
            run.update(check_outputs(tree))
            if before != manifest(source_paths(tree), tree):
                raise ValueError(f"replay {n} modified its frozen inputs")
        if ext_before != {str(p): digest(p) for p in external}:
            raise ValueError("host tools changed during replay")
        if tool_before != manifest([pinned_tools / p for p in tool_before], pinned_tools):
            raise ValueError("frozen compiler changed during replay")
        if before != manifest(source_paths(ROOT), ROOT):
            raise ValueError("live inputs changed; receipt cannot attest current production")
        receipt["passed"] = True
    except Exception as exc:
        receipt["error"] = f"{type(exc).__name__}: {exc}"
    receipt["finished_utc"] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({k: v for k, v in receipt.items() if k in ("passed", "error", "runs")}, indent=2))
    return 0 if receipt["passed"] else 1


if __name__ == "__main__":
    sys.exit(main())
