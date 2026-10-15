# Announcement: the scheduler queue explains the delayed global load

Continuation 220 makes compiler-research progress, with no new verified C bytes.
The previous decompilation continuation 219 made progress by closing operand-view
hypotheses. The separate homepage request is complete. Production remains
135,502 / 200,698 bytes (67.51537135397463%); 65,196 bytes remain, including all
1,638 runtime bytes. No production source, compiler, proof or denominator changed.

## Native evidence

The complete 816-byte f7C08 still differs at 24 bytes. The baseline source and
context hashes equal checkpoint 219. Native SHC 5.0 revisions 10, 31 and 32,
using identical ordinary flags and source, produce byte-identical 816-byte
contexts. All three fail strict acceptance at 0x7c18. Revision switching therefore
does not address this particular discrepancy.

A read-only SHCGEN trace records 318 events. It emits R4-to-R14 first, then the
global-address load to R11, before the ID load. Final prologue scheduling occurs
in SHCASM, rather than GEN or PEP. The SHCASM entry expansion at 0x401db0 creates
descending register saves before body operations. Its ordinary general-register
save loop is at the decompiled LAB_00402225 branch. The stream reader 0x414990
expands opcode 0x20 through that routine; 0x41bfc0 collects the scheduling buffer.
These function identities are static reverse-engineering evidence; no claim is
made that all branches in the entry routine have been dynamically exercised.

A native SHCASM trace records 345 instructions in input, dependency and selected
phases (1,035 events). Every permutation, instruction identity and all 650 graph
edges verify. The first buffer places the global load at node 11, after register
saves, frame adjustment and the parameter move. Its only incoming dependency is
the R11 save, node 4. The original ROM's earlier global load would obey every
recorded dependency. The PC-relative word changes from DB27 at 7C20 to DB29 at
7C18, addressing the same authenticated pool word 06064880.

## Why a legal move is not selected

A second read-only SHCASM replay records 115 events: 21 input rows and 94 queue
transitions, including 52 eligibility decisions. Hardware breakpoints observe:

- 0x41cb7c: the eligibility predicate's return value;
- 0x41cf20: insertion into the ready queue;
- 0x41be7f: selection from that queue;
- 0x41c2d0: the untouched initial instruction buffer.

The independent Python model reproduces all 94 transitions, including cursor,
hazard masks, pending predecessor counts and queue endpoints. It also reproduces
every observed eligibility result. This model is scoped to the first buffer's
instruction classes; it is not a claimed complete SHCASM implementation.

The queue scans ready nodes in increasing input order. After the immediate
10-to-R10 instruction (node 15) is enqueued at cursor 14, the next scan starts
from the beginning. The R9 save (node 6) takes the memory slot at cursor 16.
The global load (node 11) is then considered at cursor 18 and rejected by the
memory/nonmemory pairing predicate. Subsequent saves consume further slots;
the global is eventually admitted at cursor 24, after the stack adjustment.
This is a queue-order effect, not an extra dependency on the stack or R9.

As a model-only counterfactual, scanning the global before the R9 save produces
exactly the ROM's first-buffer order, with the same dependency graph. No compiler,
intermediate, object or ROM was changed to produce this counterfactual. It earns
no matching credit and is not a proposed compiler patch.

## Validation and next work

All three read-only replays preserve fresh untraced output. Both SHCASM replays
also preserve the independently captured native assembly. GEN SUA/ASA equal
fresh controls; GEN ASA differs from a later cleanup snapshot because later
stages update it, as recorded explicitly in EMIT_REPLAY_RESULTS.json. One native
assembly capture was independently assembled and resolved to the ordinary
816-byte compiled context, including 15 relocations.

The initial copied GEN replay helper failed because a textual adaptation changed
Path.parent to Path.emit. Its original failure log is retained; the corrected
helper and both resumed traces pass. No running job was restarted on a polling
timeout. Three independent analysis/verifier commands exit zero.

The evidence rules out more scalar/array casts and these three compiler revisions
as explanations for the prologue. Next inspect legitimate entry/stream formation
at 0x401db0/0x414990, or address the separate call/index/remainder scratch choices
without treating their global-load side effect as assumed. To reproduce the ROM
with the same graph, something must alter the ready-list input order or scheduler
state; merely showing that the load is dependency-safe is insufficient.

Artifacts: tools/shc_probe/cases/session_announcer_schedule220. The prior complete
production gates remain checkpoint 218: 263 tests per checkout. They were not
rerun for this research-only continuation. Production pins and both ROM images
are independently rechecked when checkpoint 220 is synchronized. The full
literal 100% goal remains active; work continues solo, without brainstorming.
