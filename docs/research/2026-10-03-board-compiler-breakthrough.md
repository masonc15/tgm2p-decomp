# SHC investigation: board-update allocation and copy coalescence

Continuation 81 applies the priority tracing recovered in the medal investigation
to board update `f_259ea`. The accepted source retains 1,412 bytes, through
`25F6E`, under the unchanged original compiler and default verifier.

## What changed

The previous best source had eight differing bytes, all register choices in the
fourth field block. Its current index `k` had `register` storage; its preserved
index `old` did not. Read-only hardware observations of SHCMDL confirm that the
register storage class adds 65,535 to each relevant allocation priority. The
ordinary `old` webs score 4; declaring that existing index `register short old`
raises their scores to 65,539. No physical register is specified.

That change fixes all eight old differences but introduces four earlier ones:
the load at 25E48 and copy at 25E58 exchange R4/R5. The next prediction tested the
real current-to-old copy in two forms. Separate assignments leave 18 differing
bytes. The chained assignment `old = k = r13->w84` in the fourth field block
recovers the original load/copy decisions and all 1,412 target bytes. The final old-index priority falls from 65,539 to 65,538 in this chained form, as its observed contribution changes from 4 to 3 before the storage bonus. Field 1 also
uses this chained form, retained from the prior best source. This is compiler
guided source recovery; no assembly, object bytes, compiler or verifier changed.

## Context fidelity and validation boundary

The historical full source contains an `if (0)` hint in the later unmatched
`f_26264`. Removing it preserves the target discrepancy but shifts `f_2631a`
six bytes and affects later references. The new production source excludes both
functions entirely. It contains six genuine preceding functions, the retained
board update, and genuine following allocator `f_25f6e` for its shared pool.

That eight-function context is 2,564 bytes and has all eight nominal roots. Only
the 1,412-byte board update is retained; 1,152 context bytes are excluded. All ten
external literal loads have original ROM targets and values. The source with
no following function fails matching because literal positions change; compiling
the target alone also fails. The following allocator has three context-only
byte differences at 25FC5, 25FDE, 25FDF. None is retained or used as a target literal.
The existing production source for other board ranges remains unchanged.

Two real compiler tests validate retained bytes, original roots, shared literals,
the sole export, and absence of retained relocations. A negative changes the
last chained assignment back to the separate copy; the default verifier rejects
the four-byte near-match at 25E48 without publishing a retained object or proof.

## Compiler evidence and limits

Case `session_board_priority81` contains the source controls, original compiler
objects, stage captures, debug replays, and hardware observations. For clean,
register-old, and exact chained sources, all native ELF sections except compiler
comment metadata reproduce the ordinary invocation. Six normal/debug middle-pass
replays and three hardware priority replays reproduce `.reg` and `.ilb` exactly.
All three register streams round-trip losslessly. The profit formula passes 549
records across 45 functions; 420 priority totals are reconstructed from observed
contributions, including 147 in the three board-update compilations.

The tracer now reads the current function symbol as well as the allocation-web
address. This prevents cross-function address reuse from corrupting joins. A
second limitation is recorded explicitly: five constructor totals per source
differ from the later diagnostic record at the same address. Allocation webs can
change between these stages, so the analysis records these differences rather
than asserting an unchanged identity. The relevant `k` and `old` bonus observations
agree; this work does not claim a complete reconstruction of all coalescence rules.

Other controls are negative: removing register storage from `k` gives 1,209 target
differences; an independently declared final preserved index gives 592; compound
increment/mask gives 290. Preserve these negatives instead of repeating them.

## Integration

Checkpoint 81 records the independent isolated and canonical clean builds, original
ROM/EPROM hashes, 232 enabled tests with zero skips, and 31 identical production
pins. The authoritative result is 126,886/200,698 bytes (63.22236%), 594/784 functions,
and 156/203 complete units. This adds 1,412 bytes to checkpoint 80. There are 73,812
bytes left, including all 1,638 runtime bytes. Literal 100% remains incomplete.
