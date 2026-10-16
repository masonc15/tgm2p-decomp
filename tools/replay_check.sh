#!/bin/bash
# Run a program image in MAME and compare it with a reference image, frame for
# frame: an attract-mode run with no input and a scripted gameplay replay,
# each hashing the rendered screen at regular checkpoints (tools/replay/replay.lua).
#
# usage: tools/replay_check.sh [options] CANDIDATE.bin [REFERENCE.bin]
#   --attract N    attract-mode frames            (default 12000)
#   --gameplay N   scripted gameplay frames       (default 24000)
#   --every N      checkpoint interval in frames  (default 100)
#   --seed N       gameplay input seed            (default 1)
#   --reads        also log data-region reads (REPLAY_READS) for both images;
#                  implies --nodrc
#   --nodrc        run MAME's SH-2 interpreter instead of the recompiler
#   --out DIR      work directory                 (default build/replay)
#
# REFERENCE defaults to build/prog.bin, the original program image. Images are
# 1 MB big-endian program images; they are split into 2b.u21/1b.u22 and run
# with the rest of the MAME ROM set from $TGM2P_MAME_ROMS (default
# $TGM2P_TOOLCHAIN/mame/tgm2p). Each run starts from the ROM set's default
# EEPROM in a fresh nvram directory. Exit 0 only when every checkpoint of both
# scenarios matches.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TOOLCHAIN="${TGM2P_TOOLCHAIN:-$([ -d /drive2/tgm2p ] && echo /drive2/tgm2p || echo "$HOME/tgm2p")}"
MAME_ROMS="${TGM2P_MAME_ROMS:-$TOOLCHAIN/mame/tgm2p}"
MAME="${MAME:-$(command -v mame || echo /usr/games/mame)}"
PY="${PY:-$TOOLCHAIN/venv/bin/python}"

attract=12000 gameplay=24000 every=100 seed=1 reads=0 nodrc=0 out="$ROOT/build/replay"
args=()
while [ $# -gt 0 ]; do
	case "$1" in
		--attract) attract=$2; shift 2 ;;
		--gameplay) gameplay=$2; shift 2 ;;
		--every) every=$2; shift 2 ;;
		--seed) seed=$2; shift 2 ;;
		--reads) reads=1; nodrc=1; shift ;;
		--nodrc) nodrc=1; shift ;;
		--out) out=$2; shift 2 ;;
		-h|--help) sed -n 2,21p "$0"; exit 0 ;;
		*) args+=("$1"); shift ;;
	esac
done
[ ${#args[@]} -ge 1 ] || { sed -n 2,21p "$0"; exit 2; }
cand="$(realpath "${args[0]}")"
ref="$(realpath "${args[1]:-$ROOT/build/prog.bin}")"
[ -f "$MAME_ROMS/1b.u22" ] || { echo "replay_check: no MAME ROM set in $MAME_ROMS" >&2; exit 2; }

run() {  # run NAME IMAGE MODE FRAMES
	local name=$1 image=$2 mode=$3 frames=$4
	local dir="$out/$name"
	rm -rf "$dir"
	mkdir -p "$dir/roms/tgm2p" "$dir/nv" "$dir/cfg"
	for f in "$MAME_ROMS"/*; do
		case "$(basename "$f")" in 2b.u21|1b.u22) ;; *) ln -s "$f" "$dir/roms/tgm2p/" ;; esac
	done
	"$PY" "$ROOT/tools/interleave.py" split "$image" -o "$dir/roms/tgm2p" > /dev/null
	local readenv=() drc=()
	[ "$reads" = 1 ] && readenv=(REPLAY_READS="$dir/reads.txt")
	[ "$nodrc" = 1 ] && drc=(-nodrc)
	(cd "$dir" && env REPLAY_MODE="$mode" REPLAY_FRAMES="$frames" REPLAY_EVERY="$every" \
		REPLAY_SEED="$seed" REPLAY_SNAPS="${REPLAY_SNAPS:-}" REPLAY_READ_RANGES="${REPLAY_READ_RANGES:-}" REPLAY_READS_FROM="${REPLAY_READS_FROM:-0}" REPLAY_READS_SKIP_BOOT="${REPLAY_READS_SKIP_BOOT:-0}" REPLAY_READS_UNTIL="${REPLAY_READS_UNTIL:-}" REPLAY_READS_PC="${REPLAY_READS_PC:-0}" REPLAY_OUT="$dir/hashes.txt" "${readenv[@]}" \
		timeout 3600 "$MAME" tgm2p "${drc[@]}" -rompath "$dir/roms" \
		-autoboot_script "$ROOT/tools/replay/replay.lua" -autoboot_delay 0 \
		-video none -sound none -nothrottle -skip_gameinfo \
		-nvram_directory "$dir/nv" -cfg_directory "$dir/cfg" -snapshot_directory "$dir/snap" \
		> "$dir/mame.log" 2>&1) || { echo "replay_check: MAME failed for $name (see $dir/mame.log)" >&2; return 1; }
	local n
	n=$(wc -l < "$dir/hashes.txt")
	[ "$n" -eq $((frames / every)) ] || { echo "replay_check: $name stopped after $n checkpoints" >&2; return 1; }
}

status=0
echo "reference $(sha1sum "$ref" | cut -c1-40)  $ref"
echo "candidate $(sha1sum "$cand" | cut -c1-40)  $cand"
for scenario in attract:$attract gameplay:$gameplay; do
	mode=${scenario%%:*} frames=${scenario#*:}
	[ "$frames" -gt 0 ] || continue
	run "ref-$mode" "$ref" "$mode" "$frames" &
	run "cand-$mode" "$cand" "$mode" "$frames" &
	wait -n || status=1
	wait -n || status=1
	[ $status = 0 ] || exit 1
	r="$out/ref-$mode/hashes.txt" c="$out/cand-$mode/hashes.txt"
	total=$(wc -l < "$r")
	distinct=$(cut -d' ' -f2 "$r" | sort -u | wc -l)
	if cmp -s "$r" "$c"; then
		echo "$mode: $frames frames, $total checkpoints every $every frames ($distinct distinct screens): identical"
		echo "  final checkpoint: $(tail -1 "$r")"
	else
		first=$(diff <(cat "$r") <(cat "$c") | grep -m1 '^<' | cut -c3- || true)
		echo "$mode: MISMATCH; first differing checkpoint: ${first:-?}"
		status=1
	fi
done
exit $status
