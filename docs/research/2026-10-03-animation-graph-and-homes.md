# Animation allocation graph and short homes — continuation140

Production remains checkpoint122: 128,316 / 200,698 bytes (63.93487%). No matching
C was integrated. Previous turn139 was progress: it recovered three genuine R9
short stores and identified inline-return boundaries. This turn reproduces the
new source's allocation graph and recovers the desired main-loop roles together
with those stores, but not the original frame and home layout simultaneously.

Case: `tools/shc_probe/cases/session_animation_allocation140/`.

## Allocation model verified against the real compiler

Read-only hardware traces compare139 shared_scope_legacy with137
signed_three_legacy_call. Their allocation candidates number89 and87. The former
orders constant2,total,mask15 at positions6,7,8; the latter orders mask15,
constant2,total. All three initially tie at profit7. This accounts for the
observed loop-register difference without changing any compiler state.

The139 graph has an anonymous long temporary already bound to internal register1
before the main allocation pass. It was initially described as a pointer; tracing
the symbol to the compiler's own tree corrects it to a long. The model now seeds
all actual prebindings. Its first assertion failure is retained in
`initial_prebinding_failure.log`.

Using heap pointers from an earlier debug replay to label the new trace also
failed for the137 control. The tracer now reads each WEB's symbol directly from
its live payload; names are resolved by symbol ID rather than assuming heap
addresses agree across runs. Both controls were retraced. The failure is retained
in `pointer_role_failure.log`.

The model recomputes profits, partition-sort positions, register eligibility,
prebindings and final positive allocation for all observed candidates. Across
both controls and five source variants, all624 candidate predictions agree with
the compiler. Sensitivity results are explicitly conditional on the observed
fixed graph and prebindings; hypothetical priority changes never enter the
compiler, verifier or coverage count.

## Genuine brightness controls

Initial chained assignments in either direction reduce constant63's priority
from10 to9 but leave the bad global allocation unchanged. Their native code is
not identical: the two chain directions differ in two text bytes, and the forward
chain differs from139's separate initialization in27 text bytes. The verifier's
initial stronger identity assumption failed and is preserved; these differences
are now recorded in `NATIVE_COMPARISONS.json`.

The common-reset source removes the two branch-local assignments of63 to both
brightness locals and performs the same reset after the branches when `c0 & 12`.
Neither branch reads brightness before that point. If either or both direction
bits are set, both locals end at63; if neither is set, both remain unchanged.
This is a consumed state update, not a dummy operation or compiler hint.

Constant63's priority falls from10 to6. The graph also changes from89 to90
candidates: the new condition contributes a real constant12 use. Mask15 and
constant2 profits rise from7 to8, ahead of total's7. The actual output restores
holdR10, mask15R9, constant2R8, and total on the stack while retaining all three
R9 short writes. Those writes use home36; the frame is112, not the original108.
The graph change means the result is stronger than a fixed-priority prediction,
but it does not establish this was the original source structure.

## Single-home combinations

The same reset is applied to139's two source forms that make later visibility
reads consume the addressed home. They eliminate the extra scalar home and
retain all four main-loop roles in a108-byte frame. Their setup value is instead
globally allocated to internal register9 (hardware R10), losing the original R9
block-cache pattern. Their addressed home is12, still not original0.

The actual short stores to home12 use R0. SH2's displacement-addressed short-store
form constrains the source register, so a naive assertion that there must be
three literal R10 stores was wrong. The failure is retained in
`initial_store_encoding_failure.log`; the final verifier records actual CFG
stores and separately checks the observed global allocation. It does not infer
native source registers solely from the allocation table.

| Source | Body bytes | Frame | Main-loop roles | Three setup R9 writes |
|---|---:|---:|---|---|
| initial_chain |1728|108|wrong|home32|
| initial_reverse |1728|108|wrong|home32|
| common_reset |1762|112|correct|home36|
| shared_home_loop_reset |1746|108|correct|absent|
| shared_home_chain_reset |1754|108|correct|absent|
| Original |1748|108|reference|home0|

All candidates still start atA604, notA608. Near-equal lengths do not imply a
near byte match; no relative, relocated or length-based credit is awarded.

## Verification and next work

Five normal compiles and five independent captures agree section for section.
Ten middle replays preserve captured REG/ILB. Nineteen hardware replays cover
sort, eligibility and block caching; their compiler hashes and untouched outputs
verify. Five native CFG stack audits resolve all accesses and restore SP at every
return. The unmodified contextual verifier rejects every candidate.

Both Nuada checkouts verify all44 production pins,25 previous frozen cases,
original ROM hashes and the unchanged progress report. All23 copied reference
ROM files still match the recorded source. Production's complete build/test gate
remains checkpoint122; private rejected probes did not require a full-suite rerun.

Next inspect the allocator's assignment and coalescing of negative physical IDs
for spr2 and sprite_id in common_reset, against the single-home control. Also
retain139 shared_scope_legacy as the source with the original brightness branch
structure; common_reset demonstrates an allocation mechanism, not an accepted
original-source reconstruction. The unresolved constraint is to preserve the
R9 setup stores, four loop roles,108-byte frame and the single original home0
together. The full100% goal stays active with72,382 bytes incomplete.
