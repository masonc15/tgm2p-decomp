# SHC expression ordering and surviving copies — continuation96

Staging expressions changes the recurrence candidate order as predicted, but
keeps extra named-value copies that change register interference. None of ten
new controls improves the nominal32-difference baseline. Production remains
**127,276 / 200,698 (63.41668%)**, with **0 new matching bytes**.

## Candidate ordering

Original402F20 prepends a new candidate to its induction-variable list. The
instruction sequence at402F58 stores the prior head in the new descriptor, and
402F6D installs that descriptor as head. This agrees with the observed candidate
orders after collecting all statements. A named expression written before the
call is pushed behind expressions collected from the subsequent call.

Hardware observations at40233D identify actual mode1 expressions, not merely
synthetic numbers. The first three are selected and the fourth hits the cap:

| Source | Order |
|---|---|
| Rank named before call (case95) | pointer, coordinate, wait, rank |
| Coordinate named before call (case95) | pointer, rank, wait, coordinate |
| Wait then coordinate named | pointer, rank, coordinate, wait |
| Wait then coordinate then rank named | pointer, rank, coordinate, wait |

Thus the new statement ordering restores the desired candidate set and changes
rank/coordinate creation order. This alone is insufficient: named values remain
in the intermediate tree and final allocation records.

## Copy persistence and loop unrolling

After recurrence creation (40F975), after invariant hoisting (40FA1B), and on both
sides of later flattening (40FB1B/40FB20), the named identifiers still occur.
They are not eliminated merely because they feed existing argument values.
Moving declarations outside the loop gives byte-identical complete contexts.

A second set of original-compiler observations localizes replication precisely:
4185C0 at40FA3D/40FA42 leaves these trees unchanged. The41CC30 call at
40FA97/40FA9C increases call-node count7→20 in baseline and both observed controls.
In the single-rank control, named-rank references increase2→6; in the wait plus
coordinate control, each named identifier likewise increases2→6. The helper's
inferred listing selects factor3 when the loop trip count is divisible by3 and
calls412E70. The direct before/after observations establish the replication in
these cases; they are not a general implementation of SHC loop-unrolling rules.

Final allocation records contain three priority4 WEBs per staged short value.
The wider-intermediate controls keep those WEBs and spill them, disproving the
hypothesis that widening alone removes a narrowing-copy obstacle. Grouping the
assignments and call into a sequenced comma expression also fails to recover the
original machine code. The comma forms are in the body; the induction update
remains standalone i++, unlike case95's rejected comma-header form.

## Ten source controls

| Source change | Positioned differing bytes | Following root |
|---|---:|---|
| wait then rank |612|12FB0|
| wait then coordinate |113|12FA8|
| wait then rank then coordinate |615|12FB0|
| wait then coordinate then rank |616|12FB0|
| wait and coordinate declared outside loop |113|12FA8|
| all three declared outside loop |616|12FB0|
| long wait and coordinate |1157|12FBC|
| all three long intermediates |1162|12FC8|
| sequenced wait/coordinate/call expression |1157|12FD0|
| sequenced wait/coordinate/rank/call expression |1154|12FF0|

All computations are real call arguments with unchanged values for i=0,1,2.
No fake consumer, dead statement, padding, or forced physical register is used.
The two outer-declaration forms are verified identical over the entire compiled
context to their inner-declaration counterparts. All ten fail the unchanged
default verifier, including controls with nominal following boundaries. The
113-difference result is worse than the baseline32 and is not credited.

## Evidence and validation

Case: `tools/shc_probe/cases/session_rank_expression96`. `prepare.py` regenerates
all ten sources from the genuine input. `analyze.py` verifies source/context
hashes, ten normal/native object comparisons,20 exact normal/debug middle-stage
replays,395 priority/profit calculations, and all ten unchanged hardware replays.
The hardware evidence contains124 snapshots and55,016 node observations.
All original.reg/.ilb outputs remain byte-identical; input configuration is
unchanged. Tracing writes hardware debug registers only.

`ORDER_ANALYSIS.json` records the actual expression trees, candidate orders,
call counts, and named-reference counts. `REGISTERS.json` and `ALL_RESULTS.json`
cover every source. Batch WATCH/REPLAY summaries contain only the last batch;
verification uses all per-source files. Earlier-input trace receipts reference
frozen case94/case95 captures and validate their source hashes. Inferred compiler
C is explicitly labeled and accompanied by original x86 disassembly.

Fresh read-only checks pass33 checkpoint84 production pins, original ROM hashes,
and fixed-denominator reports in both nuada checkouts. There were no production
edits or new full builds. Checkpoint84 remains the latest full validation gate
with234 enabled tests.

## Next action

Park staged rank-expression and independent-counter rewrites. Keep case92's
nominal32-difference source. Return to the independent mode0 pointer tie and
inspect whether the order of the existing external declarations affects symbol
identity, WEB construction, or tied sorting without adding temporary values.
Case91 already closed local p/q declaration and increment reordering; do not
repeat those. A bounded external-declaration control must retain every original
function boundary, use the existing declarations only, and compare both compiler
records and complete native output before drawing a conclusion. No change to
coverage or verification rules is authorized by this research result.
