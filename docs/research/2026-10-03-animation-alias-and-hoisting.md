# Animation aliases, common expressions and loop hoisting — continuation150

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**.
All ten complete-unit candidates are strict rejects; no matching credit is added.
The previous goal turn reverified existing ROM copies and added no new decompilation evidence. This continuation advances the compiler investigation with reproducible distinctions between alias binding, common expressions and loop hoisting.

## Controls and results

Case: `tools/shc_probe/cases/session_animation_alias150`.
Sources are generated from frozen149 controls. The standalone decrement is
`n = n - 1` throughout. Every brightness reference feeds an existing field write.
No extra consumers, padding, forced registers, compiler changes or relaxed comparisons are used.

| Control | Body bytes | Frame | spr1 kind/frequency/home | Static spr1 reads |
|---|---:|---:|---|---:|
| const_pointer | 1762 | 108 | addressed / 9 / 16 | 5 |
| const_pointee | 1762 | 108 | addressed / 9 / 16 | 5 |
| const_both | 1762 | 108 | addressed / 9 / 16 | 5 |
| split_explicit | 1762 | 108 | addressed / 9 / 16 | 5 |
| preload_explicit | 1766 | 108 | addressed / 9 / 16 | 5 |
| shared_explicit | 1756 | 108 | addressed / 7 / 36 | 4 |
| inline_pair | 1752 | 108 | addressed / 9 / 20 | 5 |
| direct_address | 1758 | 108 | addressed / 9 / 16 | 5 |
| direct_address_preload | 1756 | 104 | addressed / 7 / 32 | 4 |
| direct_value_preload | 1752 | 108 | ordinary / 5 / 44 | 4 |

Original: 1748 bytes, frame108, spr1 home16, four static read sites. Every candidate begins at A604, rather than the required A608. Sizes and read counts do not establish identity or runtime access counts.

## Observed compiler decisions

1. **Qualifiers do not solve alias sharing.** All allocated sections of the three const controls are byte-identical to149 `split_refs_decrement`, checked directly. Two separately bound pointers retain two short dereferences even when both loads precede either write. Const short dereferences are counted alongside unqualified ones in the IR audit.
2. **Expression shape controls what is shared.**149 `brightness_shared_ref` shares one load but retains two mutable multiply assignments.150 `shared_explicit` uses `(long)*brightness_id * 64` in both initializers. Its middle tree contains one load and one multiply, copied through an anonymous common temporary. The native CFG has four spr1 read sites. Its merged offset homes also disturb the leading slot order; reducing calculation count alone is insufficient.
3. **Inlining does not make distinct pointer bindings equivalent.** A void inline helper consumes two pointer arguments and both brightness values. It retains six loops and creates no anonymous out-of-line calls, but expands to two separate loads and multiplies. The added spill group shifts tbl/total/spr1 to12/16/20.
4. **Address-taking survives expression simplification.** `direct_address` replaces each pointer temporary with `*(&spr1)`. Before allocation, the tree contains plain spr1 identifiers, no address or short-dereference operators in that region, yet spr1 remains an addressed kind2 record. It retains home16, frame108 and the six desired logical register roles; its body is four bytes shorter than149's split candidate, but still has an extra brightness read.
5. **Address-taking affects loop hoisting.** The paired preloaded sources differ only by `*(&spr1)` versus `spr1`. Both merge the multiply. The addressed form retains it inside the main loop; the ordinary form moves it before the loop and consumes the resulting temporary inside. The addressed form has frequency7/home32/frame104; the ordinary form has frequency5/home44/frame108. `HOIST_COMPARISON.json` records source lines and exact tree excerpts proving the placement.

The first audit expected both preloaded controls to retain one multiply in the brightness block. The ordinary control contradicted that expectation because SHC hoisted the multiply. The audit now verifies this distinction explicitly rather than ignoring the missing node.

## Validation and next action

Ten successful stock native compiles and driver captures, twenty middle replays,
ten CFG audits and ten read-only hardware home traces. Captured native sections match their controls; all middle and hardware replays reproduce captured REG/ILB exactly. All ten sources retain36 exports, six loops and zero inline-return gotos or anonymous local calls. Home models reproduce **273 records and1,380 weighted contributions**. `verify.py` runs source reproduction, native/CFG checks, home/frequency models, alias/hoisting assertions and production rechecks.

Both Nuada checkouts retain44 production pins,35 prior frozen cases, the original ROM hashes and the fixed progress denominator. Last full production build/test gates remain checkpoint122's independent clean builds and243 tests; no production files changed here.

Preserve149 `split_refs_decrement`,148 `c2_mask_explicit` and150 `direct_address` as complementary controls. Avoid another const or inline-pair sweep. The next useful compiler question is whether the late generator can reuse a spr1 load after middle-pass frequency collection, and specifically what invalidates that reuse across the first brightness byte store. Compare the original's single load with150's direct-address two-load sequence before trying another source rewrite. No generic SHC alias or common-expression pass has yet been located in the static decompilation; these conclusions are limited to the observed controls.
