# SHC checksum constant ordering

Continuation109 follows108's bank-count investigation. The last turn made
progress by establishing where emission first diverges and ruling out assignment
splitting as a neutral change. This turn investigates tied constant allocation
in30880. No production source, compiler, verifier or coverage definition changes.

## A verified allocation prediction

The current genuine source keeps the bank base0x24060000 inR9 and mask0x100 inR8;
mask0x300 has no permanent register. The ROM keeps0x100 inR9 and0x300 inR8, loading
the bank base into the actual pointer inside the loop.

Read-only hardware traces observe candidate creation, unstable sorting,
caller-register eligibility and final allocation. The recovered allocator
reproduces all29 baseline entries exactly. Holding graph, scores and eligibility
fixed, only one of the six orders of these three constants gives the desired
allocation while preserving every other register:

```
0x300, 0x100, 0x24060000
```

The actual baseline order is0x100,0x24060000,0x300. Of198 same-category pair swaps
and156 variable moves, none achieves the complete desired allocation. Two of240
constant moves do in the abstract model. One moves0x300 ahead of0x100 and the
bank base. The other moves a numeric1 across hash buckets and cannot be realized
by discovery ordering with this fixed descriptor set. It is not a source fix.

## How the compiler creates this order

Recovered SHCMDL routine415C20 selects a descriptor bucket using `value & 0x7f`
and appends new distinct descriptors to that bucket. Routine4159E0 visits
buckets in ascending order and creates eligible candidates. All three bank
constants occupy bucket0; their discovery order is therefore significant.

A new hardware tracer records415C20 arguments without changing compiler memory.
For the baseline, the collector sees0x100 at IR point163 and the bank base at
point160 in the same block, then0x300 at point168 in the conditional body. The
ordering is not simply ascending source position. The observed collection order
matches the later candidate birth order for all15 bank-constant observations
across five fully traced native controls.

Moving the genuine pointer initialization after the bank-register writes changes
the order to0x100,0x300,0x24060000. Both masks then receive permanent registers,
but their registers are reversed relative to the ROM. A second fixed-graph
model, independently verified against this candidate's29 allocations, predicts
that swapping those first two constant candidates would fix their placement.
This is a useful partial source effect, not a full function match.

The other controls explain why arbitrary reordering is insufficient. Consuming
a masked bank value adds a real value lifetime and changes profits from-3 to-2.
The goto forms change live-range interference, allowing both masks to share a
physical register at separate times. They do not preserve the graph used by the
baseline prediction. The model's hypothetical exact result is not evidence that
one of these source controls is exact.

## Native results

All ten controls use real source operations and the normal three-function
context. There are no dummy consumers, synthetic padding, physical-register
bindings, compiler patches or weaker verification rules.

| Control |30880 differing bytes|Context bytes|Original274-byte suffix exact|
|---|---:|---:|---|
|Baseline|122|1216|Yes|
|Pointer initialization after bank writes|151|1216|Yes|
|Consumed masked bank value|125|1216|Yes|
|Branch body spelled with gotos|298|1208|No|
|Inverted bank guard|318|1204|No|
|Guarded do loop|290|1228|No|
|Pointer placement plus guarded do|302|1224|No|
|Wide counter plus preceding form|301|1212|No|
|Named consumed mask|306|1220|No|
|Two named consumed masks|326|1228|No|

Every complete30880 and30A44 proof rejects. Moving a function or matching its
shifted bytes does not earn credit. The baseline's actual roots and binary are
also verified identical to108's baseline.

## Evidence

The frozen case is `tools/shc_probe/cases/session_checksum_constants109`.
`analyze.py` validates10 source/native-object comparisons,20 exact middle-stage
replays,751 score records and20 hardware-traced middle-stage runs. It verifies
146 sort positions,146 birth positions,146 allocation predictions and146
eligibility entries. Collection traces contain498 calls. Traced `.reg` and
`.ilb` files are byte-identical to the native captures. The comparison ignores
only the compiler object's `.comment` section.

`model_order.py` runs two independently verified graph models. Together they
check396 swaps,480 constant moves,312 variable moves and12 three-constant
permutations. `GRAPH_ANALYSIS.json` retains raw graph observations. Its opaque
symbolic-address descriptor numbers can change between compiles; its raw
cross-source difference counts are not proof of semantic graph changes or
identity. Each source's complete allocation prediction is separately checked.

Both checkouts retain checkpoint104's35 production pins, original ROM hashes,
and127326/200698 matched bytes (63.441585865%). No new matching bytes. The236-test
full-suite pass remains historical from104, not newly run for these negative
research controls. All73372 remaining bytes, including1638 runtime bytes, remain
incomplete. Literal100% stays active.

## Next action

Park these30880 constant-order source families. Preserve107's correct-register
baseline rather than promote the lower-quality controls. Investigate the
adjacent490-byte30A44 routine independently while retaining the genuine preceding
context and protected274-byte suffix. A fresh original-ROM comparison records
54 differing bytes there: initial zero isR6 in ROM versusR3 in the candidate,
and the initial table addresses occupy oppositeR4/R5 registers. Trace that
function's initialization and call-argument allocation before further source
experiments. `NEXT_TARGET.json` binds this next step to current source, binary
hashes and both setup disassemblies.
