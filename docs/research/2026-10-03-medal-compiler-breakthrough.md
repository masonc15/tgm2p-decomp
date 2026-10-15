# Compiler priority tracing recovers the medal-draw loop

Verified checkpoint80 adds the complete 210-byte function at 216D0–217A2.
Matching coverage is **125,474 / 200,698 = 62.518809%**, with 593/784 functions
and 155/203 units complete. The fixed denominator is unchanged. There are
75,224 bytes remaining: 73,586 non-runtime and 1,638 runtime bytes.

## The source change

The frame-table address is initialized before the icon-table address. The loop
reads `*frames` in the draw call and advances the pointer afterward using:

```c
frames = frames + 1;
```

With SHC v5.0r32 and the original flags, this source produces a raw-exact
572-byte context: the draw loop, both following functions, all original entry
points, and the shared literal pools. Only the previously unmatched210 bytes
are newly counted. The existing192-byte skill update and170-byte tail retain
their original source files and verification. New source: src/medal_draw_216d0.c.

The equivalent-looking `frames += 1` yields18 mismatching bytes. Postincrement
inside the call gives the earlier43-byte mismatch. The default verifier rejects
both; semantic similarity alone is not matching credit.

## How the compiler investigation made this possible

Earlier captures established that literal-pool order follows pre-scheduling
reference order. Moving the frame address earlier fixes that order, but a named
postincrement pointer receives a lower allocation priority than SHC's generated
induction temporary. Generic source rearrangements did not fix both problems.

Read-only hardware breakpoints now observe shcmdl's priority constructor at
0x40ff00. They record each node's contribution, the register-storage bonus,
and the final priority. Every observed .reg/.ilb output equals its original
capture. The contributions explain the decisive difference:

| Pointer update form | Weighted contributions | Priority | Allocation/outcome |
| --- | --- | ---: | --- |
| Original generated induction temporary | 1+4+6+6+2 | 19 | Original R14 allocation, wrong pool order in old source |
| Named pointer, postincrement | 1+2+2+2+0 | 7 | R8, correct pool order but wrong registers |
| Named pointer, ordinary assignment | 1+4+6+6+2 | 19 | R14, exact complete context |
| Named pointer, compound assignment | measured separately | 13 | R12, 18 differing bytes |
| Named pointer with `register` | 7+65,535 | 65,542 | R14 but widespread allocation/stack changes |

The generated induction updates use internal opcode0x5f, which receives an
additional contribution; named postincrements use opcode0x3c. Ordinary
assignment recovers the high-value update representation. The successful
hypothesis was written to STEP_PREDICTION.md before compilation.

The trace reconstructs73 variable-chain priority totals across five runs.
The previously recovered profit formula (priority minus average conflicting
priority) checks101 logical records across seven source cases. The successful
source reproduces the original allocation vector: R14/R13/R12 priority19,
R11 priority12, R10 priority9, R9 priority9, and icon pointer R8 priority7.
These are observations of the unchanged compiler, not patched optimizer output.

## Rejected alternatives and validation

Nine source experiments are retained in session_medal_hybrid80. Separating the
frame lookup, moving the icon expression, register hints and const/initializer
changes all remain rejected. Seven captured forms have the correct table order;
only ordinary pointer assignment combines it with the correct code and roots.
The initializer/const forms reproduce the previously rejected indexed-pointer
bytes exactly. No additional source permutations are needed for this function.

Production changes are limited to the new source, tools/test_medal_draw.py,
and one splits.txt line. The tests validate the full native context, retained
210 bytes, all three roots, six external literal dependencies, the sole retained
export, absence of retained relocations, and rejection of the compound-update
control. The verifier and compiler configuration are unchanged.

Two independent clean `make split check report` builds pass both original EPROM
SHA-1s and combined ROM SHA-1 c84cf538962ddbb377290301d76d75d8b542e5a2.
The isolated suite passes230 enabled tests with zero skips. All29 production
pins, reports, linked native contexts and retained objects agree between the
canonical and isolated checkouts. Original compiler objects differ only in
three .comment metadata bytes.

Checkpoint evidence: tools/shc_probe/cases/session_checkpoint80_medal_draw.
Research evidence: tools/shc_probe/cases/session_medal_hybrid80.
The 100% goal remains active. The next useful application is another loop with
an allocation mismatch, using observed priority contributions to choose a real
source transformation instead of a broad permutation search.
