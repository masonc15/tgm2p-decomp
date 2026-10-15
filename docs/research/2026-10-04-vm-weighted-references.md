# VM weighted references and recovery of the original stack shape

Continuation 228 adds no C credit. Production remains 135,594 / 200,698 bytes
(67.561211%); 65,104 bytes remain, including 1,638 runtime assembly bytes.
Twenty-one complete-context objects fail strict equality. One additional source
is rejected by the native compiler before it creates an object.

The concrete recovery is `flat_rows_address_both.c`: it has the original
44-byte frame and the original sprite/col/line/row/done/str/bufs stack offsets.
The four letter slots are still permuted, y and the width table swap R8/R9,
and the complete original-address context is not equal. No range is credited.

## The weighted-reference mechanism

Continuation 227's captures already contained the individual frequency terms.
The new model independently reconstructs 115 frequency records from 452 weighted
references across four frozen parent cases and seven fresh controls. It matches
each native result before checking grouping and stable frequency sorting.

For ordinary values, a node contributes its reference count only when it is its
own reference head. For addressed memory values, a referenced node contributes
its nonzero use count, or one otherwise. Each count is multiplied by one plus
native loop nesting depth. Those are observed native rules, not fitted target
frequencies.

In the paired-goto parent, the relevant nodes have depth zero. Row has frequency
4, col 3 and line 2. In structured_named_chain, row's two initial reads contribute
4 and its increment/test contribute 2 each: 8. Col's two shared reads contribute
6 and its increment 3: 9. Line has two shared reads, contributing 6. This puts row
before line, unlike ROM. The native tree contains a shared `line + col` temporary
used by the two terminators.

Taking the address in the genuine col initialization or line assignment prevents
that shared address expression. With col addressed, its memory frequency is 15,
line becomes 9, and row stays 8. With line addressed, col becomes 12, line's memory
frequency becomes 11, and row stays 8. Both restore the col/line/row slot order.
These are ordinary `*(&col)` / `*(&line)` expressions; there are no extra stores,
volatile accesses, padding or fabricated values.

The separate frequency-2 temporary comes from the cast of the global row-buffer
base in `(char *)&g_607930c[0]`. Native block 2 defines that cast before the loops.
Declaring the same externally located row data as a flat char array removes the
cast and its hoisted temporary while retaining `(char)(row * 30)`. Native objects
confirm the frame shrinks from 48 to 44. The original signed-byte offset and
four stack sprite stores/eight loads remain. The byte array is used solely as
raw row storage; its declaration introduces no object or layout change.

## Source controls

| Control | Context bytes | Differences including missing bytes | Frame |
|---|---:|---:|---:|
| baseline | 756 | 693 | 48 |
| address_col | 756 | 670 | 48 |
| address_line | 756 | 671 | 48 |
| address_both | 756 | 670 | 48 |
| array_base | 756 | 695 | 48 |
| flat_rows | 752 | 659 | 44 |
| flat_rows_address_line | 752 | 673 | 44 |
| flat_rows_address_col | 752 | 673 | 44 |
| flat_rows_address_both | 752 | 673 | 44 |
| flat_rows_commuted_add | 752 | 659 | 44 |
| letters_init_target | 752 | 673 | 44 |
| letters_decl_target | 752 | 673 | 44 |
| letters_init_reverse | 752 | 670 | 44 |
| letters_decl_reverse | 752 | 673 | 44 |
| letters_init_swap | 752 | 673 | 44 |
| letters_decl_swap | 752 | 673 | 44 |
| flat_letters | 752 | 670 | 44 |
| derived_letters | 752 | 666 | 44 |
| chained_letters | 752 | 669 | 44 |
| implicit_letters | 752 | 656 | 44 |
| implicit_flat_letters | 752 | 656 | 44 |

SHC rejects `col[line]` with diagnostics 2200 (Index not integer) and 2233
(Illegal array reference), despite the equivalence permitted by ordinary C.
A fresh native compile reproduces both errors and creates no object. The
initial runner's nonzero aggregate result and compiler log are preserved.
The explicit form `*(col + line)` compiles but produces exactly the same bytes
as the corresponding `line[col]` control.

Flat_rows_address_col, flat_rows_address_line and flat_rows_address_both are
byte-identical. Three declaration-order changes are also byte-identical to that
parent. Three initialization-order changes alter 13, 39 and 11 bytes respectively
but yield no match. They must not be conflated with declaration-order controls.

Named pointers to flat letter storage, pointers derived from the complete
array's byte representation, chained derived pointers and implicit address
expressions all remain negatives. The two implicit forms (struct versus flat
letter storage) are byte-identical. Cross-element derived arithmetic starts at
the byte representation of the entire array rather than walking beyond a single
char member. These controls retain the same four letter destinations.

## What remains different

| Value | ROM offset | Flat rows with addressed counters |
|---|---:|---:|
| sprite ID | 0 | 0 |
| col | 4 | 4 |
| line | 8 | 8 |
| row | 12 | 12 |
| letter3 | 16 | 24 |
| letter0 | 20 | 20 |
| letter2 | 24 | 28 |
| letter1 | 28 | 16 |
| done | 32 | 32 |
| str | 36 | 36 |
| bufs | 40 | 40 |

Native REG records put y in R8, and reachable PC-relative loads put 0x0603585a
(the width table) in R9. ROM uses y/R9 and width/R8. Both allocation candidates
have priority 20; recovering the frame changed the allocation tie. The original
letter-slot permutation is also controlled by equal-frequency negative homes
and their incoming order, rather than by declaration order alone.

All flat-row contexts here remain 752 bytes versus the required 760. Their
EE9C entry lands six bytes early. Length, partial register recovery, or the
already matched tail cannot justify accepting shifted code.

## Verification

`validate.py` passes seven checks: complete allocation replay, statement-tree
ownership and expression analysis, home sorting, weighted frequencies, reachable
SH2 storage, diagnostic reproduction and aggregate authentication. Seven native
captures assemble exactly to their corresponding strict compiler contexts.
Twenty-one fresh read-only hardware replays preserve native and independent
control REG/ILB outputs: 889 allocation events, 3,385 layout events and 94 home
events, 4,368 total. Seven home models reproduce 73 final records. Parent evidence
is read from frozen continuation 227 and included in the cumulative manifest.

No production source, split, compiler, acceptance gate or denominator changed.
The last full build/test evidence remains checkpoint 225's two clean builds and
266 tests per checkout. Full suites were not rerun for this research-only change;
current production pins, report and ROM hashes are verified on both checkouts.

## Next action

Keep flat_rows_address_both as the structurally recovered frame control. Use
its native allocation graph to determine the smallest meaningful arithmetic or
expression change that gives y R9 and the width table R8 without reviving the
extra spill. Then inspect the equal-frequency letter group ordering through
native allocation and home sorting. The derived and implicit forms provide
measured alternatives. Do not repeat declaration permutations or signed versus
unsigned terminators: they cannot explain the missing match in these parents.
The full original-boundary exact-byte gate remains required before any credit.
