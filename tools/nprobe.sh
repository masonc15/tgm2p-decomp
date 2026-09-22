#!/bin/bash
# Sync tools/ to nuada and run the SHC probe there. Args pass through to probe.py.
set -euo pipefail
cd "$(dirname "$0")/.."
rsync -a --delete tools/ nuada:~/workspace/tgm2p-decomp/tools/
args=""
[ $# -gt 0 ] && args=$(printf '%q ' "$@")
ssh -o BatchMode=yes nuada "cd ~/workspace/tgm2p-decomp && /drive2/tgm2p/venv/bin/python tools/shc_probe/probe.py $args"
