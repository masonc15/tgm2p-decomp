# SHC checksum tail: an early return clears a cached address

Continuation111 reduces the 490-byte `f_30a44` mismatch from 23 to 17 bytes.
All 440 bytes at 30A76–30C2E now match the ROM, including the counter update and
epilogue. The 274-byte following function remains exact at its original address.
The full target proof still rejects. No production source was integrated and no
new C bytes are credited: checkpoint104 remains 127,326 / 200,698 (63.441585865%).
Literal 100% remains active and incomplete.

## Source change and observed mechanism

The successful control, `outer_early_return.c`, replaces the outer nesting:

```c
if (g_60b19a0->flags == 0) {
    if (g_60b19ac)
        g_60b19ac--;
    else
        return 0;
}
return 1;
```

with a genuine early return:

```c
if (g_60b19a0->flags != 0)
    return 1;
if (g_60b19ac)
    g_60b19ac--;
else
    return 0;
return 1;
```

The same flags and counter are read, the same counter update is performed,
and the return values agree. Nothing is added to production to force a register.
The native function boundaries and context size remain unchanged.

Hardware execution breakpoints in the unmodified code generator show why this
works. Before the baseline counter test, R3 has cache flag 0x40. At selector
418900, its occupied rank is 4; R2 is last-used and R1 is recently used (rank 3).
R0 is the only rank-2 candidate, so it wins. The decrement selects R0 again.
The early-return lowering emits a real return-one value before the later
counter block. At the corresponding later selector calls, R3's cache flag is
clear. The recent mask also changes from 2 to 1. R3 wins both selections.
The early-return target has 209 emitter events versus 208 in the baseline;
the final native output merges the return paths into the required layout.

Two fixed-other-state model checks clear only R3's occupied flag in the old
selector snapshots. Both predict R3, agreeing with the real source experiment.
These are offline predictions, not modifications to a compiler process.
The decompiled `00418ea0.c` corroborates the occupied-rank rule; `0042ae20.c`
sets the 0x40 constant/address-cache flag and `0042aff0.c` clears it. The exact
cache-clearing call site on the early-return path was not separately traced;
the before/after cache states and selected registers were directly observed.

Three small standalone fixtures isolate the cache behavior from the checksum
loop. Equality and logical-NOT forms retain R3's occupied cache state. The early
return clears it and selects R3 for both counter operations. Their allocation
and history differ from the full function: the reduced equality fixture selects
R1/R2 rather than full-context R0/R0. They demonstrate the cache mechanism, not
an independently matching reconstruction of the ROM routine.

## Other controls and remaining setup problem

| Control | Target differing bytes | Result |
|---|---:|---|
| baseline | 23 | Continuation110 best |
| io_signatures | 23 | Entire context identical to baseline |
| draw_signature | 23 | Entire context identical to baseline |
| all_signatures | 23 | Entire context identical to baseline |
| counter_early_return | 33 | Rejected |
| q_first | 23 | Not identical to baseline |
| return_result | 39 | Rejected |
| signed_flags | 460 | Shifts both later function roots; rejected |
| local_tail_flags | 28 | Rejected |
| local_tail_pointer | 34 | Rejected |
| cast_tail_flags | 23 | Entire context identical to baseline |
| outer_early_return | 17 | Best; full proof still rejects |
| switch_tail_flags | 73 | Shifts suffix root; rejected |

The graphics I/O byte signatures are supported by the existing definitions in
`src/gfx_2bf58.c`; narrow drawing parameters are supported by existing matching
callers, rather than an available definition of f_e490. Neither improves code.

The remaining 17 differences all lie before 30A76. The two setup address
registers remain reversed (R4/R5), and initial zero uses R3 rather than R6,
which affects scheduling of the y initialization. Read-only tracing of the
baseline's late GEN remapping pass records three events and no remappings.
Its initial table assignments are therefore already wrong before that pass.

A fixed-graph model reproduces all 25 baseline allocations. Of 181 same-category
creation-order swaps, 27 swap the setup addresses while preserving every other
allocation. None of 20 variable-only moves does so. These are abstract models:
many swaps cross constant hash buckets and have no demonstrated source form.
The zero remains unallocated in every exact-address target prediction; its
caller-register eligibility is rejected. Symbolic descriptor numbers are local
to a compilation and must be resolved from that run rather than hardcoded.

The next experiments should explain zero's eligibility/lifetime and the
constant collection order together, using `best_context.c`. Preserve the
440-byte matched body/tail and the exact suffix. A setup prediction that fixes
only R4/R5 is insufficient to claim a full solution.

## Verification and evidence

`tools/shc_probe/cases/session_checksum_finish111/analyze.py` verifies 13 native
controls against independently captured compiler objects (excluding `.comment`
metadata), 26 exact normal/debug middle-stage replays, and 978 score records.
Fifteen hardware replays verify 50 allocations, 48 eligibility entries, 415
scratch choices, and 466 target/reduced emitter events. Traced and normal GEN
outputs agree; `.sua` also agrees with the native-driver snapshot. Known `.asa`
cleanup-snapshot metadata differences are retained explicitly in replay JSON.

`TAIL_CACHE_ANALYSIS.json` contains both observed selector snapshots and the
counterfactual predictions. `INITIAL_ORDER_MODEL.json` records the restricted
setup search. `VERIFIED_RESULTS.json` retains every full-proof rejection.
`best_context.c` preserves the successful 17-difference candidate.

A fresh read-only production check verifies all 35 checkpoint104 pins, original
combined ROM hashes, and the fixed progress report in both remote checkouts.
No full build or test suite was rerun for these research-only negative proofs;
the 236-test result remains checkpoint104's result. The supported compiler,
production sources, verifier, and fixed 200,698-byte denominator are unchanged.
