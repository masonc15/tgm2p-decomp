# Animation stack lifetimes — continuation132

The genuine staged sprite-address calculation removes two loop-hoisted pointer caches from the draft and reproduces the original 108-byte local frame. It does not match the routine: the emitted code still starts at A604 instead of A608, occupies 1694 instead of 1748 bytes, and allocates `total` to R8 while spilling `hold`.

## Original stack audit

`stack_audit.py` follows reachable SH2 control flow, delay slots, entry-relative stack offsets, outgoing arguments and conservative call clobbers. It checks equal stack positions at joins and restored entry SP at every return. Unlike a linear disassembly, it does not classify literal pools as instructions. The audit covers 777 reachable original instructions and all five native candidates, with no unresolved stack accesses or stores of affine stack addresses into stack slots. It is a structural abstract analysis, not a full semantic equivalence proof.

The original has 27 four-byte local slots, plus 32 bytes of saved registers. `total` is at local offset12: initialization A62E; display A9E0; forward update AAB8/AAC0; comparison/reset AAE4/AAF4; reverse update AB38/AB40; negative test/restore AB64/AB72. The display reads displacement24 after 12 argument bytes have been pushed. Four calls to f_e63a accumulate 48 outgoing bytes before caller cleanup; treating those calls as cleaning their own arguments would give incorrect slot identities.

The three pointers passed to ACDC at A7E6, ABE8 and ABFE point to selection at local offset4. This audit finds no escape of total's address and gives no basis for adding local volatility.

## Actual extra lifetimes

The baseline has 28 local slots. Serialized register records, compacted stack slots and native accesses identify anonymous sprite-pointer caches at local offsets40 and48, plus a separate count-promotion slot76. Six SHLL2 instructions form the two sprite offsets before the main loop; stores A7E4/A7F4 retain the pointers for later sprite flag and brightness accesses. Meanwhile spr2 and the later input byte c1 share offset4 because their original draft lifetimes do not overlap.

The staged source calculates the real sprite offset inside each flag branch and the paired brightness stores, using consumed `long sprite_offset` and typed sprite-pointer expressions. It removes both persistent pointer caches and makes spr2 and c1 occupy distinct slots. The remaining anonymous count-promotion slot moves to64. Removing two caches while ending one slot reuse reduces the frame by one four-byte slot, from112 to108. No padding, dummy consumer, forced register, compiler patch or production flag change is involved.

| Native source | Start | Body bytes | Local frame |
| --- | --- | ---: | ---: |
| Original ROM | A608 | 1748 | 108 |
| Baseline | A604 | 1676 | 112 |
| Typed byte view for flags | A604 | 1702 | 112 |
| Typed byte views for all sprite fields | A604 | 1716 | 116 |
| Stock optimize=0 control | AA80 | 2116 | 72 |
| Staged sprite offsets | A604 | 1694 | 108 |

The optimize=0 control saves only PR (4 bytes), whereas the normal candidates save32 bytes. Its smaller local frame is not evidence of matching. Every candidate fails the unchanged strict boundary verifier. Relative byte comparisons are diagnostic only; staged code still differs in1628 compared positions.

## Verification and next experiment

Five native compiles succeeded. Three newly captured native intermediate pairs and six exact middle-stage replays passed; the unchanged baseline reuses its frozen continuation131 capture. The optimize=0 control was compiled, linked and stack-audited, not traced. Register serialization round trips cover the complete36-function source unit. `verify.py` reproduces sources and asserts the cache slots, slot reuse, frame sizes and resolved stack accesses. `PRODUCTION_RECHECK.json` verifies44 production pins, seventeen prior frozen cases, ROM hashes and all23 copied reference files in both remote checkouts. No new production build/test claim is made: checkpoint122's clean builds and243 enabled tests remain the last production gate.

Start the next experiment from `staged_sprites.c`. Remeasure its allocation graph before evaluating `total` versus `hold`; the continuation130 fixed-graph priority12 prediction cannot be carried over unchanged. The staged code still gives total priority15 and R8, and spills the long-lived hold value. The remaining slot order also differs from the original. Prior case89 explains how genuine staged updates can suppress invariant extraction; this turn verifies native/intermediate results, not a fresh observation of the precise SHCMDL creator PC.

Production remains **128,316 / 200,698 bytes (63.93487%)**. New verified C bytes: **0**. The 100% goal remains active.
