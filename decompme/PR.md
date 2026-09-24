Title: Add SHC v5.0 (Release 32) for Saturn

Hi! I'm working on a matching decomp of [Tetris: The Absolute The Grand Master 2 PLUS](https://github.com/masonc15/tgm2p-decomp), a Psikyo SH-2 arcade game. It turns out it was built with Hitachi SHC v5.0, which decomp.me already has for Dreamcast. But those entries hard-code `-cpu=sh4 -endian=little -fpu=single -macsave=0`, so they can't produce SH-2 code.

This adds `shc-v5.0r32-sh2` to the Saturn platform. It reuses the Dreamcast `shc-v5.0r32` package through `base_compiler`, the same way the [IRIX IDO compilers reuse the N64 ones](https://github.com/decompme/decomp.me/blob/b908a4f32af89721fc0db3d8155ffe9696a91c13/backend/coreapp/compilers.py#L1084-L1089), so nothing is needed in the compilers repo. It gets its own command line (`-cpu=sh2 -endian=big`) and a small `shc-sh2` flag class. That class drops `-fpu`/`-round`, which have no effect on SH-2, and adds `-division` and `-macsave`. Leaving those unset gives the compiler defaults, `-division=cpu` and `-macsave=1`, which I checked by compiling with and without them.

I tested it on a local instance, and scratches for two functions from the ROM compiled and scored 0. `ruff check`, `ruff format --check`, `mypy` and Biome pass too. One caveat is that [rof2elf.py](https://gist.github.com/Mc-muffin/c2d3f30e50c5c5749f994973441c503a) pads alignment gaps with 0x00 while this ROM has 0xFF, so a literal pool padding halfword can show up as a diff.

I only added r32, since that's the release I matched against (r26 through r31 give identical output on this game). The other SHC releases could go in the same way if anyone needs them.

Let me know if this approach makes sense. Thanks for taking a look!
