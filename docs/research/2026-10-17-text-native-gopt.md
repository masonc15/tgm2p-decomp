# Native text-helper optimization metadata control

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%). This packet is research only.

This follows the original-ROF investigation in
`2026-10-17-text-native-object.md`. The falsifiable prediction was that a
compiler-generated optimization sidecar would enable the native optimizer to
shorten the cross-section width call and the e3e0 wrapper. The call changes;
the wrapper does not shorten to the ROM length.

## Prerequisite and ordinary tool provenance

The vendor's [SH compiler feature history](https://www.renesas.com/ja/document/apn/sh-compiler-application-note-4-reference-additional-functions?r=1054731)
places inter-module optimization support in the 5.0-to-5.1 changes (A.5).
The [later compiler manual](https://www.renesas.com/en/document/mat/superh-cc-compiler-assembler-optimizing-linkage-editor-users-manual-ade-702-372a)
describes `goptimize` as emitting additional information for link-time
optimization (printed page 23). This is a version prerequisite, not evidence
that the original ROM used that workflow. The later manual is not treated as
a specification for the installed older tool's command grammar.

The unchanged 5.1r13 compiler was run on the identical genuine sectioned
source from the previous packet. Source SHA-256:
`39fb6195add8d72e10907e9344c022d488b220b8f9cb20b421d62870223f8a8b`.
Native trace shows `shcprm` creates `shciop/src.iop`, then `shctil` installs
the final optimization sidecar there. This information is separate from ROF.
The initial capture copied only top-level files and lost that directory.
SIDECAR_CORRECTION.json records the evidence and one identical recapture
before execution. The final sidecar is 5,215 bytes, SHA-256:
`0d3c0b86d440b9cb5723e9421256a2b6462e3a2e01e78805bc088c638f1f660d`.

Both compiler runs complete. The existing ROF-to-ELF converter then rejects
the longer 5.1 relocation expressions. No relocation was invented or patched.
The native linker instead resolves the original and optimized ROF directly,
using identical explicit section origins and the 23 existing real external
symbol addresses. The native optimizer's successful trace opens the actual
`shciop/src.iop` and reports five real functions optimized. Neither successful
native link reports unresolved externals.

The old DEFINE syntax is `symbol(value)`, not the later `symbol=value` form.
The parser failure is recorded in the ledger. The compiler's 5.1 intermediate
files are nested in temporary directories, so the older top-level capture
monitor did not preserve those intermediates. No authenticated MDL replay is
claimed for these runs.

## Measured result

Packet: `research/2026-10-17-text-native-gopt/`. EXPERIMENT.json,
SIDECAR_CORRECTION.json and LINK_PLAN.json precede their respective work.
The bounded total is **21 executable stages**: 16 compiler, three native
optimizer, and two native linker launches. No source variants were tried.

PPREFIX is 892 bytes, 16 more than the earlier 5.0r32 control. It begins at
ROM e074. P therefore begins at its actual successor e3f0, not rebased to
the ROM's e3e0. Optimizing P changes its total length from 328 to 324 by
removing a literal; the first wrapper remains 52 bytes. Actual roots are
unchanged by native optimization:

| Function | Actual root | ROM root |
| --- | --- | --- |
| f_e074 | e074 | e074 |
| f_e39c | e3ac | e39c |
| f_e3b8 | e3c8 | e3b8 |
| f_e3e0 | e3f0 | e3e0 |
| f_e412 | e424 | e412 |
| f_e490 | e4a2 | e490 |

At e3f4 the optimizer replaces the width-function address load with a NOP.
At e400 it replaces JSR R3 with BSR e3ac. This is a positive control for
native branch rewriting, but the predicted 52-to-50-byte wrapper reduction
is falsified. PPREFIX is byte-for-byte unchanged by optimization; P has 21
changed bytes within the common extent, including relocated literals.

Raw nominal-address differences are identical before and after optimization:

| ROM interval | Differing bytes |
| --- | ---: |
| e3b8-e3e0, retained palette | 40 / 40 |
| e3e0-e412 | 47 / 50 |
| e412-e490 | 123 / 126 |
| e490-e520 | 142 / 144 |

The independent ABS reader validates all block checksums, uses absolute
object-record positions, and rejects unresolved relocations. Each section
has two uncovered bytes in the native ABS container: P offsets 136-137 and
PPREFIX offsets 14a-14b. They remain explicitly unknown, never fabricated
padding or counted as matches. The displayed wrapper prefix is fully covered.
RESULT.json and NATIVE_MEASUREMENTS.json retain these measurements.

## Route decision

The missing sidecar explains why the previous native optimizer control did
nothing. It does not explain the ROM's exact pool flush, compact wrapper,
and preserved predecessor. The later manual's optimization table (printed
page 88) identifies branch-size optimization as the branch control already
used; no separate documented NOP-removal control was found in that check.
Do not escalate this into a blind optimizer option matrix or transplant a
5.1 sidecar onto a 5.0 object. Reopening needs genuine producer evidence that
predicts both the e3d0 pool flush and the original 50-byte wrapper without
moving retained neighbors.

There is no acceptance receipt, coverage change, required-suite result,
whole-ROM candidate qualification, cold replay, or dashboard publication for
this failed candidate. Canonical source, toolchain, converter, split, proof,
and runtime exceptions remain unchanged.
