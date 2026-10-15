# Bank setup scratch-register history

Continuation 114 explains the bank input's R2 choice, but adds no matching C.
Production remains **127,816 / 200,698 bytes (63.68574%)**. The current checksum
source has 122 differing bytes in f_30880; continuation 113's allocation-correct
diagnostic control has 223. These are distinct baselines.

## Measured cause

At the unsigned-word load immediately before the left shifts, SHC calls its
scratch selector from 0x421159. In the diagnostic control, R3 has cache flag
0x40 and is the last-used register. R2, R1 and R0 are empty and unpenalized.
The recovered ranking gives R3 rank5 and the other three rank2. The descending
scan selects R2. All input shifts consequently use R2 before copying to R4.

The preceding scratch call returns to 0x40182f inside the control-flow recipe
at 0x4016a0 and chooses R3. The decompiled recipe constructs a label-address
operand. This supports attributing the cached symbol to branch generation;
the cache flags and selector result are directly observed, while that symbol's
semantic identity is inferred from the caller implementation.

Moving the real loop-index initialization before the bank calculation leaves
the relevant scratch state unchanged. In contrast, an equivalent audio early
return removes the cached R3 value and leaves R2 last-used. R3 now has rank2,
R2 rank3, and the actual compiler chooses R3. This confirms that the history
matters, but still does not produce the ROM's R1.

Offline counterfactuals show that making R2 last-used, recent, occupied or
excluded would select R1 in the baseline state. No compiler state was mutated
to obtain a candidate. These model results are conditions to explain, not
source fixes or matching credit.

## Bounded source controls

Nine full native contexts were compiled with the original toolchain:

| Control | Target differences | Context bytes |
| --- | ---: | ---: |
| Allocation-correct baseline from 113 | 223 | 1220 |
| Consumed short flags value | 381 | 1212 |
| Consumed wide flags value | 381 | 1212 |
| Early loop-index initialization | 268 | 1216 |
| Chained hi/lo zero initialization | 222 | 1220 |
| Chained lo/hi zero initialization | 223 | 1220 |
| Audio early return | 359 | 1232 |
| Consumed short zero initializer | 223 | 1220 |
| Loop-index value initializes sums | 387 | 1212 |

The short/wide flags controls are byte-identical. The lo/hi chain and short-zero
control each reproduce the diagnostic baseline exactly. All nine fail ordinary
full-function verification. None improves on the 122-difference production
context. Their actual linked roots and neighboring-range failures are recorded;
equal context lengths are not treated as proof of matching boundaries.

## Evidence

`tools/shc_probe/cases/session_checksum_scratch114` contains nine independent
native-capture comparisons, 18 exact normal/debug middle replays, 695 logical
register score checks and six unchanged-compiler hardware replays. The scratch
model predicts all 586 observed choices; 567 first-function emitter events
confirm the bank input registers. `BANK_SCRATCH.json` preserves the exact states
and emitted operations. `SCRATCH_COUNTERFACTUALS.json` is explicitly offline.

Normal/traced GEN outputs agree, and emitted `.sua` equals the native capture.
Cleanup-captured `.asa` metadata differences remain recorded. Production checks
on both remotes confirm all 37 checkpoint112 pins, the original ROM hashes and
unchanged progress. No production source, split, verifier or compiler changed;
no full production suite was rerun for these rejected research candidates.

## Next target

Park the checksum controls until a distinct explanation predicts the required
R1 state without disturbing the recovered allocations. The current remote
progress report confirms the 172-byte task allocator at 2452E–245DA remains
assembly. A fresh compile of the current production task context reproduces
its six register-byte differences, with the 1574-byte preceding and 718-byte
following ranges unchanged. The exact source and native result are saved as
`task_current.*`; the remote report snapshot is `CURRENT_REPORT.json`.

The task allocator loads the free-slot pointer into p/R6 and copies it into
t/R7; the ROM loads t/R7 and copies it into p/R6. Earlier pointer spellings,
clear helpers, return types and table types are closed in case57. The next
action is to inspect the compiler's optimized assignment tree and coalescing
for these two genuine pointer values, then select a source experiment from
that evidence. Do not repeat the historical permutation search. Literal 100%
remains active and incomplete, including all 1,638 runtime bytes.
