# Playfield loop priorities — checkpoint 249

The complete 420-byte unit at 1913C–192E0 now matches the original ROM. This
preserves 104 previously retained bytes and adds the complete 316-byte player
update at 191A4. The only executable change from the best genuine candidate is
to spell the two clear-loop increments as `j = j + 1` instead of `j++`.
Both loops use a short counter over 0..2 or 0..3; there is no overflow or change
to the addresses cleared, traversal, calls, or externally visible behavior.

## Measured compiler behavior

A fresh original-compiler baseline reproduced the prior 12 differing bytes.
Read-only hardware breakpoint traces captured the original middle-pass sort
and eligibility decisions. Normal and diagnostic replays reproduce both REG
and ILB files exactly. The independent allocation model reproduces all 18
assignments for each of baseline, ordinary-loop, and index-compound controls.

A fixed-graph counterfactual model identified priority sensitivity, motivating
consumed source-operation experiments rather than another declaration sweep.
Eight controls produced these full-unit results:

| Source form | Bytes | Differences | Existing prefix preserved |
|---|---:|---:|---|
| Baseline | 420 | 12 | Yes |
| Chained byte clear | 420 | 49 | Yes |
| Word-loop postincrement subscript | 452 | 320 | No |
| Character-loop postincrement subscript | 432 | 279 | No |
| Predecrement dispatch index | 420 | 13 | Yes |
| Separate compound dispatch index | 420 | 4 | Yes |
| Separate ordinary dispatch index | 420 | 4 | Yes |
| Compound loop increment | 420 | 12 | Yes |
| Ordinary loop increment | 420 | 0 | Yes |

The successful form changes only the two loop-counter priorities, from 11 to
17 and 14 to 23, in the captured graph. Every other priority and every conflict
edge is unchanged. The actual downstream compiler output is the evidence for
the final physical-register match; the fixed-graph hypothetical solutions are
not presented as a direct prediction of the complete successful source form.
Three fresh Mac objects reproduce independently on Nuada, including the exact
candidate and the 4-byte near-match. All actual roots remain 1913C/1919E/191A4.

## Integration and acceptance

The previous `c-prefix` unit needed context-only 191A4. Now the entire original
unit, including its shared literal pool, is compiled and linked as normal `c`.
No bytes are sliced from the object, no external tail-proof exception is added,
and no build or proof code changes. The full-unit path is the existing standard
build mode and validates the linked ROM and both EPROM images. This removes
the old prefix-extraction concern about the tail call to 10228 without changing
that tail or weakening extraction checks.

Evidence is in `tools/shc_probe/cases/session_playfield_priority249/`:
`verify.py`, `VERIFICATION.json`, allocation traces/models, eight controls,
independent original compiler captures, and clean production gate receipts.
Production validation results are recorded in each checkout's gate and
verification JSON. The fixed denominator remains 200,698 bytes.

The first canonical test attempt ran concurrently with the existing dashboard
publisher's automatic check/report rebuild. Ten errors all arose in
`build.read_symbols()` when `sh-elf-nm` read production objects being rewritten.
The clean canonical build itself passed. The failed test log and gate receipt
are retained as canonical-first-tests.log and canonical-first-gates.json.
After publication completed, the same full suite passed under the publisher's
existing import.lock, with no source or test edits. Future canonical test runs
should acquire that lock to prevent the same build race. The test retry is
explicit in canonical-gates.json and retry_canonical_tests.py.

Both independent clean split/check/report builds and both 266-test suites pass
with zero skips. Original and rebuilt 1 MiB ROMs retain SHA-256
`d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7`.
The authoritative report is 135,910 / 200,698 bytes (67.71866187007345%),
613 / 775 report functions, 206 units, 166 complete. There are 64,788 bytes
remaining, including all 1,638 compiler-runtime bytes. The live tailnet
dashboard and its active watchers are verified in DASHBOARD_VERIFICATION.json.
