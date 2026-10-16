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

A unit is accepted only when both of these hold:

1. `tools/difftest` reports `MATCHED_TRIALS` over 500 complete trials (none
   step-limited) for every function the unit defines.
   `tools/nonmatching_check.py` runs this for every listed unit, each function
   with its own HOME, and exits 0 only if all of them pass.
2. The shiftable build with the unit passes the full MAME replay check:
   `make shiftable NONMATCHING=1` and then `tools/replay_check.sh
   build/shiftable-nm/tgm2p.bin`, a 12,000-frame attract run and a
   24,000-frame scripted gameplay replay, screen hashes compared with the
   original ROM every 100 frames.

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

The accepted units are those whose functions neither replay executes (from the instruction-fetch trace of both runs). For them the replay
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

9,644 bytes of unresolved assembly in all. Evidence, from the first nine
(`251d8b2`) and the four after them:

```
$ python tools/nonmatching_check.py
...
11/11 functions MATCHED_TRIALS over 500 complete trials   (first nine units)
6/6 functions MATCHED_TRIALS over 500 complete trials     (field_8c96, board_2657c, board_queue_274c0, gfx_2ac4c)
$ make shiftable NONMATCHING=1
shiftable: 0x100000 bytes sha1 04c5d8cea7913cf4516ea81fae647b4a02ad0db6 differs from the original ROM; ...
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
Whether a pinned replay may count as the replay check is an acceptance-rule
question for Colin, so no unit that the replays execute is accepted yet.

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
