# Original TGM2 as a compiler reference — continuation120

All six supplied arcade ZIPs and Switch NSP/RAR files were copied from macbook, SHA-256 verified locally and on nuada, and retained separately from the production ROM. Local inputs are `inputs/comparison-roms/` beside this repository; nuada inputs are `/drive2/tgm2p/reference-roms/macbook-2026-10-03/`. Every one of the 23 arcade ZIP members matches MAME 0.289 CRC-32 and SHA-1. The joined Plus image is exactly the current target.

Evidence: `tools/shc_probe/cases/session_rom_comparison120/`.

The 186-entry cross-ROM search covers 185 remaining ASM functions plus the already matched field scan. It returns candidates for 156 entries, with 42 best normalized candidates exact and 92 at least 95%. These are search results, not proven function identities or matching C credit. Normalization removes PC-relative displacement changes; extents can include pools and shared data. A heuristic extent finder can stop early at branches. Focused hard near-matches generally preserve the same register choices in both games.

A useful real difference appears in the save context: TGM2+ adds `sum++` to the checksum. Original checksum `0x11d0c–0x11d8e` is 130 bytes; Plus `0x131e8–0x1326c` is 132 bytes. The following writers begin with different alignment and scheduling. A naive search aligned the original writer at `0x11d90`; call-target evidence corrects it to `0x11d8e`. Scheduling causality from alignment remains a hypothesis, not an established result.

`paired_save.py` compiles genuine paired four-function sources with the same SHC v5r32 flags and each game's independently observed external symbol bindings. The only source behavior change is removing Plus's checksum increment; function names and the first-line address metadata identify the original layout. Both produce 668-byte contexts with all four function roots at their original addresses. Standard prefix validation accepts each game's checksum and dirty setter, with the checksum's three external-literal proofs. Writers remain rejected with 88 differing bytes each; readers differ by 34 Plus bytes and 36 original bytes. Two fresh native captures reproduce the compiler sections excluding comment metadata. Four normal/debug SHCMDL replays exactly reproduce `.reg` and `.ilb`.

This paired oracle lets future allocator hypotheses be tested against two authentic builds. Neither original-game bytes nor rejected Plus code count toward the target. New verified Plus bytes: **0**.

The two Switch packages have valid, bounded PFS0 tables with six members each, including encrypted NCA archives. Container parsing does not verify NCA contents or signatures. No decrypted game payload is available from those files yet; a question about an already decrypted game-data directory remains pending. This does not block the arcade work.
