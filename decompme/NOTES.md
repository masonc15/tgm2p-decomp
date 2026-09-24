# decomp.me patch notes (submitted as [decomp.me#2115](https://github.com/decompme/decomp.me/pull/2115))

decomp.me already hosts Hitachi SHC v5.0/v5.1, but only on the Dreamcast platform, and that command line hard-codes `-cpu=sh4 -endian=little -fpu=single -macsave=0`. `decomp.me-saturn-shc.patch` adds SHC v5.0 Release 32 to the existing `saturn` platform (SH-2, big-endian) under the ID `shc-v5.0r32-sh2`. IDs have to be globally unique because `backend/coreapp/compilers.py` keys `_compilers` by ID.

It's a single decomp.me PR, with no change to the decompme/compilers repo. The new entry sets `base_compiler=SHC_V50R32`, so it runs out of the Dreamcast `shc-v5.0r32` package (`compilers/dreamcast/shc-v5.0r32`, which already ships `bin/shc.exe` and `rof2elf.py`). It needs no `compilers.linux.yaml` entry of its own. That's how the IRIX IDO compilers reuse the N64 packages (`IDO53_IRIX` sets `base_compiler=IDO53`, and there is no `irix:` section in the yaml). The trade-off is the same one IRIX accepts: a host that disables Dreamcast loses this compiler too.

The patch applies against decompme/decomp.me `main` at `b908a4f` and touches three files:

- **`backend/coreapp/compilers.py`**: `SHCSaturnCompiler`, `SATURN_SHC_CC` and `SHC_V50R32_SH2`, plus its `_all_compilers` entry under `# Saturn`. The definition sits after the Dreamcast block because it references `SHC_V50R32`.
- **`backend/coreapp/flags.py`**: the `SHC_SH2_FLAGS` flag class.
- **`frontend/src/lib/i18n/locales/en/compilers.json`**: the display name and flag labels.

`compilers-saturn-shc.patch` is the older two-PR route. It gives Saturn its own image in decompme/compilers (a `values.yaml` entry plus the rendered Dockerfile), and the decomp.me side would then need the `compilers.linux.yaml` line back and would have to wait for that image to publish. It's kept only as a fallback in case the maintainers prefer a separate package.

## Flags

The new flag class drops two options and adds two:

- **Dropped: `-fpu=` and `-round=`.** The Dreamcast `SHC_FLAGS` class offers them. On nuada they were accepted with `-cpu=sh2` but changed nothing, even in code using float and double.
- **Added: `-division=cpu|peripheral|nomask` and `-macsave=0|1`.** The Dreamcast command line hard-codes these, and SH-2 targets actually vary them.

`-aggressive=2` stays, because it really does change SH-2 code (float multiplication for constant division). Everything in `COMMON_SHC_OLD_FLAGS` is inherited.

The r32 help text doesn't show defaults, so they were measured:

- Leaving `-macsave` off gives the same bytes as `-macsave=1`, while `-macsave=0` changes `field_clear_flag`.
- Leaving `-division` off calls the same `__divls`/`__divlu` helpers as `-division=cpu`. `peripheral` and `nomask` switch to `__divlsp` and `__divlspnm`.

The 1997 Hitachi manual (`refs/`) confirms the meanings: `peripheral` uses the SH-2 on-chip divider with the interrupt mask raised to 15, and `nomask` uses the divider without changing the mask.

## How it was checked

Against clean upstream `main`:

- `git apply --check` passes.
- `ruff format --check`, `ruff check` and mypy pass ("no issues found in 66 source files").
- Biome 2.0.6 passes on the JSON.
- Under Django, the compiler resolves to `SHCSaturnCompiler` on `saturn` at `compilers/dreamcast/shc-v5.0r32`, with the `shc-old` → `shc-sh2` flag classes and no duplicate IDs.

The `cc` string was run on nuada under `/bin/bash -euo pipefail`, the way the sandbox runs it, with real `unix2dos`, `shc.exe` under wibo, and the stock `rof2elf.py --isa=sh2`. The output is `elf32-sh` (big-endian).

- `field_clear_flag` differs from the ROM only in its 2 relocated pool halfwords.
- The EEPROM file differs only in 10 relocated halfwords and 7 alignment-padding halfwords.

## Things to know before opening the PR

- **Style.** From recent merged external PRs: a title like "Add SHC v5.0 (Release 32) for Saturn", a body of a few short sentences (what it adds, which project needs it, how it was tested), and no pinging for reviews or Actions approval. A first-time contributor's CI waits for a maintainer to approve the Actions run. In August 2026 a long, pushy, LLM-written compiler PR (#2046) was publicly rebuked and later reverted, and the FAQ was tightened (#2070).
- **FAQ.** It says "We generally only accept contributions from people who use the site or are actively involved in the wider decompilation community", and asks for an issue first for ideas no existing issue covers. Compiler additions in practice skip the issue.
- **Open refactor.** `ethteck`'s open PR #2042 ("cromper") moves `compilers.py`, `flags.py` and `compilers.linux.yaml` under `cromper/`. If it lands first, this patch needs a rebase onto the new paths. The content near the SHC code is unchanged.
- **Pool padding.** `rof2elf.py` zero-fills alignment gaps (`add = b"\x00" * ...`, line 411 of the gist). EPROM-built targets like this one have `0xFF` there, so a pool-padding halfword can show up as a diff. That affects data only, and the fix belongs in `rof2elf.py` (Mc-muffin's gist), not in decomp.me.
- **`-pic=1`.** It is inherited from the common flags. It made the stock `rof2elf.py` fail on the EEPROM file ("Relocation expression too big"). That's a rof2elf limitation shared with Dreamcast, and this game doesn't use PIC.
