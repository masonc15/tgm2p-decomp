# SHC row-address recipe selection — checkpoint235

Production remains checkpoint225: **135,594 / 200,698 bytes (67.56121%)**.
65,104 bytes remain, including 1,638 runtime assembly bytes. Ten full-context
compiles are strict negatives: two frozen calibration sources and eight new
ordinary-source forms. No production source, split, compiler, or metric changed.

## Native selection mechanism

A new read-only hardware observer captures entry and return of SHCGEN's
0x41DD10 recipe selector. Three fresh replays observe 246 addition-selection
calls. Each traced SUA/ASA output equals an independent untraced replay, and
SUA equals the normal-driver capture. The prior cleanup ASA metadata differences
are explicitly verified. The emitter observer also reads each selected static
72-byte recipe record through node metadata+0x80; all modeled records and their
instruction recipes equal the original PE image bytes.

The selector runs twice for the row-address addition. Its first pass estimates
both child states as 3; the second uses their actual generated storage states.
An independent model reconstructs the type-class lookup, configuration matrix,
and applicable predicate programs directly from the original PE tables and raw
entry snapshots. It predicts all six observed selections:

| Source | First recipe | Actual child states | Final recipe |
| --- | --- | --- | --- |
| baseline | 0x4511F8 | 0,3 | 0x450FD0 |
| named base += | 0x4542E0 | 1,0 | 0x454170 |
| addressed base += | 0x4542E0 | 3,0 | 0x454258 |

For these controls, state 0 represents the generated offset temporary, state 1
the allocated named base register, and state 3 the symbolic or memory operand.
This is a scoped interpretation, not an exhaustive definition of every SHC state.
The ordinary-sum type table is 0x448D98/configuration 0x448D48. Compound assignment
uses 0x449350/configuration 0x4492C0. The compound paths additionally evaluate
opcode predicates 0x0E/0x02 and the register-R0 predicate 0x11, then fallbacks.
The verifier originally assumed direct matrix entries only and failed closed;
it now models those observed predicates and rejects any unsupported condition.
Inferred decompiler output and an original selector disassembly are retained.

## Source experiments

| Source | Context bytes | Raw differing/missing bytes | Result |
| --- | ---: | ---: | --- |
| baseline calibration | 744 | 710 | ADD R1,R3; next product R2 |
| named base += calibration | 744 | 711 | ADD R3,R4; next product R3 |
| addressed local base | 748 | 663 | ADD R2,R1; extra memory traffic |
| local struct member | 752 | 629 | ADD R2,R1; extra memory traffic |
| addressed struct member | 752 | 629 | identical to local struct member |
| one-element pointer array | 768 | 704 | ADD R2,R0; extra memory traffic |
| global buffer as struct | 744 | 710 | identical to baseline |
| address of global array | 748 | 639 | ADD R1,R2; next product R3 |
| unsigned-long address addition | 744 | 710 | identical to baseline |
| reversed unsigned-long addition | 744 | 710 | identical to baseline |

All additions' complete emitted operand descriptors are independently predicted
from the original recipe words and raw metadata. Address-taking/aggregate storage
can replace allocated R4 with scratch storage, but the measured forms introduce
stores/loads and fail the full match. Integer address arithmetic is target-specific
32-bit pointer conversion; with row 0..2 it computes exactly the original buffer
address plus 0,30,60 without overflow. Both source orders still normalize to the
same offset-first GEN addition and identical machine bytes. These results rule
out these exact forms; no broader claim about all source expressions follows.

## Validation and continuation

Ten fresh normal-driver captures assemble independently to the strict contexts.
Ten output-preserving emitter replays contain 2,979 emissions; three selector
replays contain 246 observed calls, of which six row-addition choices are modeled.
Both validation commands pass. Calibration sources and contexts equal frozen234.
No new middle allocation/layout models or full production suite were run.
Checkpoint225's two clean 266-test gates, original/rebuilt ROM hashes, and 64 pins
remain production evidence; a new dual-mirror hash receipt is saved separately.

The previous goal turn completed homepage verification; this continuation adds
new compiler evidence and source negatives. Goal remains active and work solo.
Next locate where the ordinary sum becomes offset-first by comparing saved
front-end/middle input trees with the selector's pre-generation child order.
The selector already receives that order, and source pointer/integer casts did
not change it. Investigate upstream canonicalization/coalescing rather than
repeat the tested declarations. Preserve full-context acceptance. Width-table
reload, initialization, color paths, and the final boundary still need recovery.
