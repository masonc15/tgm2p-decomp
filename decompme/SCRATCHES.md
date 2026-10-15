# Public decomp.me scratches

Refreshed 2026-10-02 under [masonc15](https://decomp.me/u/masonc15), after [SHC support merged](https://github.com/decompme/decomp.me/pull/2115). All 23 original URLs were preserved and refreshed; 112 scratches were added. The account now has **135 scratches: 134 compile successfully, one is explicitly compiler-blocked, and 89 score 100%**. No match overrides are enabled.

The publication snapshot contains **121,792 / 200,698 linked code bytes (60.68421%)**. It was copied from nuada's working tree into an isolated directory and rebuilt with `make check report`; both EPROM hashes and the whole-image hash pass. That working tree was ahead of local commit `2f819bb`. This records the publication snapshot, not a claim that every listed integration is already in that commit. Published source hashes and exact retained ranges are in [publication-2026-10-02.json](publication-2026-10-02.json).

The compiler is `shc-v5.0r32-sh2`, on the Saturn platform for big-endian SH-2 support. TGM2+ itself is an arcade game. Each successful upload was compiled again through the public API after saving; the complete paginated account listing was read back to verify all 135 IDs, saved scores, and disabled match overrides.

## Reading the scores

Site scores are not linked-ROM coverage. Scratch ranges overlap, legacy targets remain available, and compiler-context functions are not necessarily matched. Do not sum scratch sizes or infer whole-project coverage from the number of green badges.

Complete C groups are byte-identical in the local linked ROM. Some public scores still differ because of object-level details: for example, the old sprite target contains literal addresses where the current source uses relocatable global symbols. Existing targets are immutable through the normal scratch update API, so their URLs and honest scores are retained rather than forcing a match badge.

Verified-prefix/range sources retain their full compiler context. Only the listed ranges are linked and counted. Their public scratch targets the first retained range, while the compiler emits the entire context; extra context and boundary differences can dominate the site score, even producing 0%. These scratches are explicitly labelled `verified ranges; context WIP`, not claimed as completely matched units. Ranges below are end-exclusive.

## Fully linked source groups

These 98 groups compile publicly. A 100% site score is additional evidence, not a substitute for the whole-ROM check.

| Source | Scratch | Public match |
|---|---|---:|
| `src/piece_44d4.c` | [DMEqS](https://decomp.me/scratch/DMEqS) | 100.00% |
| `src/rom_header.c` | [J85sf](https://decomp.me/scratch/J85sf) | 100.00% |
| `src/rand.c` | [Mf4uj](https://decomp.me/scratch/Mf4uj) | 100.00% |
| `src/f_7f38.c` | [qQCwy](https://decomp.me/scratch/qQCwy) | 100.00% |
| `src/field_check.c` | [I9QnN](https://decomp.me/scratch/I9QnN) | 100.00% |
| `src/main_loop.c` | [x45Rt](https://decomp.me/scratch/x45Rt) | 100.00% |
| `src/text.c` | [1XU2g](https://decomp.me/scratch/1XU2g) | 100.00% |
| `src/stubs_e828.c` | [jcn2l](https://decomp.me/scratch/jcn2l) | 100.00% |
| `src/f_11fe8.c` | [zxCi7](https://decomp.me/scratch/zxCi7) | 100.00% |
| `src/f_18fec.c` | [Wb6Ok](https://decomp.me/scratch/Wb6Ok) | 100.00% |
| `src/effect_230a8.c` | [tBulW](https://decomp.me/scratch/tBulW) | 99.96% |
| `src/code_23828.c` | [Sf8y6](https://decomp.me/scratch/Sf8y6) | 100.00% |
| `src/frt.c` | [sWQw4](https://decomp.me/scratch/sWQw4) | 100.00% |
| `src/sprite_2e06c.c` | [fdDVO](https://decomp.me/scratch/fdDVO) | 99.91% |
| `src/eeprom.c` | [RfEOY](https://decomp.me/scratch/RfEOY) | 100.00% |
| `src/f_50e0.c` | [wrF0E](https://decomp.me/scratch/wrF0E) | 100.00% |
| `src/f_51f4.c` | [inSqz](https://decomp.me/scratch/inSqz) | 100.00% |
| `src/f_9c0c.c` | [WdHW9](https://decomp.me/scratch/WdHW9) | 100.00% |
| `src/f_d28c.c` | [gIhVO](https://decomp.me/scratch/gIhVO) | 100.00% |
| `src/f_e520.c` | [CB28B](https://decomp.me/scratch/CB28B) | 100.00% |
| `src/vm_e834.c` | [F7NIx](https://decomp.me/scratch/F7NIx) | 100.00% |
| `src/vm_e988.c` | [tIOa4](https://decomp.me/scratch/tIOa4) | 100.00% |
| `src/vm_ead0.c` | [Wk4Ru](https://decomp.me/scratch/Wk4Ru) | 100.00% |
| `src/vm_empty_eeb0.c` | [ntc1C](https://decomp.me/scratch/ntc1C) | 100.00% |
| `src/vm_f0e8.c` | [DytzL](https://decomp.me/scratch/DytzL) | 100.00% |
| `src/vm_f5cc.c` | [anqTD](https://decomp.me/scratch/anqTD) | 100.00% |
| `src/hud_f958.c` | [OiLvh](https://decomp.me/scratch/OiLvh) | 100.00% |
| `src/f_100a4.c` | [MG0u4](https://decomp.me/scratch/MG0u4) | 100.00% |
| `src/hud_101b8.c` | [E67ky](https://decomp.me/scratch/E67ky) | 100.00% |
| `src/hud_10b28.c` | [NIo1J](https://decomp.me/scratch/NIo1J) | 100.00% |
| `src/hud_11254.c` | [ze6dP](https://decomp.me/scratch/ze6dP) | 100.00% |
| `src/f_11518.c` | [mgQpM](https://decomp.me/scratch/mgQpM) | 100.00% |
| `src/f_1175c.c` | [UKnX1](https://decomp.me/scratch/UKnX1) | 100.00% |
| `src/f_1199c.c` | [TzgLh](https://decomp.me/scratch/TzgLh) | 99.92% |
| `src/f_11e98.c` | [WAelT](https://decomp.me/scratch/WAelT) | 100.00% |
| `src/f_120c0.c` | [OpQN7](https://decomp.me/scratch/OpQN7) | 100.00% |
| `src/rank_13484.c` | [OooSR](https://decomp.me/scratch/OooSR) | 100.00% |
| `src/rank_149b4.c` | [iMr6s](https://decomp.me/scratch/iMr6s) | 100.00% |
| `src/state_1715c.c` | [KJQn9](https://decomp.me/scratch/KJQn9) | 100.00% |
| `src/task.c` | [2880L](https://decomp.me/scratch/2880L) | 100.00% |
| `src/effect_176e0.c` | [31HBh](https://decomp.me/scratch/31HBh) | 100.00% |
| `src/effect_18064.c` | [tuDGx](https://decomp.me/scratch/tuDGx) | 100.00% |
| `src/draw_req_185f0.c` | [v0JJh](https://decomp.me/scratch/v0JJh) | 100.00% |
| `src/time_18708.c` | [XDBhc](https://decomp.me/scratch/XDBhc) | 100.00% |
| `src/field_clear.c` | [zqa1f](https://decomp.me/scratch/zqa1f) | 100.00% |
| `src/f_18be0.c` | [oYjHh](https://decomp.me/scratch/oYjHh) | 100.00% |
| `src/f_19014.c` | [aJpDN](https://decomp.me/scratch/aJpDN) | 100.00% |
| `src/f_192e0.c` | [Ns8ya](https://decomp.me/scratch/Ns8ya) | 100.00% |
| `src/item_19414.c` | [IhOp9](https://decomp.me/scratch/IhOp9) | 100.00% |
| `src/item_19d68.c` | [jbwY9](https://decomp.me/scratch/jbwY9) | 100.00% |
| `src/item_1a7c8.c` | [yaWBP](https://decomp.me/scratch/yaWBP) | 99.92% |
| `src/item_1ade8.c` | [3eBcQ](https://decomp.me/scratch/3eBcQ) | 99.92% |
| `src/item_1b3d4.c` | [sPRkB](https://decomp.me/scratch/sPRkB) | 99.85% |
| `src/item_1b6f8.c` | [Ru36l](https://decomp.me/scratch/Ru36l) | 99.85% |
| `src/item_1ba20.c` | [sIITf](https://decomp.me/scratch/sIITf) | 99.94% |
| `src/item_1c200.c` | [cHbZR](https://decomp.me/scratch/cHbZR) | 100.00% |
| `src/item_1c708.c` | [apU8O](https://decomp.me/scratch/apU8O) | 99.95% |
| `src/item_1d1bc.c` | [qkgnA](https://decomp.me/scratch/qkgnA) | 100.00% |
| `src/item_1d640.c` | [Za62X](https://decomp.me/scratch/Za62X) | 99.94% |
| `src/item_1dc78.c` | [bOn1y](https://decomp.me/scratch/bOn1y) | 100.00% |
| `src/item_1e480.c` | [PNPDm](https://decomp.me/scratch/PNPDm) | 100.00% |
| `src/item_1e5b4.c` | [VbR4q](https://decomp.me/scratch/VbR4q) | 100.00% |
| `src/f_1eb1c.c` | [VHe5T](https://decomp.me/scratch/VHe5T) | 100.00% |
| `src/f_1ed78.c` | [SHnIJ](https://decomp.me/scratch/SHnIJ) | 100.00% |
| `src/f_1ef7c.c` | [FEZru](https://decomp.me/scratch/FEZru) | 100.00% |
| `src/f_1f5b4.c` | [yA29s](https://decomp.me/scratch/yA29s) | 99.96% |
| `src/f_20018.c` | [MuuMO](https://decomp.me/scratch/MuuMO) | 100.00% |
| `src/f_20134.c` | [k85k3](https://decomp.me/scratch/k85k3) | 100.00% |
| `src/effect_20250.c` | [4Jy4L](https://decomp.me/scratch/4Jy4L) | 100.00% |
| `src/effect_205f0.c` | [vZnfk](https://decomp.me/scratch/vZnfk) | 100.00% |
| `src/effect_206e0.c` | [syTu5](https://decomp.me/scratch/syTu5) | 100.00% |
| `src/effect_207d0.c` | [vxdvu](https://decomp.me/scratch/vxdvu) | 100.00% |
| `src/effect_20b24.c` | [MbPKo](https://decomp.me/scratch/MbPKo) | 100.00% |
| `src/effect_20c14.c` | [Zouuc](https://decomp.me/scratch/Zouuc) | 100.00% |
| `src/effect_20d60.c` | [09Fts](https://decomp.me/scratch/09Fts) | 100.00% |
| `src/effect_20f78.c` | [IZc5S](https://decomp.me/scratch/IZc5S) | 100.00% |
| `src/effect_21068.c` | [4HmHb](https://decomp.me/scratch/4HmHb) | 100.00% |
| `src/medal_strip.c` | [MtCE1](https://decomp.me/scratch/MtCE1) | 100.00% |
| `src/medal.c` | [pZaMc](https://decomp.me/scratch/pZaMc) | 100.00% |
| `src/f_21bc8.c` | [7p5rf](https://decomp.me/scratch/7p5rf) | 100.00% |
| `src/task_2261c.c` | [GdVpn](https://decomp.me/scratch/GdVpn) | 100.00% |
| `src/effect_23198.c` | [VJnUN](https://decomp.me/scratch/VJnUN) | 100.00% |
| `src/effect_23288.c` | [zuo8a](https://decomp.me/scratch/zuo8a) | 100.00% |
| `src/effect_23378.c` | [vlUlB](https://decomp.me/scratch/vlUlB) | 100.00% |
| `src/effect_23468.c` | [m76D6](https://decomp.me/scratch/m76D6) | 100.00% |
| `src/effect_23558.c` | [Dv85e](https://decomp.me/scratch/Dv85e) | 100.00% |
| `src/effect_23648.c` | [baUnU](https://decomp.me/scratch/baUnU) | 100.00% |
| `src/effect_23738.c` | [hE9do](https://decomp.me/scratch/hE9do) | 100.00% |
| `src/objpool_2528c.c` | [XVYjW](https://decomp.me/scratch/XVYjW) | 100.00% |
| `src/board_2635c.c` | [REJIb](https://decomp.me/scratch/REJIb) | 100.00% |
| `src/f_2afc0.c` | [I29Ld](https://decomp.me/scratch/I29Ld) | 100.00% |
| `src/frame_2b4e8.c` | [Ev9lQ](https://decomp.me/scratch/Ev9lQ) | 100.00% |
| `src/gfx_2bf58.c` | [QRMr0](https://decomp.me/scratch/QRMr0) | 100.00% |
| `src/input_2d2bc.c` | [auHaI](https://decomp.me/scratch/auHaI) | 100.00% |
| `src/sprite_2dbcc.c` | [6VfhE](https://decomp.me/scratch/6VfhE) | 100.00% |
| `src/sound.c` | [ZzK6o](https://decomp.me/scratch/ZzK6o) | 100.00% |
| `src/runtime_strncmp.c` | [Sxsgn](https://decomp.me/scratch/Sxsgn) | 100.00% |
| `src/runtime_memset.c` | [GxGqi](https://decomp.me/scratch/GxGqi) | 100.00% |

## Verified ranges with unmatched compiler context

These 27 groups compile publicly. Use the byte-verified ranges below, not their context-dependent site percentages, to understand the linked contribution. Several groups contain multiple retained ranges.

| Source | Scratch | Verified linked ranges |
|---|---|---|
| `src/f_2f68.c` | [yVk0R](https://decomp.me/scratch/yVk0R) | `0x2f68-0x3c88` |
| `src/startup_tail.c` | [c5vJz](https://decomp.me/scratch/c5vJz) | `0x1c8c-0x20be` |
| `src/f_541c.c` | [ltBRx](https://decomp.me/scratch/ltBRx) | `0x541c-0x57d4` |
| `src/field_7004.c` | [8Cema](https://decomp.me/scratch/8Cema) | `0x7004-0x7324` |
| `src/item_8164.c` | [ThylR](https://decomp.me/scratch/ThylR) | `0x8164-0x81cc` |
| `src/field_81cc.c` | [YoqKb](https://decomp.me/scratch/YoqKb) | `0x8314-0x8518` |
| `src/field_helpers_881a.c` | [0Zkp3](https://decomp.me/scratch/0Zkp3) | `0x881a-0x88fe`, `0x95b8-0x96ac` |
| `src/field_flag_counter_907c.c` | [4TEfY](https://decomp.me/scratch/4TEfY) | `0x907c-0x9098` |
| `src/menu_lookup_be04.c` | [ieqt9](https://decomp.me/scratch/ieqt9) | `0xbe04-0xbe28` |
| `src/vm_empty_f292.c` | [cRa5V](https://decomp.me/scratch/cRa5V) | `0xf292-0xf29a`, `0xf4c6-0xf5cc` |
| `src/hud_ranges_f958.c` | [n4OB3](https://decomp.me/scratch/n4OB3) | `0x103d0-0x1050e`, `0x1065e-0x10788`, `0x10942-0x10b28`, `0x10d94-0x11254` |
| `src/save_checksum_131e8.c` | [3HHFZ](https://decomp.me/scratch/3HHFZ) | `0x131e8-0x1326c`, `0x13378-0x13380` |
| `src/rank_tail_149b4.c` | [cTOxr](https://decomp.me/scratch/cTOxr) | `0x14bce-0x14d50` |
| `src/draw_15f10.c` | [svsy0](https://decomp.me/scratch/svsy0) | `0x15f10-0x15fae` |
| `src/playfield_1913c.c` | [PXb45](https://decomp.me/scratch/PXb45) | `0x1913c-0x191a4` |
| `src/effect_209a4.c` | [ieMn2](https://decomp.me/scratch/ieMn2) | `0x209a4-0x20a9c` |
| `src/savestats_211a2.c` | [e2LLF](https://decomp.me/scratch/e2LLF) | `0x211a2-0x21206` |
| `src/medal_tail.c` | [01lfH](https://decomp.me/scratch/01lfH) | `0x21862-0x2190c` |
| `src/task_23f08.c` | [YWDZo](https://decomp.me/scratch/YWDZo) | `0x23f08-0x2452e`, `0x245da-0x248a8` |
| `src/objfile_248a8.c` | [EI8yn](https://decomp.me/scratch/EI8yn) | `0x248a8-0x251cc`, `0x2521e-0x2528c` |
| `src/board_255dc.c` | [Mh67L](https://decomp.me/scratch/Mh67L) | `0x255dc-0x259ea`, `0x25f6e-0x26264`, `0x2631a-0x2635c` |
| `src/board_tile_callback_27310.c` | [0lCPz](https://decomp.me/scratch/0lCPz) | `0x27310-0x2733a` |
| `src/board_setters_2774a.c` | [Ch18s](https://decomp.me/scratch/Ch18s) | `0x2774a-0x277d0` |
| `src/palette_29bf8.c` | [YptMG](https://decomp.me/scratch/YptMG) | `0x29bf8-0x29f94`, `0x2a08c-0x2a0d4` |
| `src/gfx_2ab56.c` | [5WlOi](https://decomp.me/scratch/5WlOi) | `0x2ab56-0x2ac4c` |
| `src/gfxblend_2af5e.c` | [vfrIU](https://decomp.me/scratch/vfrIU) | `0x2af5e-0x2afc0` |
| `src/checksum_30c2e.c` | [l7rRH](https://decomp.me/scratch/l7rRH) | `0x30c2e-0x30d40` |

## Public compiler-path blockers

Five groups use small `#pragma inline_asm` helpers. The local wrapper compiles these with `-code=asmcode` and then `asmsh`; the public wrapper currently rejects them with `2808 (E) Illegal option for #pragma inline_asm`. The first failed upload is retained with a prominent blocked label. The other four were not uploaded. None is claimed as publicly compile-verified.

| Source | Publication |
|---|---|
| `src/vm_ef04.c` | [YLt0d](https://decomp.me/scratch/YLt0d) — compiler-path blocked |
| `src/vm_f6f8.c` | Not uploaded: same compiler-path blocker |
| `src/wave_27e70.c` | Not uploaded: same compiler-path blocker |
| `src/board_28334.c` | Not uploaded: same compiler-path blocker |
| `src/f_2bc64.c` | Not uploaded: same compiler-path blocker |

## Preserved legacy targets

These nine original scratches are smaller or older targets than the current source groups. Their descriptions now point to the current group where applicable. Only `v23048b` received a new source here, improving from 83.86% to 99.66%; pasting complete groups into the other short targets made the site diff worse, so their original source was preserved. The other fourteen original URLs appear in the source-group tables above.

| Target | Scratch | Public match | Current status |
|---|---|---:|---|
| `v23048b (legacy subset)` | [WiQI3](https://decomp.me/scratch/WiQI3) | 99.66% | Locally linked; [current group](https://decomp.me/scratch/GdVpn) |
| `setters_2e1b8 (legacy subset)` | [NwT4b](https://decomp.me/scratch/NwT4b) | 93.50% | Locally linked; [current group](https://decomp.me/scratch/ZzK6o) |
| `p_60e8b (WIP)` | [T0pNx](https://decomp.me/scratch/T0pNx) | 86.06% | Still WIP / unlinked |
| `o_2f304 (legacy subset)` | [EXTom](https://decomp.me/scratch/EXTom) | 98.13% | Locally linked; [current group](https://decomp.me/scratch/ZzK6o) |
| `o26k (WIP)` | [XLikp](https://decomp.me/scratch/XLikp) | 33.66% | Still WIP / unlinked |
| `draw_2090c (WIP)` | [9WBUS](https://decomp.me/scratch/9WBUS) | 93.58% | Still WIP / unlinked |
| `c_2f2cc (legacy subset)` | [yAZ4w](https://decomp.me/scratch/yAZ4w) | 0.00% | Locally linked; [current group](https://decomp.me/scratch/ZzK6o) |
| `stubs_23f08` | [MPWP4](https://decomp.me/scratch/MPWP4) | 100.00% | Locally linked; [current group](https://decomp.me/scratch/YWDZo) |
| `field_clear` | [Pw19Z](https://decomp.me/scratch/Pw19Z) | 100.00% | Locally linked; [current group](https://decomp.me/scratch/zqa1f) |

## ROM verification

```text
2b.u21:         783e8413b11f1fa08d331b09ef4ed63f62b87ead OK
1b.u22:         3f7e81756470c173cc17a7e7dee91437571fd0c3 OK
build/tgm2p.bin: c84cf538962ddbb377290301d76d75d8b542e5a2 OK
linked code:    121792 / 200698 (60.68421%)
```
