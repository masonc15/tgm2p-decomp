# SHC candidate creation and the F200 register tie

Continuation 99 follows the allocator diagnosis from continuation 98. No new
matching bytes were accepted. Checkpoint 84 remains 127,276 / 200,698 bytes
(63.41668%), with 73,422 remaining.

## Observed creation mechanism

Three original-source replays and five lookup controls use hardware execution
breakpoints after variable construction at 415134, after constant construction
at 415B66, and before sorting at 405ECC. Compiler instructions, configuration,
and data are unchanged; all eight replays reproduce original .reg/.ilb exactly.

The 104 observed candidate appends reproduce the linked-list order copied into
the sort table. All 96 variable creations occur at identifier opcode70 with a
kind1 use at the head of its member chain, followed by connected definitions.
This corroborates the use traversal in 414E90 and the connected-member grouping
in 4152F0/4153B0. It does not establish a universal last-use rule: creation is at
the first eligible use encountered for that group.

In baseline, register-result, and reversed-initialization controls the relevant
order remains tbl, y, x. Reversing the declarations changes the symbols assigned
to y/x, but not these semantic roles in the construction order. Their uses in
the row-offset calculation and subsequent column lookup determine that order.

## A source realization of the virtual ordering change

With the real sprite result declared register, this consumed pointer rewrite:

```c
tbl += x;
e = (long *)tbl[y * 21];
```

creates tbl, x, y instead. Its complete interference graph matches the baseline
by semantic role. All 13 physical assignments match the target mapping: all
baseline assignments are retained except spr moves R6 to R5 and zero R5 to R6.
This is an actual compiled source result, unlike continuation 98's model-only
candidate swap.

It is still a failed decompilation candidate. It moves the column addition ahead
of the row multiplication. The original performs MULS.W early, interleaved with
VM operand loads; this control performs it after the column addition. The
resulting instruction scheduling and temporary registers differ in 57 bytes.
The following entry and both retained ranges remain exact. Register assignment
alone is insufficient to claim a match.

## Other controls

| Control | F200 differing bytes | Following F292 root |
| --- | ---: | --- |
| `(tbl + y*21)[x]` | 47 | F292 |
| `(tbl + x)[y*21]` | 47 | F292 |
| Advance tbl by x, then index by y*21 | 57 | F292 |
| `tbl[x + y*21]` | 113 | F290 |
| `tbl[y*21 + x]` | 113 | F290 |
| Row offset `y*20 + y` | 133 | F296 |
| Row offset `y + y*20` | 133 | F296 |
| Row offset `y*7*3` | 36 | F292 |
| Row offset `y*3*7` | 36 | F292 |

The nested operand reversals produce identical full contexts to each other, as
do the combined-index reversals. Both retain y-before-x construction. The two
20-plus-one forms raise y's priority from 2 to 3 and restore y/R7 with x spilled,
but change the generated arithmetic and other allocations; their next root
shifts four bytes. Both factored multiplications reproduce the register-result
control, retaining its wrong x/y allocation and 36 differing bytes.

Nine new native object comparisons, 18 exact middle replays, and 156 independent
score checks (including the three original controls) authenticate the results.
All raw checks use original nominal positions and unmodified prefix validation.
No default proof passes for F200. No new full build/test run is warranted for
these private negative controls; checkpoint 84's 234 enabled tests and clean
builds remain the latest whole-project validation.

Evidence: `tools/shc_probe/cases/session_vm_web99/ANALYSIS.json`,
`CREATION_ANALYSIS.json`, `REGISTERS.json`, `ALL_RESULTS.json`, and `FREEZE.json`.
Inputs from case98 are pinned by `INPUT_PINS.json`.

## Next action

Park the tested F200 declaration, lookup-order, and row-factor families. Carry
the observed score/sort/candidate-construction method to f251CC, the 82-byte
object-list gap between two retained C ranges. Current splits still leave it
ASM. Its canonical source is `src/objfile_248a8.c`; historical full-context
checks reported k/R7, s/R4, hardware-base/R5 versus ROM k/R4, s/R5, base/R7.
The archived follow-up rejected 453 compiled source permutations without an
allocator trace. Recompile the current canonical context and authenticate both
retained ranges before using that historical diagnosis. Then identify those
three candidates' scores, conflicts, sort order, and binding. Do not repeat the
archived declaration, qualifier, helper, or neutral-expression sweeps.

The full 100% goal remains active. Work continues solo without brainstorming.
