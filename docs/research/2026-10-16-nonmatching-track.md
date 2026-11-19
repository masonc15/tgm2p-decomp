# Non-matching track

2026-10-16. A second, separate path for the unresolved code: functionally
equivalent C, linked only by the shiftable build (`make shiftable
NONMATCHING=1`, [shiftable-build.md](2026-10-16-shiftable-build.md)). It never
counts as matching coverage. The matching build, `make check`, `make status`
and every coverage figure ignore it; `splits.txt` is unchanged.

- `splits_nonmatching.txt` lists the units: `<start> <end> c-nonmatching
  <source>`. Each replaces part of an unresolved `asm` range, normally one
  function or the functions of one range, from its entry to the next.
- Sources live in `src/nonmatching/`, one per unit, and name their functions
  `f_<rom offset>`.
- Ranges the [hand-written scan](2026-10-16-handwritten-asm-scan.md) classifies
  as hand-written stay assembly.

## Acceptance

A unit is accepted only when both of these hold, as Colin set them on
2026-10-16:

1. Difftest, for every function the unit defines. `tools/difftest` reports
   `MATCHED_TRIALS` over 500 complete trials, none step-limited. A function
   whose original does not return on some random inputs is marked
   `difftest-nonreturning` in its source. It passes instead when:
   - its 500 trials show no difference, with step-limited trials compared up
     to the limit;
   - every step-limited trial hit the limit on both sides, so the C hangs on
     exactly the inputs where the original hangs;
   - up to five of those inputs still hang on both sides at ten times the
     limit (`difftest.py --status`, which records no events);
   - inputs on which the original returns also match: the random trials that
     completed, plus the inputs listed as `difftest-inputs f_x: ...`, run as
     trials with `difftest.py --input`.

   `tools/nonmatching_check.py` checks all of this for every listed unit, each
   function with its own HOME, and exits 0 only if all of them pass.
   `difftest-rom-end` and `difftest-steps` give a verified function end, or a
   larger step limit for an original that needs it on large random inputs.
   `difftest-returns f_x: f_y=LO..HI (why: ...)` keeps the stubbed results of
   f_x's calls to f_y within the range the real f_y returns, where its code
   proves that range and f_x indexes its frame or a table with the result
   (2026-11-17, [mod-build.md](2026-11-17-mod-build.md)).
2. The MAME replay check. `make shiftable NONMATCHING=1` and then
   `tools/replay_check.sh build/shiftable-nm/tgm2p.bin` run a 12,000-frame
   attract run and a 24,000-frame scripted gameplay replay, comparing screen
   hashes with the original ROM every 100 frames. Which mode applies depends
   on whether the replays execute the unit:
   - A unit they never execute must pass as it stands.
   - A unit they execute passes with `--pin-idle`. The frame wait's idle
     counter reads as 40,000 in both images, so the random sequence no longer
     depends on cycle counts, and the check also records the real idle count
     of every frame, and the game's own frame counter `g_6065644`. A
     replacement that overruns the frame time makes the game miss a frame,
     and the check fails. Until 2026-11-17 a missed frame was read as "the
     candidate ran no idle loops in a frame where the original did". That
     test also fires on the original's own code moved by padding, so the
     check now compares the frame counter instead, which falls behind the
     original's when a frame is missed
     ([mod-build.md](2026-11-17-mod-build.md); Colin left the decision to
     that session, which adopted it). `tools/idle_report.py` gives each unit's smallest idle
     count over the frames its code ran in, against the original's over the
     same frames, and the frames it missed.

Accepted units are logged in `GOAL.md` under a separate "Non-matching track"
heading.

## How a unit is linked

`tools/shiftable.py` compiles the unit with the pinned SHC v5.0r32 and its
header's flags, and links it in place of the replaced range. Undefined names
resolve to the shiftable build's unit symbols, as for matching C. A replaced
range exists only as its function entries (`_f_<rom>`). Any other reference
into it fails the link, for example a jump into the middle of a function from
outside, or a pointer to an internal label. The build enforces these rules:

- Addresses only as symbols. A literal-pool word that looks like a code, RAM
  data or ROM data address fails the build, since an integer would not move
  with the layout. So `(struct frame *)0xa767c` becomes `(struct frame
  *)d_a767c`. BSS and hardware registers stay integers.
- No writable data. A unit that defines `.data` or `.bss` fails the build: the
  game's state lives at fixed addresses, so it is declared `extern`. Constant
  data (string literals, local initializers) goes to ROM data ahead of the
  filler, which shrinks to keep the image at 1 MB.
