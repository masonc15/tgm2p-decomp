# SHC addition operand descriptors — checkpoint234

Production remains checkpoint225: **135,594 / 200,698 bytes (67.56121%)**.
Three calibration sources and six new source forms all fail the original-address
EC0C–EF04 gate. No new C credit;65,104 bytes remain, including1,638 runtime bytes.

## Direct descriptor evidence

The immutable PE table at0x44A3D8 contains these authenticated words:

| Recipe operand index | Descriptor | Observed interpretation |
| --- | --- | --- |
|94|0x0C400000|register byte at current node metadata+0x2C|
|169|0x10100000|first child's computed operand|
|170|0x14100000|second child's computed operand|

The enhanced read-only emitter observer records GEN node heads, metadata and
children at each recipe-driven addition. The original resolver instructions
and inferred decompiler output are retained, with the latter labeled inferred.
For the eight modeled row-address additions, an independent decoder uses the
raw metadata and original table words to reconstruct both complete12-byte
emitted operands exactly. Recipe flag word is zero in these observations.
This is source-to-emission diagnosis; descriptors and compiler state are not changed.

## What the different source forms do

The baseline's ordinary sum is GEN opcode0x40, type0x80. Its first child is
the computed offset; its second child is the array base. Recipe[96,0,94,169,250,0]
adds temporary R1 into the first-child result R3. The source's commutative
spelling does not provide an independent destination choice.

Explicit char-pointer and void-pointer casts change the GEN result type to0x40,
but preserve the operand order and exact machine bytes. Taking the address of
element0 emits one additional zero-add in GEN; native output is still identical.
Thus the type distinction itself does not solve the destination mismatch.

Seeding line then using += produces opcode0x50 and recipe[96,0,170,94,250,0]:
the second-child offset R2 is added to temporary R1. Its intermediate line
store/reload remains. A named base with += also produces opcode0x50, but uses
[96,0,170,169,250,0], adding offset R3 into allocated first-child base R4.
These are the complementary controls from checkpoint233.

Two sequenced forms test consumption of the assignment result. A comma-sequenced
line assignment becomes opcode0x40 and adds temporary R1 into offset R2.
Consuming a distinct row_base += result also becomes opcode0x40, with offset
first and named base second; it adds R4 into R3. Source semantics are documented
in SOURCE_SEMANTICS.md: the comma operator separates the two writes to line,
and the consumed-base form writes two distinct objects. Neither is the earlier
rejected unsequenced double modification of y.

Subtracting a negated offset preserves base R1 and the later product R3, but
native code retains NEG R3,R3; SUB R3,R1 instead of original ADD R3,R1.
No post-compilation peephole or compiler patch removes this extra instruction.
It remains a strict negative with a744-byte context and702 differing/missing bytes.

## Validation and observer correction

Three validation commands authenticate the PE descriptors, reconstruct eight
addition operand pairs, verify the retained NEG control, and independently
resolve all nine objects/rejections. Nine normal-driver captures are separately
assembled back to their strict object bytes. Three calibration source/context
hashes agree with frozen checkpoint233.

Nine corrected GEN replays record2,668 emissions. Every traced SUA/ASA output
equals its fresh untraced control; SUA also matches the native capture. Cleanup
ASA metadata differences are preserved and independently checked. No compiler
instructions, intermediate inputs, configuration, ROM or production source changed.

The initial observer inverted a fallback flag in its optional decoded operand
field. Its raw node/metadata and emitted bytes were unaffected. Initial outputs
are preserved under initial_observer; the corrected observer was rerun for all
three initial controls. Final validation uses the corrected outputs and decodes
the relevant raw metadata independently. OBSERVER_REPAIR.md records this limitation.

No full production suites were rerun for this research-only change. Checkpoint225's
64 pins, original/rebuilt ROM hashes and two266-test clean validations remain
production evidence, with fresh mirror/hash checks recorded separately.

## Next investigation

The remaining constraint is a base accumulator held in scratch R1, with no
intermediate line store and no allocation of a named base to R4. Pointer type
casts and consuming assignment results have now been tested and do not provide
that combination. Inspect the recipe selection/coalescing conditions for the
ordinary sum against its children, using baseline, line_seeded and
named_base_accumulate as fixed controls. Do not change descriptor words or force
registers. The broader initialization, width reload, color-path and boundary
mismatches remain. Goal active; work remains solo.
