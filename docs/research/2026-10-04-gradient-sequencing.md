# Gradient expression sequencing — continuation200

No production bytes changed. Current Nuada report remains131780/200698 bytes
(65.66084%);68918 bytes remain across44 incomplete units. The prior goal turn
completed ROM transfer verification and analysis, so it counts as progress,
not a wait or a blocked turn. The goal remains active, solo, without brainstorming.

Fresh native compiles in `tools/shc_probe/cases/session_gradient_sequence200/`
cover23 controls. Every target29F94–2A08C fails the unchanged strict checker;
no `.kept.o` is produced. Original native objects and linked contexts are saved.
All23 preserve the already matched924-byte prefix. Some change the following
72-byte range or its root; these are negative evidence, never retained output.

| Control | Target differing bytes | Context bytes |
| --- | ---: | ---: |
| Baseline |6|1244|
| Red, blue, or all updates as `(acc += delta, acc)` |6 each|1244|
| Red unsigned-int accumulator with unsigned-long division cast |6|1244|
| Red update in separate statement |143|1236|
| All updates in separate statements |148|1232|
| Red initialized at declaration before the branch |219|1236|
| Six chained-zero initialization orders |144,145,145,169,144,169|1236 each|
| Red/green/blue/all `acc -= -delta` |155,164,162,193|1260,1260,1260,1272|
| OR terms RBG/GRB/GBR/BRG/BGR |41,41,63,112,99|1244 each|

The four six-byte controls are byte-identical to baseline, including its
protected tail. The unsigned-int form is an additional reproduction control,
not evidence of a new type-search avenue. All component deltas fit signed short,
so unary negation is within signed-int range; the subtraction forms preserve
32-bit unsigned accumulation. Color expressions access separate accumulators,
so changing their OR order preserves values without unsequenced conflicting writes.
No fake uses, fixed registers, padding, new inert branches, or patched IR were used.

## What the compiler traces establish

Six fresh native assembly captures independently assemble and resolve to the
strict compiler contexts. Six full allocator replays and three priority replays
produce1026 events. Each traced `.reg` and `.ilb` equals both a fresh untraced
control and its native capture. Native frontend status5 is reproduced, as in199.
The model verifies all observed candidate profits, unstable sort order, and
register allocation; it does not claim stack-layout equivalence from spill IDs.

The red comma form changes only red accumulator120's priority in the normalized
allocation graph:65538 to65540. Clashes and bounds stay identical. This differs
from199's expanded-assignment mechanism: the update stays opcode0x50, with no
assignment bonus, but its common node's count becomes1. Its loop multiplier2
adds a use weight2. The initialization/read/update weights are1/2/2 versus the
baseline's1/2/0. The home remainsR5, so final bytes remain unchanged despite
different compiler decisions. Source syntax and final-byte equality alone would
have hidden this distinction.

The entry initializer stays charged: it belongs to block kind2, not exempt kind1.
Its priority remains65538 and its three contributions remain1/2/0. Red and the
zero constant each gain conflicts with c1/c2. Red profit rises41390 to43689 and
its home changesR5 toR13, explaining why this is not the desired lower-weight fix.
The unsigned-int control's normalized graph is identical to baseline.

The traced RBG expression keeps the red/green/blue-delta homesR5/R12/R11;
GRB gives greenR5/redR12/blue-deltaR11 and exchanges red/green delta homes.
Neither produces redR11/blue-deltaR5. Both keep actual function roots nominal
but alter the following range's bytes, which includes shared literal data.
No untraced graph explanation is asserted for the other permutations or chains.

`analyze.py` authenticates graph/weight conclusions; `verify.py` independently
resolves original objects, roots, raw differences, source hashes, protected ranges,
all nine replay equalities, and six assembly captures. No whole-project tests
were rerun for rejected diagnostic sources. Checkpoint192 remains the last
production gain with54 production pins and253 enabled tests.

## Next investigation

Close these gradient sequencing, chained-zero and subtraction spellings. Inspection
confirmed that the similarly close F200 and save-writer paths already have direct
allocation investigations; they are not fresh alternatives. `NEXT_TARGET.json`
authenticates the source of the genuine1504-byte palette initializer2A576 and
records its historical evidence separately from current verification.

Start from both canonical `src/gfx_2ab56.c` and the preserved
`session_gfx_predecessor_followup/phase_01.c`. The latter's initializer is1504 bytes
but begins12 bytes early and has227 raw body differences; that old score is not
a new match. Reproduce actual roots and the canonical retained246-byte suffix,
then trace the global-base/zero candidate tie and actual component expression
scheduling. Do not repeat the handoff's pointer/type/helper/neutral-AST sweeps or
pretend the shifted root is nominal. The full100% objective is unchanged.
