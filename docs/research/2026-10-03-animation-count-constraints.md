# Animation count and setup constraints — continuation143

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**.
No matching C was integrated. Previous continuation142 made progress by
recovering homes0/4/8 without losing loop analysis. This continuation combines
that source with the common brightness reset and recovers the original frame,
main-loop register roles and homes0/4/8/12 simultaneously. Setup registers and
the remaining homes still differ.

Case: `tools/shc_probe/cases/session_animation_constraints143/`.

## Native count evidence and bounded controls

The original reads the header count into R10 at A62A, promotes it at A66E,
stores a long copy at home72, tests the short count at A6B6, and eventually
decrements R10 at A7EA before saving last at home60. These instructions were
read again from the pinned ROM and saved in ORIGINAL_COUNT_AND_SETUP.json.
The long store does not prove that the source count copy was declared long.

Starting from142 void_call_id, the first four variants test a long count copy,
removal of the count copy, a named promoted loop bound, and140's consumed
common brightness reset. The reset preserves the final local brightness state:
neither directional branch reads brightness before the common point. It remains
a diagnostic source form because its emitted reset control flow differs from
the original's two branch-local resets.

The next two variants reuse hold for the initial header count, with and without
the common reset. The last two express the counter handoff as a copy followed by
a decrement and last assignment, or as a decrement of the already reused hold.
Every introduced variable or assignment supplies an existing computation. No
dummy uses, forced registers, padding, compiler edits or weakened comparisons
are present. All65,536 signed-short count values have a promoted subtract-one
bound in [-32769,32766], and the loop exits at an index in [0,32766].

| Source | Function bytes | Frame | Main-loop result |
| --- | ---: | ---: | --- |
| count_long | 1752 | 104 | constant2=R9, total=R8; mask15 unallocated |
| direct_count | 1752 | 104 | mask15=R9, total=R8; constant2 unallocated |
| limit_local | 1756 | 104 | identical allocated sections to142 control |
| common_brightness | 1744 | 108 | original loop roles recovered |
| count_hold | 1756 | 104 | identical allocated sections to142 control |
| count_hold_common | 1744 | 108 | identical allocated sections to common_brightness |
| decrement_handoff | 1744 | 108 | hold=R11, constant2=R9, mask15=R8 |
| reuse_decrement | 1724 | 104 | hold=R11, total=R9, constant2=R8 |

Eight native compiles therefore yield six distinct native texts. The three
identity controls compare every allocated ELF section, including flags and
alignment; they are not counted as independent byte improvements.

## The best combined source and its limits

common_brightness retains all six detected loops and puts sprite_id, sel, tbl
and total at offsets0,4,8,12. It uses a108-byte frame with hold=R10, mask15=R9,
constant2=R8 and total on the stack, as in the original.

Its header count is nevertheless inR9 and its setup sprite value inR10, opposite
the original. It produces none of the original three R9 home stores. spr1 is at
44 instead of16; y is at16 instead of24; other homes also differ. The common
brightness reset emits a different control-flow shape. All candidates still
begin at A604 instead of A608 because the preceding f_A226 remains four bytes
short. All eight fail the unchanged strict contextual verifier.

The real compiler graph gives the setup sprite priority6/profit-5 and the count
priority4/profit-6. Their disjoint reuse with later loop registers explains why
matching the loop assignment alone does not fix the setup assignment.

## Why the handoff trials do not solve the register exchange

Changing the count to long still makes SHC reuse an anonymous long for the
zero-test, as observed in129. The short-count common source retains an explicit
promotion of named short n. The 32-bit native count store therefore remains
consistent with the short-copy source; it is not grounds to insist on long.

Renaming the initial count to hold does not join its live range to the running
hold counter. The final REG file contains two distinct entries with the same
hold symbol: the early entry shares cnt and occupies R9; the later entry occupies
R10. The allocated native sections are byte-identical to the separate-name
control. A named loop bound likewise changes no allocated native bytes.

The explicit decrement forms do change the compiler graph: hold priority rises
from21 to23 or25. Both allocate hold toR11 and lose the recovered assignment.
This distinguishes a real def/use change from a source-name change, but does
not produce a match. Keep the original assignment chain in the retained control.

## Validation and next evidence target

Eight ordinary compiles equal eight independent driver captures. Sixteen
normal/debug middle replays preserve REG/ILB. Twenty-four hardware replays
preserve the captured intermediates. The models reproduce all706 allocation
candidates,211 home records and1,034 frequency contributions across the eight
sources. All six loops remain recognized. Eight control-flow stack audits have
balanced returns and no unresolved accesses; no extra helper calls survive.

Both Nuada checkouts retain all44 checkpoint122 production pins, the original
ROM hashes, and28 prior frozen cases. Full clean build and243-test gates remain
checkpoint122's historical results; they were not rerun for private negative
probes. No production source, target, denominator or runtime rule changed.

Keep common_brightness as the combined frame/loop/home control and141's
original_brightness as the setup-store control. The next comparison should
focus on the actual value flow from the header count into hold and on why the
setup sprite is globally allocated instead of using the observed block cache.
Do not repeat the source-name reuse or named-bound forms. A direct subtraction
into hold followed by the last copy, and the corresponding assignment order,
remain distinct consumed expressions to inspect against the original in-place
decrement. Verify their real allocation before accepting any local similarity.
The literal100% goal remains active, with all1,638 runtime bytes still incomplete.
