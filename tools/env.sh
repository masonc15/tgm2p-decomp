# Source this at the start of a session: . tools/env.sh
# Puts this host's TGM2+ toolchain on PATH and exports the variables the
# Makefile and tools read. The root is $TGM2P_TOOLCHAIN if already set, else
# /drive2/tgm2p (nuada) or ~/tgm2p (nephele). See tools/toolchain.py.
if [ -z "${TGM2P_TOOLCHAIN:-}" ]; then
    if [ -d /drive2/tgm2p ]; then TGM2P_TOOLCHAIN=/drive2/tgm2p; else TGM2P_TOOLCHAIN="$HOME/tgm2p"; fi
fi
export TGM2P_TOOLCHAIN
export SHC_COMPILERS="${SHC_COMPILERS:-$TGM2P_TOOLCHAIN/compilers}"
export PY="$TGM2P_TOOLCHAIN/venv/bin/python"
export OBJDIFF="$TGM2P_TOOLCHAIN/bin/objdiff-cli"
case ":$PATH:" in
    *":$TGM2P_TOOLCHAIN/bin:"*) ;;
    *) export PATH="$TGM2P_TOOLCHAIN/bin:$PATH" ;;
esac
