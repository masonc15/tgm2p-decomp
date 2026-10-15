# The conversion recipe behind the 2E4C register mismatch

Continuation79, checkpoint73 unchanged at 125,264 / 200,698 bytes.

A fourth hardware execution breakpoint now records shcgen's recipe builder
at 0x420900. It reads the node, its children, allocation metadata, and recipe
bytes before temporary selection. No compiler code, configuration, or data is
modified. The earlier selector/emission breakpoints remain active. Both
baseline and promoted-control runs preserve all 63 previously observed selector
inputs and outputs, as checked by analyze.py. Traced/control generator outputs
agree; the original captured .sua still agrees. The previously documented
cleanup-snapshot .asa offset limitation remains.

## What the live trace establishes

The disputed allocation is for the conversion node immediately before the
comparison, not the comparison's own result. The node opcode is 0x20, with
source type byte0x08 and result type byte0x10. Its recipe address is0x44e438,
header bytes00 03, and first allocation descriptor0x180000. Original x86 and
inferred pseudocode identify this descriptor as the ordinary unrestricted
allocation path (descending scratch scan, no forced register number).

| Captured state | Baseline | Promoted int variable control |
| --- | --- | --- |
| Node metadata preferred mask at +0x12 | 0 | 0x10 (R4) |
| Node metadata available result mask at +0x14 | 0 | 0 |
| Live mask at +0x16 | 0x70f0 | 0x70e0 |
| Cached destination at +0x74 | absent | R4 |
| Conversion outcome | new R2 temporary | existing R4 destination |

Baseline allocation24 receives exclude0x70f0, preferred0, order0. Since R3 was
last selected and the recent mask is empty, the already verified selector
chooses R2. The promoted control has an assigned/cached R4 destination and
therefore does not perform a new scratch allocation for this conversion.

This explains why casting the comparison constant is ineffective and why the
promoted-variable workaround changes more than one register. A useful source
change would need to change conversion reuse or the earlier scratch state,
while preserving the matched instruction sequence and actual function roots.
The original source that produces R1 remains unresolved.

## Two additional source checks

A consumed short local copy used for the comparison gives94 differing bytes in
the nominal context and shifts the following function root. An assignment to a
short local consumed directly by the comparison reproduces the original240-byte
context exactly, including its two rejected bytes. Neither supplies the missing
preference. Both sources, original objects, linked images and default-verifier
rejections are retained in session_comparison_recipe79. No inline assembly,
physical-register binding, padding or dead liveness hint was added.

## Next direction

Keep this benchmark and its observed allocator states pinned. Avoid more
unmotivated comparison casts or short-variable spelling changes. Apply the
compiler-stage tools to the medal loop's known combination of literal ordering
and long-lived allocation: that frontier needs the frame address referenced
first while retaining the original induction-variable allocation. Check the
existing full-context controls before proposing a new source form.

ANALYSIS.json checks the captured recipe states, unchanged selector traces, and
short-assignment native identity. recipe_builder.x86 preserves the original
machine instructions behind the inference. freeze.py checks all27 production
pins and both ROM hashes. Last full integration validation remains checkpoint73's
228 enabled tests and two independent clean builds. No new production credit.
