"""Locate this host's TGM2+ toolchain (compilers, venv, objdiff-cli, permuter).

The root is $TGM2P_TOOLCHAIN when set, else /drive2/tgm2p (nuada) or ~/tgm2p
(nephele), whichever exists. Every host uses the same layout under it:
bin/ (objdiff-cli, sh-elf-* on macOS), compilers/, venv/, decomp-permuter/,
perm/ and roms/. tools/env.sh exports the same settings for shells.
"""
import os
from pathlib import Path


def _root() -> Path:
    if os.environ.get("TGM2P_TOOLCHAIN"):
        return Path(os.environ["TGM2P_TOOLCHAIN"])
    for candidate in (Path("/drive2/tgm2p"), Path.home() / "tgm2p"):
        if candidate.is_dir():
            return candidate
    return Path("/drive2/tgm2p")


ROOT = _root()
COMPILERS = Path(os.environ.get("SHC_COMPILERS", ROOT / "compilers"))
OBJDIFF = Path(os.environ.get("OBJDIFF", ROOT / "bin" / "objdiff-cli"))
PERMUTER = ROOT / "decomp-permuter"
PERM = ROOT / "perm"
