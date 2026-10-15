# Exact palette initializer — checkpoint 211

Production gains **1,504 verified C bytes**, reaching **133,284 / 200,698
(66.41023%)**. There are 67,414 bytes remaining, including all 1,638 runtime
assembly bytes. Literal 100% remains the objective.

`src/gfx_2ab56.c` now retains 0x2A576–0x2AC4C: the complete initializer and
the four already matched tail functions, totaling 1,750 bytes. Its genuine
predecessor at 0x2A0D4 supplies compilation context only. Those 1,186 bytes
still differ from ROM and earn no credit. The retained range has no external
literal loads. Compiler flags, toolchain and strict matching rules are unchanged.

## What broke the plateau

Checkpoint 210 had two 1,504-byte candidates with only three differing bytes:
one reversed condition and its two store operands. Restoring the positive
condition recovered the branch but assigned two color values to the wrong
registers.

This continuation established that a branch-local object-pointer alias can
coalesce into the existing pointer while leaving an inactive candidate in
SHC's later unstable sort. It changes allocation ordering without introducing
an address calculation or moving the subtraction. The alias supplies the
pointer actually used by the color store; it is ordinary semantic C.

The matching source repeats the same pattern in all four expanded iterations:
name the object pointer inside the positive red and blue arms, and inside the
green else arm. Keep positive conditions throughout. Assign `o = &PFX(n)`
at color positions 0, 6 and 8. The offline model predicted all 27 target color
and mask homes, and native compilation then produced a strict byte-for-byte
match. Fresh capture, untraced control, read-only origin trace, independent
assembly and object resolution confirm the result.

The exact research source is
`tools/shc_probe/cases/session_gfx_delta211/predicted_repeat_0_6_8.c`.
`generate_alias_source.py` reproduces it from the recorded source pattern.
`FIRST_REPEATED_HYPOTHESIS.json` records the prediction used before compilation.

## Research evidence

The case contains 66 native controls: 63 new and three carried. One strict
initializer match and 65 negatives are retained. Nineteen native origin
graphs, nineteen independently assembled captures and ten flow traces give
29 unchanged-output replays and 3,536 events. The flow audit checks 120 color
conditions. The winning graph has 88 candidates; the coalesced pointer has
priority 175. The allocator model reproduces every native home, including
all 27 target color/mask assignments.

Failed named-delta, destructive-update, narrowing and named-color controls
explain why simple arithmetic rearrangements add instructions or disturb
allocation. A named color value coalesces with its source, increases priority
by two, and changes candidate order, but its tested forms do not match.

Recorded offline searches, never compiler input modifications:

- 2,048 named-delta pointer placements: no exact home set.
- 6,144 color-6 pointer-alias placements: no exact home set.
- 4,096 named-color pointer placements: no exact home set.
- 49,152 single-alias placements across all colors: no exact home set.
- 565,248 pairs of alias placements: no exact home set.
- 129,024 repeated RGB alias patterns and pointer placements: two exact-home
  predictions. The first was compiled and accepted; the second remains an
  uncompiled hypothesis because the first already supplies the exact function.

Native traces calibrate the three-assignment winning source as well as
independent pointer placements and alias patterns. The analysis independently
re-evaluates every reported exact-home prediction. No compiler or IR bytes,
physical registers, objects, ROM bytes or proof rules were patched.

## Production validation

The isolated and canonical checkouts each completed `make clean`,
`timeout 240 make split check report`, and the full unittest suite with
`PREFIX_COMPILE_TESTS=1 PERM_SETUP_TESTS=1`. Both passed 255 tests with zero
skips, retain all 605 matched functions, and rebuild the original 1 MiB ROM:
SHA-1 `c84cf538962ddbb377290301d76d75d8b542e5a2`.

The canonical first attempt started before the source transfer fully finished.
Its first check passed, but report generation caught the stale assembly split
through linker placement assertions. That failed attempt is preserved. After
confirming source, split and test hashes, the complete clean pipeline was
rerun successfully. No failed gate was treated as passing.

The new native tests protect the full retained initializer/tail range, absence
of external literal dependencies, and exclusion of the unmatched context.
An equivalent source with the pointer aliases removed is correctly rejected.

The dashboard publisher was held during canonical deployment, then restored
and explicitly run after all gates passed. The live tailnet page contains a
133,284-byte snapshot and the watcher is active. The homepage link remains in
Nuada's Gaming section.

Build logs, reports, proofs and ROM hashes are saved under
`tools/shc_probe/cases/session_checkpoint211_palette`. The next work should
use this verified initializer as stable context while addressing the preceding
palette-update function's remaining differences, or another incomplete unit.
