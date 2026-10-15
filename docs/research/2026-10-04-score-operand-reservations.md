# Score operand reservations — continuation168

Production remains checkpoint161: 128,398 / 200,698 bytes (63.9757247%).
This continuation adds compiler evidence and rejects six source controls. It adds
zero verified C bytes. The literal 100% goal remains active.

## What constructs the exclusion mask

Read-only hardware-breakpoint replays of the original SHC5.0r32 executable now
connect the parent recipe planner, operand dependency calculation, and scratch
selector. All eight replays preserve `.sua` and `.asa` byte for byte against a
fresh untraced control. Saved `.sua` also matches the original native capture.
The cleanup snapshot's `.asa` differences are recorded explicitly in each replay
receipt. The tracer changes only hardware debug registers, never compiler code,
data, or general registers.

`verify.py` independently checks 1,376 observed operand dependencies, 185 parent
mask calculations, and all 12 scratch selections. It links each scratch request
to exactly one matching source/destination pair in the separate parent trace.
The complete compiler hash and relevant original instruction bytes are pinned.

The dependency helper at `0x42c190` returns the explicit register bit for the
observed direct-register and fixed-offset-memory operands (kinds1 and8).
Indexed-memory kind9 adds R0. Symbolic absolute memory (kind13), constants
(kind7), and empty operands return zero. This model deliberately claims only the
observed integer operand kinds/registers, not unobserved floating-point cases.

The parent planner at `0x421980` accumulates dependencies selected by its recipe.
At `0x421ebd`, it adds the recipe control word's low four bits. At `0x421ed9`, it
adds source dependencies and combines the signed low16 bits with the inherited
mask. The argument's upper16 bits are not consumed by this calculation.
The child planner at `0x422290` adds source dependencies; at `0x4227d3` it adds
destination dependencies immediately before calling the scratch selector.

| Guard operation | Recipe control | Explicit recipe reservation | Baseline incoming mask |
|---|---:|---:|---:|
| Global flags for immediate bit test | `0x180001` | R0 | `0x7cf1` |
| Signed threshold word load | `0x180080` | none | `0x7cf0` |
| Player level word load | `0x180080` | none | `0x7cf2` |

The baseline threshold source is symbolic absolute memory, kind13. Its result
is R1, giving final exclusion `0x7cf2`; the selector chooses R0 for the address.
The actual player field read is kind8/R14 and contributes `0x4000`, not an
implicit R0 reservation. Inferring an R0 reservation from the eventual indexed
SH instruction was therefore incorrect for this IR stage.

The reduced guard shows the same recipe distinction with incoming masks
`0x40f1` and `0x40f0`. The genuine local-int-mask variant also uses the same
threshold recipe, with result R2 and final exclusion `0x7cf4`. It selects R3 in
the different cache/register state already established in167. No additional
recipe reservation explains its improved threshold pair. Its whole function
still has43 differences and is rejected.

## Source controls

All changes preserve the following `f_7956` source text. Its inherited
`f_17834` prototype remains unresolved; this is not an ABI-clean claim.

| Source form for the level read | Raw target differences | Context bytes | Boundary outcome |
|---|---:|---:|---|
| Baseline |19|1456|Aligned, byte mismatch|
| Byte-pointer plus `0x322` |19|1456|Entire context identical to baseline|
| Target-width integer address plus `0x322` |19|1456|Entire context identical to baseline|
| Named pointer to the field |476|1460|Following root shifts+4|
| Inline-requested unsigned-short accessor |731|1464|First root shifts+6, following+10|
| Inline-requested int accessor |740|1464|First root shifts+8, following+10|

The accessor entries report actual emitted boundaries; they do not infer whether
individual call sites were expanded. No helper bytes, shifted roots, or partial
instruction similarities receive credit. Independent verification regenerates
all source variants, checks their hashes, original ELF addresses with the RAM
mapping, context bytes, and strict rejection results.

## Verification and next action

`tools/shc_probe/cases/session_score_operand168/RESULTS.json` contains the model
checks, guard records, source results and scope limitations. The three trace
sources and replay scripts reproduce the measurements. `PRODUCTION_RECHECK.json`
confirms all46 production pins and54 previous case freezes in both Nuada
checkouts, unchanged ROM hashes, and128398/200698 coverage. All23 reference ROM
files still match the recorded source manifest. No production source changed;
the full clean-build/test gates remain checkpoint161's245 enabled tests and
independent clean builds.

This closes the fixed-field-address syntax and hidden threshold-recipe
reservation hypotheses. A subsequent score experiment must explain the earlier
allocation of the threshold result and level operands, or a genuine source
lifetime that makes R0 unavailable; adding an exclusion bit in the model is not
a solution. Trace the result allocator and child-node live registers before
`0x421980`, using the baseline, reduced guard, and int-mask control as a matched
comparison. Do not repeat the address spellings or accessor controls above.
