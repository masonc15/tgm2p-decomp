# SHC register allocation: observe the scores before changing source

Continuation76 follows the literal-pool investigation. Production checkpoint73
remains 125,264 / 200,698 bytes. No new matching bytes are claimed.

## New capability and verified result

The original shcmdl.exe contains allocation diagnostics. Its common-options
structure stores a diagnostic mask at offset0x12c; bit0x02000000 prints the
logical-register list, priorities, profits, lifetime conflicts, intermediate
tree and physical-register table. In a private replay, changing only this
configuration bit produces the diagnostics while leaving both.reg and.ilb
byte-identical to the normal captured pass. No compiler executable is patched.
Production builds never consume the replay configuration or its outputs.

Four cases each pass an ordinary replay and a diagnostic replay: reduced_old,
reduced_reordered, frames_pointer and frames_indexed. The first two are the
previous turn's diagnostic reductions; the latter two are new genuine complete
medal contexts. The initial harness expected exit0, but successful SHC component
passes return1 under the normal driver too. The existing successful strace
verified that convention before the assertion was corrected. The failed log is
preserved; intermediate-byte equality remains mandatory.

Static inspection of shcmdl's function0x411430 gives:

    profit(value) = priority(value)
                    - integer_average(priority(conflicting values))

The integer average is zero for an empty conflict list. This is a register-use
ranking, not simply a frequency count. The decoded diagnostic records reproduce
the formula for all 59 logical registers across the four first-function examples.
Function0x4074e0 sorts the register candidates by descending profit;0x406450
walks candidates under physical-register and interference constraints. Equal
scores are handled by its particular partition sort, not a demonstrated stable
source-order rule. We have not yet reproduced the complete allocator.

## Why the new candidates fail

Moving just the frame-table address earlier is semantically reasonable, but
changes the values presented to the allocator:

| Role | Original reduced source | Explicit advancing frame pointer |
|---|---|---|
| Frame traversal | Compiler temporary, profit11, R14 | Named frames, profit-1, R8 |
| Icon base | Named t, profit-2, R8 | Named t, profit-1, R9 |
| Context size |572bytes in full original|572bytes|
| Full-context original roots |216D0,217A2,21862|216D0,217A2,21862|

frames_pointer assigns both table addresses before the loop and consumes the
frame pointer with*frames++. It gets the desired pool order and preserves both
following functions byte-for-byte, but has 43 differing bytes in216D0–217A2.
The unchanged default verifier rejects it at216D6. It is not integrated.

frames_indexed uses frames[i] instead. Its context grows to576bytes, moving the
following roots to217A6 and21866; the default boundary check rejects it. The
compiler retains an extra address setup and changes the tied register scores.
These are useful counterexamples to the assumption that earlier address setup
alone preserves the original allocation.

The earlier reordered example also keeps the loaded frame value live across
the shift-helper call; its frame-size comment is60, compared with56 for old.
Moving the base address instead avoids that particular increase, but does not
preserve the allocation ranking. Both effects must be accounted for.

## Reproducibility and implementation locations

Artifacts are in `tools/shc_probe/cases/session_register_stages76`:

- ExportSHC.java and headless logs:805 shcmdl and628 shcgen function exports,
  zero reported decompiler failures. These are inferred pseudocode, not original
  compiler source or a verified recompilation of the compiler.
- replay.py and REPLAY_RESULTS.json: four normal and four diagnostic middle-pass
  replays, configuration-byte changes, compiler hashes and exact output checks.
- decode_reg.py and REGISTERS.json: a decoder based on the actual output writer,
  lossless binary round trips and all59 profit/conflict checks.
- Source, normal-driver native artifacts, pass captures and default verifier
  results for both rejected candidates. Earlier captures remain in frozen75.

Relevant shcmdl addresses:

| Address | Responsibility |
|---|---|
|0x40ee90|Reads the diagnostic mask from common options.|
|0x414bf0|Coordinates register allocation and diagnostic dumps.|
|0x411430|Computes priority minus mean conflicting priority.|
|0x4074e0|Sorts candidate pointers by descending profit.|
|0x406450|Assigns physical slots under register/interference constraints.|
|0x40ef50|Selects the physical register mapping table.|
|0x421a50|Writes logical number, physical number and type to.reg.|
|0x422540|Writes associated source-symbol IDs.|
|0x422000|Writes lifetime-range count and endpoints.|
|0x4219b0/0x421a10|Write function header and matching end marker.|
|0x4227f0|Prints priority, profit, source association and conflict links.|

The decoder consumes the whole stream, verifies matching function markers and
range bounds, then re-encodes the exact original bytes. It does not rewrite any
compiler output used by the project. shcgen has been imported for the next stage
of address-placement analysis; no claim about its internal algorithm is made yet.

## Next action

Use these logs to trace priority construction and tie ordering, then check
shcgen's placement of the two hoisted addresses. The target needs the original
high-ranked frame traversal and low-ranked icon base while presenting the frame
literal first. Apply the same diagnostic replay to the2E4C register-only mismatch
as an independent benchmark; it should reveal whether the remaining difference
is candidate rank, conflict constraints, or later instruction selection.

No production source, compiler binary, split, verifier or denominator changed.
Checkpoint73's228-test suite and two clean builds remain the last full validation.
These private experiments are not a replacement for the integration gate.
