# TGM trilogy ROM reference analysis — continuation121

Copied the entire directory `macbook:/Users/colin/Games/TGM_trilogy/Roms/`:

- Local: `inputs/tgm-trilogy-roms/` beside this repository.
- Nuada: `/drive2/tgm2p/reference-roms/tgm-trilogy-2026-10-03/`.
- 23 files, 61,822,311 bytes. Every file's size and SHA-256 matches the source manifest on both destinations.

Evidence and rerunnable scripts are in `tools/shc_probe/cases/session_trilogy_roms121/`.
`SOURCE_MANIFEST.json` identifies source files; `INVENTORY.json` identifies every loose or archived payload; `SHARED_DATA.json` and `SHAPE_ROTATIONS.json` record the comparisons.

## Contents and value

There are 52 payload occurrences, including archived members. All 40 TGM2/TGM2+ occurrences match the previous arcade inputs byte for byte. The loose original TGM2 program pair joins to SHA-1 `2fca1c7eae6254a16a51e8812ad97f7541127f5b`; the archived Plus pair joins to our unchanged target `c84cf538962ddbb377290301d76d75d8b542e5a2`. The graphics, sound and default EEPROM data are also duplicates. This directory supplies no additional TGM2 revision.

The new content is TGM1 (`tgmj.zip`) and its board firmware (`coh3002c.zip`, also duplicated inside `tgmj.zip`). The six TGM1-specific payloads and current board BIOS/security payloads match MAME 0.289 CRC-32 and SHA-1 definitions. Both copies of `qsound.bin` instead match the older MAME 0.180 entry marked `BAD_DUMP`, not the current `dl-1425.bin`. They are identified legacy sound firmware, not an alternate game program.

Primary references, also saved with their versioned filenames in the case:

- [MAME 0.289 TGM2/Plus ROM definitions](https://github.com/mamedev/mame/blob/mame0289/src/mame/psikyo/psikyosh.cpp)
- [MAME 0.289 TGM1 and ZN-2 definitions](https://github.com/mamedev/mame/blob/mame0289/src/mame/sony/zn.cpp)
- [MAME 0.180 legacy QSound definition](https://github.com/mamedev/mame/blob/mame0180/src/devices/sound/qsound.cpp)
- [MAME 0.289 QSound definition](https://github.com/mamedev/mame/blob/mame0289/src/devices/sound/qsoundhle.cpp)

## Concrete shared data

TGM2+ ROM `0x3644c–0x3660c` contains 448 bytes for seven tetrominoes, four rotations each. The entire block occurs exactly in TGM1 `ate-05m.3h` at file offsets `0x71790` and `0x169f08`. Including the two preceding empty 64-byte slots gives a 576-byte match from TGM2+ `0x363cc` to TGM1 `0x71710` and `0x169e88`.

The existing TGM2+ collision reconstruction indexes the data as `[piece][x][rotation][y]`. For each of the 28 rotations, the analysis verifies precisely four occupied cells with values 1, 2, 3, 4. This authenticates a shared piece representation across the games and provides concrete file offsets for finding TGM1 consumers. It does not establish that the surrounding algorithms or two TGM1 copies are identical builds. The TGM2+ collision function `f_35f0` is already retained C; this discovery adds no C credit on its own.

Six selected level-dependent timing arrays, with lengths determined by their existing source indexing, do not occur wholesale in the TGM1 CPU ROM regions as 8-bit, little-endian 16-bit, or little-endian 32-bit arrays. That limited negative result does not rule out common timing logic or differently compressed tables. Ten `$Id:` strings in `ate-05m.3h` identify SDK library files (`sys.c`, `intr.c`, `bios.c`, `backup.c`, `jama.c`), rather than supplying source for the remaining SHC routines.

## Effect on the completion effort

TGM1's PlayStation-based CPU code is a reference for game semantics. It is not a sample from the TGM2+ SHC backend. Use the shared shape offsets to locate rotation/collision consumers only when an unresolved behavior question warrants it. For the current register allocation and scheduling plateau, the original TGM2/Plus paired save experiment in continuation120 remains the more direct compiler oracle: same SHC target, authentic checksum difference, two verified original layouts.

Production remains checkpoint118: **128,316 / 200,698 bytes (63.934868%)**, with **72,382 bytes remaining**. No production source, split, compiler, verifier, ROM target or denominator was changed. The goal remains active. The fresh production receipt verifies all 41 pins and ROM/report hashes in both nuada checkouts. The last complete build/test gates remain checkpoint118's independent clean builds and 243 enabled tests.
