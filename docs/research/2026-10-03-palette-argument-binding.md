# Palette argument binding — continuation 127

Production remains checkpoint 122: **128,316 / 200,698 bytes (63.934867%)**.
No new C bytes are accepted. This pass recovers the palette helper's original
register choices through real argument conversions and narrows the remaining
instruction-order difference to six operations.

The previous turn is classified as progress: it closed callback-prototype and
busy-loop hypotheses for A226, and selected the neighboring ACDC helper from
specific assembly differences. This pass investigates those differences in the
frozen, genuine 36-function menu context from continuation 125.

## A concrete declaration mismatch

The draft declared `f_2b0ea(short, long)` and passed its assembled value directly
as a long. The retained implementation in `src/f_2afc0.c` instead accepts an
unsigned-byte palette ID and a `short **` list. Its queue consumer interprets the
second value as that pointer. The retained `f_2b3e8` parser builds the same kind of
pointer from two words and explicitly converts it before calling.

Three interface controls isolate the effects:

| Call interface | Palette local | Assembled-value local | Call instruction for ID |
|---|---|---|---|
| Original draft: short, long |R4|R5|Direct argument|
| Short, pointer, with actual pointer conversion |R4|R13|Direct argument|
| Byte, pointer, with actual pointer conversion |R11|R13|MOV R11,R4|
| Unspecified parameters, pointer argument |R11|R13|EXTS.W R11,R4|

The last row reproduces the original palette-ID and value registers, its
sign-extension instruction, and the five callee-saved registers. No register
qualifier, physical register binding, padding, or dummy use was introduced.
An explicit int/pointer prototype produces exactly the same native sections as
the unspecified-parameter control. Therefore, this demonstrates the importance
of argument conversions, not a unique reconstruction of the original prototype.
These interface controls remain diagnostic; no cross-unit interface change was
integrated into the retained callee or its callers.

## Direct compiler observation

Three fresh driver captures reproduce their native object sections, ignoring only
compiler comment metadata. Six normal/debug middle-pass replays reproduce the
complete captured REG and ILB outputs for all 36 functions. Three hardware traces
observe the unmodified SHCMDL allocation table before and after outgoing argument
binding, incoming parameter binding, and general allocation.

The native call sites at 414D34 and 414D39 bracket routine 405220. The former is
its CALL instruction; the latter is the next instruction after it returns.
The observations for ACDC are:

| Stage | Draft `a`, `v` | Converted arguments `a`, `v` |
|---|---|---|
| Before outgoing-argument binding |Unassigned, unassigned|Unassigned, unassigned|
| After outgoing-argument binding |R4, R5|Unassigned, unassigned|
| Before general allocation |R4, R5|Unassigned, unassigned|
| After general allocation |R4, R5|R11, R13|

Both versions have the same nine logical-register priorities and allocation
profits. The shift is caused by early argument binding, not a change in priority.
All 27 target profit checks pass across the three captured sources. The compiler
trees show direct identifier arguments in the draft and pointer/int conversion
nodes in the converted case. The saved inferred 405220 listing supports the
identifier-node gate; the before/after hardware observations establish the actual
binding changes. Traces use execution breakpoints and memory reads only. Every
traced REG/ILB output equals its control and native capture.

## Count and sequence address expressions

The original first loads a pointer to the count member at offset 20. Naming that
real pointer for the first two count reads reproduces this address calculation.
The final negative-selection correction still rereads through the global font
pointer, as in the original.

The font's sequence pointers start at byte offset 24. Expressing the access as
`((short **)g_60792f0)[*sel + 6]` reproduces the original indexed load with a
24-byte displacement. This uses the actual 32-bit pointer layout; no unused
object or arbitrary displacement was inserted.

The resulting `count_word_sequence.c` has 64 reachable instructions, or 128 code
bytes, just like the original. A diagnostic control-flow walk separates code from
literal pools. Instruction identities agree outside indices 28–33. Within those
six positions, the same six operations appear in a different order: loading the
font pointer, scaling the selection, adding its address, loading the selected
sequence, and loading the callee address and marker constant.

This diagnostic comparison resolves branch destinations to instruction indices
and literal loads to their values. **It is not byte-match verification and awards
no coverage.** The native helper begins at AC94 instead of ACDC, 72 bytes early.
Its interval through the next function is 168 bytes instead of 164. Pool placement
and the six-operation schedule remain different. Every emitted case fails the
unchanged original-address/original-byte verifier.

A commuted `i[array]` spelling was also tried. SHC rejects it with errors 2200,
2233 and 2222; its source and log are preserved. No object is claimed for that
case. Seven native compilations and this one rejected source are recorded.

## State and next work

`session_palette_interface127/ANALYSIS.json`, `LINK_ANALYSIS.json`,
`REGISTER_ANALYSIS.json`, and `BINDING_ANALYSIS.json` bind the conclusions to the
source, native objects, instruction walk, compiler tables, and strict rejection.
Source preparation reproduces all inputs. Production rechecking passes all 44
pinned files, original ROM/report hashes, and twelve prior frozen cases in both
nuada checkouts. No production source, compiler, map, verifier or denominator
changed. The prior independent clean builds and 243 enabled tests remain the
production validation; no fresh full suite is claimed for private experiments.

The next useful application is the preceding A608 routine, whose size and pools
keep this helper displaced. Its draft declares `f_253bc`'s fourth parameter as
`struct anim *`, while the retained definition in `src/objpool_2528c.c` takes a
long and stores the address bits. Inspect that real call interface and its three
consumed address arguments with the same binding method. Also retain the precise
six-operation ACDC schedule as a target for later generator/scheduler analysis.
Do not repeat the rejected commuted subscript or treat instruction identities as
accepted bytes. The literal 100% goal remains active with 72,382 bytes incomplete.
