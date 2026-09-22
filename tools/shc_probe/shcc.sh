#!/bin/bash
# Compile one C file with a given Hitachi SHC version for big-endian SH-2.
# usage: shcc.sh <shc-version-dir> <out.o> <src.c> [extra shc flags...]
# Runs shc.exe under wibo the same way decomp.me does, then converts the
# SYSROF object to ELF with rof2elf.py so sh-elf-objdump/objdiff can read it.
set -euo pipefail
COMPILERS="${SHC_COMPILERS:-/drive2/tgm2p/compilers}"
VER="$1"; OUT="$(realpath -m "$2")"; SRC="$(realpath "$3")"; shift 3
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
cd "$WORK"
cp -r "$COMPILERS/$VER/bin/." .
# SHC wants DOS line endings and an 8.3-ish name.
sed 's/$/\r/' "$SRC" > src.c
SHC_LIB=. SHC_TMP=. "$COMPILERS/wibo" ./shc.exe src.c \
	-comment=nonest -cpu=sh2 -endian=big -sjis -string=const "$@" -object=src.obj \
	> shc.log 2>&1 || { cat shc.log; exit 1; }
# Unwritten gaps (alignment before literal pools) are 0xFF in the EPROM image;
# rof2elf_fillff.py is rof2elf.py with its gap fill changed from 0x00 to 0xFF.
python3 "$COMPILERS/${ROF2ELF:-rof2elf_fillff.py}" src.obj "$OUT" --isa=sh2 > /dev/null
