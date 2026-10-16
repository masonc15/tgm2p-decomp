# Flag sweep over the near-miss drafts

2026-10-16. `tools/flag_sweep.py` recompiles every existing near-miss draft
under each documented SHC v5.0r32 optimization option combination and reports
how each function's residual changes.

## Prediction (recorded before the full run)

The batch is far larger than the 24-compile default, so the prediction comes
first. No combination other than a draft's own flags makes a near-miss
function exact. The evidence: 164 of 166 linked sources match under
`-macsave=1 -optimize=1 -speed`, and the remaining two are the sound and EEPROM
files that pin `-optimize=0`. The documented near-miss residuals are register
allocation ties and pool-order differences that move with source shape and
translation-unit context, while these flags change code generation wholesale.
Expected observable: `-optimize=0`, `-nospeed` and `-size` change instruction
selection and raise residuals everywhere; `-noloop` changes only drafts with
counted loops (`-speed` already implies loop unrolling); the inline node
counts change only drafts with small static helpers. The stopping condition is
one pass over the full grid; any improvement under a flag set the build does
not already use is reported to Colin rather than integrated.

## Method

The option set comes from SHC's own help text (`shc.exe` with no arguments
prints it): `OPtimize=0|1`, `SPeed|NOSPeed|SIze`, `INLine[=<node-count>]` and
`NOINLine`, `NEstinline=<number>`, and `LOop|NOLOop`. The grid is their cross
product: optimize {0, 1} × {default, `-speed`, `-nospeed`, `-size`} ×
{default, `-noinline`, `-inline`, `-inline=10`, `-inline=20`, `-inline=40`,
`-inline=80`} × {default, `-loop`, `-noloop`} × {default, `-nestinline=3`},
minus `-nestinline` with `-noinline`: 312 combinations plus each draft's own
flags, 313 compiles per draft. Every other flag a draft carries (in practice
`-macsave=1`) is kept, and the compiler is always the pinned v5.0r32 through
`tools/shc_probe/shcc.sh`. Combinations whose `.text`, relocations and
function symbols are identical are compiled once and reported together.

Each function is scored with funcscore's per-function comparison (halfwords
over the ROM function's code, pool loads compared by value, outside calls
excused); the residual is the number of differing halfwords. A residual of
zero is a funcscore match, not a byte-exact unit: pool placement and the
surrounding unit still have to pass the strict range proof.

The 120 drafts: 13 research cases (`research/**/*.c` with a header), the 43
historical near-miss drafts in `tools/shc_probe/cases/wip/`, the 60 genuine
context drafts behind the `c-prefix`/`c-range` entries (their unretained
functions are the near-misses of the ranges `make status` lists), and the four
probe positive controls. One research case, `51f4-tu/tplA.c`, is a template
that does not compile on its own.

```
$ python tools/flag_sweep.py
...
120 drafts, 304 near-miss functions; 12 become exact under some flag set;
91 improve only under flag sets the build does not use; 2426 s
```

37,247 compiles produced 1,213 distinct objects. Results are in
`build/flag_sweep.json` (gitignored); `--only SUBSTR` reruns one draft.

## Controls

The controls behave as the README's pinning predicts. `field_clear_flag` is
exact under 66 of the 313 sets, exactly those equivalent to `-optimize=1
-speed` (`-inline`, `-inline=20`, `-loop` and `-nestinline=3` change nothing
under `-speed`), and wrong under `-noloop`, `-size`, `-nospeed` and
`-optimize=0`. `field_check_flag` is exact under 40. The EEPROM control is
exact under all 157 `-optimize=0` sets, which ignore the speed and inline
options. So the grid does reach different code, and the build's flags are the
ones that reproduce the controls.

## Effect of each option on the 304 near-miss functions

Each row changes one thing from `-optimize=1 -speed` and counts the functions
whose residual falls, stays or rises (all 304 functions, including those of
the optimize=0 drafts, which every optimize=1 set changes):

| Change from `-optimize=1 -speed` | Better | Same | Worse |
|---|---:|---:|---:|
| `-inline`, `-inline=20`, `-loop`, `-nestinline=3` | 0 | 294 | 10 |
| `-inline=10` | 9 | 276 | 19 |
| `-inline=40` | 17 | 235 | 52 |
| `-inline=80` | 17 | 198 | 89 |
| `-noinline` | 14 | 269 | 21 |
| `-noloop` | 26 | 177 | 101 |
| no `-speed` (default or `-nospeed`) | 27 | 120 | 157 |
| `-size` | 33 | 87 | 184 |

Most functions do not move at all under the inline and loop options; the speed
and size options change almost everything and mostly for the worse.

## Functions that reach a funcscore match

Twelve function entries reach residual zero under some set. Six are functions
already linked as C from other sources (`f_2e4c` in three research drafts,
`f_1ede` and `f_2058` in the `0x1914` context, `f_e63a` in the `0x9c0c`
context), so they say nothing new. The six unresolved entries:

| Function | Draft | Halfwords | Baseline residual | Exact under (examples) |
|---|---|---:|---:|---|
| `f_2d06` | `research/2026-10-16-20be-tail/{cmb_j_i,li}.c` | 39 | 10 | `-optimize=1 -inline=40 -loop` (no `-speed`), `-inline=80 -loop` |
| `f_2d06` | `research/2026-10-16-20be-tail/v4.c` | 39 | 10 | `-optimize=1 -size -inline -loop` |
| `f_e412` | `research/2026-10-16-e3e0-helpers/sa.c` | 63 | 24 | `-optimize=1 -speed -inline=80` |
| `f_e490` | `research/2026-10-16-e3e0-helpers/sa.c` | 65 | 22 | `-optimize=1 -speed -inline=80` |
| `f_e412` | `research/2026-10-16-e3e0-helpers/h1.c` | 63 | 35 | `-optimize=1 -speed -inline -noloop` |

Whole-draft effects matter more than single functions, since flags apply per
translation unit:

- `e3e0-helpers/sa.c` under `-optimize=1 -speed -inline=80`: total residual
  113 → 88, both `f_e412` and `f_e490` exact, no function made worse. This is
  the cleanest result of the sweep.
- `startup_dispatch_1914.c` (canonical context) under `-optimize=1 -speed
  -inline=10`: total 2,382 → 2,226, exact functions 6 → 8, no regressions,
  but the newly exact ones are already C elsewhere.
- `20be-tail` drafts under `-optimize=1 -speed -inline=40 -loop
  -nestinline=3`: total 2,825 → 2,641 but five functions that were exact
  regress, so no single set fits that unit.

Sixty-two of the 172 unresolved near-miss entries improve under some set; the
best ratios after the table above are partial (for example `f_e3e0` 39 → 4 and
`f_2a92` 94 → 54).

## Outcome against the prediction

The prediction is falsified in part: three unresolved functions reach a
funcscore match under flag sets the build does not use, and in one draft
(`sa.c`, `-inline=80`) without hurting anything else. Nothing was integrated.
Every one of these sets lies outside the build's three (`-macsave=1
-optimize=1 -speed`, `-macsave=1 -optimize=0 -speed`, `-macsave=1
-optimize=0`), so whether `0xe3e0`–`0xe520` or `0x20be`–`0x2e4c` may be built
with a different inline node count is Colin's decision. A match would still
need the full unit to pass the strict range proof, and a per-file inline
setting is a new hypothesis about the original build, which should first be
tested against the matched neighbours in the same files (a counter-control:
`-inline=80` must leave every already-matched function in that unit exact).
