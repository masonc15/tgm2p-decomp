# VM allocation ties: every stack slot and saved-register role recovered

Continuation 229 adds no C credit. The best structural control,
`chain_2031.c`, recovers the original 44-byte frame, all stack locations and
all seven saved-register roles. Its complete 760-byte original-address comparison
still fails: the emitted context is 752 bytes, with 665 differing or missing
bytes. This is an allocation result, not an instruction-level match.

Production remains 135,594 / 200,698 bytes (67.561211%); 65,104 remain, including
all 1,638 runtime assembly bytes. No production source, gate, split, compiler,
or denominator changes.

## Recovering y and the width table

The frozen continuation-228 graph has y priority 20. A read-only Python model
reconstructs native allocation, then varies only that priority from 14 to 28.
Only priorities 21 and 22 swap y/width into the desired R9/R8 without changing
another positive allocation. This is explicitly a fixed-graph hypothesis;
no synthetic priorities or altered IR were sent to the compiler.

Native source experiments distinguish total references from allocation groups:

- `y = -y; y += 62` splits y into priority-16 and priority-6 groups. Their sum
  is 22, but the draw-call value spills. Subtract-then-negate similarly splits
  into 14 and 8. Combined totals are not usable allocation priorities.
- A named height temporary leaves the useful y group at 16 and spills it.
- `y = y * 30 - y` produces priority 24 and takes R10, disturbing another role.
- Separate 30-pixel pitch and row-gap arithmetic reaches priority 22 but emits
  different arithmetic from the original multiply by 29.
- Crucially, replacing only `y *= 29` with `y = y * 29` keeps one allocation
  group, raises priority 20 to 22, and recovers y/R9 and width/R8. It retains
  the original multiplication value and the 44-byte frame.
- A named width-table pointer makes y win R9 but spills the pointer. Its const
  and non-const forms produce identical bytes and are not a solution.

Seventeen native priority models verify 2,647 events, including individual
contributions and every observed final group. Full positive allocation and
negative-home coloring are independently reconstructed from the native graph.

One early nested-assignment probe, `y = 62 - (y *= 29)`, modifies y twice in a
single full expression. Its semantics are not justified under the legacy C
sequence-point rule. It is explicitly excluded from matching candidates in
SOURCE_SEMANTICS.json and retained only as compiler diagnostic evidence. It also
fails the byte gate. No work is promoted from this source. There are 32 eligible
controls plus this one disqualified diagnostic probe, all 33 strict negatives.

## Recovering the four letter slots

The expanded multiply leaves the incoming letter-slot order 1,0,3,2. All four
have memory frequency 6. A fixed-graph model tests 35 extra-use patterns totaling
at most three pointer dependencies. None recovers every target offset while
preserving the saved allocations. This is a conditional result: real pointer
expressions can also change constants and graph structure.

The native linear pointer chains do exactly that: they reduce the allocation
table from 33 to 32 candidates. All six chains with letter1 as the terminal
pointer are tested. Their first three pointers have frequency 7 and letter1
has frequency 6. The chain in order 2,0,3,1 recovers the desired incoming order:

```c
letter2 = (char *)&g_6079366 + 4;
letter0 = letter2 - 4;
letter3 = letter0 + 6;
letter1 = letter3 - 4;
```

All cross-element arithmetic is within the complete array's byte representation.
These are the same four real destination addresses at offsets 0,2,4,6; no padding,
extra data effects, dummy live values or physical-register forcing is used.

| Value | ROM and chain_2031 stack offset |
|---|---:|
| sprite ID | 0 |
| col | 4 |
| line | 8 |
| row | 12 |
| letter3 | 16 |
| letter0 | 20 |
| letter2 | 24 |
| letter1 | 28 |
| done | 32 |
| str | 36 |
| bufs | 40 |

The saved roles also agree: buf/R14, sprite base/R13, x/R12, i/R11, 254/R10,
y/R9 and width table/R8. Native REG records, graph allocations and reachable
literal loads independently support those roles. The original 0xEC0C–0xEC8C
prologue is authenticated against the pinned ROM and its concrete stack-store
words and literal addresses are checked by inspect_original.py.

Changing the successful chain back to `y *= 29` retains every stack slot but
returns y/width to R8/R9. Both variants are valuable controls for the subsequent
code-generator investigation. Local pointer const qualifiers and declaration
initializers are byte-identical to the expanded-multiply parent. Unsigned byte
line storage is also identical. An unsigned column changes emitted code but
still fails; its nonnegative bounded count semantics are recorded explicitly.

## Full-context results

