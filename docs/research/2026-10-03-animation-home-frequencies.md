# Animation stack-home frequencies — continuation141

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**.
No matching C was integrated. This continuation explains the negative-register
home ordering and recovers three R9 stores at home0 together with frame108.
The main-loop registers, other homes, function root and exact bytes still differ.

Case: `tools/shc_probe/cases/session_animation_homes141/`.

## Observed compiler mechanism

Read-only hardware breakpoints observe SHCMDL at 0x406e80 (memory-group
append), 0x408400 (frequency record creation), 0x4221f0 (before grouping and
sorting), and 0x422410 (after grouping, sorting and renumbering). Every traced
REG and ILB file equals the untraced native capture. The compiler is unchanged.
The copied compiler listings are inferred decompiler output, not original source.

Ordinary spilled WEBs already have negative IDs, with compatible lifetimes
potentially sharing an ID. Memory/address groups receive fresh negative IDs
after those ordinary groups. Frequency grouping at 0x4221f0 only combines
records that already share an old ID; it does not merge an addressed short
with an unrelated ordinary spilled value. Group frequencies are summed,
records are sorted by descending frequency with stable ties, and negative IDs
are reassigned. The counter advances for chained records too, explaining gaps
in the final negative IDs. Serialized unique IDs determine the compact home
order; a raw negative number alone is not always a byte offset.

The model reconstructs grouping links, summed frequencies, stable ordering,
renumbered IDs and final serialized REG assignments for all **194 records in
seven observed graphs**. Heap addresses are used only within their own trace;
source names come from live symbol IDs. Functions are selected by their debug
names because an inline helper adds a debug function record.

Selected controls show why the earlier helper put the sprite first:

| Source | Sprite frequency/home | sel frequency/home | tbl frequency/home |
| --- | --- | --- | --- |
| 140 common_reset | sprite_id 6 / 36 | 12 / 0 | 11 / 4 |
| 140 shared_home_loop_reset | sprite_id 8 / 12 | 12 / 0 | 11 / 4 |
| 139 shared_scope_legacy | sprite_id 6 / 32 | 12 / 0 | 11 / 4 |
| 135 inline_ref_all | spr2 8 / 0 | 7 / 4 | 6 / 8 |

The helper control reduces the estimated frequencies of other locals. Merely
changing the sprite's declaration position cannot account for this ordering.

## Consumed source experiment

The new sources retain direct scoped-pointer setup operations from139/140 and
use the inline `sprite_lookup(short *id)` helper only for the two visibility
operations in the later loop. The helper returns `&g_606006c[*id]`; its result
is consumed by the original w2c bit update. No dummy uses, forced registers,
padding, compiler modifications or comparison changes are present.

| Source | Function bytes | Frame | Sprite home | Actual R9 stores |
| --- | ---: | ---: | ---: | --- |
| original_brightness | 1752 | 108 | 0 | three, all home0 |
| common_brightness | 1752 | 108 | 4 | none |
| chain_common_brightness | 1748 | 108 | 0 | none |

The original-brightness form places sprite_id, sel and tbl at0,4,8. Its three
reachable stores are A758, A76C and A7A0. The second sprite initialization call
at A754 has `exts.w r9,r4` in its delay slot. The count WEB uses R10 and the
setup sprite value uses R9. Here the latter is a positive global allocation;
this is distinct from139's block-local cache mechanism.

However, the main loop still assigns hold=R10, constant2=R9, total=R8 and
mask15=R4. The original requires hold=R10, mask15=R9, constant2=R8 and total
on the stack. spr1 is at36 rather than16. All candidates start at A604 rather
than A608 because the earlier f_A226 context is still four bytes short.
Equal function length in the chain form therefore does not establish a match.
All three fail the unchanged strict contextual verifier.

Combining the140 common brightness reset with this helper does not preserve
the recovered roles. It also makes k and c0 share an ordinary home with summed
frequency9, ahead of sprite_id's8. The chain form raises sprite_id to9 and
wins that stable tie, but sel/tbl move to8/12. This is concrete evidence that
the previously useful changes cannot simply be combined independently.

## Validation and next step

Three native compiles equal their separately captured native object sections.
Six normal/debug middle-stage replays preserve REG/ILB. All seven hardware
home traces preserve those intermediates, and the home model predicts every
observed record and serialized assignment. Three control-flow stack audits
have no unresolved accesses; all return paths balance. All36 real functions
remain exported, no extra helper calls survive, and the inline helper only
adds the expected intermediate debug record.

Both Nuada checkouts retain all44 checkpoint122 production pins, the original
ROM hashes and26 prior frozen evidence cases. The full build and243-test
suite remain checkpoint122's historical result; they were not rerun for these
private negative probes. No production source or denominator changed.

The MacBook ROM transfer was freshly reverified on the source, local copy and
Nuada: all23 files, 61,822,311 bytes, exact SHA-256 agreement. Its useful shared
TGM1 shape data and duplicate TGM2/Plus payloads are documented separately in
`2026-10-03-trilogy-rom-reference.md`; these files add no C credit.

Keep original_brightness as the new setup/home control and137/140 as loop-role
controls. Next inspect the loop graph and frequency contributions around the
two visibility helper returns before choosing a further consumed source change.
The remaining issue is simultaneous recovery of the loop roles and home order,
as well as the earlier context root. The literal100% goal remains active.
