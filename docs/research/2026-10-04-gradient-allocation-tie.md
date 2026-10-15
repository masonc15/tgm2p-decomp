# Palette-gradient allocation tie — continuation199

Production remains checkpoint192: **131,780 / 200,698 matching bytes
(65.6608436556%)**, with **68,918 remaining**, including all1,638 runtime bytes.
No matching C was added. The preceding turn made progress by rejecting and
explaining wave lifetime controls and selecting this smaller, independent target.

## Fresh baseline and full allocation model

The canonical palette source freshly reproduces exactly six differing bytes in
29F94–2A08C (248 bytes). All13 function roots are nominal. The preceding924 bytes
and following72 bytes remain exact in the1244-byte context. This confirms the
older continuation42 lead without treating its historical results as current.

SHCMDL's target is function symbol112, the penultimate of13 `.reg` records.
The complete22-candidate allocator model reproduces profit sorting, eligibility,
bounds and physical homes. No generator or final-scheduler inference is needed
to locate the baseline discrepancy:

| Variable | Symbol | Priority | Profit | Native home | ROM home |
|---|---:|---:|---:|---|---|
|red accumulator ar|120|65538|41390|R5|R11|
|green accumulator ag|121|65538|41390|R12|R12|
|blue delta db|119|65538|41390|R11|R5|

These three mutually conflicting values tie. Native sorting visits ar, ag, db in
that order. The ordinary initial constructor sequence around them is
ar, dr, n, ag, dg, ab, db. Source names and declaration permutations do not by
themselves expose this use-order structure, which the old searches did not model.

## Expression traversal also changes weighted priority

Four controls expand genuine accumulator additions. Red explicit `ar = ar + dr`
and red reversed `ar = dr + ar` produce the same native bytes:17 differences.
Blue-only reversed addition remains byte-identical to baseline. Reversing all
three expanded additions produces19 differences. All preserve nominal roots,
context length, and surrounding bytes.

The three traced expression graphs retain identical normalized conflicts and
bounds. Their constructor order changes as predicted by reversing the affected
accumulator/delta first-use pairs. However, each expanded accumulator also gains
exactly two priority points. Thus a hypothetical permutation of the baseline
array alone is not a complete prediction of native output.

Read-only weighted-priority observations explain the extra cost for ar:

| Contribution | Compound addition baseline | Expanded red assignment |
|---|---:|---:|
|Initialization outside loop|1|1|
|One loop-weighted ar read|2|2|
|Loop update|0|2|
|Ordinary register-hint bonus|65535|65535|
|Total|65538|65540|

The baseline update is opcode0x50. The explicit update becomes assignment0x5F,
which receives an extra count in this block; loop multiplier2 makes it cost2.
The initial assignment has count0 plus assignment bonus1, with no loop multiplier.
The read has count1 and loop multiplier2. These observed sums reproduce native
priority values. Red expansion additionally swaps the red/green delta homes;
it does not resolve the desired ar/db tie.

## Other controls and bounded offline model

Two meaningful shared-zero initialization forms were tested: initialize ab to
zero then copy it to ag/ar, or initialize ag/ab to zero then copy ab to ar.
They produce26 and36 differing bytes. Their21/22-candidate graphs were traced
and reproduced; neither is a match. These are consumed initialization copies,
not unused values or artificial dependence barriers.

The offline model predicts that reducing ar's priority by one or two points,
with all other graph properties fixed, would change only the desired ar/db homes.
This is a sensitivity result, not an implemented C solution or matching credit.
A separate bounded search modeled all65,535 nonempty combinations of toggling
the ordinary register bonus on the16 source variables. None produces exactly
the desired homes while leaving all other homes unchanged on this fixed graph.
It does not prove that all C sources or all compiler behavior are impossible.
No modified priority, IR, object, instruction, or compiler was ever executed.

## Verification and harness correction

Case: `tools/shc_probe/cases/session_gradient_graph199/`.
Seven fresh strict-negative controls (six new/one carried), six complete native
allocator graphs, two weighted-priority traces, and six independently assembled
captures are authenticated. The eight replays record820 events. Every replay's
`.reg` and `.ilb` equals both a fresh normal control and the native capture.
All seven strict contexts are1244 bytes and retain all protected surrounding bytes.
No retained `.kept.o` exists. Native target differences are6/17/17/6/19/26/36 for
baseline/red_reversed/red_explicit/blue_reversed/all_reversed/zero_from_blue/
red_from_blue respectively.

The replay harness initially assumed frontend exit1 and rejected exit5. Adding
message resources did not change that result. Inspection of the successful native
driver trace proved that its frontend also exits5 after three inherited pointer
assignment warnings; the driver then completes normally. The harness now derives
and verifies the expected frontend status from that native trace, rather than
assuming1. Both middle-pass normal and traced runs still require their successful
stage statuses and exact output equality. This corrects the harness assumption;
no game source or warning policy was altered.

`analyze_priority.py` checks normalized graphs, priority contributions and model
receipts. `verify.py` independently resolves ELF relocations, checks original ROM
hash, source/context hashes, roots, exact differences and protected bytes, and
verifies all replay/capture equalities. Offline hypothesis scripts are clearly
separate from native compiler inputs. Production pins and ROM hashes remain
unchanged. The last full production gates are checkpoint192's two independent
clean builds and253 enabled tests.

## Next action

Preserve compound-update cost while investigating a genuine expression form
that changes candidate construction order, or identify a genuine lifetime change
that reduces ar's cost without introducing other home/stack differences. A
sequenced compound update whose result is consumed by the color calculation is
one bounded source question; it must be checked for extra read contributions.
Do not repeat expanded-addition, shared-zero, declaration/name, helper or blind
hint families already covered. Validate the actual graph and strict native bytes
before retaining anything. The literal100% goal remains active, solo.
