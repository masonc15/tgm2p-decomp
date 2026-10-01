#!/usr/bin/env python3
"""stdio MCP server for the TGM2+ Ghidra project, built on ReVa's headless mode.

Runs on nuada; Claude Code on the Mac starts it per session over ssh (see
`claude mcp get ghidra`). It copies $TGM2P_GHIDRA/proj-ram/tgm2p.{gpr,rep}
to a temp dir, opens the copy in a headless Ghidra (2 GB heap) through ReVa's
RevaHeadlessLauncher, and bridges ReVa's loopback HTTP MCP endpoint to stdio.
Working on a copy lets any number of sessions run at once (a Ghidra project
can only be opened by one process) and keeps the reference analysis clean:
renames, comments and structs an agent makes last for its session only.
--in-place opens the real project instead, so edits persist, and fails if
another session has it open.

Run with $TGM2P_GHIDRA/reva-venv/bin/python (ReVa 7.3.1 + pyghidra 3.1.0).
The ReVa extension is installed in the Ghidra install's Ghidra/Extensions.
The process exits when stdin closes (the ssh session ends).
"""
import argparse
import asyncio
import os
import shutil
import signal
import sys
import tempfile
import time
import uuid
from pathlib import Path

TGM2P_GHIDRA = Path(os.environ.get("TGM2P_GHIDRA", "/drive2/tgm2p/ghidra"))
PROJ_DIR = TGM2P_GHIDRA / "proj-ram"
PROJ_NAME = "tgm2p"
INSTALL_DIR = Path(os.environ.get("GHIDRA_INSTALL_DIR", TGM2P_GHIDRA / "ghidra_12.1.4_PUBLIC"))
VMARGS = os.environ.get("TGM2P_GHIDRA_VMARGS", "-Xmx2g -XX:ParallelGCThreads=2 -XX:CICompilerCount=2").split()
# Tool groups left out by default: diffing two programs and writing/running
# Ghidra scripts aren't needed to read the analysis.
DEFAULT_DISABLED = "diff,scripting"


T0 = time.monotonic()


def log(msg: str) -> None:
    print(f"[tgm2p-ghidra {time.monotonic() - T0:5.1f}s] {msg}", file=sys.stderr, flush=True)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--in-place", action="store_true", help="open the real project (edits persist; one session at a time)")
    ap.add_argument("--disable-tool-groups", default=DEFAULT_DISABLED,
                    help=f"comma-separated ReVa tool groups to turn off (default {DEFAULT_DISABLED!r}; '' for all)")
    args = ap.parse_args()

    if not (PROJ_DIR / f"{PROJ_NAME}.gpr").exists():
        sys.exit(f"no project at {PROJ_DIR}; run tools/ghidra/build_project.sh")

    tmp = None
    if args.in_place:
        proj_dir = PROJ_DIR
    else:
        tmp = proj_dir = Path(tempfile.mkdtemp(prefix="tgm2p-ghidra-"))
        shutil.copy2(PROJ_DIR / f"{PROJ_NAME}.gpr", proj_dir)
        shutil.copytree(PROJ_DIR / f"{PROJ_NAME}.rep", proj_dir / f"{PROJ_NAME}.rep",
                        ignore=shutil.ignore_patterns("*.lock*"))
    log(f"project {proj_dir}/{PROJ_NAME}.gpr")

    def interrupt(*_):
        raise KeyboardInterrupt
    for sig in (signal.SIGHUP, signal.SIGTERM):
        signal.signal(sig, interrupt)

    code = 0
    launcher = None
    try:
        from pyghidra.launcher import HeadlessPyGhidraLauncher
        gl = HeadlessPyGhidraLauncher(install_dir=INSTALL_DIR)
        gl.add_vmargs(*VMARGS)
        gl.start()
        log("JVM up")
        from java.io import File
        from reva.headless import RevaHeadlessLauncher
        from reva_cli.stdio_bridge import ReVaStdioBridge

        api_key = f"ReVa-{uuid.uuid4()}"
        launcher = RevaHeadlessLauncher(None, True, True, File(str(proj_dir)), PROJ_NAME, api_key)
        if args.disable_tool_groups:
            launcher.setDisabledToolGroups(args.disable_tool_groups)
        launcher.start()
        if not launcher.waitForServer(60000):
            raise RuntimeError("ReVa server did not start")
        log(f"ReVa ready on 127.0.0.1:{launcher.getPort()}; program path is /prog.bin")
        asyncio.run(ReVaStdioBridge(launcher.getPort(), api_key=api_key).run())
    except KeyboardInterrupt:
        pass
    except BaseException as e:  # noqa: BLE001 - report, then exit hard below
        log(f"error: {e!r}")
        code = 1
    finally:
        try:
            if launcher is not None:
                launcher.stop()
        finally:
            if tmp is not None:
                shutil.rmtree(tmp, ignore_errors=True)
            log("stopped")
            # Jetty/Ghidra threads can keep the JVM alive; don't leave an
            # orphan on a shared box.
            os._exit(code)


if __name__ == "__main__":
    main()
