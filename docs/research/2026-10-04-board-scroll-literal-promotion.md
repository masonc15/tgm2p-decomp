# Board-scroll literal promotion — continuation187

This compiler investigation adds **zero verified C bytes**. Production remains
130,980 / 200,698 bytes (65.2622348005%). All 18 source controls fail the strict
800-byte nominal function range; none is integrated.

## What the compiler actually promotes

Six output-preserving SHCMDL replays for baseline, consumed input, and named
clamp limit contain 536 events. The reconstructed factor-times-use totals verify
18 constant priorities. Every one of 56 tracked mask members is rewritten from
literal opcode 0x71 to identifier 0x70, with the observed logical register,
negative temporary symbol, and original type restored through a conversion parent.
The read-only traces use hardware breakpoints and preserve native REG and ILB.

Baseline and consumed input promote twelve tracked 0x3f00 members, five 0x3ff
members, and four 0x1ff members. Their priorities are 48, 17, and 16. The named
limit reduces tracked 0x3f00 members to five and its priority to 17. The early
conditional `(*pc & 0x3f00) < 0x3f00` is not among these promotion members.
The twelve members are the four unrolled copies of the final attribute mask,
cap comparison, and cap store. Thus this is evidence for every tracked member,
not a claim that every source occurrence is promoted.

The five 0x3ff members consist of the pre-loop high-word mask and four unrolled
output masks. ROM instead reloads this mask as scratch while retaining 0x3f00,
0x1ff, and the a6 address. The mismatch is now located in real candidate
lifetimes and allocation, rather than unexplained mask spelling.

## Historical source hints and full graph models

The saved historical sweep generator's mask23 is reconstructed and authenticated
against copied generator/baseline hashes. It is not presented as a recovered
historical object. Existing ordinary C `register` hints are combined with genuine
pointer updates and consumed board input. There are no physical-register bindings,
dummy effects, compiler patches, or modified compiler inputs.

Three additional output-preserving graph replays contain 283 events and reproduce
all profits, sorting, and positive assignments for 25, 25, and 26 candidates.
Corresponding ordinary register hints have a 65,535 priority increment in these
observations. The combined control assigns logical homes in this order: pc, tbl,
pb, pa, px, base, attr, counter. That still differs from the original layout.

The graph profit is priority minus average conflicting priority. In these combined
controls, 0x3ff and the aa/ac state addresses are allocated before the higher
intrinsic-priority 0x3f00 mask. Their different lifetimes change the conflict
average. Historical-copy and combined-historical controls obtain an eight-byte
stack frame, but that isolated agreement does not constitute a matching function.

Packing the high mask, splitting it into compound assignments, changing unsigned
wrapping expressions, and consuming the table increment all remain strict
negatives. The unsigned-modulo output is byte-identical to baseline.

## Native controls

Three controls are carried unchanged from continuation186; fifteen were newly
compiled. Whole nominal differences below include missing bytes and exclude
bytes beyond the 800-byte target range. Diagnostic similarity is not C credit.

| Source | Compiled bytes | Differing or missing nominal bytes |
| --- | ---: | ---: |
| baseline | 792 | 534 |
| input_consumed | 804 | 643 |
| copy_with_limit | 808 | 711 |
| result_modulo | 792 | 534 |
| result_shift | 846 | 714 |
| result_short_modulo | 840 | 755 |
| hi_shift | 802 | 707 |
| historical_core23 | 798 | 705 |
| historical_with_copy | 798 | 690 |
| historical_copy_state | 798 | 690 |
| combined_historical_hints | 798 | 708 |
| combined_hints_copy | 798 | 675 |
| combined_for_updates | 802 | 659 |
| combined_table_consumed | 808 | 640 |
| hi_pack_mask | 798 | 725 |
| combined_hi_pack | 802 | 597 |
| hi_compound | 772 | 753 |
| combined_hi_compound | 794 | 672 |

## Verification and next action

`verify.py` independently resolves the original ELF relocations, checks original
and linked roots, source/output hashes, strict rejection and absent retained
objects. It authenticates all nine traces (819 events) against fresh untraced
controls and native object sections, except source-date comments. It also verifies
the historical reconstruction, mask models, three full allocation models and all
52 production pins. Two verifier attempts stopped for missing copied helper files;
after those files were supplied, the complete verification passed. These were
verification setup failures, not extra compiler experiments.

The next focused experiment is to capture the allocation graphs for the existing
high-mask packing and compound-assignment controls, compare their early/late hi
webs and constant conflicts to the verified combined graph, and test a source
change only when that comparison gives a concrete prediction. Do not repeat the
mask-spelling or parameter-copy families without new causal evidence.

No full suite was rerun for these excluded experiments. Checkpoint183 remains the
last independent clean-build and 251-enabled-test result. Original ROM hashes and
current progress reports are separately rechecked at checkpoint187. The literal
100% goal remains active.
