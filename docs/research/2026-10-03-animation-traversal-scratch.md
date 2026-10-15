# Traversal scratch history and mask dependency — continuation148

Verified production remains checkpoint122:128,316 /200,698 bytes (63.93487%),
72,382 bytes remaining. No C was integrated. This continuation recovers a
larger set of constraints in one native candidate, but still fails exact matching.

## Source experiments and native results

| Control | Body bytes | Result |
|---|---:|---|
| reverse_sum |1740| All allocated sections equal147 lastpos_explicit |
| staged_explicit |1744| Named consumed step; six target roles, zero scratchR3 |
| staged_compound |1744| Named consumed step; loop constants exchanged |
| c0_mask_explicit |1748| Six target roles plus R10/R2 zero test; xhome16 |
| c2_mask_explicit |1748| Six target roles plus R10/R2 zero test; xhome20 |

The first test reverses operands in the existing lastpos addition. Exact
allocated-section comparison proves it is a native no-op, including flags and
alignment. The next two introduce a consumed local `long advance` containing
the existing traversal increment, then apply explicit or compound accumulation.
They produce distinct native code but do not repair the zero-test constraint.

Frozen147's `prefix_decrement` preserves the desired R10/R2 zero test with
compound traversal but exchanges constant2/mask15. Its fixed-graph model
predicts c0 or c2 priority+2 as a solution. Existing trace terms show each
compound mask's assignment contributes0, while an ordinary assignment in that
loop can contribute2. Two independent controls therefore use:

```c
c0 = c0 & 0xf0;  /* c0 control, replacing c0 &= 0xf0 */
c2 = c2 & 0xf0;  /* c2 control, replacing c2 &= 0xf0 */
```

Both compile with unchanged stock SHC to holdR10, mask15R9, constant2R8, total
on stack, header countR10 and setup spriteR9. Both retain the native sequence
`EXTS.W R10,R2; TST R2,R2`, frame108, first homes sprite_id0/sel4/tbl8/total12,
three word stores from R9 to home0, and the original1748-byte body length.
The c2 trace verifies its priority rises12->14 while lastpos stays7. The
allocator model reproduces all88 candidates and the resulting six roles.

The c2 form is the next baseline because x retains original offset20; the c0
form puts x at16. Neither matches the ROM. Both still start atA604 instead of
A608 due to the preceding unresolved function. The c2 homes include spr1=48
(original16), lastpos=36 (44), cnt=56 (72), last=64 (60), y=16 (24). Other
instruction/register/pool differences remain. Equal function length does not
establish matching bytes, and relative comparisons receive no credit.

## Why the zero-test scratch differs

Two stock-generator scratch traces cover147 prefix_decrement and lastpos_explicit.
The selector model predicts all6298 decisions, including708 in fA608. At the
zero test, both calls have preferred mask0, excluded/used mask0x7ff0, recent
mask0 and the default descending scratch scan. Compound traversal reaches the
selection with last-scratchR3, so the selector choosesR2. Explicit traversal
reaches it with last-scratchR1, so it choosesR3. Changing only this recorded
last-scratch field in the offline model switches the predicted selection in
both directions. No compiler state was changed.

The model inherited from119 lacked the no-candidate return.24 calls elsewhere
in the full translation units exclude all fifteen candidates and return65535.
Inspection of stock FUN_00418900 confirms its local candidate remains-1 in that
case. The model now represents that branch explicitly; all24 returns are
checked against full exclusion, rather than skipped or treated as matches.

Two generator traces of147 lastpos_explicit and148 reverse_sum each record
578 emissions and238 recipe events. Both observe the R10-to-R3 extension at
source line361 and preserve captured SUA plus untraced SUA/ASA. The cleanup
snapshot's ASA symbol-position differences remain recorded, not normalized.

The first generator trace failed its function-index assertion because the
middle-stage debug list includes helper entries in a different order: fA608
is eighth there, sixth in generator traversal. The revised tracer selects its
verified raw symbol162, requires exactly one target, and records the actual
index. The failed attempt is retained separately. No game-source failure or
compiler patch was involved.

## Validation and next work

Five distinct native controls, five watched captures, ten normal/debug middle
replays and five CFG audits pass. All retain six loops, no inline-return gotos,
36 exported real functions and no unnamed local calls in the audited body.
Eight successful hardware replays preserve the applicable stock intermediate
outputs: two generator, two scratch and four c2 allocator/variable/home traces.
The c2 models reproduce50 variable priorities from220 weighted terms and27 home
records from135 contributions. `verify.py` reruns these analyses and checks the
joint native constraints and strict rejection for every source.

`recheck_production.py` verifies44 production pins and33 prior frozen cases in
both Nuada checkouts, unchanged original ROM/report hashes and all23 reference
ROM files. Full production build/test gates were not rerun for private probes;
checkpoint122 remains the latest full gate evidence.

Evidence: `tools/shc_probe/cases/session_animation_traversal148/`.
Next inspect actual spr1/cnt/lastpos uses determining home frequency, starting
from c2_mask_explicit. Preserve the now-coexisting zero-test, setup stores and
loop-register constraints. Do not repeat operand spelling or staged-step probes
without new evidence: their effects are recorded here.
