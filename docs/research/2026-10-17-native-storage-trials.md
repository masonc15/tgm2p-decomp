# Native result and board-scroll storage trials

Canonical coverage remains **138,670 / 200,698 linked C bytes (69.093862%)**.
These two research packets add **0 bytes and 0.000000 percentage points**.
The agreed completion threshold remains 199,060 linked C bytes with exactly
1,638 documented assembly bytes. No canonical C, split, exception, compiler,
or proof-tool changes were made.

## Match controller at 0x9180

The complete genuine 0x85d8–0x9b90 translation unit and earlier ABI experiments
were reviewed first. The caller requires an int return. The final empty
comparison is also real: ROM 0x959a sign-extends R10, 0x959c compares against 2,
and 0x959e branches to the immediately following instruction. This observation
supports preserving that comparison, not adding synthetic dead code elsewhere.

The ordinary combined-int baseline reproduces a 5,516-byte context. Its target
entry is exactly 0x9180, but the successor is 0x95b0 instead of 0x95b8, and the
1,080-byte target interval has 919 raw differences. The already-matched
98-byte dispatcher and 228-byte helper interval remain exact.

Authenticated middle-pass output identifies the fifth stack slot: a promoted
int copy of the short result, symbol -4 / leaf 52. The native baseline stores
it at SP+12 and reloads it for both later comparisons. The ROM instead uses
fresh sign extensions of the resident short and a 16-byte frame. The draft
uses 20 bytes.

A single register-storage-class control predicted removal of this promoted
spill. It failed: the same promoted copy remains at SP+8, the frame stays
20 bytes, the successor moves to 0x95be, and raw target differences rise to
992. Both protected intervals remain exact. Register preference changes
allocation but does not remove the common-expression lifetime. Close this
storage-class hypothesis; any reopening needs new promotion-lifetime evidence.

Full evidence: `research/2026-10-17-field9180-context/REPORT.md`.

## Board-scroll producer at 0x2657c

The complete genuine seven-function draft, row-writer stack ABI, actual literal
ownership, and accepted redraw wrappers were reviewed. The ROM uses a 28-byte
frame: an initial field pointer at SP+0, row count at SP+12, scaled pixel height
at SP+0 after pointer death, and a promoted field value at SP+24 in the later
positive-scroll calculation. These are real storage observations; no padding
or arbitrary volatile declarations were introduced.

The ordinary baseline reproduces 3,320 context bytes. The next entry is
0x26c6c instead of 0x26cb0, and the target interval has 1,721 raw differences.
The combined limit variable occupies R9, the current index occupies R8, and
the initial local coordinate spills. The native frame is 24 bytes. The exact
cause of the complete frame discrepancy is still unresolved.

One control separates row count from scaled pixel height at the multiplication
by 16. This changes the predicted allocator mechanism and restores the current
index to R9. However, pixel height still takes R8, the initial coordinate still
spills, and the frame remains 24 bytes. The successor and context size remain
unchanged; the target has 1,719 raw differences. All later measured roots and
neighbor residual counts remain unchanged. No displaced wrapper output is
eligible to replace the canonical exact 140-byte range.

Close this dimensional split control. Further work needs a genuine lifetime
explanation for both the original coordinate in R8 and the field pointer on
the stack. The ROM independently computes the initial coordinate twice while
the draft shares it through common-expression elimination; the source-level
cause remains unproven.

Full evidence: `research/2026-10-17-board2657c-storage/REPORT.md`.

## Diagnostic authentication and limits

Each packet used 22 executable compiler stages: two ordinary eight-stage
captures and two three-stage frontend/normal/debug replays, within its recorded
24-stage budget. Native, normal-replay, and diagnostic REG and ILB bytes match
for all four sources. Only ordinary native objects were linked for measurement.
Every actual root and fixed-address raw residual is retained in the packets.

No candidate passes exact admission. Consequently these research objects were
not integrated, cold-replayed, or published as coverage. No new test-suite
claim is made. Existing baseline whole-ROM and cold-replay evidence remains
recorded in the solo resumption report.
