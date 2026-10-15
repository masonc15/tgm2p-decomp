# Score comparison child planning — continuation175

No additional matching C bytes. Production remains checkpoint161: 128,398 /
200,698 bytes (63.9757247207%), with 72,300 remaining including 1,638 runtime
assembly bytes. The literal100% goal remains active. Work continues solo.

## What the compiler trace establishes

Three hardware-breakpoint replays compare joint_not, guard_label_positive, and
joint_level_reverse. Every traced SUA/ASA output equals a fresh untraced control;
the SUA also equals its previous native intermediate capture. The old cleanup
ASA contains advanced symbol positions and is not the output control. Compiler
code and data were not patched. The trace uses execution breakpoints only.

Each trace contains seven comparison-parent snapshots, two result requests,
and twelve subsequent emitter calls. All planning snapshots and result requests
have emitter count134. Both child results are therefore allocated before the
first load is emitted. Parent/child links and metadata identities stay stable.
Recipe45C0C8 is selected before first-child planning; the parent returns with
recipe45BDD0. No child-order swap occurs in the observed interval.

| Source form | First / second result | Recent masks before requests |
| --- | --- | --- |
| joint_not | thresholdR1 / levelR2 | 5 / 10 |
| guard_label_positive | thresholdR2 / levelR1 | 3 / 12 |
| joint_level_reverse | levelR1 / thresholdR3 | 5 / 3 |

Joint and positive forms emit threshold load, field-level load, EXTU.W, CMP/GT.
Reversed form emits field-level load, EXTU.W, threshold-address/load, CMP/GE.
The native reversed object moves the independent threshold-address load before
EXTU.W, while preserving selected registers. This demonstrates scheduling after
emission for this sequence; it does not reconstruct the whole scheduler. It
also rules out using that scheduler to fix these particular register choices.

## Native C controls

All eight variants retain genuine consumed comparison semantics, without
register forcing or dead operations. Subtraction fits the signed32-bit domain
of signed16 threshold and unsigned16 level. Equality disjunctions access
ordinary nonvolatile values with no intervening calls.

| Variant | Target byte differences | Context bytes |
| --- | ---: | ---: |
| not_greater | 15 | 1456 |
| greater_zero | 15 | 1456 |
| less_or_equal | 460 | 1460 |
| greater_or_equal | 461 | 1460 |
| conditional_value | 22 | 1456 |
| reverse_conditional | 29 | 1456 |
| threshold_difference | 451 | 1460 |
| level_difference | 455 | 1460 |

The first two entire contexts equal joint_not. All eight fail strict matching.
The positive-label best remains13 target-byte differences. No source was
integrated and no production rule or denominator changed.

## Handoff

Preserve this score evidence and avoid repeating Boolean inversion, comparison
casts, simple locals, equality disjunctions, subtraction, or compiler-version
sweeps without a new mechanism. A distinct unresolved source question exists
in following f_7956: its f_17834 call evaluates three RNG expressions and passes
four arguments, while retained f_17834 has a three-parameter narrow definition.
Inspect caller and callee ABI and RNG order before changing declarations; all
RNG calls must remain consumed as in the original. The definition alone does
not prove the original caller prototype. This audit has not yet been performed.

Rechecked the remaining-unit inventory before selecting this lead. Literal-pool
heuristic entries are not established functions; f_2452e is already retained.
Rank12AD4 and board27B50 drafts have existing context/layout blockers and should
not be recounted as new work.

Evidence: tools/shc_probe/cases/session_score_order175/{RESULTS.json,verify.py,
ORDER_REPLAY_RESULTS.json,replay_order.py,trace_order.c}. A fresh read-only
production check verifies46 pins,61 prior frozen cases, report and ROM hashes
in both Nuada checkouts, plus all23 reference ROM files. Full build/test gates
remain checkpoint161's independent clean builds and245 enabled tests; no new
full build or suite is claimed for this research-only continuation.
