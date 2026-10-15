# VM letter lifetimes, loop structure and stack ordering

Continuation 227 adds no C credit. All 19 complete EC0C–EF04 contexts fail the
unchanged strict gate. Production remains 135,594 / 200,698 bytes (67.561211%);
65,104 bytes remain, including all 1,638 runtime assembly bytes.

The useful recovery is `paired_named_chain.c`: ordinary named letter pointers
plus chained initialization recover the original 44-byte frame and saved-register
roles, including the width-table address in R8. Its stack ordering, scheduling,
literals and function boundaries still differ. Matching size or register roles
is not a complete function match.

## Native source controls

Every candidate retains the four addressed sprite stores and eight direct slot
loads established in continuation 226. The reachable SH2 decoder confirms this
for all 19 controls and the original, excluding literal pools.

| Control | Context bytes | Byte differences including missing bytes | Frame |
|---|---:|---:|---:|
| unpaired | 792 | 724 | 48 |
| paired | 756 | 734 | 32 |
| paired_single_latch | 756 | 734 | 32 |
| paired_manual_latch | 756 | 734 | 32 |
| paired_late | 744 | 703 | 32 |
| paired_late_single_latch | 744 | 703 | 32 |
| paired_late_manual_latch | 744 | 703 | 32 |
| unpaired_named_letters | 796 | 738 | 52 |
| paired_named_letters | 768 | 708 | 44 |
| paired_late_named_letters | 756 | 734 | 44 |
| structured_continue | 760 | 589 | 48 |
| structured_else | 760 | 589 | 48 |
| single_label | 748 | 731 | 32 |
| paired_named_chain | 760 | 707 | 44 |
| first_terminator_unsigned | 760 | 707 | 44 |
| conditional_terminator_unsigned | 760 | 707 | 44 |
| both_terminators_unsigned | 760 | 707 | 44 |
| structured_named_letters | 760 | 603 | 48 |
| structured_named_chain | 756 | 693 | 48 |

Two sets of loop-latch spellings each produce byte-identical code: paired,
paired_single_latch and paired_manual_latch; and the corresponding paired_late
trio. Combining the loop increment with a label therefore does not change these
allocations. Three signed/unsigned character-lvalue terminator controls are
byte-identical to paired_named_chain. These routes are closed for these parents.

Structured_continue and structured_else have equal sizes and equal difference
counts against ROM, but differ from each other in 15 bytes. The aggregate
verifier caught and corrected an initial assumption of equality. Both remain
strict negatives; neither is accepted because of its summary counts.

## Letter address placement and interference

Read-only hardware breakpoints capture three middle-pass trees and temporary
creator callsites. Unpaired and structured_else create four common-expression
temporaries at caller 0x40e788, then five hoister temporaries at 0x41c476. Paired
and single_label create the same four common temporaries but none at 0x41c476.
Their later expression temporaries instead come from 0x40a1fe and 0x40a02e.
The observed explicit-goto controls suppress the relevant hoisting. The precise
native loop eligibility predicate has not yet been isolated, so this is limited
to the tested sources rather than a claim about every goto.

The corrected block observer follows each statement's child/sibling tree and
checks every node belongs exactly once to its block's native order interval.
Eight models verify all block ownership, letter offsets 0/2/4/6, native priorities
and pairwise interference:

- Unpaired defines hoister temporaries -5 through -8 before the loops and copies
  them to branch expressions -1 through -4. The four resulting allocation groups
  have priority 8, all six pairwise clashes, and four distinct negative homes.
- Paired and single_label compute the base inside the branch chain. Their four
  letter expressions have priority 6, no mutual clashes, and reuse one negative
  home. This explains their 32-byte frames.
- Explicit letter0 through letter3 pointers are initialized before the loop and
  used by the real byte store and draw call. They have priority 7, all six mutual
  clashes and four distinct homes. Paired_named_letters restores a 44-byte frame.

The compiler's common-temp definition expression has two explicit identifier
leaves overall; named pointers have three (definition plus two reads). The
initial layout model's uniform three-leaf assumption is preserved with its
correction. No observer output was changed to satisfy that assertion.

## Register and stack-home recovery

Paired_named_letters keeps constant zero in R8. Chaining the three genuine zero
initializations as `i = row = col = 0` removes that choice; paired_named_chain
loads 0x0603585a into R8 on a reachable instruction, matching the original width
table. Final native REG records independently verify buf/R14, x/R12, i/R11 and
y/R9. The sprite base and 254 constant retain R13 and R10.

SHCMDL's later home ordering is a separate pass from register allocation. Four
native home traces verify frequency grouping and stable descending sorting for
all 42 records against final serialized REG output. Negative home IDs before
this pass must not be treated as final stack slots.

| Value | ROM stack offset | Paired named chain offset | Structured named chain offset |
|---|---:|---:|---:|
| sprite ID | 0 | 0 | 0 |
| col | 4 | 8 | 4 |
| line | 8 | 24 | 28 |
| row | 12 | 4 | 8 |
| letter3 | 16 | 16 | 16 |
| letter0 | 20 | 20 | 20 |
| letter2 | 24 | 32 | 12 |
| letter1 | 28 | 28 | 24 |
| done | 32 | 12 | 32 |
| str | 36 | 36 | 36 |
| bufs | 40 | 40 | 44 |

Paired_named_chain has sprite frequency 12, row 4, col 3, done/letters/line 2
and str 1. Structured_named_chain restores loop weighting: sprite 36, col 9,
row 8, letters/line 6, done 4 and str 3. It also keeps an extra hoisted temporary
with frequency 2 at offset 40, growing the local frame to 48. The structured
combination therefore recovers some original ordering while losing the frame.
No source padding, dummy effects or physical-register forcing was introduced.

## Reproduction and preserved failures

Run `validate.py` on Nuada with the established Python/toolchain. Its six checks
cover allocation, layout, pipeline, home sorting, reachable storage and the
aggregate verifier. The latter independently resolves all 19 ELF contexts,
authenticates source and object hashes, checks all six function roots and strict
rejections, and assembles eight independently captured native outputs.

Twenty-four hardware replays preserve fresh control and captured REG/ILB bytes:
1,030 allocation events, 3,991 layout events, 6,832 pipeline events and 54 home
events; 11,907 total. There are eight allocation/layout models, four pipeline
models and four home models. Native compiler binaries, flags and inputs remain
unmodified. Breakpoints are hardware execution observations.

Initial setup and layout-observer errors are retained. The initial layout walker
followed root siblings across statements; the corrected walker excludes that
edge and independently validates block order ownership. Initial aggregate
verification rejected the mistaken structured-control equality assumption, then
a decoder-field mismatch (`symbols` list versus `symbol` scalar). Both failures,
verifiers and logs are saved; final validation alone is marked passed.

Evidence is frozen in `tools/shc_probe/cases/session_vm_latches227/`. No production
source, split, acceptance gate or denominator changed. Checkpoint 225's two clean
builds and 266 tests per checkout remain the most recent full suites; they were
not rerun for this research-only checkpoint. Current production pins and ROM
hashes are checked separately on both checkouts.

## Next action

Use structured_named_chain and paired_named_chain as complementary controls.
Trace the weighted reference contributions for row and line, and the origin of
the extra frequency-2 hoisted temporary. In particular, compare the original's
two terminator address calculations with the native shared line+col expression.
The signed/unsigned terminator spelling does not separate that expression.
Identify the native common-expression/hoisting decision before another source
sweep. Preserve ordinary C semantics and recover the original stack ordering and
44-byte frame together; only then attempt instruction/pool scheduling. Every
accepted range still requires complete original-address byte equality.
