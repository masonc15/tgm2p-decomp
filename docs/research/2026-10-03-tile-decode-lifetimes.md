# Tile decoding lifetimes: corrected roles and a pre-allocation row cache

Checkpoint84 remains **127,276 / 200,698 bytes (63.41668%)**. Eight new private
source controls preserve the preceding432 matched bytes but fail the unchanged
full-function gate. No production bytes are added.

The most informative source is `session_tile_decode88/split_both.c`. Separate
signed-short decoded values feed the existing four signed divisions in the
original order. Both quotient lifetimes are shorter. Compared with86's input,
x priority falls7→5 and y8→6. The inner py value receives the original R8.
The original divisor R13 and zero R12 also return, alongside iterator R5,
xx R6, yy R7, px R14, record R11, tile R10, bit512 R9, and mask15 R4.
The following function begins at the correct2769E; positioned target byte
differences fall416→330. This is still a negative candidate with a36-byte frame
instead of32, incorrect entry allocation/scheduling and incorrect decode storage.
Original and candidate each contain227 reachable instruction words and four
calls, so those counts alone do not establish matching.

## Bounded source controls

The input is frozen86's record_byte_compound, copied into this case. The first
three controls combine the signed-word decode and division in one assignment.
The next three introduce a real decoded_x and/or decoded_y short, then assign
the quotient at the original division position. The signed-word conversion is
preserved before division, including negative values. No dummy read, consumer,
branch, padding, qualifier or physical-register forcing is introduced.

| Source | Frame bytes | Following dispatch | Positioned byte differences |
|---|---:|---|---:|
| Combined y |40|27696|437|
| Combined x |40|2768E|447|
| Combined both |36|2769C|417|
| Separate decoded y |40|2768E|450|
| Separate decoded x |40|27690|452|
| Separate both |36|2769E|330|
| Original decoding, word-index destination |36|2769A|421|
| Separate both, word-index destination |36|2769C|395|

The final two controls restore the natural longword index expression on these
newly allocated contexts. This tests whether changing address units removes the
extra cached row offset. Both fail to do so. Old standalone destination cases
were inspected before these combined controls; their unchanged source forms
were not reswept.

## Extra stack word traced into compiler IR

Original ROM27634–2763E calculates `(px & 15) << 7` inside the successful
clipping branch. In split_both, native275F6–27602 calculates the same row offset
before the column loop and stores it atSP+16.27640 reloads it for the tile store.
The original uses no such cache. This accounts for an extra four-byte live
memory value, while other stack slots are also rearranged.

Crucially, the compiler's **register allocation before tree** dump already
contains the synthetic unsigned-long symbol−3 assignment for the mask/shift,
outside the column loop. The physical allocator consumes an existing temporary;
it does not create this computation. The saved tree excerpt and exact native
opcodes are checked by audit_target.py. Negative symbol numbers are compiler
identifiers scoped to this function, not source variables or stable global IDs.

The next action is to observe the earlier SHCMDL optimization pipeline
(0040F8B0, before its00414BF0 allocator call) and identify the first stage that
introduces this row-offset assignment. Inferred pipeline/entry routines are
preserved, but **the creating pass is not yet established**. If the temporary
already exists at SHCMDL entry, inspect the preceding compiler stage instead.
Use unchanged-output replays to authenticate observations. Do not assume the
allocator sort or another destination-expression permutation can remove it.
Retain split_both and the earlier compound-decoding baseline as complementary
controls: their decoded-value storage differs from the original in different ways.

## Validation and reproduction

Eight source controls regenerate exactly. Eight independent native driver
captures match original compile sections excluding compiler comments. Sixteen
normal/debug middle-stage replays reproduce their captured.reg/.ilb exactly,
and all230 WEB CHAIN profit checks pass. Symbol aliases join diagnostic roles;
logical register numbers are not assumed stable. Five CFG-based disassemblies
exclude literal-pool data. Seven variable roles and four constant assignments
are asserted explicitly for split_both.

Evidence: tools/shc_probe/cases/session_tile_decode88. Run prepare.py, run.py,
capture.py --watch, replay.py, analyze_named.py, disassemble.py, audit_target.py.
The capture/replay summary JSON files describe the final two-control batch;
per-control native files, logs and outputs cover all eight and are independently
checked by analyze_named.py. No new full-game or unrestricted portable-C proof
is claimed for these rejected experiments.

Both production checkouts freshly pass all33 checkpoint84 pins, original ROM
hashes and current reports. The234 enabled tests and two clean builds remain
historical integration evidence from84; no full build was rerun for private
negative candidates. The full100% goal remains active with73,422 bytes remaining.
