# Palette hardware base, saved palette index, and stack-address tie

Continuation205 is excluded evidence, not an integration. Production remains
131,780 / 200,698 verified C bytes. The previous continuation made concrete
progress by explaining a conflict-cost change and installing a working reducer.
This continuation uses fresh native compilation and unchanged-output traces.

## Findings

The direct hardware constant is eligible for low registers: its eligibility
reject flag is zero. Its priority10/profit-16773 places it too late to retain a
register. The ordinary integer buffer base has priority15/profit-16768 and
spills. This is an ordering problem, not a special prohibition on the address.

Adding a hinted hardware pointer before the guards makes the original
interleaved color source use iR4/vR5/oR6, but assigns hardwareR7 and kR14.
The hinted hardware pointer has unhinted weight7, versus k's5. Additional
buffer hints displace more important variables. An offline, verified-graph
sensitivity experiment finds that raising only k's weight by2 would recover
five main homes in the interleaved graph. It finds no six-home solution,
including the buffer, across189 k/base weight pairs; the paired graph has no
five-home solution across its189 pairs. These are diagnostic calculations,
never compiler modifications or matching credit.

The genuine do-loop, entry initialization and indexed-object source controls
do not raise k's weight. The indexed-object form happens to put the next
function at2A576, but its predecessor differs in1063 bytes and the initializer
is1500 rather than1504 bytes. A correct start alone is insufficient.

The ROM computes its destination row once at2A354–2A360, before the copy loop.
An explicit row pointer lowers hardware use weight7->3 but changes the conflict
graph enough to push color intoR14. Saving the palette index into an ordinary
unhinted short also fails. Saving it into a standard `register short pal`
recovers the intended main homes without changing the interleaved color order:

| Source value | Native register |
| --- | --- |
| Inner counter i | R4 |
| Packed color v | R5 |
| Object o | R6 |
| Outer counter k | R7 |
| Hardware base pals | R14 |
| Saved palette index pal, after color loops | R5 |

The saved index's unhinted weight is4; the hardware weight is3. Native traces
independently authenticate41 positive-symbol priority calculations, including
usage counts, assignment bonuses, loop multipliers and neighbor averages.
Moving the palette-index load before the writes is consistent with the ROM's
copy loop; the target's palette hardware and object storage occupy distinct
address ranges. This is a source hypothesis, not a whole-function proof.

The named integer base still spills. Typed and char-pointer variants do not
retain it either. Direct `buf[i]` writes with `((short *)buf)[i]` reads preserve
the five main homes and recover the original68-byte local stack frame.
The resulting predecessor is1176 bytes, ten bytes short, and differs in709
raw positions against the1186-byte ROM function. Its initializer remains a
strict negative with1267 body differences. No new C bytes can be counted.

In this direct form the buffer address becomes one category0/type1 stack-
address candidate with priority20/profit-18930, tied with several constants:
00FC0000,16,FFFFFC00,FC000000. Current sorting gives the first three available
high homes to green-mask R10,16 R9,and signed-mask R8. Red-mask and stack-address
candidates remain unallocated. The ROM instead retains bufferR10,signed-maskR9,
and red-maskR8, with green-mask/16 materialized where needed. The raw stack-
address candidate payload is12; do not confuse that compiler payload with an
unverified final frame displacement.

## Evidence and next step

Case: `tools/shc_probe/cases/session_gfx_bases205`.
24 strict negatives comprise22 new controls and2 carried controls.27 native
replays preserve both REG and ILB bytes, with5038 events.24 complete allocator
graphs and24 independently reassembled captures pass. The verifier recomputes
roots, bytes, differences, source/object provenance and absence of kept matches.

No production changes occurred, so checkpoint192's independent clean builds
and253 enabled tests remain the applicable full gates. Fresh manifest,54
production-pin,report and original/rebuilt ROM checks cover both Nuada trees.

Next inspect constant creation/insertion and the exact unstable tie ordering
for `pal_buffer_direct`, function symbol53/REG record0. The five main homes,
standard saved-pal hint and68-byte local frame are useful constraints. Seek a
real source expression or declaration change explaining the constant ordering;
never force registers, patch priorities, insert padding, change the denominator
or weaken the matching checks. Do not repeat this continuation's hint, lifetime,
row, palette-index or buffer variants unchanged.
