# Cell sprite f_2090c match

f_2090c (0x2090c-0x209a4, 152 bytes) now matches as its own unit,
`src/draw_2090c.c`, a `c` row in `splits.txt`. The non-matching packet
`src/nonmatching/draw_2090c.c` is gone.

## What was wrong

Every earlier draft declared `f_1159c` with int parameters, the way
`src/effect_207d0.c` and `src/effect_209a4.c` do, and kept the cell row in an
`int y`. With that prototype the row is summed into r3, the `exts.w r14` for
the `n < 0` clamp is scheduled late and 22 bytes differ. The HUD units
(`hud_f958.c`, `hud_ranges_f958.c`, `hud_101b8.c`, `hud_11254.c`) declare the
same callee as

```c
extern void f_1159c(long, short, short, unsigned char, short);
```

With that prototype and `short y`, SHC passes the row without narrowing it,
sums it in r0 as the ROM does (`mov.w @(2,r7),r0; add r3,r0; ...; mov r0,r5`)
and puts `add #-10,r5` in the clamp branch's delay slot. The body is otherwise
the old draft.

## Why a unit of its own

The unit before it, `src/effect_207d0.c`, ends with a pool at 0x208e8 shared by
f_207d0 and f_2085a. f_2090c's own literals follow its `rts` at 0x20992, and
the unit after it, `src/effect_209a4.c`, keeps all of its literals in one tail
pool after f_20a9c. A function inside either neighbour would have put its
constants in that unit's shared pool. Both neighbours also need the int
prototype: switching `effect_209a4.c` to the short one grows f_20a32 by
10 bytes unless its `y, x` become `short` too.

## Cast control

Before the prototype was found, `y = (short)(... - 10);` with an int prototype
reached 4 differing bytes: it gives the r0 sum and the early clamp test but
leaves `add #-10,r0; exts.w r0,r5` where the ROM has `mov r0,r5;
add #-10,r5`. Reordering the terms changes nothing, since SHC canonicalizes
them, and every other placement of the cast is worse (20 to 137 bytes).

## f_20a9c, the same shape

f_20a9c in `src/effect_209a4.c` draws the same kind of sprite. With the short
prototype (`long` first parameter, so the two calls in f_209a4/f_20a32 get
`(long)` casts), `short y, x` in f_20a32, and `short sy` and a single
expression in f_20a9c, f_209a4 and f_20a32 stay exact and f_20a9c drops from
26 to 4 differing bytes. What remains is in the prologue: the ROM loads
`#0x314` (the shake pointer) before `add #-4,r15` and `#0xdf` after it; ours
has them the other way round. Pointer forms, declaration order, statement
order, parameter widths and dead lines all leave the 4 bytes. That draft is
not admitted.

## Verification

`make split check report` passes both chip hashes at 158,556 / 200,698
(79.0023%). `make shiftable` is identical to the ROM, `make mod-check` plays
the replays like the original, and `make shiftable NONMATCHING=1` with
`tools/replay_check.sh --pin-idle` is identical over attract 12,000 and
gameplay 24,000 frames.
