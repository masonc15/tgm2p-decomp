# SHC sprite-result allocation: F200

Continuation 98 investigates the five-byte F200 mismatch in the genuine 23-function
F200–F5CC context. No production source or proof changed. Checkpoint84 remains
127,276 / 200,698 bytes (63.41668%); 73,422 bytes remain.

## What the trace establishes

The freshly compiled baseline has all 23 function roots at their original ROM
positions. F200–F292 rejects with exactly five differing bytes: the result is
assigned R6 and the store-zero value R5, whereas the ROM uses R5 and R6. The
retained F292–F29A and F4C6–F5CC checks both pass under their existing rules.
The source hash is
`e55a6b158e637e6e5cd2e4b3f17763e123ad25fa7a35906fd32925e788891d56`.

SHCMDL has already made the assignment before SHCGEN scratch selection. Hardware
breakpoints at 405ECC/405ED1 and 414D48/414D4D observe the original sort and
allocator without changing compiler instructions or data. The original .reg and
.ilb outputs are reproduced exactly. The allocator at 406450 visits registers
first, then candidates in score order; this is not a node-first greedy allocator.
Call-argument and parameter handling preassign v0 and a to internal register1
(R4) before general allocation.

The recovered score is priority minus the integer average of conflicting
priorities. Baseline spr has priority 3, but clashes with v0 (priority 65537), v3
(2), the final address (2), and zero (2). Its resulting score is -16382. The zero
has priority 2 and score 0. It receives R5 ahead of spr, which receives R6.

A genuine `register short spr` declaration adds 65535 to spr's priority. This
fixes the final result/zero allocation, but also changes the partition sort's
handling of an earlier tie. x and y both retain priority 2 and score -26215; their
order reverses, leaving y spilled and x in R7. F200 then differs in 36 bytes,
although the retained ranges still pass. This fresh byte count is distinct from
older instruction-count scores.

## Controls and validation

All rows are unmodified ordinary C compiled with the production compiler and
flags. Counts are bytes differing at the original F200–F292 positions; shifted
boundaries remain failures regardless of those counts.

| Source control | Differing bytes | Following F292 root |
| --- | ---: | --- |
| Baseline | 5 | F292 |
| Shared short zero, used in call and stores | 124 | F294 |
| Shared long zero, used in call and stores | 5 | F292 |
| Shared const short zero | 119 | F294 |
| Named destination pointer | 46 | F292 |
| Register sprite result | 36 | F292 |
| Register result, swap x/y initialized declarations | 41 | F292 |
| Register result, split x/y declarations (either order), k initialized earlier | 121 | F290 |
| Register result, all declarations split, original initialization order (either x/y declaration order) | 36 | F292 |

The shared long zero has byte-identical output across the entire context. The
all-declarations controls preserve the original load order. The partial split
controls move k's initialization ahead of x/y and are therefore explicitly not
pure declaration-order tests.

Five sort traces reconstruct 65 candidate positions exactly. Each observed
candidate table still puts y before x, including reversed source load order and
both fully split declaration orders. Ordinary source declaration order does not
supply the candidate-table swap needed here.

A deliberately limited fixed-graph model reproduces 26 physical assignments
across baseline and the register-result control. All 256 combinations of ordinary
register hints on the eight existing locals fail to produce only the desired
final swap while holding that graph and preassignment fixed. A virtual x/y
candidate-table swap would work in this model, but no tested source form causes
it. This is a counterfactual diagnosis, not an exhaustive proof about C sources
and not an executed compiler modification.

Validation receipts in `tools/shc_probe/cases/session_vm_result98`:

- 11 native object comparisons, excluding only .comment;
- 22 exact normal/debug middle-pass replays and 143 independently checked scores;
- 5 hardware sort/allocation replays and65 exact sort positions;
- 148 scratch-selector predictions, all matching observations;
- traced and control generator .sua/.asa outputs are identical to each other;
- generator .sua matches the original cleanup capture, but captured .asa positions
  differ on replay. GEN_REPLAY_RESULTS.json records that limitation;
- 33 production pins plus ROM hashes and progress reports rechecked in both
  canonical and isolated nuada checkouts.

Negative private controls do not justify another full production build. The
latest whole-project validation remains checkpoint 84's 234 enabled tests and
independent clean builds; those tests were not rerun for this investigation.

## Next experiment

Trace candidate construction rather than repeat these source permutations.
The inferred original code at415920 appends candidates to the list headed by
4405CC;405EA0 copies that list in order before sorting. Variable construction
calls415920 at41512F from414E90; constant construction calls it at415B61 from
4159E0. Observe the variable creation and traversal order for baseline,
register-result, and reversed-load controls. That can distinguish the actual
ordering mechanism from symbol/declaration/initialization order before choosing
another genuine expression form. Inferred decompiler output alone is not a
completed observation of those construction calls.

All sources and observations are private research artifacts. No dead consumers,
physical-register forcing, padding, binary patches, verifier changes, or new
matching-byte credit were used. The full 100% goal remains active and incomplete.