- Literal islands. SHC shares literal pools between neighbouring functions,
  so kept code (a `c-prefix` unit or an assembly neighbour) can load a pool
  word that lies inside the replaced range. 0x1288 in the retained 0x1278
  prefix, for example, loads 0x13a8 inside `f_1340`. Each such word is copied
  to an island at the start of the replacement unit, as a symbol expression
  when it is an address, and the load is pointed there.
- Alignment. Each replacement unit ends where the original ended, modulo 4.
  The code after it then keeps the alignment that its pc-relative `mov.l`
  loads and pools depend on.
- Code size. The game's random sequence depends on frame timing, which
  depends on the boot code's copy and checksum loops, so on the size of
  `.text` ([shiftable-build.md](2026-10-16-shiftable-build.md#timing-the-games-randomness-counts-idle-loops)).
  `--text-size N` pads `.text` with `nop` to exactly N bytes. `make shiftable
  NONMATCHING=1` uses 0x3107c, the original plus 1,024, a size whose boot
  phase matched the original in the padding experiments. `--keep-size` (pad
  each unit to its original size) is a stricter option for units that fit.

## Where the C comes from

Drafts already existed for most functions. Some came from earlier matching
attempts (`research/`, `tools/shc_probe/cases/wip/`). Others are the genuine
context functions in the `c-prefix`/`c-range` sources, compiled for compiler
state but not retained. A triage ran difftest on up to three drafts per
SHC-shaped function. A helper then extracted the passing functions (with the
draft's declarations and static helpers) into one source per unit and
converted what functional equivalence requires:

- Address literals became `d_`/`g_` symbols.
- The settings menu's static tables (`names[30]`, `off9`, `ypos`) and its
  string literals became references to the original tables and strings in
  the RAM data image (`g_6032b8c` and others). Difftest compares call
  arguments, and the original passes pointers to those copies.
- Helpers the original compiled inline stay inline: the draft of `f_13380`
  called `f_13378`, which SHC had inlined, and the replacement writes its
  two statements instead.

Functions without a passing draft are drafted from the disassembly.

## Accepted units

The first eighteen accepted units, and `settings_d394.c`, are those whose functions neither replay executes (from the instruction-fetch trace of both runs; `gfx_2ac4c` turned out to run, see below, and passed this replay all the same). For them the replay
shows that linking the replacement C, its islands and the moved code leaves
the rest of the game frame-identical. It cannot show that the functions
themselves behave alike; difftest covers that part.

| Unit | Range | Bytes | Functions | Draft |
|---|---|---:|---|---|
| `item_7fa0.c` | 0x7fa0–0x8156 | 438 | f_7fa0 | context in src/item_8164.c |
| `field_ready_8b86.c` | 0x8b86–0x8c96 | 272 | f_8b86 | context in src/field_ready_8b20.c |
| `vm_ec0c.c` | 0xec0c–0xee9c | 656 | f_ec0c | context in src/vm_tail_ee9c.c |
| `vm_f29a.c` | 0xf29a–0xf4c6 | 556 | f_f29a, f_f3a2 | context in src/vm_empty_f292.c |
| `save_1326c.c` | 0x1326c–0x13378 | 268 | f_1326c | context in src/save_checksum_131e8.c |
| `draw_2090c.c` | 0x2090c–0x209a4 | 152 | f_2090c | wip/draw_2090c.c |
| `effect_20a9c.c` | 0x20a9c–0x20b24 | 136 | f_20a9c | context in src/effect_209a4.c |
| `shuffle_21d94.c` | 0x21d94–0x225e4 | 2,128 | f_21d94 | wip/shuffle_21d94.c |
| `board_setters_277d0.c` | 0x277d0–0x278c0 | 240 | f_277d0, f_27862 | context in src/board_setters_2774a.c |
| `field_8c96.c` | 0x8c96–0x907c | 998 | f_8c96 | drafted from the disassembly (old drafts differed) |
| `board_2657c.c` | 0x2657c–0x26f64 | 2,536 | f_2657c, f_26cb0, f_26e18 | drafted from the disassembly |
| `board_queue_274c0.c` | 0x274c0–0x2769e | 478 | f_274c0 | context in src/board_queue_2733a.c |
| `gfx_2ac4c.c` | 0x2ac4c–0x2af5e | 786 | f_2ac4c | wip/gfx_2ac4c.c |
| `rankcode_238a8.c` | 0x238a8–0x23e3e | 1,430 | f_238a8, f_238fc | drafted from the disassembly and src/rankcode_draw_23e3e.c |
| `settings_a608.c` (was `settings_a226.c`) | 0xa608–0xacdc | 1,748 | f_a608 (f_a226, f_acdc, f_ad80, f_af8c matched 2026-11-18) | settings packet of 2026-10-16 |
| `settings_b3c0.c` | 0xb3c0–0xbd8a | 2,506 | f_b3c0, f_b892, f_bc02 | settings packet of 2026-10-16 (retired 2026-11-18: now matching C in src/settings_reset_9c0c.c) |
| `settings_be28.c` | 0xbe28–0xd28c | 5,220 | f_be28, f_c21a, f_cac8, f_cf68 | settings packet of 2026-10-16 (retired 2026-11-18: now matching C in src/settings_reset_9c0c.c) |
| `checksum_30880.c` | 0x30880–0x30a44 | 452 | f_30880 | staged packet of 2026-10-16 |
| `main_780.c` | 0x780–0x1108 | 2,440 | f_780 | packet of 2026-10-16 |
| `field_setup_12e2.c` | 0x12e2–0x1896 | 1,460 | f_12e2, f_1340 | packet of 2026-10-16 |
| `field_step_20be.c` | 0x20be–0x2e4c | 3,470 | f_20be, f_29ea, f_2a92, f_2c0a, f_2d06, f_2d54 | packet of 2026-10-16 |
| `field_3c88.c` | 0x3c88–0x3e2e | 422 | f_3c88 | packet of 2026-10-16 |
| `field_4044.c` | 0x4044–0x44d4 | 1,168 | f_4044 | packet of 2026-10-16 |
| `allclear_57d4.c` | 0x57d4–0x5b80 | 940 | f_57d4 | packet of 2026-10-16 (retired 2026-11-19: now matching C in src/field_51f4.c) |
| `field_5b80.c` | 0x5b80–0x601a | 1,178 | f_5b80 | packet of 2026-10-16 (retired 2026-11-19: now matching C in src/field_51f4.c) |
| `field_61d0.c` | 0x61d0–0x7004 | 3,636 | f_61d0, f_667c, f_67f4 | packet of 2026-10-16 |
| `field_7324.c` | 0x7324–0x7658 | 820 | f_7324 | packet of 2026-10-16 |
| `speed_7658.c` | 0x7658–0x7956 | 766 | f_7658 | packet of 2026-10-16 |
| `field_7c08.c` | 0x7c08–0x7f38 | 816 | f_7c08 | packet of 2026-10-16 |
| `field_flag_9180.c` | 0x9180–0x95b8 | 1,080 | f_9180 | packet of 2026-10-16 |
| `field_96ac.c` | 0x96ac–0x9b90 | 1,252 | f_96ac | packet of 2026-10-16 |
| `settings_d394.c` | 0xd394–0xe39c | 4,104 | f_d394, f_d8e0, f_d962, f_dbe8, f_e074 | packet of 2026-10-16 (retired 2026-11-18: now matching C in src/settings_reset_9c0c.c) |
| `text_width_e39c.c` | 0xe39c–0xe3b8 | 28 | f_e39c | packet of 2026-10-16 (retired 2026-11-18: now matching C in src/settings_reset_9c0c.c) |
| `text_e3e0.c` | 0xe3e0–0xe520 | 320 | f_e3e0, f_e412, f_e490 | packet of 2026-10-16 (retired 2026-11-18: now matching C in src/settings_reset_9c0c.c) |
| `hud_1050e.c` | 0x1050e–0x105ba | 172 | f_1050e | packet of 2026-10-16 |
| `hud_10788.c` | 0x10788–0x10942 | 442 | f_10788 | packet of 2026-10-16 |
| `rank_display_12fa8.c` | 0x12fa8–0x131e8 | 576 | f_12fa8 | packet of 2026-10-16 |
| `save_13380.c` | 0x13380–0x13484 | 260 | f_13380 | packet of 2026-10-16 |
| `rank_14d50.c` | 0x14d50–0x15104 | 948 | f_14d50, f_14f1c | packet of 2026-10-16 (2026-11-19: f_14d50 now matching C in src/rank_tail_149b4.c; f_14f1c remains as `rank_14f1c.c`, 0x14f1c–0x15104) |
| `rank_15104.c` | 0x15104–0x15794 | 1,680 | f_15104, f_152d4 | packet of 2026-10-16 (2026-11-19: f_15104 now matching C in src/rank_tail_149b4.c; f_152d4 remains as `rank_152d4.c`, 0x152d4–0x15794) |
| `rank_15794.c` | 0x15794–0x15f10 | 1,916 | f_15794 | packet of 2026-10-16 |
| `draw_15fae.c` | 0x15fae–0x163fc | 1,102 | f_15fae | packet of 2026-10-16 |
| `draw_163fc.c` | 0x163fc–0x1715c | 3,424 | f_163fc, f_16938 | packet of 2026-10-16 |
| `board_26264.c` | 0x26264–0x2631a | 182 | f_26264 | packet of 2026-10-16 |
| `board_26ff0.c` | 0x26ff0–0x27310 | 800 | f_26ff0, f_2722c | packet of 2026-10-16 |

45 units and 58,956 bytes of unresolved assembly in all. Everything after
`rankcode_238a8.c` was accepted on 2026-11-17
([mod-build.md](2026-11-17-mod-build.md)): the three settings units and
`checksum_30880.c` by the strict replay (sha1 `ed4d3dd3`), `settings_d394.c`
likewise (`bd8ad056`), and the 26 units the replays execute by the pinned
replay of the whole list (`60f22cbe`, which is what `make shiftable
NONMATCHING=1` builds now). Evidence for the first fourteen, from the first nine
(`251d8b2`), the four after them (`251cb6e`) and `rankcode_238a8.c`:

```
$ python tools/nonmatching_check.py
...
11/11 functions MATCHED_TRIALS over 500 complete trials   (first nine units)
6/6 functions MATCHED_TRIALS over 500 complete trials     (field_8c96, board_2657c, board_queue_274c0, gfx_2ac4c)
2/2 functions MATCHED_TRIALS over 500 complete trials     (rankcode_238a8)
$ make shiftable NONMATCHING=1
shiftable: 0x100000 bytes sha1 987d89efd8f9d31290fbb549e74598c468624a8e differs from the original ROM; ...
$ tools/replay_check.sh build/shiftable-nm/tgm2p.bin
attract: 12000 frames, 120 checkpoints every 100 frames (96 distinct screens): identical
gameplay: 24000 frames, 240 checkpoints every 100 frames (237 distinct screens): identical
```

Difftest's instruction coverage of these functions is 72–100%. The
instructions no trial ran (`difftest.py --cov`) are literal-pool words the
estimate counts as code, and in `f_f29a`/`f_f3a2` the unreachable half of
an `abs()` of a value whose sign is already known.

## Functions the replays execute

For a function that runs during the replays, the strict replay compares
cycle counts as well as behaviour. The frame wait in `f_2b4e8` counts idle
loops into `g_606000c`, and the system random `f_2beca` multiplies its state
by that count. Any replacement that takes a different number of cycles per
frame therefore changes the piece sequence, even when it behaves
identically. The pilot unit `text_e3e0.c` (the text-drawing helpers, difftest-clean) diverged in gameplay at frame 2,000 for this reason.
`replay_check.sh --pin-idle` makes every read of `g_606000c` return 40,000 in
both images, which removes this one timing channel and compares everything
else. With it, the pilot matched over the full 12,000 + 24,000 frames.
Colin accepted the pinned replay for units the replays execute on
2026-10-16, with the condition that no frame is missed (see Acceptance).

The executed units were accepted on 2026-11-17, once the missed-frame test
read the frame counter: 26 units (`video_init_1108.c`'s range had become
matching C), all 40 functions passing difftest, and the 45-unit image
passing the full pinned replay with the frame counter identical to the
original's. [mod-build.md](2026-11-17-mod-build.md) has the evidence. In the
first run of the 41 units, on 2026-10-16 in the cloud session, the marks
showed `gfx_2ac4c` running in 1,237 frames, which the entry-fetch trace had
missed, and it had passed the strict replay anyway.

The pinned replay also catches real differences that difftest misses. With
all 28 difftest-clean units linked it diverged in attract mode at frame
2,311. A per-unit bisection found `rank_15104.c`, and a MAME trace of the
original showed `f_152d4`, the title screen loop, running there. The draft
indexed the background animation `d_aa7a8` in 12-byte frame records where
the ROM steps 24 bytes. Difftest had reached that loop in almost no trial:
its stub for the frame wait `f_2b4e8` returns a random nonzero value, so
the screen returns at its first wait (26% instruction coverage). After the
fix, the 28 units together match over the full pinned replay (attract
12,000 and gameplay 24,000 frames, all 360 checkpoints identical).

## Ranges that stay assembly

`0x400`–`0x780`, the boot module that runs from ROM before the program is
copied to RAM, is hand-written. The scan found strong evidence in five of its
nine routines (reset, manual reset, the shared idle loop and two interrupt
handlers). Two probes with SHC v5.0r32 rule out the remaining four:

- Three idle loops at 0x674, 0x67a and 0x680 are `nop; bra self; nop`. SHC
  compiles `for (;;);` to `bra self; nop`, with no `nop` before the branch.
- The free-running-timer handler at 0x6ac saves r0/r1, addresses the timer
  registers through `mov.l` pool words, and ends in `rte`. SHC's `#pragma
  interrupt` on the same body saves r2–r6, loads `0xfffffe10` as a
  sign-extended `mov.w` constant, and never uses r0 as the base.

`0x225fa`–`0x2261c` holds no instructions: it is the literal pool of the
matched `f_225e4` (a `c-range` whose retained bytes end with its code), so
there is nothing to replace.
