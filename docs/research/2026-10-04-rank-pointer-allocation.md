# Rank-display pointer allocation — continuation 178

Production remains 129,088 / 200,698 bytes (64.3195248582%). This investigation
adds zero verified C bytes. All 48 checkpoint177 production pins and original
ROM hashes were freshly checked. The last full gates remain checkpoint177's
247 enabled tests and independent clean builds; they were not rerun for these
research-only files.

## Native source controls

The input is continuation92's authenticated `best_context.c`, compiled with
unmodified SHC v5.0r32, SH2 big endian, `-macsave=1 -optimize=1 -speed`.
The true context begins at ROM 0x120c0. The target is all 1,236 bytes
0x12ad4–0x12fa8, with 13 contextual function roots. Baseline context length is
4,404 bytes. Its 32 target differences fall in only two regions:

- 0x12bde–0x12c70: 14 bytes, pointer registers p/q reversed.
- 0x12d24–0x12daa: 18 bytes, implicit ordinal/y induction registers reversed.

All 28 native candidates reject under the unchanged strict range verifier.
`RESULTS.json` records independently relocated original object bytes, roots,
source hashes, region counts, and actual baseline byte equality.

| Form | Differing target bytes | Context bytes |
| --- | ---: | ---: |
| Baseline | 32 | 4404 |
| Both pointer increments consumed by call | 48 | 4404 |
| Only p / only q consumed | 65 / 67 | 4404 |
| Both updates written as assignments | 57 | 4404 |
| Both pointer and y updates as assignments | 39 | 4404 |
| Named y / ordinal / both in case1 | 635 / 1100 / 595 | 4420 / 4504 / 4416 |
| Reordered named ordinal/y in case1 | 610 | 4416 |
| Consumed named y / ordinal in case1 | 755 / 1148 | 4408 / 4412 |
| Five arithmetic-order controls | 32 each | 4404 each |
| Derive case0 y from i | 1136 | 4512 |

The consumed-pointer form fixes the second region completely, but introduces
48 differences in the first. The assignment form introduces another 18-byte
register discrepancy in case3. Assigning y explicitly removes that third
region while leaving the two original regions unmatched. This is function-wide
allocation coupling, not evidence of a local semantic error.

## Output-preserving compiler evidence

Three original SHCMDL replays (baseline, consumed pointers, assigned pointers)
produce 128 events each: four allocation tables plus 124 eligibility returns.
Hardware breakpoints only observe execution. Fresh untraced controls, traced
REG/ILB outputs, and native driver intermediates are byte-identical. Native ELF
sections also equal the ordinary compilation, excluding only `.comment`.

The read-only model independently reproduces all 32 candidates' profits,
partition-sort order, and final allocations in each replay. Repeated source
symbol IDs remain separate web IDs; their lifetimes are not collapsed.

| Source form | p priority | q priority | p register | q register | y register |
| --- | ---: | ---: | --- | --- | --- |
| Postincrements in loop update | 13 | 13 | R10 | R12 | R8 |
| Postincrements in call arguments | 7 | 7 | R8 | R10 | R12 |
| Explicit assignments in loop update | 19 | 19 | R12 | R13 | R8 |

The ROM wants p=R12, q=R10, y=R8. At baseline, p, q, and the two anonymous
case1 induction webs all have profit -10. Lowering pointer priorities changes
the partition sort and swaps the two case1 webs into the required registers.
Explicit assignments over-promote the pointers instead.

A hypothetical fixed-graph priority calculation finds seven p/q combinations
that would give all five selected registers their desired homes, including
17/15 and 18/14. These are explanations only: they neither establish a valid
source transformation nor justify editing priorities or physical registers.
The prior named-variable controls demonstrate why real source edits must be
recaptured: they also alter the graph, unrolling, and function boundaries.

## Next action

Trace the native priority accumulation for the same p/q webs in the baseline
and explicit-assignment controls, before profit sorting. Determine why one
real update form contributes six additional priority units, and whether any
required source operation or compiler normalization was missed. Do not repeat
arithmetic commutations, named case1 induction locals, or equivalent pointer
compound updates. Do not add dummy consumers or force physical registers.

Evidence: `tools/shc_probe/cases/session_rank_consumed178/`, including
`verify.py`, `GRAPH_MODEL_RESULTS.json`, three compiler replays, and `FREEZE.json`.
