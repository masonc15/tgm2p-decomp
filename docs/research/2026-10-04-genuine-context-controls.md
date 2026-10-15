# Genuine predecessor controls — continuation184

The board-clear match established that SHCGEN's recent-register mask can survive
function boundaries. This continuation tests that mechanism against two other
real source contexts. It produces **zero additional matching C bytes**.

## Score 7658

Six native controls preserve the current matched speed body at 7956. The baseline
score source is from `src/speed_7956.c`; the alternative uses the earlier positive
label form from `session_score_temporary170/guard_label_positive.c`. Genuine field
functions from `src/field_7004.c` precede them. The two structure views preserve
their original offsets; explicit 32-bit pointer casts reconcile drawing prototypes.
No synthetic preceding function, register binding, padding, or compiler-state
modification is introduced.

| Context | Actual score root | Score differences at emitted root | Actual speed root |
| --- | --- | --- | --- |
| standalone baseline | 7658 | 19 | 7956 |
| standalone labeled | 7658 | 13 | 7956 |
| previous 7324 baseline | 7648 | 19 | 7946 |
| previous 7324 labeled | 7648 | 13 | 7946 |
| full 7004 baseline | 765C | 19 | 795A |
| full 7004 labeled | 765C | 13 | 795A |

Every emitted score body is byte-identical to its corresponding standalone body,
not merely equal in mismatch count. All 690 physical speed bytes remain exact,
but shifted copies earn no credit. The existing separately retained production
speed source is unchanged. All six score controls fail the strict verifier.

Three fresh hardware-breakpoint SHCGEN replays preserve SUA/ASA relative to fresh
untraced controls and preserve native SUA. Native and captured object sections
agree apart from the compiler's source-date comment. Cleanup-snapshot ASA differs
because output positions advance; native ASA equality is not claimed.

There are 55 events and 15 independently predicted scratch-register selections.
The score function enters with recent mask 0 standalone versus 6 after either
field context. Nevertheless, all five sampled selector inputs, cache states,
dependency masks and selections are identical across the three cases. The entry
history has been overwritten before these requests. Changing this predecessor
context cannot explain the remaining score differences in these sources.

## Row renderer 2090C

Two native controls use the existing `session_effectrow_next/baseline.c`. The
second prepends the genuine 207D0 and 2085A functions from `src/effect_207d0.c`.
Those 316 preceding bytes match ROM exactly and place 2090C at its nominal root.
The complete following 536-byte context equals the standalone output byte for
byte. The nominal 152-byte target window still differs in 36 places; 209A4 starts
18 bytes early. Both controls reject. The 36-byte count includes bytes from the
next function and must not be described as a correct-length row-renderer body.

## Independent validation and continuation

`session_score_context184/verify.py` resolves original ELF relocations independently,
checks input-section alignment, linked roots, raw ROM differences, source/object
hashes, fresh replay outputs and every sampled selector prediction. It also
validates `session_effect_context184` and all 52 current production pins.
Both Nuada production reports and original ROM hashes remain unchanged.

Authoritative coverage remains checkpoint183: **130,980 / 200,698 bytes
(65.2622348005%)**, with **69,718 remaining**, including all 1,638 runtime assembly
bytes. Its 251-test full-suite result is historical; excluded diagnostic controls
do not justify rerunning the full suite. No source, split, proof or denominator
changes were made. The literal 100% goal remains active.

The trilogy transfer was rechecked at 11:31 UTC: all 23 files and all 52 payload
occurrences in both destination copies match their saved hashes. MacBook SSH timed
out, so source changes since the successful 09:57 UTC check remain unverified.
The 448-byte TGM1/Plus shape table and all 28 rotations were rechecked. This adds
no C credit and supplies no additional TGM2 revision.

Next inspect the board-scroll 27B50 allocation graph: the genuine historical
candidate keeps cache addresses in callee-saved registers while spilling the
ROM's base, attributes and counter. Trace SHCMDL's priorities and interference
before home selection, using the source in `session_boardscroll_receipt19`.
The previously tested context/type/loop/pointer/helper families should not be
repeated. Unlike this continuation's entry-state controls, that investigation
targets the established persistent-register and 20-byte-versus-8-byte-frame gap.