| Control | Context bytes | Differences including missing bytes | Frame |
|---|---:|---:|---:|
| baseline | 752 | 673 | 44 |
| split_negation | 756 | 711 | 48 |
| subtract_negate | 748 | 654 | 48 |
| nested_negation | 752 | 673 | 44 |
| multiply_assignment | 748 | 655 | 48 |
| height_temporary | 756 | 712 | 48 |
| pitch_minus_gap | 756 | 675 | 44 |
| row_pitch_gap | 756 | 636 | 44 |
| row_pitch_expression | 752 | 687 | 48 |
| row_pitch_full_expression | 748 | 693 | 48 |
| named_width | 752 | 668 | 48 |
| const_width | 752 | 668 | 48 |
| expanded_multiply | 752 | 670 | 44 |
| letter_chain_target | 752 | 667 | 44 |
| letter_chain_forward | 752 | 667 | 44 |
| letter_const_initializers | 752 | 670 | 44 |
| letter_initializers | 752 | 670 | 44 |
| unsigned_col | 756 | 635 | 44 |
| unsigned_line | 752 | 670 | 44 |
| unsigned_col_line | 756 | 635 | 44 |
| expanded_multiply_unshared | 776 | 681 | 44 |
| unsigned_col_unshared | 780 | 712 | 44 |
| chain_0231 | 752 | 667 | 44 |
| chain_0321 | 752 | 667 | 44 |
| chain_2031 | 752 | 665 | 44 |
| chain_2301 | 752 | 665 | 44 |
| chain_3201 | 752 | 667 | 44 |
| letter_chain_target_compound | 752 | 669 | 44 |
| chain_0231_compound | 752 | 669 | 44 |
| chain_0321_compound | 752 | 669 | 44 |
| chain_2031_compound | 752 | 667 | 44 |
| chain_2301_compound | 752 | 667 | 44 |
| chain_3201_compound | 752 | 669 | 44 |

Restoring separate color writes inside each branch gives the original four
static color-store paths, but the repeated constant 63 takes R8 and displaces
the width table again. The expanded unshared control is 776 bytes; its unsigned
column counterpart is 780. They are measured alternatives, not accepted code.

## Remaining instruction differences

The structural candidate loads the letter base as global+4 and performs its
letter initialization before copying bufs and extracting the VM argument. ROM
loads the unadjusted letter base and initializes the letter pointers after the
buffer copy, argument extraction and counter setup. This is directly visible
in the authenticated original prologue and native captured assembly.

The expanded multiply also uses R9 as an intermediate product before moving
62-minus-product back into R9. ROM loads 62 into R9 and subtracts a product held
in a scratch register. Shared versus separate color paths additionally change
constant allocation. Function entry EE9C is still six bytes early in chain_2031;
no shifted tail, partial body, or size match is credited.

## Verification and preserved failures

The final nine-check validation passes allocation and negative coloring,
complete statement ownership, home sorting, priority arithmetic, reachable SH2
storage, both isolated sensitivity models, original prologue authentication and
aggregate strict-object authentication. Seventeen native captures independently
assemble to their strict compiler contexts. All 33 source/object comparisons
retain the unchanged original address and byte gate.

Sixty-eight read-only hardware replays preserve freshly generated control and
captured REG/ILB output: 2,065 allocation events, 8,155 layout events, 226 home
events and 2,647 priority events; 13,093 total. Seventeen home models reproduce
175 records. Hardware breakpoints observe the original compiler; no instructions,
IR, compiler settings or output bytes are patched.

An SSH transfer timed out and the subsequent runner was mistakenly invoked
before the transfer result had been inspected. It failed on the absent runner
file before any compiler job began. That incident is retained in TRANSFER_NOTE;
retries waited for terminal failure and then successful transfer. Final source
semantics explicitly excludes the nested-assignment probe described above.

The frozen case is tools/shc_probe/cases/session_vm_ties229/. Checkpoint 225's
two independent clean builds and 266 tests per checkout remain the last full
production suites. They were not rerun for research-only changes. Current
production pins, report, and both ROM hashes are rechecked on both checkouts.

## Next action

Keep chain_2031 and chain_2031_compound as the paired controls. Move the real
initializations toward the original prologue ordering and investigate loading
the unadjusted letter base without losing its recovered slot permutation.
Then compare the native late assignment/operand selection for the two multiply
spellings and the color constant's allocation with the original instruction
stream. Avoid another blind priority or declaration-order sweep: the allocation
mechanisms are now measured. Complete function bytes and original boundaries
remain the acceptance requirement.
