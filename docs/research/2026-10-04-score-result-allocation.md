# Score result allocation — continuation169

The remaining score mismatch is reproduced by the original compiler's result
allocator, before the address scratch selection studied in168. This case
explains121 observed result-register selections, their preferences, inherited
masks, and the121 actual result-slot writes. It adds no matching C bytes.

## Result allocation before address allocation

The caller at `0x421137` prepares the scratch-selector arguments inside
`0x420900`. The new hardware-breakpoint tracer records the node, first child,
second child, their128-byte metadata records, recipe, inherited mask, dependency
mask, slot index, preference inputs, and all four scratch-cache records. It
observes the selector return at `0x421159` and the stored register immediately
after the original write at `0x421165`. No general register, compiler data, or
instruction is modified.

All121 observed allocations have category `0x180000` and selector order0. The
exclusion argument equals inherited mask OR dependency mask. Its inherited low16
bits match the node metadata at offset `0x16`. The preference calculation matches
the original instructions for all121 observations:

- When the result slot's use count is2, use node metadata word `0x12`.
- Otherwise, if the inversion flag is set, use its inverted low four bits.
- Otherwise, prefer no register.

The general category, fallback, and floating-point branches are not claimed.
The selector model from163 predicts all57 baseline,8 reduced-guard, and56
local-int-mask events. Every result is then stored in the observed slot. Each
of the three fresh codegen replays preserves `.sua` and `.asa` against its saved
untraced control; `.sua` also matches the original native capture.

## The threshold and level results

The threshold load is a promotion node (opcode32) over the signed-short global
child (opcode112, source symbol57, symbolic-memory operand kind13). It has no
preferred result register in any of the three cases.

| Input | Threshold exclusion, low16 | Threshold result | Following level result |
|---|---:|---:|---:|
| Baseline |`0x7cf0`|R1|R3|
| Reduced guard |`0x40f0`|R1|R3|
| Named int mode mask from167 |`0x7cf0`|R2|R0|

In the baseline threshold snapshot, R2 retains the global-address cache entry
and R3 retains4096. R0 and R1 both have selector rank3; the descending scan breaks
the tie in favor of R1. R2 ranks5 and R3 ranks4, so neither wins. This is not an
additional child-operand reservation or a hidden preference for R1.

In the int-mask control, R1 holds the global address, R2 is empty and unpenalized,
and R3 is the last selected register. R2 has rank2 and wins. The subsequent
address scratch request selects R3, as recorded in167/168, but the level result
then chooses R0. Thus the control still does not reproduce the complete target
sequence, whose threshold-address/result/level registers are R3/R2/R1.

The trace closes the missing explanation for the result side of that partial
pair. It does not make modifying the recorded cache state a valid source fix.

## Native controls

Two new source variants name the level value, as unsigned short or int, and
assign it exactly at its existing short-circuit evaluation point. Both types
represent all possible unsigned16-bit field values. Both variants consume the
value in the actual comparison and preserve the following function's source.

| Control | Raw differences in target range | Context bytes |
|---|---:|---:|
| Baseline, SHC5.0r32 |19|1456|
| Named unsigned-short level |667|1460|
| Named int level |667|1460|
| Unchanged baseline, SHC5.0r10 |19|1456|
| Unchanged baseline, SHC5.0r26 |19|1456|

Both level locals shift the following root by4 bytes and fail strict boundaries.
Their full compiled contexts differ from each other at11 bytes; equal scores do
not mean equal output. The baseline target's766 bytes are identical under
r10/r26/r32. The r26 entire context matches r32; r10 differs at363 bytes in the
following function, starting at context offset794. The verifier checks these
scopes explicitly. The previously tested r28/r31 baseline contexts are frozen
in167. Production compiler selection is unchanged.

The following `f_7956` retains the unresolved inherited `f_17834` prototype.
These controls do not establish a clean ABI for that function.

## Evidence and next action

Evidence is in `tools/shc_probe/cases/session_score_results169/`. `verify.py`
regenerates the source variants, independently checks ELF root addresses and
native bytes, validates the compiler executable/instruction pins, and checks all
traced preferences, exclusions, selections and slot writes. Replay control
outputs are retained for direct comparison. No production source is changed.

The two compiler versions tested here do not explain the score discrepancy;
stop repeating baseline5.0 version controls and single-use named threshold/level
locals. The result and address selectors are now understood for this corpus.
The next useful question is what causes the temporary register history around
the preceding immediate bit test: caller `0x4239f9` requests scratch results
before/after the global-address request, without a new low-level emitted
instruction. Trace these requests back to their node and cache invalidation
operations. A source hypothesis must explain the complete R3/R2/R1 sequence,
not only the address/result pair.

Production remains checkpoint161:128398/200698 (63.9757247%),72300 remaining,
including1638 runtime assembly bytes. The verification receipt checks46
production pins and55 prior freezes in both Nuada checkouts. The unchanged
full-build/test evidence remains checkpoint161's245 enabled tests and independent
clean builds. The literal100% goal remains active, solo, without brainstorming.
