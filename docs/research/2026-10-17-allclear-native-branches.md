# Line-clear call planning and coordinate ABI

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%). These are compiler observations
and rejected source controls, with no acceptance or coverage change.

## Native planning observation

Packet: `research/2026-10-17-allclear-native-branches/`.
Two archived complete sources from the genuine 0x51f4 context were compiled
unchanged by the ordinary pinned driver. T1 uses two `if/else` calls; Y1 uses
two conditional-expression arms. Earlier evidence established the tradeoff:
T1 merges call tails but gives all_clear R10; Y1 recovers all_clear stack
storage and constant32 R10 but leaves two call tails using different registers.

The registered prediction was that native GEN would expose a history difference
at the two callee scratch selections. Actual before-GEN snapshots support
normal, scratch-observer and emission-observer replays whose SUA, ASA and SUD
streams exactly match native output. The observers use hardware breakpoints
and read process memory; no instructions or operands are patched. Total:
**22 compiler executable stages**, within the recorded 24-stage cap.

Function8 is f_57d4. The calls are identified by their argument setup
(0 to R7, 6/4 to R6, 10 to R5, field to R4) and the original object's resolved
f_179b8 literals. Native traces refine the earlier explanation:

| Source | GEN emissions already made | Selector caller | Incoming last | Selected |
| --- | ---: | --- | ---: | --- |
| T1 first call | 150 | 0x408a1f | 3 | R2 |
| T1 nonempty else | 155 | 0x40182f | 2 | R3 |
| T1 second call | 155 | 0x408a1f | 3 | R2 |
| Y1 first call | 145 | 0x408a1f | 3 | R2 |
| Y1 second call | 145 | 0x408a1f | 2 | R3 |
| Y1 expression selection | 145 | 0x407ac7 | 3 | R2 |

Both Y1 call registers are selected **before either arm is emitted**. The
statement form instead makes its second selection after the first call and an
intervening else callback. This is an extra selection, not a generic reset of
all scratch history. At the two call selections, exclusion is 1, preferred is
0, recent mask is 0, used mask is 0x7ff0, order is 0 and mode is 0. Scratch
ages are [42,17,10,36] and flags are zero. The changing last value distinguishes
the relevant selections. This is observational evidence; no forced-state
output is claimed as a source reconstruction.

The result explains why moving the same calls into an expression removes the
numbered address web but loses tail merging. It does not supply a new valid
source form. Repeating conditional or short-circuit call spelling without a
new planning mechanism remains closed.

## Genuine coordinate declaration control

Packet: `research/2026-10-17-allclear-coordinate-abi/`.
Review of the accepted callee in `src/effect_176e0.c` found that f_179b8 consumes
both coordinates as `short`, while the archived caller declares them as `int`.
One new bounded control changes only those two parameter types to short.
The ROM caller's fourth R7=0 argument remains in place; the callee's unused
parameter count cannot be recovered merely from consumed registers. This is a
coordinate-width control, not proof of the entire original prototype.

The ordinary eight-stage driver and one native-authenticated diagnostic MDL
replay use **nine stages**. The final REG is identical to T1. ILB changes four
bytes, while native SUA and ASA are identical. The entire independently linked
4,068-byte context is byte-for-byte identical to T1. The diagnostic allocation
still gives all_clear priority5, profit-3,14 clashes with sum123. Thus the
coordinate correction does not change the allocation or call-planning mechanism;
stop this control without a type matrix.

## Raw result and boundaries

| Measurement | T1 | Y1 | Short-coordinate T1 |
| --- | ---: | ---: | ---: |
| Context bytes | 4068 | 4092 | 4068 |
| Actual f_57d4 root | 57d4 | 57d4 | 57d4 |
| Actual f_5b80 root | 5b70 | 5b88 | 5b70 |
| Actual f_601a root | 6022 | 603a | 6022 |
| Raw differences 57d4..5b80 | 705 | 545 | 705 |
| Retained prefix 51f4..57d4 differences | 0 | 0 | 0 |
| Retained 601a..61d0 differences | 412 | 427 | 412 |

The following function owns shared literals used by the target. None of these
sources restores that function's nominal root or the later protected helper
range. Original relocations are resolved without rebasing, normalizing scores
or patching output. Each packet's `measure.py` and `verify.py` reproduce the
raw comparisons and diagnostic assertions without launching SHC.

No candidate strict acceptance, required suite, whole-ROM qualification, cold
replay or dashboard publication is claimed. A new hypothesis must jointly
explain all_clear's allocation and statement-style call planning while retaining
real roots and pools. The measured coordinate widths alone do not do so.
