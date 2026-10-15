# Readiness function: native cache eviction evidence

Continuation 216; production remains 135,400 / 200,698 bytes (67.46455%).
The 92-byte f_8b20 function remains assembly. No production or proof changes.

The historical genuine source reproduces all first-function instruction bytes
except eight register-operand bytes. The pointers and mask already have their
required SHCMDL homes. Six allocation models reconstruct their native graphs;
the remaining mismatch is in SHCGEN scratch allocation, not those homes.

Read-only native tracing observes a cache eviction at emitter 28. R1 holds
0x802 and R3 holds 0xfffff8ff; both have age 7. The original cache helper's
descending tie order selects R3. Its count-two gate clears that cache entry.
At emitter 33, the first field clear's AND node has use count 2, no preferred
register, last register R0, and no recent-register mask. R2 and R3 both rank 2,
so descending scratch order picks R3; the original ROM uses R2.

A consumed unsigned-long update variable splits the OR and AND into separate
statements. The trace changes the second constant's age to 8 and evicts R1,
leaving R3 cached. This recovers R2 for the first clear and R3 for the final
tail call. It is still a negative: the temporary gets R5, moves the pointers
to R6/R7, preserves the offset in R0 and removes the required MOV.W reload.
The function shrinks from 92 to 90 bytes and the first call also changes.
No trace observation licenses changing compiler state or retaining those bytes.

All 39 native source controls fail strict extraction. They cover real consumed
aliases, ordinary register hints, pointer views, equivalent guards, bitwise
operand order, comma sequencing, and split or nested flag expressions. The
best remains eight differences. No dead branch trick, padding, dummy use,
forced register, compiler patch, IR patch, object patch, or ROM patch is used.
Whole contexts are diagnostic: later context functions are not fully matched.

Verification independently resolves all 39 object contexts, checks their source
and result hashes, roots and mismatch bytes, and confirms no retained object.
Six native assembly captures independently reproduce their object contexts.
Fifteen read-only compiler replays preserve fresh native SUA/ASA or REG/ILB
outputs across 2,024 events. All 1,587 scratch selections and 20 cache decisions
are reconstructed. Cleanup-snapshot ASA metadata differences are recorded;
only the fresh untraced controls are claimed byte-identical for both outputs.
model_graph.py and verify.py both exit 0; see VALIDATION.json and VERIFICATION.json
in tools/shc_probe/cases/session_ready_alloc216.

Next test genuine expression sequencing that changes constant-cache age while
keeping the expression result in R0 and the required reload. Avoid repeating
the closed aliases and guard forms. The full 100% goal remains active, solo.
