# tgm2p-decomp

A matching-decompilation effort for *Tetris: The Absolute – The Grand Master 2 Plus* (Arika, 2000), the Psikyo PS5V2 arcade board version. The target is the 1 MB big-endian SH-2 program held in `2b.u21` + `1b.u22`. ROMs are not included; `roms` is a symlink you point at your own verified set (`mame -verifyroms tgm2p` should say it's good).

## Which compiler built this ROM

The game was built with **Hitachi SHC v5.0, Release 32**, targeting `-cpu=sh2 -endian=big`, with optimization chosen per source file. Releases 26, 28 and 31 produce byte-identical output on every probe so far, so the evidence can't separate them from Release 32. Release 32 is simply the newest of the four, and it's the one the decomp.me patch uses. Release 10 and all six v5.1 releases are ruled out. GCC (the `cygnus-2.7-96Q3` build decomp.me offers for Saturn) is ruled out on sight: its prologues differ, and it never saves MACL.

Per-file flags seen so far:

| Source file (by ROM region) | Flags |
|---|---|
| EEPROM driver, `0x2fdd8`–`0x304ec` | `-optimize=0` |
| Playfield helpers (`0x188ac`, `0x8518`), ROM-header accessor (`0x1260`) | `-optimize=1 -speed` |

MACL is saved as callee-saved, which is SHC's default (`-macsave=1`), so no flag is needed for that.

### Evidence

Every number below comes from `tools/shc_probe/probe.py`. It compiles each case with every SHC build decomp.me hosts, then compares the object halfword by halfword against the ROM region named in the case header. The only bytes it excuses are the 4-byte literal-pool words the object relocates (addresses of external symbols) and the displacement field of `bsr`/`bra` calls whose target lies outside the function. Everything else must match exactly: instructions, register choice, scheduling, intra-function branches, pool constants and pool placement. Scores are per function, measured over that function's own byte range inside the region.

Results under v5.0r32 (the full 11-version matrix is in `build/final_probe.txt` after a run):

| ROM | Function | Flags | Match |
|---|---|---|---|
| `0x2fdd8` | `eeprom_send_bit` | `-optimize=0` | 86/86 (100%) |
| `0x2fe84` | `eeprom_write_enable` | `-optimize=0` | 108/108 (100%) |
| `0x2ff5c` | `eeprom_write_disable` | `-optimize=0` | 112/112 (100%) |
| `0x3003c` | `eeprom_read_all` | `-optimize=0` | 225/225 (100%) |
| `0x301fe` | `eeprom_erase_all` | `-optimize=0` | 184/184 (100%) |
| `0x3036e` | `eeprom_write_all` | `-optimize=0` | 191/191 (100%) |
| `0x1260` | `f_1260` (ROM-header field → global) | `-optimize=1 -speed` | 6/6 (100%) |
| `0x126c` | `f_126c` (empty) | `-optimize=1 -speed` | 6/6 (100%) |
| `0x188ac` | `field_clear_flag` | `-optimize=1 -speed` | 82/82 (100%) |
| `0x85be` | `field_inc_398` | `-optimize=1 -speed` | 13/13 (100%) |
| `0x8518` | `field_check_flag` | `-optimize=1 -speed` | 81/83 (97.6%) |

That's ten functions at 100%: six unoptimized and four optimized. Be honest about the optimized four, though: `f_126c` is an empty function and `field_inc_398` is a single increment. The optimized evidence that really counts is `field_clear_flag`, a two-level unrolled loop, plus the small `f_1260`.

Here's why the version is pinned. Six v5.0 and v5.1 releases fail each test:

- **The EEPROM file** (`0x714` bytes, six functions) is 100% only under v5.0r10–r32. Every v5.1 release scores 15.5%, because v5.1 dumps literal pools at different points.
- **`field_clear_flag`** is 100% only under v5.0r26 and later. v5.0r10 unrolls the loop differently and scores 6.1%.

Only v5.0r26, r28, r31 and r32 pass both.

### Things the matching work turned up

These are the rules to expect when decompiling the rest of the game.

- **Literal pools are shared.** SHC emits one literal pool for a run of functions and dumps it after an unconditional branch. That can land in the middle of the *next* function, as it does throughout the EEPROM driver. A function can only be byte-matched when the source around it is in place, so cases are written as runs of functions in file order.
- **Unoptimized register choice is stateful.** At `-optimize=0`, SHC rotates its temp registers (`r0`–`r3`) with a counter that carries across every earlier statement in the file. The same function body gets different registers depending on what came before it. Unoptimized code therefore has to be decompiled from the start of its source file. The EEPROM driver starts on a fresh file, which is why it matches with no preamble.
- **Pool padding is `0xFF`.** Unwritten alignment gaps before a pool read as `0xFF` in the ROM (erased EPROM), while `rof2elf.py` zero-fills them. The probe uses `rof2elf_fillff.py` on nuada, which is the stock script with its one gap-fill byte changed.
- **Cache-through addresses.** The code addresses hardware through the SH-2 cache-through mirror (`0x2xxxxxxx`): `0x23000004` is the EEPROM port MAME maps at `0x03000004`, `0x24000000` is sprite RAM (`0x04000000`), and `0x2004002c` reads the ROM header at `0x4002c`.
- **Runtime helpers live in RAM.** SHC's variable-shift runtime routines sit in RAM (`0x6030944` for left shift, `0x6030a04` for right shift), copied there at boot.

### Near misses worth coming back to

These are in `tools/shc_probe/cases/wip/`.

- **`field_check_flag` (97.6%).** The only difference is the order of two hoisted loads in the prologue: the table pointer and the `0x2000` mask. Declaration order, `register` and every option tried leave it unchanged.
- **`f_23048` (72.9%).** The second half matches exactly. The prologue allocates its frame by pushing argument registers, which only six prologues in the whole ROM do.
- **`f_2f304` (79.6%).** It needs the rest of its source file for the unoptimized register phase.
- **`f_26f64`/`f_26faa`, `0x60e8`, and the sprite helpers at `0x2e06c`.** Real structural progress, but not converged.

## Layout and tools

The heavy work runs on nuada: `/drive2/tgm2p` holds the SHC compilers, wibo, `rof2elf`, Ghidra 12.1.4 and the Python venv, with the repo mirrored at `~/workspace/tgm2p-decomp` there.

- `tools/interleave.py` joins the two EPROM halves into `build/prog.bin` and splits a rebuilt image back apart, checking SHA-1s.
- `tools/fn.py` prints annotated listings with function extents and literal pools, and `--scan` classifies every call target.
- `tools/nprobe.sh [probe.py args]` syncs `tools/` to nuada and runs the probe there, for example `tools/nprobe.sh --cases opt_188ac` or `tools/nprobe.sh --diff v5.0r32 --cases opt_8518` for a side-by-side listing.
- `tools/shc_probe/shcc.sh` compiles one file with one SHC build under wibo and converts it with `rof2elf`.
- `tools/ghidra/` holds the headless scripts: memory map setup, seeding functions from `tools/seeds.py`, and exporting per-function decompiled C.
- `tools/mame_cov.lua`, `tools/run_cov.sh` and `tools/tracecov.c` capture MAME trace windows for execution coverage, streamed through a FIFO.
