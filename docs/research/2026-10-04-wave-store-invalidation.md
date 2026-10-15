# Wave address sharing across stores — continuation196

The remaining wave target is 0x2808a–0x28334 (682 bytes). Production remains
checkpoint192: 131,780 / 200,698 matching bytes (65.6608436556%), with 68,918
remaining, including all 1,638 runtime bytes. This investigation adds no C bytes.
The preceding turn completed the requested ROM verification; it did not improve
matching coverage. This turn supplies new causal compiler evidence.

## Observed compiler rule

Read-only hardware execution breakpoints locate the divergence in SHCMDL's
second 413EA0 traversal, between 40F8C0 and 40F8E0. Hardware write watchpoints
independently record changes to the two selected array nodes' common/shared
fields. Compiler executable, IR, and emitted instructions are never patched.

4199D0 handles the selected array expression. It scans the list at 459118 and
compares opcode, type with low two bits masked, child tag, and fields +32/+36.
For these array nodes the latter fields are 896/976 (the 224-element array's byte
size and the x field's 0x3d0 offset). The lookup is not the separate 419DD0
constant hash lookup. Matching nodes link through 419D40; otherwise 419DA0 makes
a new representative and 41A1A0 inserts it into the list.

In the baseline chained assignment `bd->x[j] = bd->x[m] = packed`, both array
lookups happen before either store clears the list. Both board dereferences
receive tag 23, so the second array matches the first. Its common pointer links
to the first, whose count becomes two and shared pointer becomes the second.

In `bd->x[m] = row = packed; bd->x[j] = row`, the first array store reaches
419CDD, calling 4203D0 (return 419CE2). This frees every list entry and resets
459118 to zero. The next board dereference sees an empty list and gets tag 36;
the second array therefore becomes a separate representative. Both counts stay
one and both shared pointers stay zero. The row local and single packed OR remain;
the additional eight bytes rebuild an array address, as established in195.

Four modeled lookup decisions agree with independent field-write observations.
This establishes the observed store barrier; it does not establish that every
SHC store or every alias class follows the same path.

## Native controls

All six controls retain the four nominal roots and all 538 preceding bytes.
Differences are counted in the fixed 682-byte target window; excess length is
reported separately. Every control fails the unchanged strict verifier, and no
retained `.kept.o` exists.

| Source | Differing bytes | Excess bytes |
|---|---:|---:|
| Baseline | 10 | 0 |
| Named row with split stores | 437 | 8 |
| Same split stores, const board pointer | 437 | 8 |
| Mirror load, const board pointer | 442 | 12 |
| Initialize j before phase load | 17 | 0 |
| Initialize m before phase load | 55 | 0 |

The two const-pointer controls are byte-identical to their corresponding
non-const controls (the mirror-load comparison uses frozen195). The two setup
reorders are ordinary independent initialization reorderings and worsen the
baseline. They do not justify another broad permutation sweep.

## Verification and next action

Case: `tools/shc_probe/cases/session_wave_alias196/`.
Six fresh output-preserving replays record 3,531 events: two pass snapshots,
two field-write traces, and two list lookup/reset traces. For every replay,
`.reg` and `.ilb` equal both a fresh untraced control and the native capture.
Two captured assembly files were independently assembled and resolved, agreeing
with their strict native compile bytes. `verify.py` recomputes source hashes,
relocations, roots, differences, protected context and replay equality.
An initial watch-tracer build failed on misleading indentation under -Werror;
formatting was fixed before the successful controlled runs.

Close the pointer-const and split-store CSE avenues. Keep the genuine chained
store while addressing the setup scheduling swap separately: inspect SHCGEN's
ordering of the final attribute OR and the j stack spill, then use a reduced
source experiment to test a predicted schedule. The allocation tie in the tail
is already independently modeled in193–195 and remains unresolved in an otherwise
matching source. No production changes or repeat full-suite run were needed;
checkpoint192's two clean builds and 253 enabled tests remain the last gates.
