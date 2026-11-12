# Inlined helpers that return: the settings menus' missing form

Solo work on phrixus, 2026-11-12. Coverage is unchanged at **139,096 /
200,698 linked C bytes (69.306122%)**; nothing here is admitted yet. In
`src/settings_reset_9c0c.c`, `funcscore.py` now gives:

| Function | Before | Now |
|---|---|---|
| `f_d962` | 25/310 | 310/310 |
| `f_dbe8` | 38/578 | 567/578 |
| `f_e074` | 37/404 | 400/404 |
| `f_e412` | 11/63 | 63/63 |
| `f_e490` | 43/65 | 65/65 |

`f_e412` and `f_e490` were already right and only needed their predecessors'
sizes. None of these is at its ROM address until the functions earlier in the
unit have their genuine sizes, so no range can be admitted yet.

## The mechanism

When SHC expands a `#pragma inline` function that contains a `return`
statement, each expansion ends in a label. A label inside a loop body or a
switch arm switches off the optimisations that cross it:

- a `while` loop around it is not rotated: it is entered with `bra` to a
  single test at the bottom;
- values the loop uses are not hoisted into a preheader, so addresses and
  constants are loaded inside each pass, at first use;
- a counted loop nested beside it is not unrolled, and its counter can lose
  its register to the constants;
- identical switch arms that each end in an expansion are not merged.

A hand-written label does the same (`while (run) { ...; unused:; }`), which is
how it was found. Without the pragma, a helper with a `return` is not inlined
at all.

The settings menus print centred text through such a helper:

```c
#pragma inline(cprint)
static short cprint(char *s, short y)
{
	return f_2bf58(s, y, (320 - f_2c0d0(s)) >> 1);
}
```

Its parameters are evaluated before the inner call and spilled, which the
hand-expanded expression never did. With it, `f_d962` is a plain `while (run)`
loop with a plain `switch`, and every structural difference is gone. The last
two differences were ordinary: `run` needs no `(void)&run` once the loop is in
this form, and the pad byte is tested directly (`g_6079304 & 0x80`) with no
local copy.

This explains the pattern recorded for `f_e074` and startup `f_780` (ROM keeps
callee addresses in `r8`-`r14`, reloads them per pass and spills loop locals)
without goto-built loops.

## How it was found

`tools/shc_probe/formsearch.py` runs a beam search over named source forms,
scored by `fdiff.py` alignment. On the tracked draft it found the do-while
entered by `goto` (118 to 208 of 269 instructions), then helper and type
changes (233). Three small batteries then asked what else gives the same three
observables (entry `bra`, loads inside the pass, pre-loop loads at first use):
any label in the loop body does, a label in each switch arm keeps the arms
apart, and a returning inline helper produces both from natural source.

`f_dbe8` and `f_e074` took two further changes: the same helper, and their
option arithmetic as separate statements. `c = (g_60792de >> 5) & 3; c++;
c = c % 3;` gives the ROM's `mov #3,r4 / and r0,r4 / add #1,r4` and puts the
divisor in the `jsr` delay slot; the compound `c %= 3` loads the divisor first.
Swapping the operands of `&` changes nothing. The pad byte is tested directly
in `f_e074` and through a local copy in `f_dbe8`.

## Open

- `f_dbe8` (11 halfwords): `&g_6079304` and the mask `~0x60` have each other's
  registers (`r13`/`r8`), and the loop test reads `run` into `r2`, not `r3`.
- `f_e074` (4 halfwords): the decrement's `and` is `and #3,r0 / mov r0,r4`
  where the ROM has `mov #3,r4 / and r0,r4`, and the same `r2`/`r3` choice at
  the loop test. Writing the decrement as two statements fixes the `and` but
  swaps two register pairs (`r8`/`r9`, `r10`/`r11`); thirteen dead-statement
  levers did not swap them back, so the two functions' increments and
  decrements are probably not yet in their common original form.
- The functions earlier in the unit (`f_a226`, `f_a608`, `f_b892`, `f_be28`,
  `f_c21a`, `f_cac8`, `f_cf68`, `f_d394`) have not been rewritten; `f_780` in
  the startup unit shows the same signs.
- Other inlined helpers with returns probably exist. A cheap test for one:
  `formsearch.py --kinds lab` on a function, since an unused label in the right
  loop reproduces the effect.

The packet is `research/2026-11-12-first-principles/` (`d962/`, `settings/`).
