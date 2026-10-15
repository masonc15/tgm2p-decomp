#!/bin/bash
# Like tools/shc_probe/shcc.sh, but compiles with -code=asmcode and assembles
# the output with asmsh (needed for #pragma inline_asm).
# usage: shcc_asm.sh <shc-version-dir> <out.o> <src.c> [extra shc flags...]
set -euo pipefail
TOOLCHAIN="${TGM2P_TOOLCHAIN:-$([ -d /drive2/tgm2p ] && echo /drive2/tgm2p || echo "$HOME/tgm2p")}"
COMPILERS="${SHC_COMPILERS:-$TOOLCHAIN/compilers}"
VER="$1"; OUT="$(realpath -m "$2")"; SRC="$(realpath "$3")"; shift 3
HERE="$(cd "$(dirname "$0")" && pwd)"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
cd "$WORK"
cp -r "$COMPILERS/$VER/bin/." .
sed 's/$/\r/' "$SRC" > src.c
SHC_LIB=. SHC_TMP=. "$COMPILERS/wibo" ./shc.exe src.c \
	-comment=nonest -cpu=sh2 -endian=big -sjis -string=const "$@" -code=asmcode -object=src.src \
	> shc.log 2>&1 || { cat shc.log; exit 1; }
[ -n "${KEEP_SRC:-}" ] && cp src.src "$KEEP_SRC"
SHC_LIB=. SHC_TMP=. "$COMPILERS/wibo" ./asmsh.exe src.src -cpu=sh2 -endian=big -object=src.obj \
	> asm.log 2>&1 || { cat asm.log; exit 1; }
python3 "${ROF2ELF:-$COMPILERS/rof2elf_fillff.py}" src.obj "$OUT" --isa=sh2 > /dev/null
"$TOOLCHAIN/venv/bin/python" "$HERE/elffix.py" "$OUT"
