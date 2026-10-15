# Speed screen coordinate homes — continuation176

No additional matching C bytes. Production remains checkpoint161, 128,398 /
200,698 bytes (63.9757247207%). All 72,300 remaining bytes, including 1,638
runtime assembly bytes, remain in the denominator. The goal remains active.

The preceding turn made progress by ruling out a scheduler fix for the score
comparison registers. This turn examines the following genuine function7956
instead. Its nominal retained range is7956–7C08, in context7658–7C08. Earlier
score bytes and shared pool797A–799C remain protected; no approximate score or
pool shift can establish a retained range.

## Caller and callee authentication

Independently resolved the current original objects for effect_176e0, f_11518,
and f_2bc64 and compared each entire text section with the original ROM.
All three match. The caller's7A54–7A74 sequence evaluates RNG(10), RNG(10),
then RNG(13), preserving the first two returns on its stack. It calls17834 with
R4=player, R5=last return+5, R6=second return, R7=first return. Pool7ABC identifies
2BECA and pool7AC0 identifies17834 under the normal code address delta.

The effect entry stores the low words of incoming R5/R6; the body uses those
as signed coordinates. It does not consume incoming R7. This does not authorize
removing an RNG call: each changes the seed. Nor does the callee body uniquely
prove the caller's original C prototype. The historical short-coordinate and
unsigned-RNG declaration trials were inspected, not repeated. A new old-style
17834 declaration produces exactly the baseline object bytes and supplies no
solution. It is a target-ABI diagnostic, not a proposed portable interface.

The drawing table pointer declaration also leaves all context bytes unchanged.
The callees authenticate the widths used by the final drawing call.

## Coordinate expression controls

The baseline is the prior select_000 candidate, with the defined multiplication
`(v-63)*1024` replacing the original negative-left-shift expression. The entire
context is byte-identical to that prior candidate. Its final drawing call
computes both coordinates in argument expressions.

The ROM computes a quotient, copies it from R0 to R4, then uses R0 for the first
multiply. R4's copy is not consumed. We did not add a dead C operation. Genuine
staged calculations naturally reproduce the copy instruction, but consume R4
and spill their offsets. They are still negative.

| Native control | Differing target bytes | Context bytes |
| --- | ---: | ---: |
| baseline | 125 | 1456 |
| staged_xy | 180 | 1460 |
| staged_yx | 199 | 1464 |
| reuse_kt | 180 | 1460 |
| reuse_k_y | 180 | 1460 |
| long_offsets | 180 | 1460 |
| oldstyle_effect | 125 | 1456 |
| pointer_table | 125 | 1456 |
| staged_register | 615 | 1464 |
| inline_quotient | 169 | 1456 |
| staged_inline_quotient | 180 | 1460 |
| inline_quotient_register_offsets | 613 | 1468 |
| short_offsets | 173 | 1464 |
| short_x_offset | 175 | 1464 |
| short_y_offset | 177 | 1464 |

The two register controls use ordinary C storage classes, never named physical
registers. Both regress sharply. The reused and long-local contexts equal
staged_xy byte for byte. Staged_inline_quotient shares its raw score but differs
at16 context bytes, so those sources are not a compiler-equivalence class.
Inlining two genuine quotient uses causes common-expression storage on the
stack. No variant passes the default range verifier.

All 65,536 signed16 values of v and both k values32/48 were checked, for131,072
pairs. Numerator and coordinate products fit signed32. Narrowing either offset
before the final short argument preserves the low16-bit sum for every possible
signed16 x, by modular congruence; no value-range invariant was invented. SHC's
observed low-word behavior is the relevant target semantics. Narrowing still
fails exact matching.

## Output-preserving compiler observations

Captured two native compiler runs and verified every ELF section except the
nonsemantic comment section against the ordinary compile objects. Replayed
SHCMDL with hardware execution breakpoints only. Both REG/ILB streams equal a
fresh untraced control and the native capture. Compiler code, compiler data,
flags, and configuration were unchanged. The REG codec round-trips every byte.
Symbol records authenticate names against compiler symbol numbers.

| Variable | Baseline home | Staged XY home |
| --- | --- | --- |
| mode | R10 | R9 |
| dy | R11 | R11 |
| k | R9 | R8 |
| v | R8 | R10 |
| quotient t | R13 | R4 |
| offset ox | absent | stack home -2 |
| offset oy | absent | stack home -1 |

The baseline has one append-memory observation and no spilled variable entries.
The staged case has five events: append-memory, two frequency contributions,
and before/after frequency-sort snapshots. Each offset has frequency1: its
assignment root contributes1, while the call's use refers back to that root and
contributes0. The sort maps ox from provisional -1 to final -2 and oy from -2
to -1. Native loads/stores confirm the two slots. This trace establishes home
selection, not the complete graph-coloring reason for spilling.

## Validation and next action

Case session_speed_coordinates176 preserves all sources, native objects,
resolved binaries, controls, traces, original disassemblies, prior input
snapshots and independent verifier. It independently resolves all15 objects,
checks linked bytes, records actual roots and protected pools, and authenticates
three whole callees. Fresh production verification checks46 pins,62 prior case
freezes, original ROM hashes, and unchanged progress in both Nuada checkouts.
Full build/test gates remain checkpoint161's independent clean builds and245
enabled tests; no full rebuild is claimed for this research-only turn.

Next inspect the allocation priorities and interference edges for mode, k, v,
t, ox and oy before memory-home sorting. The bounded home trace explains the
stack result but not why adding two one-use expressions changes the earlier
mode/k ordering. Compare baseline and staged source before another rewrite.
Do not repeat the15 controls or remove the unused-argument RNG event.
