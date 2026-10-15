# Animation loop analysis and call lifetimes — continuation142

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**.
No matching C was integrated. Continuation141 made progress by explaining home
ordering and recovering R9 stores at home0. This continuation identifies why
its returning helper conflicts with the desired loop register allocation, then
recovers homes0/4/8 with all six detected loops preserved.

Case: `tools/shc_probe/cases/session_animation_loops142/`.

## Returning helpers change more than home frequencies

The140 direct-access control has six entries in SHC's loop table: three earlier
loops, the main loop, and its two inner step loops. The141 visibility lookup
introduces two inline-return gotos; its loop table retains only the three earlier
loops. Returning through a helper in either visibility branch alone has the same
effect. Inlined void helpers that perform the real hide/show update introduce no
return gotos and preserve all six loop entries. This is verified for these sources
and compiler settings, not claimed as a universal rule for every inline function.

The frequency tracer records each contributing reference's raw reference count,
root pointer, basic block and loop depth. The reconstructed formula weights each
effective use by one plus its detected loop depth. All **1,023 contributions to
216 frequency records across eight sources** reproduce the compiler's observed
frequency arguments. The existing grouping/sorting model also reproduces all216
final home records and their serialized REG assignments.

For example, without the main and inner loop entries, tbl falls from frequency11
to6, sel from12 to7 and spr1 from4 to3. The earlier apparent home improvement
therefore came partly from removing loop weighting on unrelated variables.

## Why changing allocation priority alone cannot fix that form

The mask15 constant is eligible for internal register1 (hardware R4) in the
returning-helper forms. With void helpers, the same eligibility predicate rejects
R4-R7. Hardware traces at the original predicate's return observe the live ranges
and all27 call points without modifying compiler memory.

In141 original_brightness, mask15's live ranges overlap **zero** calls, and R4 is
accepted. In142 void_helpers, those ranges extend around the main loop and overlap
**15** calls; all four caller-saved registers are rejected. The relevant compiler
routine, inferred at0x405750, checks call points against the candidate's live
ranges. Both traces reproduce their untraced REG and ILB exactly.

Five source graphs plus the141 control reproduce all **531 allocation candidates**:
priorities, profits, partition-sort order, prebindings, eligibility and positive
register assignment. Bounded sensitivity changes on the observed fixed graphs
find no solution for the returning-helper cases. This is only a diagnostic model,
not a proof over all source programs; no changed model value enters the compiler.

## Five consumed source experiments

All retain the real36-function compilation unit and stock flags. The first four
change only how the existing visibility operations obtain or update the sprite:

| Source | Bytes | Frame | Sprite-ID home | Detected loops | R9 home stores |
| --- | ---: | ---: | ---: | ---: | --- |
| hide_only | 1752 | 108 | 0 | 3 | three at0 |
| show_only | 1710 | 108 | 4 | 3 | none |
| void_helpers | 1724 | 104 | 8 | 6 | none |
| single_lookup | 1696 | 108 | 0 | 3 | none |
| void_call_id | 1756 | 104 | 0 | 6 | none |

`hide_only` uses the returning lookup only in the hide branch; `show_only` uses
it only in the show branch. `single_lookup` computes one pointer before the
visibility branch and consumes it in the selected update. `void_helpers` inlines
separate void functions that directly perform the original bit updates.

The last experiment addresses the frequency problem without discarding loop
analysis. Starting from void_helpers, it assigns `sprite_id = spr2` immediately
before the existing second f_253bc initialization call, then passes sprite_id
instead of spr2. The same value is consumed by the same call; there is no dummy
read or additional call. Sprite-ID frequency rises from10 to12 and ties sel's12.
Stable ordering places sprite_id first, yielding the original first three homes:
sprite0, sel4, tbl8. All six loops remain recognized.

This is a useful new control, but not a match. Its main-loop assignment is still
hold=R10, total=R9, constant2=R8, with mask15 unallocated. The setup sprite value is
R8, and the frame is104 rather than108. The original requires total on the stack,
mask15 inR9, a108-byte frame and three setup stores fromR9. Other homes also differ.
All five candidates begin at A604 rather than A608 because the earlier f_A226
context remains four bytes short. Each fails the unchanged contextual verifier.

## Verification and next work

Five ordinary native compiles equal separately captured native object sections.
Ten middle-stage normal/debug replays and22 read-only hardware replays preserve
the captured intermediates. Five control-flow stack audits have balanced returns
and no unresolved stack accesses. All36 real functions remain exported, and no
unnamed helper calls survive. Native results and intermediate records, including
the first and second experiment batches, are retained separately.

Both Nuada checkouts preserve all44 checkpoint122 production pins, both original
ROM hashes, and27 prior frozen cases. Full clean build and243-test gates remain
checkpoint122's historical results, not newly run gates for these private probes.
The fixed denominator and all1,638 incomplete runtime bytes remain unchanged.

Use void_call_id as the new control for recovering homes while retaining loop
analysis. Its graph's diagnostic sensitivity identifies the consumed count and
setup-value live ranges as candidates for inspection; do not convert hypothetical
priority changes into compiler edits or artificial source uses. Compare genuine
count/setup expressions and the140 brightness control against this graph, then
check native stores, all homes, loop roles and strict bytes together. Retain141
as a separate setup-store control. The literal100% goal remains active.
