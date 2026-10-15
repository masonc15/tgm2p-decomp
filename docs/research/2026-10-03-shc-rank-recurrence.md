# SHC loop recurrence construction — continuation94

The original SHC middle pass creates the two tied rank counters in a known order.
Equivalent addition operand reversals cannot change this order. Explicit source
counters alter the loop transformation itself; they are not interchangeable with
compiler-created recurrences. All seven source controls remain rejected by the
normal contextual verifier. New matching bytes: **0**. Production remains
**127,276 / 200,698 (63.41668%)**.

## Direct compiler evidence

Use the genuine full context from `session_rank_prefix92/best_context.c`, copied
as `session_rank_recurrence94/input.c.txt`. Target f12AD4 has 32 positioned byte
differences and nominal roots12AD4/12FA8. Its preceding nonmatching context is
not credited. Two windows remain: mode0 pointer allocation and mode1 counters.

Hardware breakpoints at40F95C/40F961 surround41C600;40F970/40F975 surround4015D0.
The first three trees contain364 nodes and synthetic identifier−1 only. After
4015D0 the tree has440 nodes and identifiers−1 through−6. Thus the recurrence
creation occurs inside4015D0, after loop-count analysis.

Inside4022D0,40AC50 at40246A creates an identifier, returned at40246F. The original
expression is in EDI and new identifier in EAX. The assignment is in ESI before
413860 at4025D1, and inserted by4025D6. Twenty-two constructor snapshots show:

| Loop | Identifier | Expression | Constructor behavior |
|---|---:|---|---|
| mode3 | −1 | i * 8 | Reuses an existing identifier and inserts assignment |
| mode3 | −2 | i * 22 + 170 | New identifier |
| mode3 | −3 | i + 1 | New identifier |
| mode1 | −4 | g6079474 + i * 8 | New identifier |
| mode1 | −5 | i * 22 + 170 | New identifier |
| mode1 | −6 | i + 1 | New identifier |

Mode1 coordinate−5 and rank−6 both have priority19/profit−10. SHC assigns them
R10/R9; the original game uses R9/R10. Mode3's separate−2/−3 pair is already
correct. Initial expressions are still arithmetic in this tree; later folding
produces initial values170 and1.

The inferred4022D0 listing contains a three-candidate limit controlled by433030.
This is a lead for further inspection, not yet a complete validated model of
candidate selection. Inferred C files are decompiler output, not original SHC
source. The accompanying x86 disassembly records the original instructions.

## Closed source controls

| Control | Positioned differences | Following root | Result |
|---|---:|---|---|
| coordinate addition reversed | 32 |12FA8| Entire compiled context identical to baseline |
| rank addition reversed |32|12FA8| Entire compiled context identical to baseline |
| both reversed |32|12FA8| Entire compiled context identical to baseline |
| equivalent if chain |1108|12FAC| Boundary failure |
| explicit short rank, ordinary update |623|12FC4| Boundary failure |
| explicit short coordinate, ordinary update |654|12FB8| Boundary failure |
| both explicit short counters |595|12FB4| Boundary failure |

Explicit counters are consumed by the existing calls, maintain i++ for the loop
trip count, and use ordinary assignments for their own updates. Over the three
iterations their values equal the original expressions without overflow. No
artificial work, dead statements, physical-register forcing, or padding is used.

The explicit rank attains priority19, but all three mode1 synthetic recurrences
are absent from final allocation records. A separate unchanged hardware replay
locates this difference within the same4015D0 stage:373 nodes before,411 after,
and only mode3 identifiers−1/−2/−3 survive. Therefore the regression precedes
register allocation. The exact eligibility condition remains to be established.

## Reproduction and validation

Case directory: `tools/shc_probe/cases/session_rank_recurrence94`.
`prepare.py` applies documented line edits in `source_edits.json` to the captured
baseline; `analyze.py` verifies exact regeneration, source/context hashes, native
object section equality, diagnostic records, and trace relationships.

Seven native captures match normal compilation. Fourteen normal/debug middle
replays exactly reproduce captured.reg/.ilb;223 priority/profit checks pass.
Three hardware tree replays,30 snapshots and11,887 node observations preserve
original.reg/.ilb byte-for-byte. Their input configuration is unchanged. Tracing
uses hardware debug registers and does not patch compiler instructions or data.
The compiler itself changes common-state fields during execution; those offsets
are recorded separately from input configuration.

`ALL_RESULTS.json`, `ANALYSIS.json`, `TREE_ANALYSIS.json`, and `REGISTERS.json`
aggregate every control. `WATCH_RESULTS.json` and `REPLAY_RESULTS.json` are
last-batch logs only; complete comparisons use the per-source capture/replay
files. Original compiler hash is recorded in each trace receipt.

Fresh read-only production checks verify33 checkpoint84 pins, original ROM
hashes, and fixed-denominator reports in both nuada checkouts. No production
source changed and no full production suite was rerun for rejected experiments.
Checkpoint84 remains the latest full build/test gate (234 enabled tests).

## Next investigation

Compare4015D0 loop eligibility and402240/402290/4022D0 candidate traversal for
baseline versus explicit rank. Explain why adding a real counter suppresses all
three mode1 transformations before attempting another source change. Observe
candidate order and the three-candidate limit directly; do not assume a fixed
register-table sensitivity prediction survives a changed intermediate graph.
Keep the32-difference nominal baseline and the mode0 pointer window independent.
