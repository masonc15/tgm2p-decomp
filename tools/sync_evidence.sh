#!/bin/bash
# Copy the gitignored experiment evidence (research/ and the case folders in
# tools/shc_probe/cases/) between this checkout and another host's.
# usage: tools/sync_evidence.sh push|pull [host]    (host defaults to nuada)
# Nothing is ever deleted; a file is only added or replaced by a newer copy.
# Tracked files (cases/wip/ and the top-level cases/*.c) are left to git.
set -euo pipefail
cd "$(dirname "$0")/.."
direction=${1:?usage: tools/sync_evidence.sh push|pull [host]}
host=${2:-nuada}
remote="$host:workspace/tgm2p-decomp"
opts=(-a --update --exclude=__pycache__)
case_opts=(--exclude=/wip/ --exclude=/eeprom_2fdd8.c --exclude='/opt_*.c')
for path in research/ tools/shc_probe/cases/; do
    extra=(); [ "$path" = tools/shc_probe/cases/ ] && extra=("${case_opts[@]}")
    case $direction in
        push) rsync "${opts[@]}" ${extra[@]+"${extra[@]}"} "$path" "$remote/$path" ;;
        pull) rsync "${opts[@]}" ${extra[@]+"${extra[@]}"} "$remote/$path" "$path" ;;
        *) echo "usage: tools/sync_evidence.sh push|pull [host]" >&2; exit 2 ;;
    esac
done
