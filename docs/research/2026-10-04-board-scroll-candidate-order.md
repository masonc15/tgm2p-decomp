# Board-scroll candidate construction and native register predictions — continuation189

Continuation188 made evidence progress by validating two hint-based allocation
predictions and isolating lifetime effects. This pass traces candidate construction
and makes two genuine C variants reproduce **all eight main ROM variable registers**.
Neither complete function matches. Production remains **130,980 / 200,698 bytes**
(65.2622348005%), with **zero new verified C bytes** in this pass.

## How SHCMDL constructs the candidate order

Five fresh hardware-breakpoint replays capture the original expression forests
before 414E10, candidate-constructor entry 415920, its append at 41597B, and the
unsorted table before 405ECC calls the 4074E0 partition sort. Traversal processes
children before their parent, following sibling and block statement lists.

For every captured source, filtering this independently reconstructed postorder to
the observed variable-creation nodes reproduces the variable candidate sequence.
Constructor calls from 415134 append variables; calls from 415B66 append constants.
Observed previous-tail, head, current-tail and web identities reproduce the entire
pre-sort table sequence. The existing partition model then reproduces its sorting.
This establishes order for the selected candidates; it does not claim a model for
all candidate-eligibility decisions among every expression node.

Variable payload-list pointers change after construction as later compiler stages
process them. An initial analysis assertion incorrectly expected those pointers
to stay fixed until sorting. The corrected check authenticates constructor versus
append payloads, then follows unchanged web identity and variable symbol through
the pre-sort table. Constant descriptors do stay fixed in these traces.

The crucial loop candidates are created at their real uses, not simply in local
declaration or pointer-initialization order. The original source orders these
independent operation groups as:

- A: write the base attributes to the x array (px and attr candidates).
- B: read the table, update the scroll amount, and write b (tbl, v and pb).
- C: conditionally advance and clamp c (pc).

The real a/base calculation follows them. These arrays occupy separate board
fields; the table is the known ROM object. The source alternatives reorder these
existing operations without adding dummy consumers or binding physical registers.

## Predicted and observed native layouts

The offline model considers six permutations of A/B/C while keeping the baseline
conflicts and priorities fixed. BCA and CBA predict all eight desired homes. Both
normal native compiles and fresh REG/graph captures confirm:

| Variable | Native register |
| --- | --- |
| pc | R4 |
| pb | R5 |
| px | R6 |
| pa | R7 |
| tbl | R14 |
| base | R13 |
| counter | R12 |
| attr | R11 |

For **BCA**, the semantic conflict graph, priorities, profits and eligibility bounds
are identical to the baseline. Only the candidate construction order changes.
This is a controlled native confirmation of the tie-order prediction.

CBA also gets the eight homes, but its 0x3f00 priority rises from 48 to 72 and its
global-address descriptor changes identity. Thus the fixed-graph premise does not
hold for the entire CBA result; only the predicted main homes are confirmed.
Neither register agreement nor correct 800-byte size counts as matching C.

## High-word lifetime follow-up

The packed BCA control keeps hi in R10, displacing the desired constants/address.
Two further real-source controls move its calculation before the state update and
split the high-word definition. Fresh traces confirm early/late hi priorities 2
and 9, and both webs spill. But attr and counter exchange their desired homes.
The early packed version keeps 0x3f00 and 0x1ff in the desired R10/R9, while keeping
0x3ff in R8 instead of the a6 global address. Its size is 814 bytes. The early
low-mask spelling produces 808 bytes and 510 differing/missing nominal bytes.
These are useful diagnostic constraints, not accepted source changes.

## Strict native controls

| Source | Compiled bytes | Differing or missing nominal bytes |
| --- | ---: | ---: |
| compound_packed | 800 | 602 |
| pack_pc_hint | 812 | 648 |
| updates_no_table_hint | 802 | 651 |
| order_bca | 800 | 634 |
| order_cba | 790 | 701 |
| bca_early_pack | 814 | 706 |
| bca_early_mask | 808 | 510 |

Seven strict negatives include three carried controls and four new normal compiles.
Nine fresh output-preserving traces contain 3,457 events: five construction-order
replays plus four allocation replays. The three carried allocation traces used as
reference inputs are not counted again as new replays. Four new complete allocation
graphs, five postorder/append/sort models and two native physical-layout predictions
verify. Six offline statement permutations are hypotheses, not six native compiles.

The verifier independently resolves original ELF relocations, validates source and
output hashes, nominal function boundaries, original/linked roots, whole-range
rejections and absent retained objects. Native object sections (except source-date
comments), REG and ILB match their original snapshots and fresh untraced controls.
All 52 production pins and original ROM/report checks remain exact in both remote
checkouts. The last full build/test gates remain checkpoint183's independent clean
builds and 251 enabled tests; excluded diagnostics do not warrant repeating them.

## Next action

Use BCA as the controlled pointer-order reference. Investigate the remaining
0x3ff retention versus a6 address, using the BCA/CBA 0x3f00 priority difference as
a concrete control for literal-use eligibility. Trace which expression nodes
enter the constant collection, then make a source prediction that preserves the
required native operations. Avoid another generic hint or mask-spelling sweep.
The literal 100% goal remains active, solo; all 69,718 missing bytes remain required.
