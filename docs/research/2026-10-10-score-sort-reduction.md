# Score allocation reductions and late-variable priority

This investigation adds zero linked-C bytes. The fresh serialized whole-ROM
build and report passed with original EPROM hashes and program SHA-256:
137,090 / 200,698 = 68.306610%. The fixed completion contract is unchanged.

## Reduced compiler examples

Starting from current src/speed_7956.c, four sources were frozen before the
eight-compile budget: full context, the prefix through f1199c, that prefix plus
the rounded quarter-level calculation, and that prefix plus real score
arithmetic. The three reductions deliberately omit game behavior and are
never candidates for integration.

| Source | Context bytes | Graph records | Sorted tie order | Internal assignments for 2 / 65535 |
| --- | ---: | ---: | --- | --- |
| Full | 1,456 | 21 | 2 then 65535 | 3 / 4 |
| Prefix | 324 | 11 | 65535 then 2 | 0 / 0 |
| Prefix plus quarter-level | 364 | 14 | 65535 then 2 | 0 / 0 |
| Prefix plus score | 492 | 19 | 65535 then 2 | 0 / 4 |

Both constants retain priority2 in every source and equal profit within each
source. The full context has profit-7; the reductions have profit-4. The
partition-sort model predicts every observed order and allocation across all
65 records. Internal assignments are allocator IDs, not architectural register
numbers; zero here means not assigned by this allocation stage.

This reproduces the order reversal in small ordinary-compiler examples.
It also establishes why order reversal alone is insufficient: the reductions
change interference and available registers. They do not demonstrate a
source-preserving way to fix the full function.

## Model-only sensitivity

All105 single transpositions of the full graph's nonconstant record order were
checked in Python without changing any compiler memory or output. Fifteen
produce the target constant assignments; four change only those two assignments.
Every one of the four swaps involves parameter p or n and a generated late
temporary. These are sensitivity results, not permission to change the ABI.
No single local-only record transposition in this tested set gives a clean
constant-only fix. Multiple permutations and changed graphs were not tested.

## Ordinary source confirmation

A second preregistered model test applied SHC's known65535 register-storage
priority bonus independently to the existing consumed q, r, d and s variables.
Only r and d predicted the target2/R7 and65535/R6 pair, so only those two
ordinary source controls were compiled. Declaration order and all computations
remain unchanged; there are no artificial consumers or physical bindings.

| Source | Score differences | Context bytes | Following entry | Protected following differences |
| --- | ---: | ---: | --- | ---: |
| Unchanged full baseline | 19 | 1,456 | 0x7956 | 0 |
| register short r | 102 | 1,456 | 0x7956 | 0 |
| register short d | 140 | 1,456 | 0x7958 | 40 |

Both ordinary controls reproduce the original four-byte pair at768e–7692,
but both fail the full strict proof. This instruction match earns zero credit.

Two further native captures and read-only graph replays establish causality
within the observed model: record identities, initial order, interference edges,
and eligibility stay unchanged. Only the selected variable priority increases
from3 to65538 before profit recomputation. The model predicts every resulting
allocation, including unwanted changes to other variables and constants. All
42 records across these two controls are checked.

Finally, eight model-only controls add1 or2 to each of q/r/d/s priorities.
Every allocation remains unchanged. No source compiles were spent on the
corresponding staged-assignment idea, because its prediction failed.

## Evidence and reproduction

- research/2026-10-10-score-sort-reduction: four native contexts, four captures,
  eight normal/debug optimizer replays, four read-only graph replays,
  model-order experiment, verify.py, and baseline validation.
- research/2026-10-10-score-late-priority: two native strict-negative controls,
  two captures, four normal/debug optimizer replays, two graph replays,
  priority-only model predictions, and verify.py.
- research/2026-10-10-score-consumed-updates: eight model-only small-priority
  results; zero source or capture compiles. Its results are reproduced by
  score-late-priority/verify.py.

All six captured assemblies reproduce the corresponding complete ordinary
objects after independent relocation resolution. Every optimizer/graph replay
preserves native REG and ILB. Graph verifiers reconstruct profits, partition
order and every assignment; native verifiers check exact original-address
bytes, roots, source/compiler hashes and protected ranges. The full baseline
retains the exact690-byte speed routine. No proof tools or canonical source
changed, and no full suite or cold replay is claimed for these rejected drafts.

## Next action

Stop the tested register-r/register-d and small-priority staging families.
A further score source attempt needs an observed change in genuine lifetime or
interference structure that reverses2/65535 while preserving the existing
h/constant4 allocation and following function. The next specific compiler
question is which edges and definition/use contributions distinguish those
two ordering outcomes; use the authenticated full and reduced graphs as
controls before inventing another source form. Do not manipulate record order
or compiler priorities to produce an admissible object.
