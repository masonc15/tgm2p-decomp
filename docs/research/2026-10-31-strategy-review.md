# Strategy review after two zero-gain campaigns (2026-10-31)

Coverage stays at **138,752 / 200,698 linked C bytes (69.134720%)**. The
2026-10-31 session added 0 bytes. It ran two campaigns: the f_bc02 literal-owner
transfer (8 stages, prediction failed) and the startup frame and loop controls
(16 stages; the frame step was confirmed, the loop step was rejected). This
review is required by the two-campaign rule in
[the workflow](../DECOMP_WORKFLOW.md).

## What the evidence now says

1. **No exact function is waiting to be harvested.** No contextual function in
   any canonical context is exact at its nominal root or at a displaced root.
   No literal pool inside an assembly range is loaded only by accepted C code.
2. **The closest near misses are closed at the compiler level.** These include
   f_1050e (GEN address-selector history), f_10788 (scratch mask), f_12e2
   (joined pointer, best archived result 5 differences), f_e39c (215 controls),
   f_7658 and f_3c88. Each route needs native producer evidence, not new
   spellings.
3. **The largest gains are layout-gated, in three chains.** In each chain the
   target body is already exact, or nearly so, in context, but sits at the wrong
   address because genuine predecessor or callee bodies have the wrong size:
   - startup 0x780: f_1108 (344 bytes) is exact when displaced; f_780 is now
     40 bytes short with the confirmed 292-byte frame (frame256);
   - board 0x2657c..0x278c0: f_27862 (94 bytes) needs f_26ff0 (-46) and four
     2-byte fixes;
   - settings 0x9c0c..0xe74f: f_bc02 plus its pool (432 bytes) needs eight
     bodies (about -378 net). Rodata ownership is already solved in
     research/2026-10-18-nephele-bg-resource-count (2,098 bytes exact).
4. **Unit extent is genuine, and truncation is invalid.** Shared literal strings
   and same-file BSRs fix the unit extents. SHC code generation depends on
   same-unit callee definitions, so contexts cannot be cut short to repair
   literal values.

## Selected next work class

Run native compiler instrumentation on the startup chain, the gated chain with
the best ratio of gain to work. The open uncertainty is specific: why ROM f_780
keeps seven loop invariants in r8..r14 (f_2b4e8, a string, 600, f_e3e0, f_e412,
0, 15) while spilling the timer, i, x and the table cursor to the stack. The
frame256 draft does the opposite. Answering it from an authenticated GEN/MDL
allocation trace on nuada, using the existing capture drivers, is evidence work,
not a spelling sweep.

The next ordinary candidate should follow only after that trace predicts the
allocation change. One measured fact stands: indexed text loops in cases 0-2
recover 12 bytes, while indexed sprite loops in case 3 lose 46-58.

Acceptance rules do not change: no padding, rebasing, truncation, modified
compiler output or new exceptions. Every layout step earns zero bytes until
f_1108 is nominal at 0x1108 and still exact; then its 344 bytes go through
c-range admission, the full test suite, two cold replays and publication.

## Addendum, later on 2026-10-31

- **Startup chain: done.** Unprototyped callees put f_780 at its genuine size
  and f_1108 was admitted (+344, coverage 139,096). See
  docs/research/2026-10-31-startup-video-match.md.
- **Settings chain: sized to -120, admission blocked.** With the exact 2,098-byte
  `.rodata` the unit is -120 bytes at f_e3e0 (docs/research/2026-10-31-settings-unit-sizing.md).
  The remaining gaps are f_c21a -110, f_d394 -32, f_cac8 +26, f_cf68 -12,
  f_be28 +8, f_dbe8 -8 and f_e074 +2.
  - Even at exact sizes, f_ad80/f_af8c/f_b3c0/f_bc02/f_e074 only match with the
    unit `.rodata` at its ROM address. The strict proof places it after `.text`.
  - Validating it in place would be new proof tooling, which needs the owner's
    decision. Until then this chain can earn no bytes.
  - The last f_be28 control (per-region f_e63a prototypes) is refused by SHC
    (error 2118).
- **Board chain: unchanged.** ROM f_2657c's 28-byte frame touches @0, @4, @12,
  @20 and @24 and passes r15 to callees, so the 24-vs-28 gap is a spill, not an
  unreferenced local. The startup frame mechanism does not transfer; no compile
  was spent.

There are now 52 consecutive zero-gain campaigns. Under the stop rule, the next
work class needs a decision: either approve a `.rodata`-at-ROM-address proof
mode for the settings unit, or name another chain. Acceptance rules are
unchanged.
