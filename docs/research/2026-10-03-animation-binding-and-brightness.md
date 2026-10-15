# Animation argument binding and brightness — continuation 128

Production checkpoint 122 remains **128,316 / 200,698 bytes (63.934867%)**.
Four source controls add no verified C bytes. The previous turn is progress:
it established the palette helper's outgoing-argument binding mechanism and
identified a concrete pointer/integer declaration discrepancy in A608.

## The animation interface is a verified negative

The draft declares the fourth argument of `f_253bc` as `struct anim *`; the
retained callee in `src/objpool_2528c.c` accepts a long and stores the address bits.
This pass tests three actual interfaces at all three calls:

- Retained short/short/short/long prototype, with explicit address-to-long casts.
- Unspecified parameters with the existing pointer arguments.
- Unspecified parameters with explicit long address arguments.

Every native ELF section, except compiler comment metadata, is identical to the
baseline in all three controls. The full 19,608-byte text is unchanged. The
baseline and legacy-long middle-pass REG files are also byte-identical, while
ILB differs. Therefore the source/interface distinction reaches the intermediate
representation but does not change allocation or emitted code here.

Read-only hardware observations bracket SHCMDL's outgoing argument binding,
incoming parameter binding, and general allocation. The table pointer, both
sprite IDs and both coordinates remain unbound through the first three snapshots
and then spill during general allocation in both interface versions. Unlike the
palette helper, there is no early R4/R5 binding for these conversions to undo.
The observations establish the allocation outcome, not every internal eligibility
rule responsible for it.

## A real brightness assignment changes priority

The baseline's constant 63 occupies R10 with allocation priority 18. Each of three
pairs of real brightness stores uses that constant twice. The control combines
the stores into chained assignments while preserving their assignment order:

```c
bri1 = bri0 = 63;  /* initialization: bri0, then bri1 */
/* At each of two input-update sites: */
bri0 = bri1 = 63;  /* bri1, then bri0 */
```

Hardware constant-priority traces reproduce the change exactly. The two loop
members and initializer contribute 8 + 8 + 2 = 18 in the baseline, versus
4 + 4 + 1 = 9 in the chained source. Constant 63 consequently receives no permanent
physical register. The second sprite ID takes R8 and the total counter spills.
The change is generated entirely from consumed C assignments; no register hint,
dead use, extra padding, or compiler modification is involved.

This is not a completed routine:

| A608 context | Interval length | Local stack reservation |
|---|---:|---:|
| Original ROM |1,748|108|
| Baseline and all interface controls |1,680|112|
| Brightness chain |1,748|116|

The chained source reaches the original interval length but increases its stack
reservation beyond the original. It still starts at A604 instead of A608, and
1,540 bytes differ when its body is diagnostically compared with the nominal ROM
body. The baseline has 1,551 differing emitted bytes and is 68 bytes short. These
relative comparisons are research measurements, not accepted or relocated code.
Every case fails the unchanged strict prefix verifier at its function boundaries.

The initial analysis incorrectly carried the interface control's five-spill
expectation over to the brightness case. Its failure is preserved in
`INITIAL_ANALYSIS_FAILURE.log`. The corrected evidence records four spilled call
variables and spr2 in internal physical register 11, SH-2 R8. No production check
was relaxed.

## Verification and next target

Five native compilations, three native/capture comparisons, six exact normal/debug
middle replays, three binding traces and two constant-priority traces pass.
REG decoding/encoding round trips pass for all 36 functions. Every traced REG/ILB
matches its control and native capture. This pass reads allocator priorities and
reconstructs the brightness contributions; it does not claim a general profit
formula for all records in this large function.

`session_animation_interface128/ANALYSIS.json`, `BINDING_ANALYSIS.json`,
`BRIGHTNESS_ANALYSIS.json` and `LINK_ANALYSIS.json` preserve the evidence and strict
rejections. Fresh checks verify all 44 production pins, original ROM/report hashes,
and thirteen prior frozen cases on both nuada checkouts. Production source, map,
compiler, verifier, runtime policy and denominator are unchanged. The prior
independent clean builds and 243 enabled tests remain the production gates; no
new full suite is claimed for these private controls.

Close the animation-interface hypothesis. A more concrete remaining discrepancy
is the count value: the ROM's zero test at A6B6 sign-extends n from R10; the baseline
loads an already-promoted long from stack offset 48 at A6B0. The compiler tree
replaces source `n == 0` with an anonymous long value (leaf 74). Investigate how
`cnt = n` and the loop-bound type create that shared value, including only count
type changes that preserve every signed-short input. Keep the brightness case as
a separate diagnostic; equal length alone is not a reason to adopt it.
The 100% goal remains active with 72,382 bytes incomplete.
