# Typed addresses and observed SHC copy merging

Continuation158 completes four ordinary C controls derived from157's preloaded
destinations. All address/brightness values are consumed by the original two
stores. Integer address views are SHC 32-bit target diagnostics; no portability
claim is made. No compiler instructions, compiler data, or allocation priorities
were modified.

## Observed mechanism

Hardware breakpoints at410CF0 and411010, with dynamic return breakpoints, record
one merge dispatcher invocation per source. Each replay reproduces the captured
REG and ILB exactly. Heap pointers are compared only within their own replay.

| Control | Merge calls | Common priority | Body bytes | Pointer spills |
| --- | ---: | ---: | ---: | ---: |
| byte_second | 2 | 8 | 1752 | 2 |
| integer_second | 1 | 6 | 1738 | 2 |
| byte_both | 2 | 8 | 1752 | 2 |
| integer_pair | 0 | 4 | 1752 | 0 |

Byte casts leave a nested assignment or identifier at the copy recognizer.
The single integer destination retains one outer cast; distinct signed and
unsigned integer destinations retain both outer casts and prevent both merges.
All five observed merges follow destination priority = old destination + source
- 2. The source becomes category3; the destination is self-bound and receives
the source definitions ahead of its previous definitions.

The integer_pair control removes the spill and restores the six selected loop
roles. It still uses b0/R5, b1/R7, dst0/R6 and dst1/R4. The graph records all six
pairwise conflicts among those four values. Consequently this graph cannot
produce the original three-register assignment, regardless of priorities.
Close duplicate-address priority tuning; changing lifetimes is required.

All controls have frame108, spr1 home36/frequency7, four static spr1 reads,
and start atA604 rather thanA608. Each forms one common multiplication and
emits one word load/three SHLL2 operations in the brightness sequence. These
local improvements do not establish a whole-function match. Every strict
prefix check rejects; no C bytes are credited.

## Verification and limitations

Four native/capture comparisons, eight middle replays, four home traces,
four graph traces, four generator traces and four merge traces pass. Models
verify111 homes/564 frequency contributions,356 allocation candidates/1212
eligibility calls, and2322 generator emissions/963 recipes. Generator SUA/ASA
match their untraced controls; cleanup-capture ASA metadata differences remain
explicit in the receipts. All rodata sections match157; text does not become
identical merely because its size or mismatch count agrees.

The initial tracer compile warning and the initial mistaken assignment-child
layout are preserved in INITIAL_TRACER_BUILD_FAILURE.log and
initial_child_layout/. MERGE_TRACER_NOTES.md records the correction. Conclusions
use only the corrected reruns. No compiler bytes were patched in either run.

Fresh checks retain44 production pins,43 earlier frozen cases, original ROM
hashes, and128316/200698 matched bytes in both Nuada checkouts. The most recent
full builds/tests remain checkpoint122's independent clean builds and243 enabled
tests; those are historical, not rerun for these private negative controls.

Evidence: tools/shc_probe/cases/session_animation_castmerge158/VERIFICATION.json,
MERGE_MODEL_RESULTS.json, INTEGER_PAIR_CLIQUE.json and PRODUCTION_RECHECK.json.
The literal100% goal remains active with72382 bytes remaining.

## Next target

Apply the recovered conversion/lifetime distinction to the146-byte F200 VM
handler. Recompile its historical five-byte near-match before relying on it.
Test consumed call-result representations while retaining both existing context
ranges; do not repeat declaration, register-hint or row-factor sweeps. This
moves the investigation away from the proven four-value animation conflict.
