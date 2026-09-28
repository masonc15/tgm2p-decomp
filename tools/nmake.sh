#!/bin/bash
# Sync the sources to nuada and run make there. Args pass through to make.
set -euo pipefail
cd "$(dirname "$0")/.."
# The agent_* and tmp* case dirs are synced by whoever works in them and may
# hold files that exist only on nuada, so --delete must leave them alone.
rsync -a --delete --exclude __pycache__ --exclude 'shc_probe/cases/agent_*/' \
	--exclude 'shc_probe/cases/tmp*/' tools/ nuada:~/workspace/tgm2p-decomp/tools/
rsync -a --delete src/ nuada:~/workspace/tgm2p-decomp/src/
rsync -a Makefile splits.txt symbols.txt README.md nuada:~/workspace/tgm2p-decomp/
args=""
[ $# -gt 0 ] && args=$(printf '%q ' "$@")
ssh -o BatchMode=yes nuada "cd ~/workspace/tgm2p-decomp && make $args"
