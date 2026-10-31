# Startup video initializer: 344 accepted bytes

Solo work on nephele recovered `f_1108`, ROM `[0x1108,0x1260)`, using ordinary
SHC v5.0r32 (`-macsave=1 -optimize=1 -speed`). Coverage rises from 138,752 to
**139,096 / 200,698 bytes (69.306122%)**, a gain of **344 bytes and 0.171402
percentage points**. The agreed goal remains 199,060 linked C bytes, and 59,964
non-exempt bytes remain. No exceptions, proof rules or tools changed.

## What blocked it

The earlier video draft reproduced all 344 bytes of `f_1108`, but at 0x10d8,
because the genuine startup predecessor `f_780` compiled 48 bytes short. A
standalone compile at 0x1108 does not match, because predecessor-body compiler
history matters. So the route was to recover the genuine size of `f_780` in
the same unit, without padding, rebasing or a synthetic predecessor.

## Predecessor recovery, one prediction per control

Each step was a frozen, falsifiable prediction with one ordinary compile.
`f_1108` stayed exact at its displaced root throughout.

| Step | ROM evidence | f_1108 root |
|---|---|---|
| draft (video byte-offset) | | 0x10d8 |
| unreferenced 256-byte local | frame 0x124 (`mov.w #-292` / `#292`); no r15 address formation; accesses only `@(0..32,r15)` | 0x10e0 |
| text loops index their tables | the inner counter counts up to 7 or 5; the draft reversed an unused counter | 0x10f0 |
| string literals | ROM text at 0x3149c is laid out as SHC literals, and each branch loads the shared label separately | 0x10f8 |
| `f_2140e`/`f_213da` unprototyped | `extu.w r4,r4` promotes the `(unsigned short)` arguments | 0x10fc |
| all `f_780` callees unprototyped | default promotion (also seen in the `f_253bc`/`f_252f8` object copy) | **0x1108** |

Rejected controls, kept as negatives:

- indexed sprite loops (case 3 keeps explicit cursors);
- prototype changes for only `f_253bc`/`f_252f8`;
- the label declared as `char[]` (identical object);
- an explicit long row variable.

A native GEN/MDL trace on nuada (REG/ILB native-equal, 11 stages) showed that
the remaining gap was in which invariants got hoisted, not an allocation tie.
Most remaining `f_780` bytes still differ (1,257 raw differences), so it earns
no coverage.

The discarded `.rodata` of the accepted source is byte-identical to the ROM's
53 bytes at 0x3149c: "WORK RAM", "O", "X", "VIDEO RAM", "EEP-ROM" and
"- Warning! -", in order, with the original `0xff` alignment fill. This is
independent corroboration of the literal reconstruction; the retained bytes do
not depend on it.

## Admission and replay

- The existing c-range proof accepts `[0x1108,0x1260)`. The context starts at
  0x780 and ends at 0x1260; the context is 2,784 bytes, of which 2,440 are
  context-only. There are no external literal dependencies; `f_1108` owns its
  whole pool.
- Isolated and canonical `make split check report` pass, with both original
  EPROM SHA-1s.
- The full configured suite (`PREFIX_COMPILE_TESTS=1 PERM_SETUP_TESTS=1`) ran
  341 tests: OK, 5 optional tests skipped.
- Two independent cold replays: see the receipts below.

Compile cost across the startup campaigns: 8 + 16 + 11 + 8 + 8 + 8 + 16 + 8 +
8 + 8 stages. One 8-stage reservation ran no compiler, after a
source-preparation error. Private packets are
`research/2026-10-31-startup-{frame,alloc-trace,textloops,kr-args,string-type,long-x,literals,promoted,unprototyped}/`
and the cold receipts `research/cold-2026-10-31-video1108-{a,b}/`.

The same evidence points at further startup work. All of `f_780` before 0xbda
is now size-exact, so its remaining differences are register and stack-slot
naming plus the mode-switch invariant set. Recovering `f_780` itself now
starts from `research/2026-10-31-startup-unprototyped/unproto.c`.
