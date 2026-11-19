# Rank result screens f_14d50 and f_15104 match (2026-11-19)

The 0x149b4 compiler unit holds three rank result screens, f_14d50, f_14f1c
and f_15104, that differ only in their palette table, banner frame and
return value. All three run the same sequence: wait for the fade, set up the
screen, show a 300-frame banner while polling coin/start, then wait for
`g_6064750` to clear, fade over ten frames and return.

## What fixed them

Starting from the context in `src/rank_switch_14b10.c` (f_14d50 130 raw
differences, f_14f1c 333, f_15104 50) the changes, in the order they paid:

1. The end-of-screen block moved inside the polling loop as
   `if (done) { ... return N; }` under `do { ... } while (1);`, with
   `short timer`, `int done`, and `if (timer) timer--; else done = 1;`.
   The fade counter is `int j` compared as `(short)j < 10`. This took
   f_15104 to 10 differences, all a timer/done/#160 register rotation.
2. The same form applied to f_14d50 and f_14f1c (f_14f1c indexes its banner
   as `&d_aa7a8[k]` with `short k = g_606000a & 15`) gave 10 and 12.
3. `fr = d_aa928; f_185f0(fr, 0, 0, 160, 40);` with a `struct frame *fr`
   local. SHC's register-candidate ranking puts #160 ahead of `done` and
   `timer` once the frame address is a separate short-lived web, which is
   the ROM's r11/r10/r9 assignment. f_14d50 and f_15104 become exact.
   Declaration position of `fr` doesn't matter.

Negative controls for the rotation, all at 10 or worse: a `pal` local for
160 (6 at best, shifts other registers), `done`/`timer` type flips, return
value temporaries, `id`/`p` temporaries for the setup calls, and
declaration permutations.

## f_14f1c (8 bytes off)

The `fr` form is worse for f_14f1c (the `k` index changes which webs exist),
so it keeps the direct `&d_aa7a8[k]` argument. `i = i + 1;` in the first
three-frame loop fixes `done`'s register (12 to 8). What remains: #160 in r9
instead of r11, and `i` and #10 sharing r11 instead of r9. Temporaries for
`g_60661a6` or `d_67d90` before the setup calls rotate the registers
correctly but schedule one literal load 2 bytes early (4 differences).
Declaration order (1,439 permutations), `if (0)` dead lines at every
statement boundary, loop forms, increment forms and `k` types don't beat 8.

## Verification

`make split check report` passes both chip hashes. All 377 tool tests pass
with `PREFIX_COMPILE_TESTS=1`,
`make shiftable` is identical to the ROM and `make mod-check` plays the
recorded replays like the original. The non-matching units become
`rank_14f1c.c` and `rank_152d4.c`; both report MATCHED_TRIALS over 500
difftest trials, and `make shiftable NONMATCHING=1` (sha1 2f817e76) with
`tools/replay_check.sh --pin-idle` is identical over attract 12,000 and
gameplay 24,000 frames.
