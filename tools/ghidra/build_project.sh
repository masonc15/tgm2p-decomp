#!/bin/bash
# Build the Ghidra project the ghidra MCP server (tools/ghidra/reva_mcp.py)
# serves. Runs on nuada: writes $TGM2P_GHIDRA/proj-ram/tgm2p.gpr, program
# "prog.bin", with ROM 0x780-0x313fc mapped at RAM 0x06000000 and functions
# named f_<ROM offset>. Rebuilds from scratch (-overwrite), with a 2 GB heap
# and two analysis threads; takes a few minutes.
#
# usage: tools/ghidra/build_project.sh
set -euo pipefail
cd "$(dirname "$0")/../.."
TGM2P_GHIDRA=${TGM2P_GHIDRA:-/drive2/tgm2p/ghidra}
GHIDRA_INSTALL_DIR=${GHIDRA_INSTALL_DIR:-$TGM2P_GHIDRA/ghidra_12.1.4_PUBLIC}
PY=${PY:-/drive2/tgm2p/venv/bin/python}  # needs capstone, for tools/fn.py
OUT=$TGM2P_GHIDRA/proj-ram
mkdir -p "$OUT"
"$PY" tools/ghidra/ram_seeds.py -o "$OUT/seeds_ram.txt"
GHIDRA_HEADLESS_MAXMEM=${GHIDRA_HEADLESS_MAXMEM:-2G} \
"$GHIDRA_INSTALL_DIR/support/analyzeHeadless" "$OUT" tgm2p \
	-import build/prog.bin -overwrite -max-cpu 2 \
	-processor SuperH:BE:32:SH-2 -loader BinaryLoader -loader-baseAddr 0x0 \
	-scriptPath "$PWD/tools/ghidra" \
	-preScript SetupMemory.java \
	-preScript MapRamCode.java \
	-preScript SeedFunctions.java "$OUT/seeds_ram.txt" \
	-postScript NameFunctions.java "$PWD/symbols.txt" \
	2>&1 | tee "$OUT/headless.log" | grep -E 'REPORT|SeedFunctions|MapRamCode|NameFunctions|ERROR' || true
grep -q 'REPORT: Save succeeded' "$OUT/headless.log" || { echo "analysis failed, see $OUT/headless.log" >&2; exit 1; }
