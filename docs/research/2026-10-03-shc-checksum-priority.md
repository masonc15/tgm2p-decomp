# SHC checksum copy recipe and constant allocation

Continuation 113 investigates the 452-byte f_30880 immediately before the
checksum display completed in 112. Production remains **127,816 / 200,698 bytes
(63.68574%)**. No new C bytes are credited. The baseline still has the fewest
full-function differences, 122; the new controls explain mechanisms but do not
pass the unchanged verifier.

## A source boundary creates the missing R4 copy

The original ROM shifts a value left 20 in R1, copies it into R4, then shifts R4
right 3 and right 17. The baseline performs all shifts in R2 without the copy.
Splitting the calculation between its two divisions creates a genuinely
consumed intermediate:

```c
unsigned long bytes;
/* ... */
bytes = ((unsigned long)g_60b19a0->flags << 20) / 8;
n = bytes / 0x20000;
```

Read-only hardware observations of shcgen show the new `mov R2,R4` between
the left and right shifts. It exists before late scheduling. This restores
the operation sequence, but the input scratch register is still R2 rather
than the original R1. Splitting before the first division instead combines
the right shifts differently and moves the following functions four bytes
early. Reusing existing v or j for the consumed intermediate produces the
same complete native context as the separate bytes variable.

The added short-lived allocation web changes average conflicting priorities.
That perturbs the sum/value register choices. Replacing the nested sum
expression with the two ordinary consumed updates restores their desired
register assignments:

```c
hi += v >> 16;
lo += v;
```

This control has 126 differences, with the following 490-byte and 274-byte
functions still exact. Of the modeled register assignments, only the bank
mask 0x300 and the address of g_60b19a0 remain incorrectly allocated.

## A predicted priority change fixes all allocation roles

The recovered allocator is checked against live sort, eligibility and final
assignment observations. On the original graph, 948 individual variable-weight
changes and 368 constant-weight changes provide no exact bank-allocation fix.
A bounded 5,400 pair search finds only abstract solutions involving very low k
weights. These are diagnostics, not source fixes or proofs of impossibility.

After adding the copy and separating sums, the measured graph admits a much
more concrete solution: lower the audio counter k from priority 5 to 1–4, or
lower q from 11 to 7, preserving the graph. This leads to the ordinary countdown
form, starting from the known nonzero count:

```c
k = 0x400000;
do {
    while (OPL_STAT & 1)
        ;
    g_60b19a4->w6 += OPL_DATA;
} while (--k);
```

The compiler measures k at **priority 3**. All other priorities and the conflict
graph, matched by source role, remain unchanged. **All 30 target allocations
are observed**, including mask 0x300 in R8, mask 0x100 in R9 and an unallocated
bank base. No compiler bytes, physical-register constraints or artificial
consumers are used.

This is not a full match: the context grows four bytes and has 223 raw target
differences. Register agreement does not establish instruction-order or layout
agreement. Moving low-sum initialization first changes 223→222; naming the
audio input changes it to 287. Those variants are rejected. The compound
decrement do-loop and the pre/compound decrement for-loops all reproduce the
126-difference control byte-for-byte and retain k's priority 5.

## Verified evidence and limits

Evidence is in `tools/shc_probe/cases/session_checksum_priority113`:

- 22 native controls compared with independent driver captures, excluding only
 compiler comment metadata;44 normal/debug middle replays reproduce native
 `.reg` and `.ilb` exactly.
- 1,693 allocation-score checks and 22 hardware replays.
- 209 observed allocations and 209 eligibility entries checked against the model.
- 781 scratch choices and 745 first-function emitter events checked.
- `COPY_RECIPE.json` records the pre-scheduling copy;
 `COUNTER_PREDICTION.json` records the realized priority prediction.
- `BINARY_IDENTITIES.json` checks complete bytes, not equal mismatch counts.
 `VERIFIED_RESULTS.json` preserves every rejection and function entry.

Normal and traced GEN `.sua`/`.asa` outputs agree. The `.sua` also equals the
native capture. Cleanup-captured `.asa` position metadata differs, and those
differences are explicitly retained in the replay results. Some batch summary
files cover only their last invocation; the verifier reads every retained
per-case intermediate and trace rather than treating those summaries as full
inventories.

Both remote checkouts retain checkpoint 112's 37 production pins, original ROM
hashes and progress report. Research-only negative controls do not warrant
another full build or tool suite; 112's independent clean builds and 238 enabled
tests remain the latest production validation. Previous frozen evidence is
unchanged. All 1,638 runtime bytes remain incomplete in the fixed denominator.

## Next experiment

Use `k_do_pre.c` as the allocation-correct diagnostic control and retain the
122-difference production context as the comparison baseline. Trace the first
divergent GEN scratch decision at the bank setup: it still chooses R2 for the
word/shift value where the original uses R1. Explain the last/recent/cache
state before choosing a consumed source change. Separately account for the
extra instruction/layout and audio read order. Do not rerun the closed cast,
temporary-reuse or decrement spellings, and do not credit the allocation-only
success as a matched function. Literal 100% remains active and incomplete.
