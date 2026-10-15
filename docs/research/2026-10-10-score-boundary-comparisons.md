# Score comparison boundary normalization

This is a bounded follow-up to the original constant-priority trace documented
in 2026-10-04-score-constant-priority.md. That trace attributed constant2's two
priority contributions to the two actual n>2 comparisons. This experiment
tests whether equivalent n>=3 boundaries can affect constant allocation and
then normalize to the target's comparison sequence. It does not rerun named
minimum storage, literal suffixes, or coefficient arithmetic controls.

## Current producer and budget

The full source is copied from current src/speed_7956.c, including its genuine
now-exact following function. The first function remains unretained. Four
ordinary compiles were recorded in advance: fresh baseline, first comparison
changed, second changed, and both changed. No compiler or flags were modified.
Each changed predicate is equivalent for every unsigned-byte input, exhaustively
checked over 0..255. No game accesses, calls, writes, or their order change.

## Result

| Form | Target differing bytes | Context bytes | Actual following entry | Protected differing bytes |
| --- | ---: | ---: | --- | ---: |
| Baseline | 19 | 1,456 | 0x7956 | 0 |
| First n>=3 | 605 | 1,464 | 0x795e | 627 |
| Second n>=3 | 698 | 1,468 | 0x7962 | 635 |
| Both n>=3 | 650 | 1,460 | 0x795a | 602 |

The first-only control emits MOV #3,R6 at0x768e and CMP/GE R6,R4 at0x76d2.
The baseline instead emits MOV #2,R6 and CMP/GT R6,R4. These observations use
raw original-address bytes, not rebased or normalized scores. The desired late
normalization does not occur in this case. Internal priority trajectories were
not traced anew; native opcode changes alone do not establish that trajectory.

All four fail the unchanged strict proof. Every variant also breaks the nominal
690-byte protected following owner. Stop this boundary-adjustment family.

## Evidence and next question

research/2026-10-10-score-boundary-comparisons contains frozen sources, hashes,
ordinary original objects, resolved contexts, disassembly, strict rejection
messages, experiment.json, and verify.py. Verification independently resolves
every object, checks roots and raw ranges, compiler/source hashes, unchanged
following source, exhaustive predicate equivalence, and the opcode observations.

No source/split/proof/compiler integration occurred and no bytes were credited.
The live whole-ROM baseline remains137090/200698=68.306610%, with61970 non-exempt
bytes still unresolved. Full baseline checks are recorded in
research/2026-10-10-piece-first-turn-liveness/VALIDATION.json.

The earlier constant-priority evidence and thirteen-byte positive-label guard
candidate remain the appropriate research starting points, not these worse
boundary variants. Before additional source trials, the next compiler question
is whether genuine definition/traversal ordering can reverse the equal-priority
constant2/65535 allocation while preserving both original comparison opcodes.
This requires a reduced, output-authenticated compiler example and a predicted
source mechanism; do not expand the boundary, suffix, named-minimum, coefficient,
or Boolean sweeps.
