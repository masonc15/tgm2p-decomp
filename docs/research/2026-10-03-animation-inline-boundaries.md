# Animation inline boundaries and short home stores — continuation139

Production remains checkpoint122, 128,316 / 200,698 bytes (63.93487%). This
continuation recovers a previously missing instruction pattern in private source
controls, but none matches the full original function. No production source,
compiler, linker policy, verifier, denominator, or runtime rule changed.

Previous goal turn classification: progress. Continuation138 excluded temporary
stack allocation and identified the loss of block caching in the helper controls.
Continuation139 changes the next action again by resolving that loss causally.

## The boundary is an inline return

Case: `tools/shc_probe/cases/session_animation_inline139/`.

The138 scoped reference helper is inlined, but each pointer return introduces an
internal goto and label before register allocation. Its first tree dump contains
three such goto/label pairs. The setup spans three candidate-bearing blocks:
134–192,194–214,216–258. Their spr2 counts are2,1,2; a later loop candidate has1.
These do not reach SHC's minimum3-use cache threshold.

A direct caller form performs the same real operations: make a short copy, take
its address into a local pointer, read the short through that pointer to select
the sprite, and modify the actual field. No helper returns or synthetic consumers
are added. It has no corresponding goto/label pairs; hardware tracing observes a
single setup block134–277 with five spr2 uses. SHCMDL allocates internal register10
(hardware R9). This is a concrete correction to the earlier alias-boundary
hypothesis: inline-return control-flow boundaries explain this tested difference.
It does not establish that alias analysis never matters elsewhere.

The boundary tracer records the block's real statement list at the allocation
gate. Three traced middle-stage replays preserve captured REG and ILB byte for
byte:138 scoped_ref_legacy,137 signed_three_legacy_call,139 scoped_legacy.
The137 control has three uses in one setup block; the new direct form has five.

## Recovered instruction pattern

The scoped direct source produces three `MOV.W R9` stack writes followed by short
loads for b2a, w18, and w2c. The legacy-call form also retains `EXTS.W R9,R4` in the
second f_253bc call's delay slot. All observations come from native code with CFG
stack tracking, not a linear decode of literal pools.

Initially the three copies occupy64,60,56 in a116-byte frame. Reusing one real
short local while keeping the pointer declarations scoped to each operation gives
`shared_scope_legacy`: three writes to the same home32 and a108-byte frame.
The stores are at candidate A75C,A774,A7AC. Original stores are A760,A77A,A7AC,
all to home0. The candidate's original spr2 value still lives at28, so there is
an extra distinct home. Matching one store address is not proof of equivalence.

The source of the short copies remains spr2; all copies feed actual array indexing.
No padding, forced register, volatile access, opaque external call, fake consumer,
assembly insertion, or compiler change was used.

## Controls and remaining differences

All nine builds use the real36-function compilation unit and unchanged production
flags. All contain36 exported game functions and no unexpected reachable local
helper calls. All start f_a608 atA604, four bytes before its original address.

| Source | Body bytes | Frame | R9 short writes |
|---|---:|---:|---|
| scoped_typed |1730|116|64,60,56|
| scoped_legacy |1728|116|64,60,56|
| shared_typed |1738|108|none|
| shared_legacy |1736|108|none|
| shared_scope_legacy |1728|108|32,32,32|
| home_chain_legacy |1724|104|none|
| home_separate_legacy |1720|104|none|
| shared_home_loop_legacy |1720|104|none|
| shared_home_chain_legacy |1720|104|none|
| Original |1748|108|0,0,0|

The first two shared controls keep a pointer alive beyond each individual
operation; they do not cache spr2 during setup. Keeping pointer lifetimes local
recovers the five-use R9 cache. Separating a setup-only value from the addressed
home, or making later loop reads consume that home, changes the value to a global
allocation: it no longer has the desired R9 block-cache behavior. Those controls
reduce the frame to104 and are rejected.

The new direct forms also disturb the main-loop allocation. In
shared_scope_legacy, hold is stillR10, but constant2 takesR9, total takesR8 and
mask15 has no global register. Therefore this form is a diagnostic control, not a
replacement for134's register-correct source or137's cached-call conversion
control. Returning the mask15/constant2/total allocation to the original while
placing the shared short at0 remains unresolved.

## Validation and next action

`VERIFICATION.json` records9 native compiles,9 independent native captures,
18 exact normal/debug middle replays,12 read-only hardware replays and9 CFG stack
audits. `CALL_AND_STORE_EVIDENCE.json` verifies the recovered short writes and call
conversion. `BOUNDARY_EVIDENCE.json` records the actual blocks, statements and
use counts. Every candidate fails the unchanged strict contextual verifier.
No relative or relocated byte comparison earns credit.

Both Nuada checkouts verify all44 production pins,24 prior frozen cases, original
ROM hashes and the unchanged progress report. All23 reference ROM files match
the recorded source. The prior complete checkpoint122 build/test gate remains
applicable; full suites were not rerun for rejected private probes.

Next inspect the global interference/tie ordering of shared_scope_legacy against
137 signed_three_legacy_call, and the allocation order of its source and addressed
short homes. Preserve the recovered R9 stores as a measured constraint. Do not
repeat inline value-argument address-taking or the now-explained inline-return
experiments. The full100% goal stays active, with72,382 bytes incomplete.
