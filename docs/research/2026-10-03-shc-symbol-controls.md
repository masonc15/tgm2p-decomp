# SHC symbol, bound, and counter controls — continuation97

Eleven source controls leave the pointer-register tie unresolved. Ten reproduce
the entire baseline context exactly; the multiply/shift control increases target
differences32→61. Direct constant-priority observation shows that the shared
literal3 still receives exactly24 points. New matching bytes: **0**. Production
remains **127,276 / 200,698 (63.41668%)**.

## Controls and results

All changes use existing computations or declarations. Named bounds feed the six
actual three-row loops. Scoped counters replace each selected loop's existing
index; they introduce no extra computation. Factored arithmetic is exact for
index0,1,2. No fake reads, dead consumers, padding, compiler patches, or forced
physical registers are used.

| Control | Positioned differences | Full context versus baseline |
|---|---:|---|
| Swap the first two external declarations |32| Identical |
| Reverse the five game-array declarations |32| Identical |
| Reverse all eleven external data declarations |32| Identical |
| Name the six bounds with short rows=3 |32| Identical |
| Use const short rows=3 |32| Identical |
| Use enum rows=3 |32| Identical |
| Express mode0 wait as i*3*4+36 |32| Identical |
| Express it as ((i*3)<<2)+36 |61| Different arithmetic |
| Give mode0's second loop a scoped counter |32| Identical |
| Give mode1's second loop a scoped counter |32| Identical |
| Give both loops independent scoped counters |32| Identical |

Every target and following root remains nominal12AD4/12FA8. All controls fail the
unchanged default verifier. The declaration results establish no effect from
these source-level reorderings; they do not establish that arbitrary internal
symbol-table mutations would be irrelevant.

Across every control, p retains priority13/profit−10 and physicalR10; q retains
priority13/profit−10 andR12. These are the opposite of the original pair. Literal3
retains priority24/profit5/R11. No rows WEB survives in any named-bound form.
Renaming and scoping the selected counters likewise does not change emitted code.

## Original constant-priority observations

Three fresh hardware replays cover baseline, named_bound, and wait_mul_shift.
Each observes six literal3 groups with loop depth1, factor4, and one use. Their
contributions are4+4+4+4+4+4=24. The extra source spelling of3 inside multiplication
does not add another contribution to this allocation candidate. Do not infer a
change in allocation cost from an extra textual literal occurrence.

Across all constants,150 contribution products reconstruct33 final priorities,
each agreeing with the diagnostic WEB record. Original.reg/.ilb remain exactly
equal to normal compilation. Input configuration and compiler instructions are
unchanged; the tracer writes hardware debug registers only.

## Verification

Case: `tools/shc_probe/cases/session_rank_symbols97`. `prepare.py` regenerates all
sources. `analyze.py` validates11 native capture comparisons,22 normal/debug
middle-pass replays,352 priority/profit calculations, all three hardware replays,
and complete-context equality for the ten unchanged controls. Aggregated results
are in `ALL_RESULTS.json`, `REGISTERS.json`, `ANALYSIS.json`, and
`PRIORITY_ANALYSIS.json`. Batch WATCH/REPLAY files cover only the last batch;
validation uses each source's original capture and outputs.

Fresh checks in both nuada checkouts verify33 checkpoint84 production pins,
original ROM hashes, and fixed-denominator reports. No production source changed
and no full build/test suite was rerun for these negative private experiments.
Checkpoint84 remains the last full gate with234 enabled tests.

## Change of target

The current report still has73,422 incomplete bytes across47 units.
`REMAINING_UNITS.json` saves that inventory with its report hash. Several rounds
of rank investigation established useful rules without increasing coverage, so
park this source family and retain case92's genuine nominal32-difference baseline.

Next target is the146-byte VM handlerF200–F292, confirmed still assembly in the
current split map. `NEXT_TARGET.json` pins the existing genuine source and
historical receipt. The source includes full972-byte context, actual empty ROM
callbacks, and the genuine F3A2 body. Its historical five-byte mismatch swaps
R5/R6 between the sprite-call result and zero used for two field stores. That is
archived evidence and must be refreshed before any current-match claim.

Begin with a fresh compile and true-root/retained-range checks, then trace the
common result/zero scratch allocation atF270–F286. Preserve the existing8-byte
F292–F29A and262-byte F4C6–F5CC ranges. The old VM STATUS.md records extensive
source sweeps; use the newer compiler models to explain the decision, rather than
repeat those sweeps. No new verification exception or coverage credit follows
from selecting this target.
