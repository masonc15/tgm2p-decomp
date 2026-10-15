# Ranking live-range obstruction (256)

Production remains checkpoint249: 135,910 / 200,698 verified bytes.

## What the original compiler chooses

Six read-only hardware-breakpoint replays observe SHCMDL sorting and allocation
for the clean ranking baseline, ordinary-loop-increment control, a real
explicit-backedge control, and an explicitly excluded historical source. Every
traced REG/ILB output equals its normal driver output. Four additional normal/
diagnostic replays agree. Four fresh Mac native objects reproduce on Nuada.

The allocation model reproduces all30 assignments for the two clean controls.
The original ROM holds10 in R8; the clean draft holds it in R11. The draft's
constant10 clashes only with the two cached callees and final counter, assigned
to R14/R13/R12. With that graph and those required assignments, R11 is its
first available general register. SHCMDL's register-first allocation will take
R11 before reaching R8 regardless of priority or tie sorting. Lowering the
constant below the allocation threshold makes it unallocated, not R8.

Thus this specific requested allocation is impossible by priority changes alone
on the current graph.360 single-priority and640 paired trials found no solution,
but the first-free-register argument supplies the stronger structural reason.
This is not a claim that the function is impossible to decompile.

## Why the old excluded source looked closer

The unchanged archived best215 source is recompiled only as an excluded
diagnostic control. Its dead assignment must never be admitted or credited.
It increases constant10's priority4→5, conflicts3→12, and extends its serialized
live ranges from[(51,52),(55,56)] to[(31,33),(36,36),(39,45),(47,52),(55,56)].
The extra conflicts include timer, done, and160. It also raises done's priority
5→6. The resulting constant is allocated R8, but the other three register
choices remain wrong. The dead assignment altered liveness as well as priority.

The comparison uses raw constant type0/value10, not the diagnostic printer's
ambiguous NUMBER CONST labels for symbolic addresses.57 full clash-graph
records and38 serialized physical-register records are checked. An initial
assumption that diagnostic and hardware replay heap addresses were identical
failed; it is preserved in INITIAL_ADDRESS_CHECK_FAILURE.log. The corrected
bridge checks function symbol154, every logical ordinal, priority, profit,
physical assignment, and the complete clash adjacency graph. It does not
equate unrelated heap pointers or silently omit mismatching records.

## Genuine structural controls

The pretest while and explicit-break for loop produce byte-identical complete
contexts to the clean baseline:464 target bytes and71 nominal differences.
An explicit backedge produces472 bytes and342 nominal differences. All three
fail unchanged admission. No compiler input consumes the offline model or its
counterfactual priorities; no production source, proof or denominator changed.

Close ranking priority-only tuning and these equivalent loop shapes. Revisiting
this target requires evidence of a genuine consumed operation, type behavior or
compilation context that changes the constant's live range while preserving
original instructions. Do not restore the dead assignment or extend these
blind source matrices. The full production suite was not rerun for these
research-only negatives.
