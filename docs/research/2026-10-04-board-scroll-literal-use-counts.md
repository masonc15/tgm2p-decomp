# Board-scroll literal representatives and shared-use counts — continuation190

The previous turn made evidence progress by confirming a pure candidate-order
change that reproduces all eight main variable registers. This continuation tests
the proposed literal-eligibility explanation. **That explanation was wrong:** BCA
and CBA admit the same mask representatives. Shared-reference counts explain the
priority difference. No new matching C bytes are added; production remains
**130,980 / 200,698 bytes (65.2622348005%)**.

## Representative selection is unchanged

Three fresh gate replays observe the original SHCMDL nodes at 4155C8, 4157F6,
41581A and 415899. Each source has 28 tracked mask nodes: 20 pass as representatives
and eight fail the initial `node+0x3c == node` identity check. Those eight are four
early 0x3f00 AND literals and four corresponding comparison literals. All admitted
nodes pass the type/flag gates and return zero from helper404CD0.

For the observed wide AND parents, helper404CD0 dispatches to405016. The actual
0x3ff/0x1ff/0x3f00 values exceed the byte-immediate ranges accepted by that branch;
the observed int/long types also follow its zero-return path. These AND constants
therefore receive no immediate exemption. Generic spelling or width changes should
not be assumed to remove a 0x3ff candidate while preserving these operations.

The initial check chooses representatives, not all eventual counted references.
Nonrepresentative nodes can still be counted through a representative's shared-use
chain. This distinction is necessary to explain the next result.

## The extra 24 priority points are fully accounted for

Three separate fresh use-count replays observe helper returns, each increment,
chain advancement and final member count at415AD4/415B2D/415B31/415B3C. Every linked
node, common representative and final count verifies against the independent
constant-weight trace.

- BCA has twelve tracked 0x3f00 representatives, each with one counted use and
  factor4: total48.
- CBA has the same twelve representatives. Three final-mask representatives have
  chains of three counted references; the other nine have one. The extra
  `3 * (3 - 1) * 4 = 24` gives total72.
- Early packed BCA again has total48.

The three longer chains contain AND, comparison and AND nodes. All their observed
helper results are zero, and every counted use is authenticated at the increment.
The analysis reconstructs **21 constant totals, 129 weighted contributions, and
66 tracked mask references** across the three sources. It does not substitute a
count of source-text constants for the compiler's actual sharing structure.

## Real named-local controls

Two normal C controls use the known global address through a named pointer; one
also names the two real masks. The locals are initialized before attributes/state
processing and consumed by the existing operations. No dummy operations, physical
register bindings, compiler changes or intermediate-input edits are introduced.

The named flag pointer gets priority9 and logical home9 (R10), displacing 0x3ff
but putting the desired a6 address in the wrong register. Naming both masks too
produces a limit variable of priority33/home9 and a separate 0x3f00 literal of
priority17/home10. The flag takes home11, while the named low mask spills. Thus the
second source wastes a retained home on a duplicate clamp value. Both fail the
whole nominal function comparison.

| Source | Compiled bytes | Differing or missing nominal bytes |
| --- | ---: | ---: |
| order_bca | 800 | 634 |
| order_cba | 790 | 701 |
| bca_early_pack | 814 | 706 |
| named_flag | 812 | 687 |
| named_masks_flag | 816 | 730 |

## Verification and next action

Five native controls comprise three carried sources and two new compiles. Eleven
fresh output-preserving replays contain **1,061 events**: three gate, three
constant-weight, three shared-use, and two full graph replays. The two new graphs
contain26/27 candidates and reproduce every profit, sort and positive assignment.

`verify.py` independently resolves original ELF relocations; checks hashes, native
object sections apart from date comments, linked roots, the entire nominal range,
strict rejection and absent retained objects; and compares REG/ILB with fresh
untraced controls and the native captures. It verifies all52 production pins.
Both remote ROMs and progress reports are rechecked separately at checkpoint190.
The last full build/test gates remain checkpoint183's independent clean builds
and251 enabled tests; these excluded experiments do not change production.

Next inspect the conversion/shared-reference path that leaves the named limit as
both a variable and a separate short-store literal. Compare the observed long
local with a correctly typed unsigned-short limit only after identifying the
conversion responsible. This tests a concrete real-data representation rather
than another ungrounded mask-spelling sweep. The literal100% goal remains active,
solo, with all69,718 unmatched bytes still required.
