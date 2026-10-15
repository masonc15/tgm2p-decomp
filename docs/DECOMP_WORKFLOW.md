# TGM2+ evidence-driven matching workflow

This guide complements the mandatory project-root [AGENTS.md](../AGENTS.md).
It adopts the useful parts of TH04's producer reconstruction, cold replay, and
negative-result discipline while preserving TGM2+'s existing completion contract.

## Sources of truth

| Question | Authority |
| --- | --- |
| Which bytes are linked as C? | `splits.txt`, `tools/build.py`, validated ROM |
| What is exempt? | `RUNTIME_EXCEPTIONS.md`; exactly 1,638 bytes |
| What remains? | Fresh `build/frontier.json`; all ASM intervals, not selected near-matches |
| Which experiments are parked? | `config/frontier_routes.json` and linked evidence |
| Does it reproduce from source? | A fresh two-run `tools/cold_replay.py` receipt |
| What happened historically? | Focused research receipts, `docs/research/`, then `GOAL.md` |
| What does the user see? | Verified published tailnet dashboard |

The byte metric includes the declared code region's pools/alignment as defined by
the existing split map; it is not an instruction-only percentage. Source discovery
uses heuristic names and groups and is kept separate from byte acceptance.

## Start or resume

Run on Nuada from `/home/colin/workspace/tgm2p-decomp`:

```sh
/drive2/tgm2p/venv/bin/python tools/frontier.py
/drive2/tgm2p/venv/bin/python tools/frontier.py --check
```

Read the selected range's existing notes and scripts before designing another
experiment. Search by ROM address, function name, and compiler mechanism. A
historical note can describe a blocker that later source has already resolved;
reconcile it against today's splits first.

For a canonical baseline, serialize the complete command with the import lock:

```sh
flock /home/colin/services/decomp-dev/import.lock make split check report
```

The shell must have `sh-elf-*` and the established SHC dependencies available.
The Makefile already selects the Nuada Python and objdiff installations. Do not
run `make clean` against production to obtain a cold baseline; the isolated runner
provides that without disrupting the dashboard or other work.

## Route mismatches before editing source

| Observation | Investigate first |
| --- | --- |
| Wrong function start/size or pool displacement | Genuine TU boundaries, predecessor body, parity, pool lifetime |
| Correct operations but scratch registers differ | Narrow types, actual liveness, SHC allocation history/IR |
| Different stack offsets or saves | Calling convention, local address-taking, spills, real inlining |
| Correct diagnostic instructions, incorrect linked bytes | Relocations, literal values, symbol mapping, masked callees |
| Missing branches/tables or suspect function end | Raw CFG, delay slots, shared-tail ownership, indirect dispatch |
| Large structural differences | Reconstruct control flow/ABI before tiny syntax mutations |

Pin SHC v5.0r32 plus its actual executables, `wibo`, converter, source header flags,
and full context. Existing broad version comparisons live outside the project at
`/home/colin/workspace/tgm2p-toolchain-audit-20261005/`; those are historical
measurements, not a substitute for checking the active binaries.

## Bounded two-agent trial

On 2026-10-10 the user authorized two GPT-6.1 Sol subagents, superseding solo
work for this trial. Assign different unresolved routines and unique research
directories, existing negatives and a concrete predicted mechanism. Cap each
assignment at 24 compiler invocations total, counting baselines and captures.
On 2026-10-11 the user raised that cap to 100 for drafting assignments (first or
whole-unit drafts of unresolved ranges with no parked route or prior near-match);
near-miss and parked-route work keeps 24.
No nested agents or edits to canonical source, splits or shared tools. The
primary agent reviews packets and alone integrates with the acceptance gates
below. Review accepted bytes and reproducible actionable discoveries before
starting another trial round. Current assignments are in GOAL.md.

## One experiment packet

Use `research/YYYY-MM-DD-<question>/` or a uniquely named SHC case directory.
Record a small `experiment.json` or equivalent executable receipt containing:

```json
{
  "target": "ROM [start,end)",
  "source_sha256": "hash of the complete genuine context",
  "toolchain": "version, binary hashes, flags, converter",
  "hypothesis": "one predicted cause of the current residual",
  "prediction": "which allocation/layout/byte observation should change",
  "max_compiles": 24,
  "replay_command": ["python", "research/.../probe.py"],
  "result": "pending",
  "raw_differences": null,
  "protected_ranges_preserved": null,
  "next_action": "set after observing results"
}
```

