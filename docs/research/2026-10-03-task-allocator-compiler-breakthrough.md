# Task allocator: a measured priority change completes 172 bytes

Continuation 115 completes **f_2452e, 2452E–245DA**, adding **172 verified C
bytes**. The entire 2,464-byte task context now matches the original ROM.
The 2,292 neighboring bytes were already retained and receive no new credit.

## Why the six-byte near-match persisted

The production source loaded a free task pointer into the clear pointer `p`,
then copied it to the saved task pointer `t`. SHC assigned p/R6 and t/R7. The
ROM has the same eventual register roles, but loads t/R7 first and copies it
into p/R6. Six instruction bytes differed.

Reversing the source assignments preserved that order in the optimized tree:

```c
t = g_607cf10[n];
p = (long *)t;
```

However, it also changed allocation priorities and profits. The baseline p
priority 18/profit 6 and t priority 15/profit 5 became p17/profit5 and t16/profit6.
SHC consequently put t in R6 and p in R7. This made the load order logical but
left the physical registers reversed throughout the function: 36 differing
bytes. The two values remain distinct, conflicting allocation webs; the
optimized tree does not discard the desired assignment order.

## Predict, then compile

Read-only hardware traces capture the sort, pre-allocation parameter binding,
eligibility decisions and final allocation. The model must preserve the parent
argument's observed prebinding to R4; starting every register unassigned would
model the wrong invocation.

With that binding accounted for, all measured assignments are reproduced. A
400-case offline search predicts that raising p's priority from 17 to 19–23 will
restore every desired physical assignment while preserving all other priorities
and the conflict graph. The search is diagnostic only; it never alters the
compiler or emitted code.

Separating one genuinely consumed pointer increment realizes the prediction:

```c
for (i = 0; i < 0x100; i += 4) {
    *p = 0;
    p += 1;
    *p++ = 0;
    *p++ = 0;
    *p++ = 0;
}
```

The compound step raises p to **priority 21**, keeps every other priority and
source-role conflict unchanged, and yields the original t/R7 load followed by
the p/R6 copy. All ten predicted allocations agree with the compiler. The
native target and both neighboring ranges match exactly.

The controls also test the prediction's limit. An ordinary `p = p + 1` raises
p to 25: the pointer roles become correct, but the loop-index/global-address
allocations change, leaving eight differing bytes. All ten assignments for
this rejected control are also predicted. A separate `p++` has the same exact
native result as `p += 1`. Production uses the compound form.

## Verification

Research evidence is frozen under
`tools/shc_probe/cases/session_task_coalesce115`:

- Six native controls compared with independent driver captures, excluding
  compiler comment metadata; 12 exact normal/debug middle-stage replays.
- 1,026 logical-register score checks and eight hardware replays.
- 40 observed allocations and 36 eligibility entries verified; the already
  bound parameter does not require an eligibility call.
- Fixed-graph predictions for priority 21 and25 agree with all 20 actual
  assignments. Both successful sources match the complete 2,464-byte context.
- The unchanged prefix verifier retains only 172 bytes, authenticates five
  external literal loads, and exports only `_f_2452e` without relocations.

Integration evidence is under `session_checkpoint115_task_allocator`.
The regression tests check the entire genuine context and retained object,
and restore the embedded postincrement to prove the wrong pointer-register
form is rejected at 2454E without an accepted object or proof manifest.

Independent clean `make split check report` builds reproduce the original ROM
and both EPROM SHA-1s. The isolated tool suite passes **240 enabled tests,
zero skips**. Both checkouts have identical **39 production pins**, reports,
linked native context and retained object. Original compiler object differences
are restricted to comment metadata.

Coverage is **127,988 / 200,698 bytes (63.77144%)**, with 598/784 functions and
160/205 units complete. The remaining 72,710 bytes include all 1,638 runtime
bytes. Literal 100% remains active and incomplete.

## Carry-forward

Keep the checksum scratch controls parked: this gain came from applying the
recovered allocator to a different function, not repeating its failed source
families. For the next near-match, inspect the optimized tree and actual
prebindings before modeling priorities. Preserve all 39 checkpoint115 pins;
the previous task source and checkpoint112 split hash are superseded. The
checkpoint adds explicit pins for the task source and its regression tests.
