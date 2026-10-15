# Workspace and hosts

Work happens on **nephele** (an Apple silicon Mac) by default. **nuada**
(Linux) is the fallback when nephele is unavailable, and it also hosts the
progress dashboard and the backup copy of the experiment evidence. Both hosts
use the same layout, so commands are identical on either one.

| | nephele (primary) | nuada (fallback) |
|---|---|---|
| Checkout | `~/workspace/tgm2p-decomp` | `~/workspace/tgm2p-decomp` |
| Toolchain | `~/tgm2p` | `/drive2/tgm2p` |
| ROMs | `~/tgm2p/roms` | `/drive2/tgm2p/roms` |
| Extra | | Ghidra + ReVa, private decomp.me, dashboard services, archives |

The git remote is `https://github.com/masonc15/tgm2p-decomp` (public). Each
checkout also has a remote for the other host (`nuada` on nephele), which can
fetch unpushed commits.

## Starting a session

```sh
cd ~/workspace/tgm2p-decomp
. tools/env.sh            # puts this host's toolchain on PATH, sets $PY
git pull --ff-only
tools/sync_evidence.sh pull   # only if you are on nuada after work on nephele
make split check report   # rebuilds the ROM and checks both EPROM SHA-1s
make status               # coverage, git state, and the largest open ranges
```

`make status` lists the unresolved ranges with their recorded hypotheses,
evidence paths, and next questions (from `config/frontier_routes.json` and
`build/frontier.json`). Read that evidence before designing an experiment.
`GOAL.md` is the chronological log of every round; `docs/DECOMP_WORKFLOW.md`
is the method and acceptance procedure.

## Toolchain

`tools/toolchain.py` and `tools/env.sh` find the toolchain root:
`$TGM2P_TOOLCHAIN` if set, else `/drive2/tgm2p`, else `~/tgm2p`. The Makefile
uses the same rule, so `make` needs no setup. Under the root:

- `compilers/`: the decomp.me SHC packages (`shc-v5.0r32` is the one that
  builds the game), `wibo`, and `rof2elf_fillff.py`.
- `bin/`: `objdiff-cli` v3.8.1; on nephele also the SH binutils and two small
  macOS adapters, `realpath` (accepts GNU `-m`) and an `sh-elf-objcopy`
  wrapper that can write to `/dev/stdout`.
- `venv/`: Python with the same pinned packages on both hosts (capstone 5.0.9,
  pyelftools 0.33, m2c, and so on; compare with `pip freeze`).
- `decomp-permuter/` (with `tools/permuter-sh2.patch` applied) and `perm/`
  for permuter runs.

On nephele `wibo` is the macOS build (`wibo-macos`, sha256 `2b3000ef...cf7c`),
the binutils are a 2.45 cross build in `~/tgm2p/cross`, and the SHC
executables are byte-identical to nuada's. A clean checkout on either host
rebuilds the same ROM, passes the full test suite with
`PREFIX_COMPILE_TESTS=1 PERM_SETUP_TESTS=1`, and passes `tools/cold_replay.py`.

## Evidence

`research/` and the experiment folders under `tools/shc_probe/cases/` are
gitignored (they hold ROM-derived binaries, compiler output, and several GB of
logs) but many docs link into them, so keep them in place and add new packets
next to them. `tools/sync_evidence.sh push|pull [host]` copies them between
hosts without ever deleting anything. On nephele a launchd job
(`com.colin.tgm2p-evidence-sync`) pushes to nuada every hour, so nuada always
has a backup and can take over.

Archived material lives on nuada in `/drive2/tgm2p/history/`, each folder with
its own tarball: unreferenced scratch evidence and the old `/drive2` scratch
directories (2026-10-15), stale project copies and audits, nephele's earlier
Codex workspace and toolchain, and older history.

## Publishing progress

Commit verified changes with a short imperative lowercase message and push to
`origin`. On nuada, `decomp-dev-import.timer` pulls `main` every 15 minutes,
runs `make check report` under the import lock, and the progress services
publish any verified increase to the dashboard. Nothing needs to be copied by
hand, and dashboard numbers are never edited by hand.

Some things only work on nuada: `tools/ghidra/` (Ghidra 12.1.4 and the ReVa
MCP server), `tools/scratch.py` (private decomp.me), `tools/nprobe.sh`, the
dashboard services in `~/services/`, and native compiler instrumentation. The
`capture_driver.c` programs in research packets set x86 hardware breakpoints
through Linux `ptrace`, so push evidence, run those packets on nuada over ssh,
and pull the results back.