This is a schema example, not a completed receipt. Preserve actual commands and
compiler logs. Prefer a few discriminating controls over multiplying equivalent
source forms. When an experiment fails, record precisely the tested forms it
rules out; do not claim all possible C expressions are impossible.

Stop when the budget expires or the predicted mechanism does not move. Update
the target's route with the negative evidence and a new observable question.
Move to another owner if that question cannot yet be tested. The queue does not
automatically enforce the compile budget; agents must follow it explicitly.

## Acceptance and cold replay

1. Resolve the ordinary compiler object's symbols and compare actual addresses,
   bytes, branch targets, and literal dependencies for the complete candidate.
2. Exercise the existing `c-prefix`/`c-range` proof when applicable. A partial
   extraction must preserve all existing ownership and dependency checks.
3. Stage the source/split change in an isolated copy. Run the appropriate tests
   and `make split check report`. Preserve the previous canonical state.
4. Integrate only after those gates pass. Run two fresh whole-ROM replays:

```sh
/drive2/tgm2p/venv/bin/python tools/cold_replay.py \
  --out research/cold-YYYY-MM-DD-unique-name
```

Use a new directory each time. The runner refuses an existing one. It freezes
source/build scripts, original ROM halves, the compiler directory, runner, and
converter; builds twice in separate trees with no inherited objects; independently
checks both ROM halves and the full program; records tool/package identities;
and rejects input drift. Logs and `receipt.json` survive failure. No compiler,
source, or coverage change is made by the runner.

The initial runner snapshot omitted `rof2elf-expr.diff`; its retained failed
receipt demonstrated that an explicit converter dependency was required. The
corrected snapshot includes that file. A receipt's `passed: true` is required;
the presence of a receipt or run directory alone is not success.

The runner attests observed tools; it is not a hermetic container or an
independent compiler provenance claim. System libraries remain host dependencies.
Existing ordinary-C builds can use mtime-based caching, so `make check` alone
does not establish that all current inputs were freshly compiled.

For changes to build/proof tooling, run:

```sh
flock /home/colin/services/decomp-dev/import.lock \
  env PREFIX_COMPILE_TESTS=1 PERM_SETUP_TESTS=1 /drive2/tgm2p/venv/bin/python \
  -m unittest discover -s tools -p 'test_*.py' -v
```

Both `PREFIX_COMPILE_TESTS=1` and `PERM_SETUP_TESTS=1` are essential on Nuada:
without them, the suite skips real-compiler cases. Report executed tests and
skips separately.

The independent SH-2 differential harness has additional tests under
`tools/difftest/`. Use it for counterexamples and localization; its stubbed calls,
synthetic state, and known sampling misses mean it cannot promote exactness.

## Publish real gains

Current services observed on 2026-10-10:

- `decomp-dev-import.timer` runs canonical validation/import periodically.
- `tgm2p-progress-on-change.path` watches the report and invokes the verified
  publisher through `tgm2p-progress-on-change.service`.
- `tgm2p-progress-refresh.timer` refreshes the rendered dashboard.
- The publisher is `/home/colin/services/tgm2p-progress/publish-on-change.py`.

After an accepted increase, release the canonical lock and invoke the existing
publication service if necessary:

```sh
systemctl --user start tgm2p-progress-on-change.service
systemctl --user status tgm2p-progress-on-change.service --no-pager
```

Check the live dashboard (`/tgm2p/` on nuada's tailnet host) and its data
against the newly verified byte count. Inspect the current service configuration
if it changes; do not invent a new publishing pipeline or manually edit totals.
No percentage increase means no new progress entry is warranted.

## Report and handoff

Give exact before/after bytes, percentage points gained, actual validation, and
the next evidence-backed experiment. Distinguish target observation, compiler
observation, runtime observation, external corroboration, and inference.

The verified baseline on 2026-10-10 was **137,090 / 200,698 = 68.306610%**.
There were **61,970 unresolved non-exempt bytes**, plus **1,638 approved runtime
assembly bytes**. These are dated observations. Future agents must recompute,
not copy this paragraph as current status.

## Accepted compiler-evidence example — 2026-10-10

The bounded Sol trial led to202 accepted rank-code drawing bytes and coverage
137,292/200,698 (68.407259%). [The evidence note](research/2026-10-10-rankcode-drawing-match.md)
shows how authentic SHCMDL address-expression observations guided seven small
parent source controls, followed by exact proof,278 tests,two cold replays and
canonical ROM checks. Correct producer layout did not earn producer coverage.
Preserve shared-pool ownership and distinguish measured source effects from
inferred compiler-internal priority effects. This is a dated accepted example,
not a promise that repeating the same spellings fixes other routines.
