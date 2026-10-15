# Palette initializer: semantic graph order and category3 entries

Production remains131780/200698 verified bytes. The previous goal turn was progress: it recovered the major predecessor homes and naturally placed the initializer at its correct address. This turn improves the aligned initializer from119 to99 differing bytes; an explicit four-iteration source has95. Neither strict-passes.

## What the remaining differences represent

The baseline's first402 initializer bytes agree. Its red/green masks use R11/R13 instead of R13/R11; blue is correctly R12. Later expanded iterations also swap f/t homes in R4/R5, changing dependent scheduling. Forty new full-context sources retain the initializer's actual2A576 root; the preceding1186 bytes are unchanged. Their initializer sizes are measured individually rather than assumed.

Reverse declaration order, separate per-color variables, separate scopes and reuse of the earlier search pointer all produce exactly the baseline bytes. Reversing loads, comma sequencing, inverted conditions, ternary selection, local mask lifetimes and explicit expansion are tested and preserved. No target comparison changes, object relocation tricks, filler code, patched compiler input or artificial uses were introduced.

## Condition order changes candidate order

Inverting only the red condition (`step <= 0` selects t, otherwise f) improves119->99. Fresh IR-origin graphs prove the semantic graph is isomorphic under candidate-index swaps23/24,36/37,48/49,60/61 (zero-based). Symbols, source lines, priorities, profits, conflicts and binding-group membership agree under that mapping. This reorders f/t identities at four red iterations. The first red pair has unequal profits52401/50941; later pairs tie at52399. Native homes therefore change at three later red pairs while the first retains its homes. The size is still1504, the root2A576, and the masks remain wrong.

All4096 independent swaps of the twelve color/iteration f/t pairs were then modeled using the authenticated native partition sort. Every hypothetical order sorts the mask candidates green/blue/red. None yields required red/blue/green. These are excluded offline predictions, not4096 native compiles and not matching credit. The result closes pair reordering on this graph; it makes no claim about different graphs.

## Loop-local masks are not constant-order fixes

A named red or green mask inside the loop adds four actual variable candidates, raising total76->80. Each has priority6, while the corresponding constant's priority falls32->16. The resulting extra copies change code size (1510/1512 bytes), despite unchanged root. Naming both masks gives84 candidates and1518 bytes. Naming blue too is redundant. These sources fail and should not be retried as a way to preserve the baseline instruction stream.

## Explicit expansion changes category3 entries

Explicit two-iteration C produces980 initializer bytes with six multiplies, not the target1504/twelve. Explicit four-iteration C, with the last two red conditions reversed, produces1504 bytes with twelve multiplies and95 differing bytes. It iterates the same bounded indices0..15 through real increments and operations.

The79-entry native graph has49 variable candidates and7 constants, exactly like the76-entry baseline. Only category3 count rises20->23. Mask priorities32 and profits-41991 stay fixed, but sorting now yields blue/green/red, hence blueR13/greenR12/redR11. This is also wrong. The lower raw difference count must not be mistaken for recovery of the masks or a complete function.

The next compiler investigation should identify creation and binding of these category3 entries. They affect unstable sort placement despite unchanged variable/constant counts and unchanged mask profit. Compare baseline, invert_r and manual_four_red; find which legitimate source lifetime/address structures create the extra groups before trying another source change. Keep both the99-byte implicit-loop candidate and95-byte explicit candidate, because their mismatches have different causes.

## Verification

Case session_gfx_init208 authenticates41 strict negatives (40 new/1 carried),20 output-preserving replays/2902 events,15 initializer allocation graphs,4 graphs with IR origins,17 independently assembled native captures, and1 constant collection. Traced REG/ILB output equals fresh untraced controls and original native captures. The graph models reproduce native profits, sort order and homes; analyze.py checks graph isomorphism, equivalent outputs, native multiply counts, category counts and offline exclusions. verify.py resolves original objects and independently recomputes all roots, scores and source/context hashes. No retained matching objects exist.

No production files changed. Checkpoint192's independent clean builds and253 enabled tests remain the applicable production gates; final checkpoint verification checks54 production pins and both Nuada ROM hashes. The full100% goal remains active, solo, without brainstorming.
