# Animation allocation tie resolved — continuation134

Separate initial brightness assignments recover all four previously wrong register roles: hold uses R10, mask 15 uses R9, constant 2 uses R8, and total spills. The local frame returns to the original 108 bytes. The source still does not match: it emits 1716 bytes at A604, versus 1748 bytes at A608.

## A model prediction realized in genuine C

The starting source is continuation133's `hold_both.c`, SHA-256 `4f2a3bcefc610a9e6dd75b69a6ad2913db00c1dd8d6c7ef16f121d2e1151405a`. It already joins hold's two live ranges and removes both persistent sprite pointer caches.

`sensitivity.py` verifies its frozen inputs, reproduces actual allocation, then evaluates 643 diagnostic changes to one candidate priority at a time on the fixed observed graph. Among the tested changes, the four desired roles occur only when total priority falls by 1 or 2, or constant 63 priority rises by 1 or 2. This model never changes compiler inputs or awards matching credit.

The constant case has a direct source explanation. Replace the initial chain with two separate assignments:

```c
bri0 = 63;
bri1 = 63;
```

The two later brightness pairs stay chained as `bri0 = bri1 = 63`. A separate comma-expression control produces identical full emitted text. Reversing the original initial assignment chain keeps the old allocation, so assignment order alone is insufficient.

Hardware observations of the stock compiler's constant-priority routine verify the exact contributions. The baseline and reverse-chain control contribute 4 + 4 + 1 = 9. Separate and comma initialization contribute 4 + 4 + 2 = 10. Total remains priority 15; hold remains priority 21. Constant 63's changed score affects the partition sort's tie ordering, allowing constant 2 to precede total and occupy R8. No compiler memory, priorities, physical registers, production flags or verifier rules are patched.

| Source | Constant 63 priority | Hold | Mask 15 | Constant 2 | Total | Frame | Bytes |
| --- | ---: | --- | --- | --- | --- | ---: | ---: |
| Baseline | 9 | R10 | R9 | unallocated | R8 | 104 | 1678 |
| Separate initial assignments | 10 | R10 | R9 | R8 | stack | 108 | 1716 |
| Comma initial assignments | 10 | R10 | R9 | R8 | stack | 108 | 1716 |
| Reversed initial chain | 9 | R10 | R9 | unallocated | R8 | 104 | 1678 |
| Original ROM | unknown | R10 | R9 | R8 | stack | 108 | 1748 |

The recovered roles refer to the main animation loop. They are not a claim that all temporary registers or stack offsets match.

## Remaining machine-code discrepancy

The new source places selection at local slot 0, total at 8, and the second sprite ID at 44. The original uses selection at 4, total at 12 and stores the second sprite ID at 0. Other stack slots also differ. The candidate has one anonymous count-promotion slot at 76.

The original sprite setup provides a concrete next target. It loads slot 0 into R9 at A750, passes its sign-extended value to f_253bc at A75C, then repeatedly writes and reads the value through slot 0:

- A760 stores R9; A76C reloads for the b2a field address.
- A77A stores R9; A77C reloads for the w18 field address.
- A7AC stores R9; A7AE reloads for the w2c field address.

These are reachable native instructions saved in `ORIGINAL_SPRITE_SETUP.json`, not literal-pool disassembly. Later original visibility operations also read the ID from slot 0. The current candidate uses fewer setup operations and different stack placement. This is evidence to inspect the compiler's short-value conversion, allocation and spill representation; it does not justify adding a volatile local, a fabricated aggregate, a dummy self-assignment or padding.

The sprite-call sequence remains intact. Original calls occur at A734/A73A (allocation), A74A/A75C (setup), and ACA2 (later position update). The new source calls those same external targets at A72A/A734, A748/A75A, and AC7E. `SPRITE_CALLS.json` records these actual CFG-resolved targets. Every candidate still fails the strict contextual boundary check, and the improved candidate has 1577 relative differing bytes. None of these diagnostics count as matched bytes.

## Verification

Four new native compiles and four independent captures pass. Eight normal/debug middle replays preserve captured `.reg` and `.ilb` bytes. Twelve hardware replays cover sort, eligibility and constant priority for all four cases. All 344 preallocation candidate sort positions, profits and positive allocations agree with the observed compiler; appended stack records are excluded from that prediction claim. All four complete 36-function register tables round-trip exactly.

Five CFG stack audits cover the original and four candidates. Every stack access resolves, every return restores entry SP, and serialized negative-register slots agree with the emitted local frames. `verify.py`, `VERIFICATION.json`, `CONSTANT_ANALYSIS.json`, `SLOT_MAPS.json` and the trace files reproduce the findings. The copied baseline's native sections match frozen continuation133.

Both Nuada checkouts retain all 44 checkpoint122 production pins, nineteen earlier freezes, ROM hashes, and all 23 copied reference-ROM hashes. The most recent production build/test gate remains checkpoint122's independent clean builds and 243 enabled tests; this private experiment introduces no production change.

**New verified C bytes: 0. Production remains 128,316 / 200,698 (63.93487%). The full goal remains active.**
