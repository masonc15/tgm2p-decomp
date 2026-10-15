# VM initialization and prologue order — checkpoint 230

Production remains checkpoint 225: **135,594 / 200,698 bytes (67.56121%)**.
No new C bytes were admitted. 65,104 bytes remain, including 1,638 runtime bytes.

## Evidence and result

The frozen `session_vm_prologue230` case compiles 24 semantically eligible
source controls. All fail the strict EC0C–EF04 760-byte context gate; no shifted
range, normalized instruction, or prologue score is accepted as a match.
Two additional controls reproduce native diagnostic 2130: this SHC version
rejects the dynamic structure declaration initializer. Neither creates an object.

Thirteen native captures are independently assembled back to the exact bytes
of their corresponding strict compile. Thirty-nine hardware-breakpoint replays
preserve REG and ILB output against fresh untraced controls and native captures:
1,561 graph events, 6,131 layout events, and 169 home events (7,861 total).
Positive and negative allocation models, complete node ownership, and native
frequency grouping/sorting all reproduce observation. Home models cover 130 records.
Seven validation commands pass. This is research validation; the full 266-test
production suites were not rerun because production source and pins did not change.

## What changed the next action

Moving buffer initialization before VM argument extraction improves instruction
order. Moving letter initialization later changes allocation ties: compound and
expanded multiply forms become byte-identical for late letters, buffer-first,
buffer-pointer-first, and independent late-letter controls. Priority alone no
longer predicts the earlier y/width swap when lifetimes change.

`chain_2301_compound_late.c` uses the genuine pointer dependency chain 2→3→0→1
and preserves all original stack slots with late letter initialization. The
`zeros_row_col.c` child uses `row = *(&col) = 0; i = 0;`, restoring the late
`MOV R1,R11` counter initialization. Both preserve the 44-byte local frame,
buf R14, sprite-base R13, x R12, i R11, 254 R10, y R9, and width-table R8.
Stack offsets remain spr 0, col 4, line 8, row 12, letter3 16, letter0 20,
letter2 24, letter1 28, done 32, str 36, with bufs at 40.

The entry diagnostic decodes instructions up to the first conditional branch
and normalizes only PC-relative loads to their actual literal values. Registers
remain physical. Its edit distance falls from 50 in `expanded` to 29 in
`zeros_row_col`; this measures only a diagnostic sequence, never decompilation
progress. The latter context remains 752 bytes with 606 differing/missing bytes,
and EE9C lands at EE96. Separate zero assignments produce 756 bytes and EE9A;
this still fails the original-address gate.

Independent pointer initializers restore the unadjusted g_6079366 literal but
use the wrong letter-slot order. Base-then-advance similarly restores the literal
while moving row and letter2 homes. No control combines every required property.

## Next compiler investigation

Start from `zeros_row_col.c`, preserving its source semantics and exact stack
allocation. Trace the late multiply/subtraction selection: ROM places 62 in R9
and the product in R3, while this source computes the product in R9 then copies
the subtraction result back. Also resolve the g_6079366+4 literal and letter
initialization order. Shared color handling still differs from the original four
static paths; reopening those paths previously changed constant allocation and
spilled the width table. Allocation similarity alone cannot establish a match.

The authoritative whole-context gate, fixed denominator, production pins, and
original/rebuilt ROM hashes remain unchanged. Goal active; solo, no brainstorming.
