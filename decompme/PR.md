Title: Add SHC v5.0 (Release 32) for Saturn

Adds SHC v5.0 Release 32 to the Saturn platform for big-endian SH-2. It reuses the Dreamcast `shc-v5.0r32` package through `base_compiler` (the same way the IRIX IDO compilers reuse the N64 ones), so no compilers repo change is needed. The Dreamcast command line hard-codes `-cpu=sh4 -endian=little -fpu=single -macsave=0`, so this one has its own (`-cpu=sh2 -endian=big`). It also has a new `shc-sh2` flag class, which drops `-fpu`/`-round` (they have no effect on SH-2) and adds `-division` and `-macsave`. Leaving those unset gives the compiler defaults (`-division=cpu`, `-macsave=1`).

I'm using it for a Tetris: The Absolute The Grand Master 2 PLUS decomp (Psikyo SH-2 arcade), which was built with SHC v5.0. I tested it with a local instance, and scratches for functions from the ROM match with a score of 0. One caveat is that rof2elf.py pads alignment gaps with 0x00 while this ROM has 0xFF, so a literal pool padding halfword can show up as a diff.

Only r32 for now; the other SHC releases could be added the same way if anyone needs them.
