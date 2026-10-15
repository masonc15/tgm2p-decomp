# Board-scroll lifetimes and predictive allocation — continuation188

The previous goal turn made evidence progress: it authenticated literal promotion
and exposed conflict averages as the next question. This pass follows that question
with native graph captures and source predictions. It adds **zero verified C bytes**.
Production stays at 130,980 / 200,698 bytes (65.2622348005%).

## Verified lifetime changes

Four carried source controls receive fresh native captures and output-preserving
SHCMDL graph replays. Packing the high word into an unsigned shift and high mask
removes the pre-loop 0x3ff member: its priority falls from 17 to 16 and it no longer
conflicts with early hi. The two hi webs remain separate, at priorities 2 and 9.
The aa/ac state addresses still conflict with both hi webs and retain their
advantage from the lower average conflicting priority.

Compound high-word assignments merge the two hi webs into one priority-11 web.
The state addresses then have 19 conflicts instead of 20 and lose their unwanted
registers. The merged hi, however, spans 22 conflicts and wins a retained register
itself. Combining an unsigned shift with the high-mask compound assignment gives
one priority-12 hi web, drops the pre-loop 0x3ff use, and yields exactly 800 bytes.
It still differs in 602 target bytes and is strictly rejected.

Thus merging hi removes the low-priority early conflict but creates a longer-lived
candidate that displaces a desired constant. This is a measured tradeoff, not a
matching fix. The original ROM keeps early hi in R12 and later spills the packed
high word, so blindly merging the web is not the desired final allocation.

## Source-hint predictions tested against native output

`predict_hints.py` considers all 256 subsets of the eight historical ordinary C
register hints on each of three fixed, verified graphs (768 modeled cases).
It changes only their observed 65,535 priority increment in an offline model;
compiler memory, instructions, and intermediate inputs are never changed.
No subset predicts all eight desired variable homes on these fixed graphs.
This is a conditional model result, not 768 native compiles or a general
impossibility result for different source bodies.

Two selected predictions are tested with normal C compilation and fresh traces:

- Packing with only the pc hint predicts six of eight ROM variable homes. Native
  output agrees with every modeled positive/unallocated variable and constant
  home. The model uses zero for unallocated; the native spill sentinel is -1.
- Removing only the table hint from the combined pointer-update source predicts
  five of eight ROM variable homes. All modeled variable and constant homes agree
  with native output.

The native models reproduce every profit, sort position and positive allocation.
These controls validate the predictive route for these changes, rather than
converting diagnostic register agreement into matching-byte credit.

## Genuine context control

Using scalar globals makes the packed source 798 bytes. Appending that source to
the genuine cache-update/board-clear compilation context produces an identical
798-byte scroll body at its correct original root. All 656 previously matched
board-clear bytes remain exact. The earlier cache-update root stays two bytes
early and remains excluded, just as in its established context receipt.
The context therefore does not repair this candidate's allocation or body.

## Native controls and validation

| Source | Scroll bytes | Differing or missing nominal bytes |
| --- | ---: | ---: |
| hi_pack_mask | 798 | 725 |
| combined_hi_pack | 802 | 597 |
| hi_compound | 772 | 753 |
| combined_hi_compound | 794 | 672 |
| pack_pc_hint | 812 | 648 |
| updates_no_table_hint | 802 | 651 |
| compound_packed | 800 | 602 |
| packed_scalar | 798 | 655 |
| packed_clear_context | 798 | 655 |

Nine strict negatives include four carried controls and five new normal compiles.
Seven fresh hardware-breakpoint replays contain 658 events. Captured/native ELF
sections (except source-date comments), REG and ILB equalities, compiler hashes,
seven complete graphs, two source predictions, original relocations and linked
roots all verify. The original 800-byte nominal range and absent retained objects
are checked independently. All 65,536 low-16-bit inputs verify the packed-bit
identity; higher input bits are discarded by the unsigned shift/high mask.

All 52 production pins remain exact. Both remote reports and original ROM hashes
are checked separately at checkpoint188. No full suite was rerun for excluded
experiments; checkpoint183 remains the last independent clean-build and
251-enabled-test result.

## Next action

Trace the construction order of the tied pointer candidates before SHCMDL's
4074E0 partition sort, using the successful native hint predictions as controls.
Connect that order to real expression/lifetime construction before trying a body
change. The original disassembly already initializes pa, px, pb in the same
natural order as the source; do not assume arbitrary initialization permutations
will solve the allocator. Do not repeat the tested packing, compound, hint-subset,
or this clear-context family without a new causal prediction. The goal remains
active, solo, with all 69,718 unmatched bytes still required.
