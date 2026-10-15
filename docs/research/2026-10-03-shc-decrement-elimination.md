# SHC assignment removal and a nine-byte object-list candidate

Continuation 101 reduces the nominal 251CC–2521E mismatch from 19 bytes to 9.
The new candidate has the required 82-byte length, all nine original context
roots, and exact retained ranges on both sides. It remains a failed whole-function
proof: no new matching-byte credit was taken. Production is still checkpoint84,
127,276 / 200,698 bytes (63.41668%).

## The final assignment disappears before allocation

Six unchanged hardware-breakpoint replays compare continuation100's
`consumed_store_final.c` (correct allocation, 80-byte routine) and
`final_assignment_index.c` (82 bytes, wrong allocation). They reproduce the
original .reg/.ilb outputs exactly and record29 snapshots /2365 tree nodes.

The consumed form's final assignment survives through40F975. The first call to
418D50 at40F99A removes it: the count of k assignments drops from three to two
at40F99F. The separate statement keeps all three assignments throughout.

Direct observations inside418E10 establish the decision and replacement:

- At418EB4, the final consumed assignment has BL=1; the separate assignment has
  BL=0. The initial and loop-body k assignments remain in both sources.
- The accepted path reaches41939B, detaches the final assignment's right-hand
  expression, and calls40A7F0 at4193B5.
- At4193BA, the expression replacing the assignment is the same add node for
  `k + (-1)`. The separate-statement replay never takes that replacement path.

This is an assignment-removal decision before register allocation, not a
scratch-register selection failure. The source's last k assignment has no later
use of its stored value when its expression result directly supplies the
subscript. The pass keeps the arithmetic value and removes the write to k.
That also removes the priority contributions needed by the80-byte candidate.
No compiler state was modified during these observations.

## Preserve the final expression; lower the node priority instead

A genuine alternative uses the first linked-list load as the outer condition
and the actual hardware-store value as the next-list subscript:

```c
for (i = 0; i < 127; i = i + 1) {
    if ((s = *l++) != 0) do {
        s = g_60611ec[((short *)0x24003800)[k] = s];
        k = k + 1;
    } while (s);
}
((short *)0x24003800)[--k] |= 0x4000;
```

These are consumed assignments in real loads, stores, and control flow. There
is no dummy value use or new runtime work. Relative to the original baseline,
s priority falls16 to11; explicit updates give k11 and i7. The three primary
values now receive k/R4, s/R5, and hardware-base/R7. This source has11 differing
bytes and preserves both retained ranges and all function roots.

The remaining allocation order is a score-3 tie among l, literal127, and the
next-list address. A hardware replay verifies all eight sort positions. The
fixed-graph model reproduces the eight assignments; none of576 permutations
that keep variable candidates before constant candidates yields the desired
complete allocation. This is a bounded order-model result, not an exhaustive
claim about C source possibilities.

## Name the real next-list pointer

Adding `short *next`, assigning `next = g_60611ec` after the initial hardware
stores and l initialization, and using it for the actual lookup changes its
candidate category and live range. Its priority stays4, but its score rises
from-3 to-2. The allocation becomes next/R14, bound/R13, and l/R12, alongside
the corrected primary values. Native output has **9 differing bytes**.

The unresolved backend behavior is now specific. The ROM uses R1 for the
next-list address and R13 for the bound. The named-pointer candidate keeps next
in R14 and uses R1 for the bound. The differing instructions are at251CC,
251D8,251DA,251DC,251F8,25204,2521C, including the corresponding save/restore.
No other instructions differ. The precise pass that chooses this emitted
register arrangement has not yet been traced.

`const short *next` and an unsigned-long address cast back to short-pointer
produce byte-identical full contexts to the nine-byte candidate. Moving the
pointer initialization before the initial stores gives12 differences and
changes the retained following range despite preserving the function roots.
A const pointer initialized in its declaration reproduces that earlier-init
context exactly. These controls are closed.

| Control | Gap differences | Following root | Both retained ranges exact |
| --- | ---: | --- | --- |
| Assignments consumed in both loop conditions | 45 | 25220 | no |
| Assignment consumed in inner condition only | 56 | 25220 | no |
| Outer condition plus consumed real store | 11 | 2521E | yes |
| Named next pointer after initial stores | 9 | 2521E | yes |
| Named pointer initialized early | 12 | 2521E | no |
| Const pointer initialized in declaration | 12 | 2521E | no |
| Pointer to const elements | 9 | 2521E | yes |
| Integer address cast at actual lookup | 9 | 2521E | yes |

## Evidence and next step

Eight native object comparisons,16 exact normal/debug middle replays,64 score
checks, six tree replays, and one sort replay pass. All whole-function raw proofs
reject. Both nuada checkouts still verify33 production pins, original ROM hashes,
and the unchanged progress report. Full-project tests were not rerun for these
private candidates; checkpoint84's234 enabled tests and clean builds remain the
latest whole-project validation.

Frozen evidence is in `tools/shc_probe/cases/session_obj_decrement101`.
`best_context.c` is an exact copy of `named_next.c`; `ALL_RESULTS.json` binds its
source, original native output, nominal roots, raw differences, and retained
proofs. `INPUT_PINS.json` pins the continuation100 compiler captures used for
tracing. `TREE_ANALYSIS.json` and `ORDER_MODEL.json` record the pass observations
and bounded model search.

Next trace the emitted R1/R13/R14 choice for `named_next`, using
`condition_outer` and continuation100's baseline as controls. Identify whether
the choice is made during late middle-pass constant handling or SHCGEN lowering
before testing further source forms. Preserve the nine-byte aligned candidate;
do not repeat the closed decrement, condition, qualifier, or initialization
forms. The full100% goal remains active; work continues solo without brainstorming.
