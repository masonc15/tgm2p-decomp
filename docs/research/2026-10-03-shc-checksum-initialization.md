# SHC initialization and allocation priority: complete checksum display

Continuation112 completes and integrates `f_30a44`, 30A44–30C2E: **490 verified
C bytes**. Both independent clean builds preserve the original ROM and EPROM
hashes. Coverage is **127,816 / 200,698 bytes (63.68574%)**, with 597 / 784
functions and 159 / 205 units complete. There are 72,882 bytes remaining,
including all 1,638 runtime bytes. The literal 100% goal remains active.

The source is `src/checksum_display_30a44.c`. Only that function is exported by
its retained object. Its genuine preceding and following functions provide
compiler context; all 726 context-only bytes are excluded from this gain. The
unchanged verifier checks 13 external literal loads. No relocations, forced
physical registers, compiler patches, dummy consumers, or relaxed proofs are used.

## From 17 differing bytes to six

Continuation111's exact body and tail left only initialization wrong. The
baseline shared zero constant is ineligible for caller-saved allocation.
Extended read-only eligibility traces capture its live ranges and the calls
that overlap them, beginning with f_2c3d0. The rejection routine at 405750 scans
these intervals and argument bindings. The zero's lifetime overlaps calls;
its immediate argument has opcode 113, not a bound variable identifier (112).
The final eligibility rejection is observed directly; the reason attributed to
that argument scan is inferred from the corresponding decompiled routine.

A real short initializer separates the short-lived initialization value:

```c
short initial;
/* ... */
p = ROM_TABLE;
q = g_60b1880;
initial = 0;
y = initial;
n = initial;
```

Every assignment feeds the actual counters. SHC constant-propagates the y read
but retains the short copy into n. Initializing y before n extends the short
value's recorded lifetime from points 11–14 to 11–17. It now conflicts with y,
whose priority is 13. The average conflicting priority rounds from 7 to 8;
`initial` retains priority 2, so its allocation profit becomes -6 rather than -5.
It remains eligible for caller-saved registers because no call overlaps this
short lifetime. The allocator chooses R6, as required by the ROM.

Hardware traces and the recovered allocation model reproduce this behavior.
The emitted zero is R6, and scheduling matches the ROM. Only six byte differences
remain, all from swapped table address registers R4/R5.

## A prediction fixes the last six bytes

The fixed-graph model first tested creation-order ties. It found abstract
solutions, but ordinary declaration/scope/address-carrier source forms did not
produce the necessary ordering. None was treated as a completed function.

The next model varied real variable priorities and recomputed all conflict
averages. It predicted that raising the loop index n from priority 5 into the
7–11 range would swap only the table addresses, preserving all other physical
assignments. This led to a specific source experiment:

```c
n = n + 1;
```

instead of `n++`. The ordinary assignment raises n's observed priority **5→7**.
The creation order and conflict graph are identical, and all 26 predicted
physical assignments agree with the actual compiler trace. R4 holds the ROM-table
address, R5 holds the RAM-table address, and the initializer remains in R6.
Both `n += 1` and `++n` retain priority 5 and the six-byte mismatch.

The native 490-byte function now matches exactly. A production regression test
restoring `n++` confirms that the unchanged verifier rejects the old mismatch at
30A58 and does not emit an accepted retained object or proof manifest.

## Verification

Research evidence lives in `tools/shc_probe/cases/session_checksum_initial112`:

- 22 native controls independently compared with driver-captured objects,
  excluding compiler `.comment` metadata; 44 exact middle-stage replays.
- 1,667 logical-register score checks and 15 hardware replays.
- 103 allocation predictions, 99 eligibility entries, 26 observed register-web
  births, 586 scratch selections and 627 target emitter events verified.
- `INITIAL_LIFETIME_ANALYSIS.json` records call intervals and the initializer's
  added y conflict; `PRIORITY_TARGETS.json` verifies the successful prediction.
- `VERIFIED_RESULTS.json` retains the successful proof and all rejected controls.
  `best_context.c` is the successful native source context.

Integration evidence lives in `session_checkpoint112_checksum_display`:

- Independent clean `make split check report` builds on the isolated coordinator
  and canonical checkout.
- **238 enabled tests, zero skips**, including positive full-function proof and
  negative postincrement rejection. The suite ran in the isolated checkout.
- All **37 production pins**, reports, linked native context, retained object,
  and original ROM/EPROM hashes agree between checkouts.
- The original compiler objects differ in three bytes, all inside `.comment`.
- Combined ROM SHA-1: `c84cf538962ddbb377290301d76d75d8b542e5a2`.
- Combined ROM SHA-256:
  `d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7`.

The earlier 111 evidence remains immutable. The new gain is precisely 490 bytes;
no portion of the unmatched f_30880 compiler context receives credit.

## Next target

The preceding 452-byte `f_30880` still has 122 differing bytes in the successful
context. Earlier work isolated its bank-shift setup and competing constants
0x100, 0x300 and 0x24060000. The newly validated next approach is to model real
variable-priority changes, recomputing conflict averages, before selecting new
consumed assignment forms. Protect the recovered high/low sum registers and the
now-matching following functions. Reuse checkpoint112's 37 production pins as
the baseline; prior checkpoint104 split hashes are intentionally superseded.
