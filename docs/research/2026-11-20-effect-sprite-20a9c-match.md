# Effect sprite f_20a9c match

f_20a9c (0x20a9c-0x20b24, 136 bytes) now matches. `src/effect_209a4.c`, which
was a `c-prefix` retaining f_209a4 and f_20a32 (0x209a4-0x20a9c), becomes an
ordinary `c` unit for 0x209a4-0x20b24, so the three functions and their shared
pool after f_20a9c's `rts` are all verified. The non-matching packet
`src/nonmatching/effect_20a9c.c` is gone.

## Two changes

The first is the one that fixed f_2090c the same day
([2026-11-20-cell-sprite-2090c-match.md](2026-11-20-cell-sprite-2090c-match.md)):
the unit now declares the HUD units' prototype

```c
extern void f_1159c(long, short, short, unsigned char, short);
```

f_209a4 and f_20a32 stay exact once their two calls pass `(long)` pointers
and f_20a32's `y, x` are `short`. With `short sy` and one expression for it,
f_20a9c drops from 26 differing bytes to 4.

The last 4 bytes were the prologue. The ROM loads the shake offset first,
then allocates the frame, then loads the width offset:

```
mov.w #0x314,r6
add   #-4,r15
mov.w #0xdf,r0
```

Ours had the two constants the other way round. Pointer forms, declaration
order, parameter widths, and dead lines all left those 4 bytes. Scaling the
column parameter in its own statement before taking the shake pointer gives the
ROM order:

```c
void f_20a9c(struct field *f, short x)
{
	struct point *d;
	short sx;
	short sy;

	x *= 8;
	d = &f->shake;
	sx = x + d->x - (f->width / 2) * 8;
	sy = (f->height - 20 - 1) * 8 + d->y - (f->height - 1) * 8 - 6;
	f_1159c((long)d_a8bdc, sy, sx, 0xba, 115);
}
```

Reusing the parameter for the whole sum (`x = x * 8 + d->x - ...`) stays at
4 bytes, and so does keeping `d`'s initializer with `x *= 8` after it. Only
the scale-then-pointer order gives the ROM schedule.

## Prefix fixture

`tools/test_c_prefix.py` still compiles this source as a prefix ending at
0x20a9c to check the r1 literal tail proof in f_20a32. That test is unchanged
and passes; the build no longer needs the proof for this unit.

## Verification

`make split check report` passes both chip hashes at 158,692 / 200,698
(79.0700%). All 377 tool tests pass with `PREFIX_COMPILE_TESTS=1`.
`make shiftable` is identical to the ROM, `make mod-check` plays the replays
like the original, and `make shiftable NONMATCHING=1` with
`tools/replay_check.sh --pin-idle` is identical over attract 12,000 and
gameplay 24,000 frames.
