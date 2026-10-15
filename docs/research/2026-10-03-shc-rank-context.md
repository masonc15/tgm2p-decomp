# Genuine rank context restored through measured allocation

Verified production remains **127,276 / 200,698 bytes (63.41668%)**. The useful
result is a clean, reproducible context with rank f12AD4 at its actual required
address and32 positioned byte differences. No shifted comparison, historical
dead statement, padding or compiler patch is used. The candidate remains
unmatched and receives no coverage credit.

## Coordinate and constant competition

The baseline prefix's synthetic x+2 value has priority8/profit1 and is spilled.
Its assignment at program point106 explicitly computes short x plus2; the
merged WEB's chain contains that point. This establishes its identity without
joining logical numbers across reflection. The constant110 has priority12 and
occupies R10; original code instead keeps x+2 in R8 and materializes110 at calls.

| Coordinate source | Coordinate priority | Coordinate allocation | f12608 bytes |
|---|---:|---|---:|
| Baseline implicit expression | 8 | spill | 1100 |
| Named long or short expression | 10 | spill | 1084 / 1100 |
| Short copy then compound addition | 11 | spill | 1100 |
| Short copy then ordinary assignment | 12 | R8 | 1102 |
| Ordinary assignment plus named color40 | 12 | R8 | **1104** |

The successful context form uses existing consumed values:

```c
short x2;
short color;
/* after the existing early-return and movement logic */
color = 40;
x2 = x;
x2 = x2 + 2;
/* the nine existing short coordinate arguments use x2;
   the eleven existing color40 arguments use color */
```

Every affected coordinate parameter was already short, so the short local
performs the same target conversion as the original argument expressions. The
same input loads and external calls remain. Naming color raises its measured
priority to12. Color, x2 and110 now have profit5, but the original allocator
selects color/R9 and x2/R8 and leaves110 without a retained physical register.
Zero remains R10. This restores the expected roles and the real context sizes.

The resulting native roots are f12608=12608, f12A58=12A58, f12AD4=12AD4 and
f12FA8=12FA8. The target's1,236 bytes have32 **positioned** differences. The
preceding12608–12AD4 context still differs in50 bytes and is unretained; correct
size is not equivalent to full matching. Earlier functions and this prefix gain
no credit. best_context.c is an exact copy of named_color.c for the next step.

## Negative rank controls and prediction limits

A real start-pointer local feeding p leaves p/q priorities13 and13, but introduces
an additional WEB and makes the target four bytes too long (next root12FAC).
It has896 positioned differences and fails the boundary proof.

A separate sensitivity calculation on the previous fixed candidate table showed
that raising the relevant i priority by3,6 or12 would sort p before q. Rewriting
only that loop's existing i++ as i=i+1 did **not** preserve the intermediate
graph: p/q priorities became5/5 and the next root moved to12F60. It has1,108
positioned differences. The source experiment is rejected; the fixed-graph
calculation is retained with its explicit failed applicability condition.
Do not report that the sort model predicted this source's output.

## Validation and next action

Seven source controls regenerate exactly: five new native compiles and the two
retained short/long inputs from91. Seven fresh native captures match sections
outside compiler comments; fourteen normal/debug middle replays preserve.reg
and.ilb exactly. Five hardware replays of unmodified SHCMDL observe prefix
function125 before/after profit sorting and physical allocation. All162 complete
sort positions and priority-minus-average-conflict calculations agree, with
unchanged.reg/.ilb. No runtime code/data or compiler configuration is patched.

The next work starts from best_context.c at correct nominal addresses. Repair
the two remaining rank allocation windows while preserving their intermediate
loop shape, and validate any source hypothesis against fresh priorities before
predicting physical allocation. Do not repeat the seven controls, restore the
eleven historical dead statements, or inflate completion with the context bytes.

Evidence: tools/shc_probe/cases/session_rank_prefix92. prepare.py reproduces all
inputs; analyze.py checks native hashes, default rejections, actual roots,
coordinate identity, complete hardware sort order and diagnostic assignments.
COUNTER_PREDICTION.json preserves the conditional model and failed source result.
Per-source captures/replays are the complete evidence; batch summary files cover
their final invocation. HARDWARE_RESULTS.json covers all five hardware replays.

Fresh production checks in both checkouts preserve all33 checkpoint84 pins,
original ROM hashes and unchanged reports. The234-test full-suite result remains
historical checkpoint84 validation; no production change was made this round.
The fixed200,698-byte100% goal remains active, with73,422 bytes remaining.
Continue solo without brainstorming or subagents.
