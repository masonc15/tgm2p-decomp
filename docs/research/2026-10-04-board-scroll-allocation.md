# Board-scroll allocation — continuation185

Continuation184 made evidence progress by ruling out genuine predecessor contexts
for score and row drawing. This continuation investigates the separate 27B50
board-scroll allocation problem. It adds **zero matching C bytes**.

The historical `session_boardscroll_receipt19/unchanged_state_2_0.c` was freshly
compiled and authenticated against its frozen source and resolved object. It still
emits 792 bytes for the 800-byte target. The original ROM retains base, attributes
and counter in registers and has an 8-byte frame; the candidate retains extra
constant/address values and spills these variables with a 20-byte frame.

## Native controls

All 16 ordinary C controls fail the unchanged strict range verifier. Counts below
include missing target bytes; longer contexts compare the nominal 800-byte window
but cannot pass the function-end check. No normalized instruction score earns C
credit. All control functions begin at the nominal 27B50 root.

| Source | Compiled bytes | Nominal differing or missing bytes |
| --- | --- | --- |
| baseline | 792 | 534 |
| limit_int | 806 | 692 |
| limit_short | 792 | 534 |
| limit_unsigned_short | 802 | 724 |
| mask_long_literal | 792 | 534 |
| mask_split_types | 792 | 534 |
| limit_and_result_mask | 808 | 713 |
| baseline_state_pointers | 792 | 533 |
| limit_int_state_pointers | 806 | 737 |
| volatile_state | 802 | 693 |
| limit_scroll_mask | 796 | 698 |
| for_pointer_updates | 804 | 609 |
| indexed_table | 828 | 737 |
| indexed_arrays | 972 | 762 |
| input_alias | 792 | 534 |
| input_consumed | 804 | 643 |

The limit controls consume actual local values in the clamp comparison, store and
final attribute mask. Long literal spelling and a short limit leave the complete
baseline output unchanged. State-pointer and volatile controls, and real array
indexing/loop-update rewrites, do not match. The input-copy controls use a local
board pointer initialized from the actual parameter, either separately or inside
the first consumed array-address expression. Neither matches. No physical register
binding, dummy consumer, padding, compiler patch or new compiler flag was used.

## Read-only compiler evidence

Four complete SHCMDL graph replays authenticate 24, 25, 26 and 25 candidates for
baseline, int limit, int limit plus result mask, and for-loop pointer updates.
Independent models reproduce every profit, sort order and positive allocation.
Four additional priority replays reproduce all **75 variable totals from 495
weighted contributions**. All eight traces preserve REG/ILB relative to both fresh
untraced controls and native compiler captures: **2,352 events** total.

In the baseline, the repeated 0x3F00 literal has priority 48, the cell pointer 41,
the table pointer 25, the three main array pointers 17, and base/attributes only 9.
The two state-address candidates have priorities 13 and 15. Naming the clamp limit
splits the graph: the real variable and remaining literal each have priority 17.
Naming the result mask creates another candidate without removing the spills.
Moving pointer updates into the loop increment raises pa/pb/px from 17 to 25 and
pc from 41 to 49; base gets a register, but the overall output still does not match.
These observations distinguish actual source-priority changes from blind retries.

An earlier allocation decision is now directly visible. At the sorted-table
snapshot no candidate is assigned. Before the general allocator at 0x414D48,
the input board-pointer candidate (symbol 55) alone has logical home 1. Native REG
maps it to **R4** in all four traced controls. The original ROM instead copies its
input from R4 to **R10** at 27B62 and uses R4 for the cell pointer. Changing later
constant priorities alone does not address this initial assignment.

## Validation and receipt qualification

`verify.py` independently resolves original ELF relocations, checks allocated
sections/alignment, original/linked function roots, source and output hashes,
strict rejection, native capture sections, trace outputs and all model results.
It authenticates all 52 checkpoint183 production pins and the unchanged ROM/report.

Two consolidated for-pointer-update trace records were overwritten by a later
local-to-remote sync. The actual event streams, native captures, fresh control and
traced outputs, successful stage-exit logs and replay stdout survived. The two
records are explicitly marked reconstructed; their evidence was revalidated from
those preserved artifacts. No native result was inferred from missing records and
no extra replay was counted. The first verifier run also found the isolated checkout
lacked the old receipt19 directory; historical comparison now uses authenticated
hashes and a copy of its existing freeze instead of assuming that directory exists.

Production remains **130,980 / 200,698 bytes (65.2622348005%)**, with **69,718
remaining**, including all 1,638 runtime assembly bytes. Both remote checkouts
retain the original ROM hashes. Checkpoint183's 251-test full-suite result remains
historical; no full suite was rerun for rejected diagnostic sources. The literal
100% goal remains active. Work stays solo, without brainstorming.

Next watch the board-pointer candidate's assigned-register field between the
0x405ED1 sorted snapshot and 0x414D48 general allocation entry. The intervening
calls include 0x405220 and 0x405EE0. Identify the actual writer and its parameter-copy
criterion, comparing baseline with the already compiled consumed-input control.
This is a new causal question; do not repeat the recorded mask, pointer, qualifier,
loop, indexing or simple input-alias source controls.
