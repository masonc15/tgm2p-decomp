# SHC profit sorting and the tile writer's remaining allocation mismatch

Checkpoint84 remains **127,276 / 200,698 bytes (63.41668%)**. This investigation
adds no matching production bytes. It explains the constant-register reversal
and identifies a separate stack allocation discrepancy in the tile writer.

## Original compiler observations

Read-only hardware execution breakpoints observe SHCMDL immediately before and
after its profit sort (405ECC/405ED1) and physical allocator (414D48/414D4D).
Each of three tile-source controls yields four complete 34-entry snapshots.
The compiler binary, instructions, data, and configuration remain unchanged.
All three traced runs reproduce their captured .reg and .ilb files exactly.

The recovered 4074E0 partition-sort model predicts **all 102 candidate
positions**, including equal-score ordering. An independent bounded check covers
3,279 lists of lengths1–7 with three possible scores. This is an implementation
model validated on these observations, not a proof of every allocator behavior.

The priority-minus-average-conflicting-priority formula also reproduces all102
observed scores, including the fifteen constant records. In the older baseline,
divisor16 scores2 and zero scores1. After the source changes that correct the
row/counter/record roles, divisor16 scores1 and zero still scores1. Both retain
priority10; the changed conflicting priorities lower the divisor's score.
The sort then visits zero at position7 and divisor16 at position8. The allocator
gives zero R13 and divisor16 R12, opposite the original ROM. These are observed
outputs of the original compiler, not just a hypothesis based on disassembly.

The register-profit sort and the later stack-frequency sort are different
routines. Preserved inferred decompilations and original call-site disassembly
identify the stages. No source-order stability assumption is used.

## Named-value controls

Three genuine source controls name divisor16, zero, or both. They use only the
existing divisions and counter initializations; no dummy consumers are added.
All preserve the preceding432 matched wrapper/producer bytes but fail the
unchanged full-function matching gate.

| Source | Frame bytes | Following dispatch | Positioned target byte differences |
|---|---:|---|---:|
| Starting record-byte candidate (86) |36|2769C|416|
| Named divisor |40|27696|424|
| Named zero |40|2769A|459|
| Both named |44|2769A|446|

The named divisor becomes a variable with priority7/profit−2 and spills.
Named zero has priority2/profit−6 and uses R4. Naming these values therefore
changes allocation more broadly than exchanging the two constant registers.
Three native captures match original compile sections, excluding compiler
comments; six normal/debug middle-stage replays reproduce .reg/.ilb exactly.
All three generated sources reproduce exactly, and88 named-control WEB CHAIN
score checks pass. These specific named-value forms are closed negative cases.

## Stack evidence and next experiment

The baseline has eight memory groups; record_byte_compound has nine. Diagnostic
frequency/reflection output independently agrees with eleven symbol-scoped
serialized spill assignments across the two controls. The current candidate's
decoded y occupies R8, while inner-loop py spills to memory group−7. The original
ROM uses R8 for the inner calculation and stores decoded y in its frame.

The next bounded experiment should combine y's existing signed-word decoding
and division into one assignment, preserving narrowing before signed division.
The hypothesis is that removing the intermediate y update lowers its priority,
allowing py to receive R8. Test y alone first, with x as a separate control.
A correct register choice would still need exact native bytes and frame size;
changing a lifetime can also change constant scores and memory sharing.
Negative memory-group numbers are not claimed to be final byte offsets.
Do not repeat the failed named-value or row/counter forms.

## Reproduction and scope

Evidence is in tools/shc_probe/cases/session_tile_sort87:
- analyze_sort.py verifies complete captured sort orders and scoring.
- analyze_named.py verifies source regeneration, native captures and replays.
- analyze_stack.py verifies memory-group reflection by source-symbol aliases.
- replay.py observes the unchanged compiler using trace_sort.c.
- compiler_evidence preserves inferred stage implementations and original x86 calls.

Historical input captures are frozen in session_tile_row86. Both production
checkouts freshly pass all33 checkpoint84 pins, original ROM hashes and current
reports. The234 enabled tests and two clean builds remain the latest full
integration gates from84; they were not rerun for these rejected private controls.
The literal100% goal remains active, with73,422 bytes still incomplete.
