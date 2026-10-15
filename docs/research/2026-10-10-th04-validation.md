# TH04 workflow adoption: validation receipt index

Observed on Nuada, 2026-10-10. No game-source or split-map changes were made.
Coverage before and after: **137,090 / 200,698 = 68.30660993133962%**.
New matched game bytes: **0**.

## Executed verification

- Canonical `make split check report`: passed, preserving the interleaved
  program and both original EPROM SHA-1s.
- New replay controls: 3 tests passed, including independent program/EPROM
  mutation rejection and denominator-drift rejection.
- Initial default suite: 164 executed tests passed, 112 opted-out tests skipped.
- `PREFIX_COMPILE_TESTS=1` suite: 269 executed tests passed; seven permuter tests
  remained opted out.
- `PERM_SETUP_TESTS=1` targeted permuter suite: all seven passed, zero skips.
  Together the latter two runs executed all 276 discovered tests successfully.
- `tools/frontier.py` followed by `--check`: passed. The inventory partitions
  the map into 137,090 C bytes, 61,970 unresolved bytes, and 1,638 exempt bytes.
- Two isolated cold builds: both passed. Both reports have SHA-256
  `f12d0d9e23a64e0076e02f25e7f657e808c292585641ee37cd31f24a08975f6b`;
  both complete programs have SHA-256
  `d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7`.

## Durable evidence

Paths below are relative to the Nuada project:

- `research/2026-10-10-th04-cold-replay-v2/receipt.json`: source/tool manifests,
  exact commands, two build verdicts, and coverage. `run1.log` and `run2.log`
  preserve output, with independent trees retained alongside them.
- `research/2026-10-10-th04-cold-replay/receipt.json`: retained first failure,
  caused by the snapshot omitting the converter's `rof2elf-expr.diff` input.
  The snapshot was corrected and both new cold runs succeeded.
- `research/2026-10-10-th04-workflow-tests.log`: initial opt-out suite.
- `research/2026-10-10-th04-workflow-native-tests.log`: 269 executed tests.
- `research/2026-10-10-th04-workflow-perm-tests.log`: remaining seven tests.
- `build/frontier.json`: generated diagnostic queue, regenerable from live splits.

The local workspace retains the successful cold receipt, test logs, generated
queue, and SHA-256 synchronization manifest under `artifacts/` and the local
project's matching research path. Documentation links were checked locally, and
deployed source/documentation files were compared against Nuada using SHA-256.

The operational test command in `docs/DECOMP_WORKFLOW.md` now enables both opt-in
variables. No new dashboard progress entry was created because coverage did
not increase. Existing watchers/timers were observed active; this was not a
new dashboard deployment or a browser rendering test.
