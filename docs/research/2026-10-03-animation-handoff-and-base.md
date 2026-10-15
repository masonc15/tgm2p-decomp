# Animation handoff and sprite-base experiment — continuation144

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**, with 72,382 bytes remaining. None of these candidates matches the original, and no production source or coverage rule changed.

## Source controls

Continuation143 recovered the 108-byte frame, the original main-loop register roles, and the first four homes together. This experiment tests genuine count-to-hold assignments and direct access to the sprite array's resolved address. The linker defines `_g_606006c = 0x606006c`; the numeric-base variants preserve that address and do not introduce volatile accesses.

| Candidate | Bytes / frame | Observation |
| --- | --- | --- |
| direct_handoff | 1744 / 108 | `hold = n - 1; last = hold;` changes ten full-unit text bytes; mask15 and constant2 exchange registers. |
| reverse_handoff | 1744 / 108 | `last = hold = n - 1;` is identical to143 common_brightness in every allocated section. |
| absolute_cached | 1684 / 104 | Numeric sprite-base constant has priority16 and takes R9; the three-use source cache does not produce R9 home writes. |
| absolute_copies | 1736 / 108 | Numeric base has priority18 and takes R9; the three explicit short copies remain at home32, emitted from R0. |
| absolute_combined | 1744 / 108 | Numeric base has priority10 and remains unallocated; setup source and main-loop register constraints still differ. |

All five preserve six detected loops, have no inline-return gotos, emit36 exported functions without additional local call targets, and have no native R9 word stores to stack. Every candidate starts at A604 instead of A608 and is rejected by the existing contextual boundary check. Relative byte comparisons are diagnostic only.

The numeric-base substitution changes nine uses in each older control and seven in the combined source. Two visibility helpers outside f_a608 retain symbolic accesses. An initial preparation guard expected at least eight body uses everywhere and stopped before generating new sources; it was corrected to assert the exact9/9/7 counts. No aggregate wrapper was created: earlier evidence rules out inventing an unsupported aggregate solely to influence stack layout.

## Generator evidence

Four hardware traces observe the stock SHCGEN generator, with untraced controls from identical inputs. Traced SUA/ASA output equals the untraced replay, and SUA equals the native driver's captured intermediate. ASA differs from the driver's cleanup snapshot because its symbol positions have advanced; those differences remain recorded and receive no matching credit.

| Control | Emission events | Recipe events | R9 memory writes |
| --- | ---: | ---: | ---: |
| 137 signed_three_legacy_call | 568 | 226 | 0 |
| 139 shared_scope_legacy | 572 | 233 | 3 |
| 144 absolute_cached | 559 | 226 | 0 |
| 144 absolute_copies | 579 | 238 | 0 |

The three stores in139 map directly to the consumed `sprite_id = spr2;` assignments at source lines362/367/377. SHCGEN emits opcode39, size1 with R9 as source. The numeric-base copy control emits the same operation and identical destination operand from R0. The native CFG audit independently resolves the stores to home32; the generator's frame operand is not treated as a native stack offset.

This rules out a later peephole pass deleting automatically generated R9 stores in these controls. It also shows that replacing the symbol with its literal address introduces a competing base-address allocation rather than the desired cached-short writeback. It does not establish the original source form or rule out other compiler paths.

## Verification and next constraint

Five native compiles and captures, ten middle-stage replays, fifteen SHCMDL hardware replays, four SHCGEN hardware replays, and five CFG audits passed their evidence checks. Models reproduce442 allocation candidates,134 home/frequency records, and646 frequency contributions. Both remote checkouts retain all44 production pins,29 prior frozen cases, and original ROM hashes. The23 reference ROM files also match their recorded source. The last full build/test gates remain checkpoint122; these private probes do not constitute a new full production test run.

Keep143 common_brightness as the combined-constraint control,137 as the authentic three-use cache control, and139 as the explicit-copy control. The unresolved distinction is the original short's home0 and R9 cache with automatic materialization, while preserving the original main-loop lifetimes. Repeating handoff order or literal-base substitutions does not address it.
