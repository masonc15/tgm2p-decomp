# Announcement: copies do not recover the remaining registers

Continuation 221 preserves production at 135,502 / 200,698 bytes (67.51537%).
The full literal 100% goal remains active, with 65,196 bytes remaining, including
1,638 runtime bytes. Continuation 220 made research progress by identifying the
scheduler queue effect; this continuation tests the separate scratch/allocation
hypotheses rather than repeating its closed scalar/array controls.

## Source controls

Twenty-two ordinary native C controls all fail strict acceptance. They preserve
the two real input-table reads and the original call signatures. No compiler,
intermediate, object, ROM or acceptance proof was changed.

| Family | Differences | Context bytes |
| --- | ---: | ---: |
| Baseline | 24 | 816 |
| Dispatch argument copy | 49 | 816 |
| Following-call argument copy | 36 | 816 |
| Late byte index copy, either declaration order | 51 | 816 |
| Late unsigned-long index | 30 | 816 |
| Register-hinted byte index | 761 | 824 |
| Register-hinted rotation result | 127 | 816 |
| Split conditional rotation assignment | 118 | 804 |
| Wide rotation result | 24 | 816 |
| Expanded pointer assignment | 24 | 816 |
| Integer-address cursor update | 33 | 816 |
| Cursor update folded into second read | 121 | 824 |
| Explicit reused wide index | 30 | 816 |
| Wide index declared after table pointer | 26 | 816 |
| Separate narrow-result modulo | 111 | 824 |
| Separate base/cursor with wide index, four order/const controls | 38 each | 816 |
| Word-address table base | 24 | 816 |
| Word-address base plus expanded cursor update | 168 | 812 |

Counts include missing bytes when a context is too short. Shifted boundaries and
partial matching instructions earn no credit. Wide result, expanded pointer
assignment and word-address base are byte-identical to baseline. The split-byte
rotation expressions preserve the mathematical modulo result for every possible
unsigned-byte input and either increment; their compiler output still fails.

## Why the attempted fixes fail

Four read-only native SHCMDL graph replays reconstruct every priority, profit,
sort order and allocation. The baseline table-pointer candidate has priority 4,
profit -11; the extended index temporary has priority 3, profit -12. The pointer
wins logical register 1 (R4), and the index gets logical register 2 (R5), opposite
the ROM. Naming the wide index removes a conflict with the original byte ID but
leaves the table ahead: respective profits become -15 and -17. It also changes
the load schedule, so the additional source name is not a local register repair.

The byte copy adds a bound-group candidate and changes a constant allocation tie
elsewhere. The scoped dispatch argument copy adds another candidate, raises the
constant 10's profit from 0 to 1, and moves it ahead of the global-pointer alias.
That swaps the global/constant homes and damages code that previously matched.

Two read-only GEN scratch replays verify all 306 choices with the recovered
selector model. At the dispatch call, both baseline and scoped-copy forms have
R0 excluded, last scratch R1, no recent mask and all R0-R3 cache entries empty.
R3 and R2 therefore tie at rank 2; the descending scan picks R3. The argument
copy leaves that state unchanged. A source remedy for this call would need to
change the relevant scratch state, not merely introduce a parameter alias.

## Validation and continuation

The six native replays preserve fresh untraced output (630 total events).
Four independent native assembly captures resolve to their ordinary compiled
contexts. All 22 source/object/root/hash comparisons, four allocation models,
306 selector predictions and boundary rejections pass the independent verifier.
Three verification commands exit zero. The frozen evidence is in
`tools/shc_probe/cases/session_announcer_scratch221`.

Production source and proofs remain unchanged; the complete build/test gates
are still checkpoint 218's 263 tests per checkout, not a new suite run. Final
checkpoint synchronization independently rechecks both ROM images and all 62
production pins in both Nuada checkouts.

Park these exact announcement copy, hint, cursor, and result-width families.
Resume the announcement only with new evidence about entry-stream order or a
specific scratch-state change. The next selected target is the complete VM
handler EEBC-EF04 (72 bytes including its pool), currently an assembly unit.
The original instructions divide a masked high word using `_divls` at 060305C0,
then store the result through a slot derived from the low input byte. Its
literal-pool/context ownership must be authenticated before any credit. The
current inventory confirms 42 incomplete units totaling 65,196 bytes. Work
continues solo, without brainstorming.
