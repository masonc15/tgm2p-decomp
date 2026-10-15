# VM priority construction, continuation 224

Production remains 135,502 / 200,698 verified bytes (67.51537%). All 23 source
controls strict-reject. The exact 104-byte baseline tail remains twenty bytes
early, at EE88 instead of EE9C. No source, compiler, proof or ROM production
changes and no new C credit.

## Native priority construction

Hardware execution breakpoints observe original SHCMDL at 415B3C/415B9B for
constant contributions and assignment, and 40FFC2/41019F for local-variable
contributions and assignment. The executable and IR are never patched. Each
replay reruns the original frontend and middle stage in a temporary directory,
then compares traced REG/ILB outputs against a fresh untraced control and native
compiler capture. Compiler hashes are recorded. Static evidence for 4159E0,
4159B0,40FF00 and411430 is saved beside the traces.

The model independently reconstructs every observed subtotal. For the baseline,
constant 63 contributes 5+5+5+5=20. Its four constant nodes each appear under two
loops; the constant scoring rule adds two plus a q-node bonus. Variable y has
one assignment worth 2 and four uses worth 3: priority 14. The variable scoring
rule weights reference/assignment counts by loop depth plus one. This is an
observed difference between two native scoring algorithms, not speculation
from the final register names. Address-symbol bonuses after the recorded
constant assignment are outside this contribution model's claim.

Seven traces validate 1,336 priority events. Seven allocation replays validate
977 events and independently model 47/47/47/47/47/41/41 candidates. All fourteen
replays preserve their compiler outputs. Seven separately assembled captures
reproduce the native compiler object bytes.

## Source consequences

Ordinary meaningful arithmetic assignments vary y's native priority gradually:

| Source calculation | Priority | Outcome |
|---|---:|---|
| y=62-row*29 | 14 | spilled |
| y=62; y-=row*29 | 16 | spilled |
| y=row*29; y=62-y | 18 | spilled |
| y=row; y*=29; y=62-y | 20 | R9 and a 44-byte local frame |

The final form recovers the original y register and frame size without register
hints, patched priorities or dummy uses. It still differs elsewhere: constant63
remains in a callee-saved register, the sprite-array base spills, and the tail
boundary/pool do not match. No combination of partial successes is counted.

Two genuine shared color-store forms and their y-copy combinations also fail.
Pairing color tails lowers constant63's priority from20 to10, independently
observed as5+5. The ROM itself shares color tails, but these source controls
change other lifetimes and pool splits. The y-copy paired form happens to place
EEBC at its original start while the earlier tail functions and total boundary
remain wrong; its nominal 104-byte interval still has62 differences.

Explicitly consuming the sprite ID address or a pointer to it creates contexts
840–852 bytes long and does not recover the original stack pattern. Numeric
sprite/width-table addresses also fail. The baseline has branch-local scratch
registers for sprite IDs; the ROM stores and reloads each returned ID from its
stack. This remains a concrete discrepancy to investigate.

## Initializer hypothesis and validation

Four genuine two-byte array initializers were compiled separately. The strict
production gate rejects their allocated .rodata section. A diagnostic resolver
checks its two zero bytes against ROM33BDC and authenticates the boot copy
3141C -> RAM6030C7C, length4EE5, mapping that pair to RAM603343C. This resolver
only measures the rejected object; it never generates a retained production
object or changes an acceptance rule. Three direct array forms again yield the
exact 104-byte tail twenty bytes early; naming an additional buffer pointer
moves it thirty bytes early with further mismatches. Initializer representation
therefore does not remove the current boundary error.

There are19 ordinary controls plus4 initializer controls. The final
model_graph.py, model_priority.py and verify.py runs all exit zero. The verifier
re-resolves every object, checks source/context hashes, all strict rejections,
initializer provenance, replay equality and the contribution models. The
baseline capture was regenerated in a clean directory after initially importing
an older capture; the initial evidence is preserved separately. Final replay
checks require exactly one REG and ILB file per capture, avoiding stale inputs.

Evidence: tools/shc_probe/cases/session_vm_priority224/. The62 production pins
and checkpoint218 clean builds /263 tests per checkout remain authoritative;
full suites were not rerun for research-only changes. The dashboard remains at
the last verified production gain.

Next inspect native definition/use-web formation and scalar promotion for the
sprite ID, especially its four independent branch assignments. The ROM's stack
stores/reloads and shared tails differ from the scratch-register webs observed
here. Do not repeat the closed y-arithmetic, register-hint, pointer-address or
initializer families without a new compiler mechanism. A retained complete
function still must satisfy the unchanged original-boundary and byte gates.
