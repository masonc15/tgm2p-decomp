#!/bin/bash
# Run one coverage scenario: MAME streams trace windows into a FIFO that
# tracecov merges into build/cov/all.{exec,call}.bin.
# usage: tools/run_cov.sh tools/scenarios/attract.lua
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SCENARIO="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
NAME="$(basename "$1" .lua)"
B="$ROOT/build"
mkdir -p "$B/cov" "$B/mame/$NAME"
[ -x "$B/tracecov" ] || /usr/bin/clang -O2 -Wall -o "$B/tracecov" "$ROOT/tools/tracecov.c"

FIFO="$B/mame/$NAME/trace.fifo"
rm -f "$FIFO"
mkfifo "$FIFO"
# tracecov reopens the FIFO for every trace window; MAME closes it on "trace off".
( while [ -p "$FIFO" ]; do "$B/tracecov" "$B/cov/all" < "$FIFO" || true; done ) &
READER=$!

cd "$B/mame/$NAME"
COV_SCENARIO="$SCENARIO" COV_FIFO="$FIFO" \
	mame tgm2p -rompath "$(dirname "$(readlink "$ROOT/roms")")" \
	-debug -debugger none -autoboot_script "$ROOT/tools/mame_cov.lua" -autoboot_delay 0 \
	-video none -sound none -nothrottle -skip_gameinfo \
	-nvram_directory ./nv -cfg_directory ./cfg -snapshot_directory ./snap -snapname '%i' \
	2>&1 | rg -v -- '-video none'
rm -f "$FIFO"
# unblock a reader waiting on open()
kill "$READER" 2>/dev/null || true
wait "$READER" 2>/dev/null || true
"$B/tracecov" -d "$B/cov/all"
