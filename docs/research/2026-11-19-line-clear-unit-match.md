# The line-clear unit matches (2026-11-19)

The functions from 0x51f4 to 0x61d0 were compiled as one file. f_57d4, the
line-clear handler, calls f_5262, f_52b6, f_5390, f_5760, f_51f4, f_541c and
f_579c with `bsr`, and its last literals sit in f_5b80's first pool, so no
part of it could be scored on its own. Earlier rounds kept 0x51f4-0x541c,
0x541c-0x57d4 (a c-prefix) and 0x601a-0x61d0 (a c-range with f_5b80 as
context) from separate drafts, while f_57d4 and f_5b80 stayed assembly with
non-matching C in `src/nonmatching/`.

`src/field_51f4.c` is now the whole unit, and every function in it compiles
byte-identical at its ROM address with no dead `if (0)` lines. It replaces
`src/f_51f4.c`, `src/f_541c.c` and `src/field_helpers_601a.c` with a single
ordinary `c` row for 0x51f4-0x61d0, which adds f_57d4 and f_5b80 (2,118 bytes):
coverage 153,868 → 155,986 / 200,698 (76.67% → 77.72%). f_61d0, which follows
in the original file and calls f_601a and f_60e8 with `bsr`, is still asm.

## f_57d4

Scored in the full unit, the first draft was 661 bytes off and 16 bytes short.
The source forms that closed it:

- Reading `g_6064880` directly at all six sites. A local
  `unsigned long *mode = &g_6064880;` swapped the registers of `mode` and `y`.
- The `f->b398++` under `f->mode & 0x80` goes through an inline helper
  (`#pragma inline(bump)`), which changes how the increment ranks.
- One merged condition, `if (special && owner->b388 + count < 8)`, before
  `f_5390`, and `if (!f->task && count > 1)` before `owner->b388 += count`.
- `if (count == 0)` in the 0x100000 flag test.
- No `break` after `f->b339 &= ~4;` in the last switch case. With the
  `break`, the test at 0x330 used r0 instead of r10.

That left 13 bytes, all displacements of pool loads near the end. They were
f_5b80's: once f_5b80 had its exact size they disappeared.

## f_5b80

A clean rewrite came out 4 bytes long with 949 bytes different. In order of
effect:

- The cell copy indexes the source row as `(y + 1) * f->width`. SHC hoists
  the loop-invariant `y + 1` out of the inner loop, sign-extends it once and
  keeps it in a stack slot, and the multiply then takes the ROM's operand
  order (`mul.l r2,r0`). A declared `long next = y + 1;` compiles 4 bytes
  long. A dead `if (0) x = 0;` in the inner loop fixed that, but the
  multiply still came out as `mul.l r0,r2`. A `short next` stores the slot as
  a word.
- The owner's piece kind is read inside the `g_6064880 & 4` test as
  `unsigned char kind = f->owner->w35c; if (kind == 2)`, with every
  argument of the `f_35f0` call spelled out through `f->owner->`. That gives
  the ROM's word load and `extu.b`, and keeps the owner pointer on the stack
  across the call.
- The owner position is a `union fixed pos[2]` read with `.part.hi`. A
  `struct pos` with `x`, padding and `y` laid out the same compiles to
  different code.
- The time bonus is `unsigned long bonus = (18000 - f->time) / 60;` followed
  by `if ((18000 - f->time) % 60) bonus++;`, with no `remain` local.

Undoing any one of the four (a `struct pos`, a `next` local, a `remain`
local, or the merged `&& (unsigned char)f->owner->w35c == 2` test) breaks the
match again: three change the size and the `remain` local swaps registers in
the bonus block.

The flag line keeps `f->cells[y * (short)f->width + x].flags &= 0xff7f;`. The
cast stops SHC reusing the copy's index and matches the ROM's `muls.w`.

## Checks

`make split check report` passes both chip hashes. All 377 tool tests pass
with `PREFIX_COMPILE_TESTS=1`. The one that had been failing since the
settings unit landed, which compiled f_b2f4 and f_b352 without the
`literal-tail-calls`, `genuine-rodata` and `boot-rom-tails` options their
rows now use, was updated to pass them. `make shiftable` is identical to the
ROM and `make mod-check` plays the replays like the original. The
non-matching units `allclear_57d4.c` and `field_5b80.c` are retired. With
the rest of the non-matching track linked in, `make shiftable NONMATCHING=1`
(sha1 ddc315d2) and `tools/replay_check.sh --pin-idle` give identical
screens over 12,000 attract and 24,000 gameplay frames, with no frame missed.
