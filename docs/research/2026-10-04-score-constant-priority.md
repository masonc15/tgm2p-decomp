# Score-function constant allocation — continuation162

The corrected `session_score7658_wordadd_next/baseline.c` freshly reproduces the 766-byte score-function near-match at ROM7658–7956: 19 differing bytes, two nominal ELF roots, 1456 context bytes, strict rejection at768E. Its source SHA-256 is `b98e5f60de3a085e07147ee70a3d9fcb7175e14ab2bd5d667e6786e102d2c0fe`. The following function still contains the inherited, unresolved f17834/RNG calling-convention assumption. Nothing from this diagnostic context is retained as new C.

## Direct compiler evidence

Read-only hardware-breakpoint observations of stock SHCMDL show constant2 and constant65535 each have priority2 and profit−7. Their interference neighborhoods are identical apart from each other. The recovered partition sort puts constant2 ahead of65535, producing internal registers3/4 (R6/R7), opposite the target's R7/R6. Both priority terms for constant2 come from the real `n > 2` comparisons, baseline lines66 and78, each with weight1. The 65535 terms are compiler-generated nodes with line0; the preallocation tree places these in the narrowed `(h−1)` and `(n−1)` expressions. There is no evidence that counting the score-expression multiplications would explain constant2's two priority terms.

The independent graph model reproduces all126 original candidates and310 eligibility events across six sources, including the partition sort and allocations. The named bonus-threshold control adds one category2 stack record with physical−1, priority0 and no clashes during allocation; this is separately checked rather than treated as an original register candidate. A first verifier assumed a fixed record set and rejected this case; that assumption was corrected. The register-hint control does not add this record.

## Consumed-source controls

| Source change | Differing target bytes | Context bytes |
|---|---:|---:|
| Baseline |19|1456|
| q widened to int or long |131|1452|
| Actual bonus threshold assigned to short, int or long |668|1460|
| Named int minimum2 for both line-count comparisons |25|1456|
| Named unsigned-short minimum2 |19|1456|
| Ordinary register-int minimum2 |57|1456|
| Both comparison literals changed to2L |19|1456|
| Both comparison literals changed to2U or2UL |681|1484|

The short minimum folds back to the original constant candidate. An int minimum removes that constant candidate, but changes other homes and does not match. The ordinary register hint raises the variable's priority, moves65535 toR6, and disturbs other allocations. It is a negative control, not a proposed production fix. Signed long literal spelling does not break the tie; unsigned comparisons alter the expression/temporary graph and regress substantially.

The q controls preserve values: unsigned16-bit level divided by4 and optionally incremented gives0..16384, representable in every tested q type. The bonus threshold controls consume the actual signed16-bit value at its original short-circuit evaluation point. Minimum2 is assigned before both branches and used in both comparisons. Signed/unsigned comparison controls are equivalent over unsigned-char n0..255. No source adds unused consumers or memory operations; f7956 text is unchanged. `prepare.py` regenerates all twelve sources from the hash-pinned baseline and the verifier checks exact regeneration.

## Validation and next action

Twelve native controls reject under the normal prefix verifier. Six representative sources have independently captured native objects equal outside comment metadata, twelve normal/debug middle replays with exact `.reg`/`.ilb` equality, six allocation traces,34 constant priority totals and92 variable priority totals. Both traced stages preserve original compiler output. `VERIFICATION.json`, `GRAPH_ANALYSIS.json`, `CONSTANT_ANALYSIS.json`, `VARIABLE_ANALYSIS.json` and `NATIVE_ANALYSIS.json` contain the receipts.

Do not repeat coefficient, q-width, named minimum or comparison-suffix sweeps. Next trace the backend's scratch-register selection at the bonus guard around7788–77A4, comparing its lifetime state with the earlier constant2/65535 homes. This will distinguish a downstream consequence of the initial tie from a separate expression-order mechanism; the present evidence does not prove that swapping the two constants alone fixes all19 bytes.

Both Nuada checkouts still match checkpoint161's46 production pins,48 preceding frozen cases, original full-ROM hashes and128398/200698 report. Latest complete gates remain checkpoint161's245 enabled tests and independent clean builds. This research changed no production source, split, compiler, proof rule, denominator or coverage: **63.9757247%,72,300 bytes remaining, zero new C bytes**. Goal active.
