# VM callee lifetimes and recovered subtraction — checkpoint 232

Production remains checkpoint225: **135,594 / 200,698 bytes (67.56121%)**.
All19 compiled contexts remain strict negatives. No new C credit;65,104 bytes
remain, including1,638 runtime bytes. No compiler, object or ROM patches.

## Improved reconstruction

`predicted_chain_0231.c` now combines the original44-byte frame, every stack
slot, all seven saved-register roles, the unadjusted g_6079366 letter-base
literal, and direct subtraction into y/R9 without the previous R3-to-R9 copy.
The multiplication result is separate from y and lives in scratch R2.
The ROM uses R3 instead. This is a better source-level explanation, not a
matched function: the744-byte context still fails the required760-byte gate,
and EE9C lands at EE8E (18 bytes early). Its710 differing/missing bytes are
not an improvement in the strict whole-context comparison.

The recovered roles are buf R14, sprite base R13, x R12, i R11,254 R10,
y R9 and width table R8. Stack offsets are spr0, col4, line8, row12,
letter3=16, letter0=20, letter2=24, letter1=28, done32 and str36; bufs at40.
The verifier authenticates the symbol-to-register mappings, literal values,
negative-home model, original-address context bytes and source provenance.

## Measured source mechanisms

The baseline is checkpoint231's expanded seeded subtraction, whose y has
priority18 but spills behind the priority19 drawing-function address.
A correctly typed local function pointer used by the three actual tail calls
lowers that address candidate to priority17. The four in-loop calls remain
direct calls. This preserves the44-byte frame and puts y in R8, but leaves
the width table in R9. Naming the callee for every call instead adds a pointer
stack home; naming it per character also changes the code and frame.

Naming the real width-table pointer gives it priority16 when initialized once,
or17 when assigned per row. At first, the width pointer spills and either the
callee address or repeated zero constant wins. Chaining the actual completion
and counter initializations as i=row=col=done=0 changes the allocation without
adding effects or dummy uses. With the per-row width pointer and tail-only
callee pointer, y/18 takes R9 and widths/17 takes R8. The44-byte frame returns.
The native sequence becomes MOV #62,R9; product in R2; SUB R2,R9, with no copy
back. The width address reload remains inside the outer loop and differs from ROM.

The new context permutes letter slots. Four measured chains show that the
first three homes follow chain positions2,0,1, with terminal letter1 fourth.
`PREDICTION.json` records the predicted0→2→3→1 chain before that probe ran.
The native result confirms letter3,letter0,letter2,letter1 at16,20,24,28.
Starting at letter0 also restores the unadjusted g_6079366 literal value.
This pattern is observed for these inputs; it is not a universal allocator rule.

Swapping the operands in the source row-address sum produces identical machine
bytes. The ROM accumulates the offset into the base register R1; this compiler
source accumulates the base into offset register R3. That difference leaves
R3 recently used and the next row product selects R2. The exact scratch-selection
cause beyond this sequence remains a hypothesis to trace, not a claimed proof.
Moving width initialization after line/y gives different bytes despite equal
raw counts. Only per-row assignment/scoped initializer and chained-done/commuted
line are verified byte-identical pairs among the recorded controls.

## Verification

Seven validation commands pass. Nineteen allocation, negative-coloring,
complete-layout, home and priority models reproduce native observations.
There are203 home records and76 output-preserving hardware-breakpoint replays:
2,396 graph,9,075 layout,260 home and3,010 priority events (14,741 total).
Every REG/ILB trace matches a fresh untraced control and the native capture.
All19 captures independently assemble to their strict compiler object bytes.
No flags, IR, native code or original-address acceptance rules were changed.

Production pins and original/rebuilt ROM hashes are rechecked on both Nuada
copies. Full production suites were not rerun for these research-only changes;
checkpoint225's two266-test clean validations remain authoritative.

## Next investigation

Use predicted_chain_0231 as the new joint-allocation control. Trace the native
row-address expression and scratch selector to recover ADD R3,R1 and the following
R3 product, while preserving stack slots and saved roles. The per-row width load,
initialization ordering, shared color-path structure and tail boundary still
need reconstruction. Do not substitute a shifted or normalized match for the
whole original-address context. Goal active; work remains solo.
