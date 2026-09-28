# tgm2p-decomp

A matching-decompilation effort for *Tetris: The Absolute – The Grand Master 2 Plus* (Arika, 2000), the Psikyo PS5V2 arcade board version. The target is the 1 MB big-endian SH-2 program held in `2b.u21` + `1b.u22`. ROMs are not included; `roms` is a symlink you point at your own verified set (`mame -verifyroms tgm2p` should say it's good).

## Which compiler built this ROM

The game was built with **Hitachi SHC v5.0, Release 32**, targeting `-cpu=sh2 -endian=big`, with optimization chosen per source file. Releases 26, 28 and 31 produce byte-identical output on every probe so far, so the evidence can't separate them from Release 32. Release 32 is simply the newest of the four, and it's the one the decomp.me patch uses. Release 10 and all six v5.1 releases are ruled out. GCC (the `cygnus-2.7-96Q3` build decomp.me offers for Saturn) is ruled out on sight: its prologues differ, and it never saves MACL.

Per-file flags seen so far:

| Source file (by ROM region) | Flags |
|---|---|
| EEPROM driver, `0x2fdd8`–`0x30880` | `-optimize=0` |
| Everything else matched so far: the main loop, playfield helpers, the RNG, timer and interrupt setup, and assorted small routines | `-optimize=1 -speed` |

MACL is saved as callee-saved, which is SHC's default (`-macsave=1`), so no flag is needed for that. Leaving the option off compiles byte-identically to `-macsave=1` on nuada, while `-macsave=0` changes `field_clear_flag`. The same kind of test shows `-division=cpu` is the default. One thing from the 1997 Hitachi manual worth knowing: `-speed` implies `-inline=20`, so small static helpers can get inlined without asking.

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
| `0x8518` | `field_check_flag` | `-optimize=1 -speed` | 83/83 (100%) |
| `0x85be` | `field_inc_398` | `-optimize=1 -speed` | 13/13 (100%) |

That's eleven functions at 100%: six unoptimized and five optimized. The optimized evidence that really counts is `field_clear_flag` (a two-level unrolled loop) and `field_check_flag` (a nested loop with hoisted invariants); `f_126c` is empty and `field_inc_398` is a single increment. Everything in `src/` matches too, and `make check` is the real test for those. The table only lists the cases the version comparison runs on.

Here's why the version is pinned. Six v5.0 and v5.1 releases fail each test:

- **The EEPROM file** (`0x714` bytes, six functions) is 100% only under v5.0r10–r32. Every v5.1 release scores 15.5%, because v5.1 dumps literal pools at different points.
- **`field_clear_flag`** is 100% only under v5.0r26 and later. v5.0r10 unrolls the loop differently and scores 6.1%.

Only v5.0r26, r28, r31 and r32 pass both.

### Things the matching work turned up

These are the rules to expect when decompiling the rest of the game.

- **Literal pools are shared.** SHC emits one literal pool for a run of functions and dumps it after an unconditional branch. That can land in the middle of the *next* function, as it does throughout the EEPROM driver. A function can only be byte-matched when the source around it is in place, so cases are written as runs of functions in file order.
- **Unoptimized register choice is stateful.** At `-optimize=0`, SHC rotates its temp registers (`r0`–`r3`) with a counter that carries across every earlier statement in the file. The same function body gets different registers depending on what came before it. Unoptimized code therefore has to be decompiled from the start of its source file. The EEPROM driver starts on a fresh file, which is why it matches with no preamble.
- **Optimized code is stateful too.** At `-optimize=1 -speed` the same thing happens, only less often: the state left by earlier functions in the file decides register choice and even instruction order. A function with *n* global stores in front of the RE medal routine (`0x21a10`) cycles its codegen with period 4, and only *n* ≡ 3 reproduces the ROM. Functions that are never emitted (unused `static`, `#pragma inline`) don't advance it. The state isn't just the registers the previous function left in use: two identical copies of the effect spawner compile differently back to back, while in the ROM all eight are identical. So a function that won't match on its own may just need its real predecessors. `medal.c` is the example: the RE medal alone tops out at 85%, but compiled after the ST and AC medals, from a fresh file, all three match. `tools/shc_probe/funcscore.py` scores each function in a file on its own, comparing pool loads by value, so a case can carry its predecessors without wrecking the score the way it does in `probe.py`.
- **Pool padding is `0xFF`.** Unwritten alignment gaps before a pool read as `0xFF` in the ROM (erased EPROM), while `rof2elf.py` zero-fills them. The probe uses `rof2elf_fillff.py` on nuada, which is the stock script with its one gap-fill byte changed.
- **Cache-through addresses.** The code addresses hardware through the SH-2 cache-through mirror (`0x2xxxxxxx`): `0x23000004` is the EEPROM port MAME maps at `0x03000004`, `0x24000000` is sprite RAM (`0x04000000`), and `0x2004002c` reads the ROM header at `0x4002c`.
- **The game runs from RAM.** The boot code at `0x400` runs from ROM and copies ROM `0x780`–`0x313fc` to RAM at `0x06000000` (the table at `0x313fc` says so, and a RAM dump from MAME agrees), so every call and function pointer in the game names a RAM address: ROM offset + `0x5fff880`. The main loop's call to `0x6014390` is the function at ROM `0x14b10`. The linker script links that block at its RAM address, so a C file that takes the address of one of its own functions gets the same pool word the ROM has.
- **SHC's runtime library is at the end of that block.** The division routines start at ROM `0x30d40` (RAM `0x60305c0` is signed 32-bit division, `0x603076c` the remainder) and the variable shifts sit at RAM `0x6030944` (left) and `0x6030a04` (arithmetic right).
- **Only a real in-file call proves two units share a file.** SHC calls a function it has already seen in the same file with `bsr` and everything else with `jsr` through the pool, so putting a caller and callee in one file when the ROM uses `jsr` breaks the caller. Two units that each match alone and also match together may or may not be one file.
- **Source shapes that SHC keeps distinct.** A chained assignment (`a = b = c = 0`) stores in a different order from separate statements. `for (i = 0, p = arr; ...)`, `for (p = arr, i = 0; ...)` and a plain loop pick different registers. `if (k)` and `if (k != 0)` differ for a `short`. Declaring a prototype's parameters `short` rather than `int` changes how arguments are pushed. Even an argument the callee ignores matters: the effect spawners only match with the task allocator declared `f_17614(void)` rather than taking the field pointer. An assignment inside a call argument (`f(..., y = *p, ...)`) is evaluated right to left with the other arguments and spills `y`. A narrow array index sometimes shows up as the byte offset truncated after the multiply (`(unsigned char)(id * 72)`), which has to be written out.
- **Inlining and tail calls follow node counts.** With `-speed`, small functions defined anywhere in the file get inlined, and a call in tail position becomes a `bra`/`jmp`. `f_11e98.c` needs three calls to an empty function after its last real call to stop both, so the original probably had statements that compile to nothing there.

### Near misses worth coming back to

These are in `tools/shc_probe/cases/wip/`.

- **`f_23048` (72.9%).** The second half matches exactly. The prologue allocates its frame by pushing argument registers, which only six prologues in the whole ROM do.
- **`f_2f304` (79.6%).** It needs the rest of its source file for the unoptimized register phase. That file runs from `0x2e1b8` to `0x2fdd8`, about 35 functions tied together by `bsr` calls. Its first function matches from a fresh file, which supports that start. The second (`g = v * 48 / 60`) loads the destination address before the multiply, and no form tried so far reproduces that.
- **`f_26f64`/`f_26faa`, `0x60e8`, and the sprite helpers at `0x2e06c`.** Real structural progress, but not converged.
- **The secret-code checker at `0x23828` (93.8%, `code_23828.c`) and a sprite draw at `0x2090c` (80.3%, `draw_2090c.c`).** Register choice only in the first. Its score moves with the file state, so it probably needs the effect functions before it in the same file.
- **`0xef04`.** It does a 16.16 fixed-point multiply with `dmuls.l` and `xtrct`, which C can't express, so it probably came from an `#pragma inline_asm` helper.

## Building

`make check` (on nuada, or `tools/nmake.sh check` from the Mac, which syncs the sources first) rebuilds the whole program ROM from source, then splits the result back into `2b.u21` and `1b.u22` and compares them against the original EPROM dumps by SHA-1. It also prints how much of the code is in C.

`splits.txt` is the map. Each line gives a start address and what builds that stretch of ROM: `asm`, `c` (with its source file) or `bin`, and each segment runs to the next line's address. The vectors (`0x0`–`0x400`) and everything after the code (`0x313fc` onward, starting with what looks like the RAM-copy table) are included as binary. The code in between is disassembled by `tools/build.py split` into `asm/*.s`. Those files are generated, not committed, and hold real instructions with labels for branch targets and literal pools. Any halfword the assembler can't reproduce exactly, mostly data tables inside the code, falls back to a raw `.short`.

To move a function into C, write the source with the usual `/* rom: ... flags: ... */` first-line header and get it to 100% with the probe. Then add a `c` line for its range to `splits.txt` and run `make split check`. The range has to be one that nothing outside it reaches into: no pc-relative load, branch or `bsr` may cross its edges, because SHC only uses `bsr` within a file and places literal pools after a run of functions. External names follow a convention the build resolves without any table: `f_2b4e8` is the function at ROM `0x2b4e8` (linked at its RAM address), `d_3afb4` is ROM data at `0x3afb4`, and `g_6060022` is whatever lives at `0x06060022`. Anything else goes in `symbols.txt`. The linker script asserts every segment starts at its ROM address, so a C unit that comes out a different size fails loudly rather than shifting everything after it. Breaking a matched file on purpose (a one-byte change in `src/eeprom.c`, or a wrong address in `symbols.txt`) makes the check fail, which is the point.

`make report` writes an `objdiff.json` and a progress report (`build/report.json`) with [objdiff](https://github.com/encounter/objdiff), the format [decomp.dev](https://decomp.dev) reads. Every asm file is a unit with a target only, labelled at each function start `fn.py` finds. Every C file is a unit whose target is rendered from the ROM, with the C object's function names and the literal-pool words written as their `symbols.txt` symbols, so objdiff pairs the functions and compares relocations as well as bytes. The same `objdiff.json` also opens in the objdiff GUI.

CI (`.github/workflows/build.yml`) runs `make check` and `make report` on every push to `main` and uploads the report as the `tgm2p_report` artifact, which is what decomp.dev picks up. The ROM can't live in the repo, so the job runs inside a private container image, `ghcr.io/masonc15/tgm2p-build`, that holds the two program ROM halves plus the toolchain. `tools/ci/Dockerfile` builds it, with every download pinned by hash to the copies used on nuada. Pull requests from forks can't pull that image, so their builds are skipped. decomp.dev has no platform for arcade games yet; [decomp.dev#50](https://github.com/encounter/decomp.dev/pull/50) adds one, and the project can be registered once that's live.

## Layout and tools

The heavy work runs on nuada: `/drive2/tgm2p` holds the SHC compilers, wibo, `rof2elf`, Ghidra 12.1.4 and the Python venv, with the repo mirrored at `~/workspace/tgm2p-decomp` there.

- `tools/interleave.py` joins the two EPROM halves into `build/prog.bin` and splits a rebuilt image back apart, checking SHA-1s.
- `tools/fn.py` prints annotated listings with function extents and literal pools, and `--scan` classifies every call target.
- `tools/cuts.py` lists the smallest ranges that can move into C on their own: runs between function starts that no pc-relative load, branch or `bsr` crosses. `--opt-only` skips units with unoptimized code, `--c` includes the ones already in C.
- `tools/shc_probe/funcscore.py <case.c>` (on nuada) compiles a case and scores each function against the ROM by itself, with pool loads compared by value, so functions can be matched in the context of the ones before them. `--diff NAME` prints a listing.
- `tools/nprobe.sh [probe.py args]` syncs `tools/` to nuada and runs the probe there, for example `tools/nprobe.sh --cases opt_188ac` or `tools/nprobe.sh --diff v5.0r32 --cases opt_8518` for a side-by-side listing. `--dir` points it at another directory of cases, which is handy for trying a few dozen generated variants of one function at once.
- `tools/shc_probe/shcc.sh` compiles one file with one SHC build under wibo and converts it with `rof2elf`.
- `tools/ghidra/` holds the headless scripts: memory map setup, seeding functions from `tools/seeds.py`, and exporting per-function decompiled C.
- `tools/mame_cov.lua`, `tools/run_cov.sh` and `tools/tracecov.c` capture MAME trace windows for execution coverage, streamed through a FIFO.
- `tools/m2c_fn.py 0x188ac field_clear_flag` turns a ROM function into GNU-as SH-2 assembly and runs [m2c](https://github.com/matt-kempster/m2c)'s `sh2` target on it for a first-draft C. It comments out SHC's MACL save and restore, which m2c doesn't model, and `--asm` prints the assembly alone.
- `tools/perm_setup.py <case> [func] [--cases DIR]` turns a probe case into a [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) directory under `/drive2/tgm2p/perm/`. The permuter keeps only the function it's permuting, so `compile.sh` splices the case's other functions back in around it. That preserves SHC's shared literal pools and its unoptimized register state. The permuter has no SuperH support upstream, so the checkout at `/drive2/tgm2p/decomp-permuter` carries `tools/permuter-sh2.patch`. Already-matched functions score 0 through this path, which is the sanity check.
- `tools/scratch.py <src.c> <start> <end> <func> "<flags>"` creates and compiles a scratch on a private decomp.me instance (on nuada, at `/drive2/tgm2p/decomp.me-local`, with `decompme/decomp.me-saturn-shc.patch` applied). The target is rendered from the exact ROM bytes, with pool words that hold a `symbols.txt` address written as that symbol, so a matching source scores 0.
- `refs/` (gitignored) holds reference material: the 1997 Hitachi SH C compiler manual (with a text dump), the SH-1/SH-2 programming manual, the SH7604 hardware manual, the SuperH assembler manual, and MAME's `psikyosh` driver source.
- `decompme/` holds the patch that adds this compiler to decomp.me's Saturn platform, submitted as [decomp.me#2115](https://github.com/decompme/decomp.me/pull/2115), along with the PR text and `decompme/NOTES.md`. `compilers-saturn-shc.patch` is the unused two-PR fallback.
