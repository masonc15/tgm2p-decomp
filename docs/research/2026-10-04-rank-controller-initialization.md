# Rank controller initialization — continuation 180

Production is unchanged at 130,324 / 200,698 matching bytes (64.9353755394%).
The new clean candidate for `f_12fa8` has 35 differing bytes out of 576, down
from 513. Its context ends at the correct 0x131e8 boundary. All 13 original
roots remain nominal, and every trial preserves all 1,236 bytes of the already
integrated `f_12ad4`. No controller bytes are credited.

## Input and call evidence

Start from continuation179's real matching rank context, with no historical
constant-false writes or invented externs. The inherited controller initialized
`done` and `t` inside a comma expression passed to `f_2164e`. Its context was
12 bytes too long, and SHC spilled the timer instead of retaining it in R8.

The existing medal-loader reconstruction declares `f_2164e(void)`. Independently
relocating its original object reproduces the entire 636-byte 0x21454–0x216d0
ROM context. The straight-line loader overwrites R4–R7 before using them and
does not read incoming arguments. This establishes that it ignores the incoming
argument registers; it does not establish the original C prototype or arity.
The caller ROM still materializes zero in R4 before this call. An early working
hypothesis that removing the argument would resolve the mismatch was too strong.

Four no-argument source controls reject. Chained initialization with no argument
restores the timer register but is four bytes short. Preserving the observed
zero-valued call with `t = done = 0; f_2164e(0);` restores the complete 576-byte
extent and leaves only 35 differences. This is a research candidate, not a new
production ABI declaration.

## Native controls

| Source family | Differences | Context bytes |
| --- | ---: | ---: |
| Inherited comma-expression baseline | 513 | 4404 |
| No argument, separate initializations | 521 | 4400 |
| No argument, either chained order | 522 each | 4388 |
| No argument, declaration initializers | 520 | 4400 |
| Constant-zero call after chained initialization | 35 | 4392 |
| Either chained initialization as argument | 35 each | 4392 |
| Comma expression ending in constant zero | 513 | 4404 |
| Call before chained initialization | 37 | 4392 |
| Four split assignment/argument placements | 513,513,514,513 | 4404 |
| Separate initializations before constant call | 513 | 4404 |
| Named input pointers, two initialization orders | 198,514 | 4388,4392 |
| Timer assignment / preincrement / compound update | 35 each | 4392 |

The three timer-update forms are byte-identical to the constant-call candidate.
The chained-argument forms are not identical despite equal scores: byte0x12fc7
changes the call argument copy into a sign extension. The independent verifier
records actual equality, rather than treating equal scores as equality.

The 35 remaining differences are localized:

- 17 bytes in initialization at 0x12fb2–0x12fc8: ROM materializes zero in R4,
  copies it to R0/R8, and stores done in the call delay slot. Candidate starts
  with zero in R8, then copies to R0 and to R4 in the delay slot.
- 12 bytes from the two input-pointer stack slots: four bytes in initial stores
  and eight in later loads. ROM puts input0 at SP+4 and input1 at SP+0; candidate
  reverses them while retaining the correct load semantics.
- Six bytes at 0x13028–0x1303a: scratch-register choices for the 360 comparison
  and following call differ. Instruction positions and literal values agree.

## Output-preserving home trace

A fresh original SHCMDL trace for function symbol174 preserves REG/ILB output
against both a fresh untraced control and native driver capture. Native ELF
sections equal ordinary compilation except `.comment`. Hardware debug registers
are the only compiler-process state written.

Six events expose three spill frequencies and the before/after ordering. The
input-address webs -3/-4 each have frequency5, independently reconstructed from
recorded uses and loop depth. The done variable (symbol177) has frequency2.
The sort preserves the tied input order and assigns homes -1/-2/-3, corresponding
to the observed input0/input1/done stack layout. This identifies the tie before
stack emission; it does not yet explain how to reverse it through required C.

## Validation and next action

All 20 original objects were independently relocated and compared against ROM.
Every candidate fails the unchanged strict verifier. All protected rank bytes,
50 production pins, current reports and original ROM hashes remain exact.
No full suite was rerun for research-only changes; checkpoint179's249 enabled
tests and independent clean builds remain the last full gates.

Next trace parent/result allocation for the initial zero in the constant-call
candidate, using the no-argument chained control to distinguish call ABI effects
from local initialization scheduling. Explain the R8-versus-R4 choice before
trying more initialization spellings. Inspect creation order of the two tied
input-address webs afterward. Do not repeat the 20 recorded forms or revive
historical dead writes.

Evidence: `tools/shc_probe/cases/session_rank_controller180/`, including the
20 sources and original objects, `verify.py`, `HOME_REPLAY_RESULTS.json`, the
medal-loader object and ROM disassembly, `RESULTS.json`, and `FREEZE.json`.
The literal100% goal remains active, with70,374 bytes remaining including1,638
runtime assembly bytes in the unchanged denominator.
