# Current-board redraw helper matched — 2026-10-13

Verified linked C increased from **137402 to 137472 / 200698 bytes**:
**68.46206738482695% → 68.49694565964784%**, a gain of **70 bytes** and
**0.034878274820875146 percentage points**. The denominator and 1638-byte
runtime exception are unchanged. 61588 non-exempt assembly bytes remain.

## Accepted change

The whole helper at [0x26f64,0x26faa) is now reconstructed as matching C.
The source retains both complete redraw wrappers and their shared pool,
[0x26f64,0x26ff0), 140 bytes total. The previously accepted 70 bytes remain
exact. Following real bodies provide context and are excluded from coverage.
Source: `src/board_redraw_lock_26faa.c`.

The original helper keeps the current slot in R14 and a sign-extended 16-bit
record byte offset in R13. Ordinary local declarations spilled the offset;
register locals for the two consumed values recovered the original register
allocation. Storing the explicitly narrowed product in a full-width offset
preserves the original extension point. Finally, an indexed byte-array lvalue
selects the compiler's indexed word-load recipe. The equivalent pointer-add
expression emitted an extra ADD instruction and displaced the following root.

The bank-column alias 0x060ad27c is the real record base 0x060ad228 plus the
w84 member offset. Both the original pool and the verified record initializer
independently establish this address and its 104-byte stride. The extern view
allocates no data. There is no hardware register binding, fabricated effect,
padding, opcode injection, changed compiler, or weakened verifier.

## Verification

- Pinned unmodified SHC v5.0r32 with the existing flags.
- Unchanged strict c-range proof accepts all 140 bytes with no external literal
  dependencies. Complete functions, actual local calls, and the whole pool are
  retained; no partial-function extraction or rebasing.
- Independent original-object review and fresh GNU relink reproduce the entire
  892-byte context. Review: `research/2026-10-13-vm-rotate-abi/review_26f64/`.
- Isolated and canonical `make split check report` pass. Both original chip SHA1s
  and program SHA256 remain unchanged.
- All 291 native-enabled tests pass with zero skips. Added tests accept both
  whole wrappers and their pool and reject the extra pointer-add instruction.
  The initial staged test invocation preceded ROM extraction and failed imports;
  building the staged tree and rerunning the full suite resolved that setup error.
- Two independent cold whole-ROM replays pass from frozen inputs, with source
  and tool identities checked. Receipt: `research/2026-10-13-next-gain/cold/receipt.json`.
- Existing publication services published the verified report; the live tailnet
  dashboard shows 137472 linked bytes. `PUBLICATION.json` records the live data.
  The homepage configuration was untouched.

## The requested 94-byte startup helper

It remains excluded, with five raw differences in the best ordinary candidate.
Authenticated native traces explain why removing its pointer copy early changes
false-arm scratch selection: the copy clears an operand mask before the false
address placeholder is allocated. Without it, false X uses R3 instead of R2,
allowing the optimizer to merge the stores and shorten the function.

The early late-stage copy pass cannot cross the add or X-load use. A different
renaming pass requires native metadata absent from the actual address recipe;
all 318 recipes in the genuine context lack its qualifying header bit. This
closes the proposed metadata route rather than treating a compiler patch as C.
Evidence: `research/2026-10-13-startup-native/round6/REPORT.md` and
`round7/REPORT.md`. The next concrete research question is which genuine false
address-expression graph changes placeholder allocation order while preserving
the direct true pointer, separate stores, nominal root 0x1340, and protected
106-byte predecessor. Do not repeat the closed cast, helper-formal, or metadata
families without a native prediction.

A separate VM rotation task-API control authenticated the real narrow callee
signature, but its entire 972-byte context was byte-identical to baseline. It
was not integrated. Full decompilation remains incomplete.
