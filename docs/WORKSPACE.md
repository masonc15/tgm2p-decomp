# Workspace and hosts

Work happens on **phrixus** (an Apple silicon Mac mini) by default. **nuada**
(Linux) is the fallback, and it also hosts the progress dashboard, the backup
copy of the experiment evidence, the private decomp.me and the x86-64
instrumentation. Both hosts use the same layout, so commands are identical on
either one. phrixus replaced nephele, the Mac that held this role until
2026-10-15, and uses the same macOS toolchain layout.

| | phrixus (primary) | nuada (fallback) |
|---|---|---|
| Checkout | `~/workspace/tgm2p-decomp` | `~/workspace/tgm2p-decomp` |
| Toolchain | `~/tgm2p` | `/drive2/tgm2p` |
| ROMs | `~/tgm2p/roms` | `/drive2/tgm2p/roms` |
| Extra | Ghidra + ReVa, MAME replay | Ghidra + ReVa, private decomp.me, dashboard services, archives |

The git remote is `https://github.com/masonc15/tgm2p-decomp` (public). The
phrixus checkout also has a `nuada` remote, which can fetch unpushed commits.

## Starting a session

```sh
cd ~/workspace/tgm2p-decomp
. tools/env.sh            # puts this host's toolchain on PATH, sets $PY
git pull --ff-only
tools/sync_evidence.sh pull   # on phrixus, if anyone worked on nuada since
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
- `bin/`: `objdiff-cli` v3.8.1; on phrixus also the SH binutils and two small
  macOS adapters, `realpath` (accepts GNU `-m`) and an `sh-elf-objcopy`
  wrapper that can write to `/dev/stdout`.
- `venv/`: Python 3.12.9 with the same pinned packages on both hosts
  (capstone 5.0.9, pyelftools 0.33, m2c, and so on; `pip freeze` is
  identical).
- `decomp-permuter/` (with `tools/permuter-sh2.patch` applied) and `perm/`
  for permuter runs.
- `mame/tgm2p/`: the MAME ROM set `tools/replay_check.sh` runs candidate
  images against.

On phrixus `wibo` is the macOS build (`wibo-macos`, sha256 `2b3000ef...cf7c`,
an x86-64 binary that runs under Rosetta), the binutils are a 2.45 cross build
in `~/tgm2p/cross`, and the SHC executables are byte-identical to nuada's.
`realpath` hands off to Homebrew coreutils, MAME is Homebrew's, and
`replay_check.sh` needs Homebrew's bash (it uses `wait -n`), which comes first
on PATH. A clean checkout on either host rebuilds the same ROM and
`build/report.json`, passes the full test suite with
`PREFIX_COMPILE_TESTS=1 PERM_SETUP_TESTS=1 SHIFTABLE_TESTS=1`, and passes
`tools/cold_replay.py`.

## Evidence

`research/` and the experiment folders under `tools/shc_probe/cases/` are
gitignored (they hold ROM-derived binaries, compiler output, and several GB of
logs) but many docs link into them, so keep them in place and add new packets
next to them. `tools/sync_evidence.sh push|pull [host]` copies them between
hosts without ever deleting anything. Run it on phrixus only (nuada has no ssh
access to phrixus): a launchd job (`com.colin.tgm2p-evidence-sync`, log in
`~/Library/Logs/tgm2p/`) pushes to nuada every hour, so nuada always has a
backup and can take over, and after any work on nuada,
`tools/sync_evidence.sh pull` on phrixus brings it back. A few old scratch
files in `cases/agent_b` and `cases/agent_e` differ from a sibling only by
letter case; phrixus's case-insensitive disk keeps one of each pair, and nuada
keeps both.

Archived material lives on nuada in `/drive2/tgm2p/history/`, each folder with
its own tarball: unreferenced scratch evidence and the old `/drive2` scratch
directories (2026-10-15), stale project copies and audits, nephele's earlier
Codex workspace and toolchain, and older history.

## Ghidra

Both hosts have Ghidra 12.1.4 with the ReVa 7.3.1 extension and a copy of the
analysed projects (`proj-ram` is the one `tools/ghidra/reva_mcp.py` serves).
They live in the toolchain root's `ghidra/` (the server finds it by the same
rule as `tools/toolchain.py`), so on phrixus the server is
`~/tgm2p/ghidra/reva-venv/bin/python tools/ghidra/reva_mcp.py`. Edits made
with `--in-place` stay on the host that made them.

## Publishing progress

Commit verified changes with a short imperative lowercase message and push to
`origin`. On nuada, `decomp-dev-import.timer` pulls `main` every 15 minutes,
runs `make check report` under the import lock, and the progress services
publish any verified increase to the dashboard. Nothing needs to be copied by
hand, and dashboard numbers are never edited by hand.

Some things only work on nuada: `tools/scratch.py` (private decomp.me; from
phrixus, forward it with `ssh -fNL 28080:127.0.0.1:28080 nuada`),
`tools/nprobe.sh`, the dashboard services in `~/services/`, and native compiler
instrumentation. The `capture_driver.c` programs in research packets set x86
hardware breakpoints through Linux `ptrace`, so push evidence, run those
packets on nuada over ssh, and pull the results back.
