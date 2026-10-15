# Initializer branch order and pointer lifetimes — continuation 210

Production remains 131,780 / 200,698 verified C bytes (65.66084%). The best
1,504-byte initializer improved from nine differences to three, at its actual
ROM address 0x2A576. Neither candidate passes the strict prefix verifier.

The two best sources are `pointer_0_1_8_10_invert6.c` and
`pointer_0_7_8_11_invert6.c` in `tools/shc_probe/cases/session_gfx_branch210`.
Their initializer bodies are identical. All 27 color and mask register homes
match the target. The remaining instructions are:

| ROM address | Target | Candidate |
| --- | --- | --- |
| 0x2A956 | 8F05 (BF/S) | 8D05 (BT/S) |
| 0x2A962 | 2241 | 2251 |
| 0x2A96A | 2251 | 2241 |

These are the third expanded iteration's red-channel condition and its two
stores. The suffix 0x2AB56–0x2AC4C remains exact. The real preceding C context
still produces 1,186 bytes with 637 differences and remains excluded.

## Native evidence

Read-only hardware breakpoints at SHCMDL 0x414D07, 0x415920, 0x414D0C and
0x414D48 record construction and the surrounding control-flow graphs. Every
traced output is compared with a fresh untraced run and the native capture.
Across nine flow traces, all 108 color conditions retain their definition
blocks, branch use blocks, and emitted polarity. Positive conditions create
the from-value candidate first; negative conditions create the to-value
candidate first. This explains why changing the condition repairs register
ordering but reverses the emitted branch.

Reassigning `o = &PFX(n)` before selected colors changes real pointer lifetime
segments. Coalescing leaves inactive candidate entries in the later unstable
sort. The initializer's active graph stays intact, but these entries change
tie ordering. Four pointer assignments, with only color 6 inverted, recover
all 27 homes; two independent native sources validate that prediction.

Moving a pointer assignment after the two color loads or before the delta
store moves its inactive entries and redistributes lifetime priority. The
initial fixed-offset prediction was wrong for colors 1 and 8. Independent
traces established that the offset follows candidate creation blocks, not a
fixed count. The corrected model reproduces the complete graphs and final
allocations for four native timing controls.

Splitting `PFX(n).max = m = max` into `PFX(n).max = max; m = max;` adds one
inactive candidate and changes early variable ordering without changing the
1,504-byte body size. Its native graph is independently modeled. Removing a
standard C register hint from a scoped color value changes priority from
65,541 to 6, a difference of 65,535. The corresponding native graph and final
allocation validate the hint model; assuming a 65,536 bonus failed validation
and was corrected before searching.

## Closed searches

All these searches are offline predictions, never compiler input mutations:

- 2,048 pointer-position subsets: best two wrong homes, no exact result.
- 24,576 subsets with one inverted condition: 115 exact-home predictions;
  the two native confirmations above have three differing bytes.
- 22,528 single timing changes: best two wrong homes, no exact result.
- 6,464 paired timing changes restricted to the 115 best position subsets:
  best two wrong homes, no exact result. This is not an exhaustive search of
  all multiple-timing combinations.
- 2,048 position subsets with the split max assignment: best two wrong homes.
- 12,288 position/hint combinations for colors 6 and 7: best three wrong homes.

The native source controls also cover direct field access, arithmetic and
comparison forms, pointer timing, max types/assignment forms, branch-local
pointer reassignment, and scoped hints. Branch-local reassignment changes
body size and fails the function-boundary gate. None is a production match.
One rejected variant, `max_split_first`, also moves eight predecessor PC-relative
literal references (seven MOV.W and one MOV.L), raising its predecessor raw
difference count from 637 to 645. The verifier records the exact eight changed
instructions; every other control preserves the predecessor byte-for-byte.

## Validation and next action

`analyze.py` runs the allocation/origin models, the 108-condition audit, and
all recorded searches with checked return codes. `verify.py` independently
resolves every contextual object and checks source hashes, body roots, exact
differences, preserved predecessor bytes, fresh trace controls and separately
assembled captures. See `VALIDATION.json` for the actual checked run and
`RESULTS.json` for counts; these files, not this prose, determine gate status.

The next useful experiment must change candidate creation or a real value's
lifetime while retaining positive branch polarity. Start from the two
three-byte sources and the positive `pointer_0_1_8_10` control. Investigate
whether a naturally named signed delta or a different real value lifetime
changes the color-6 pair without adding instructions. Preserve unsigned
color-load semantics and signed subtraction; assigning the subtraction back
to an unsigned short would change negative deltas. Do not repeat the closed
position, timing, branch-local pointer, or scoped hint searches unchanged.

No compiler, IR, physical register, object, ROM, denominator, or validation
rule was altered. Literal 100% remains the goal, including runtime assembly.
