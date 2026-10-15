# SHC object-list allocation: 251CC

Continuation 100 applies the observed allocator model to the remaining 82-byte
object-list routine. It produces the required register allocation in a private
source candidate and matches its first 60 bytes exactly. Its final decrement is
two bytes shorter than the ROM, so the routine remains rejected and uncredited.
Production remains checkpoint 84: 127,276 / 200,698 bytes (63.41668%).

## Refreshed baseline

The canonical `src/objfile_248a8.c` was copied and freshly compiled with the
production SHC and flags. All nine context roots match their original addresses.
The retained 2340-byte prefix and 110-byte following range pass independently.
The gap has 19 differing bytes, reproducing the archived three-register cycle:

| Value | Baseline register | ROM register | Priority | Score |
| --- | --- | --- | ---: | ---: |
| k, output index | R7 | R4 | 5 | 0 |
| s, linked-list node | R4 | R5 | 16 | 12 |
| Hardware base 24003800 | R5 | R7 | 7 | 2 |
| i, outer counter | R6 | R6 | 5 | 0 |

Eight candidates enter the general allocator without preassignments. The
recovered register-major model reproduces all eight allocations, and the
partition-sort model reproduces the observed order. No combination of ordinary
register hints on the four existing locals fixes this unchanged graph.

## Measured source priorities

Read-only hardware observations reconstruct variable and constant contributions.
For k, the initial assignment contributes1, the inner use3, the increment0,
and the final use1. Ordinary explicit index update raises its inner contributions
to6+3, giving priority11. Ordinary outer-counter assignment raises i from5 to7.

The node s has two inner uses grouped at weight6. Reusing the value of the actual
hardware store as the next-list subscript removes one of those references:

```c
for (s = *l++; s;) {
    s = g_60611ec[((short *)0x24003800)[k] = s];
    k = k + 1;
}
```

The assignment stores s and supplies that same short value to the table lookup;
it does not reread hardware or add a dummy consumer. The resulting s priority
falls16 to13. A separate final `k = k - 1` raises k from11 to13. Together with
`i = i + 1`, these are the priorities predicted by the fixed-graph model to
produce the desired allocation. Actual compilation confirms that allocation.

## Remaining discrepancy

`consumed_store_final.c` matches original bytes251CC–25208 exactly: saves,
initial stores, both loops, branches, and their register operands. At25208 the
literal displacement differs because the subsequent function and pool move two
bytes earlier. The substantive final-index difference is:

```text
ROM:        mov r4,r0; add #-1,r0; exts.w r0,r0
Candidate:             add #-1,r4; exts.w r4,r0
```

The candidate updates k in place and has80 bytes instead of82. The following
function begins2521C instead of2521E. The raw oracle rejects it with21 positioned
byte differences; the following retained range also rejects. The 60-byte prefix
is diagnostic evidence, not accepted partial-function coverage.

Consuming `k = k - 1` directly as the final subscript restores the82-byte shape,
but SHC removes the extra k assignment from its allocation representation: k's
priority returns to11 and the register cycle reappears. Named short/long final
indices remain separate candidates and also fail. Expanding the final load/store
introduces a synthetic address candidate. A postdecrement spelling, three genuine
initial-store counter forms, and a row-scoped running index all fail. The
row-scoped form keeps k and row_k as distinct live values rather than producing
the model's hypothetical merged priority19 candidate.

## Controls

All differences below are at original nominal positions; moved boundaries are
failures regardless of the counts.

| Control | Gap differences | Prefix retained | Following range retained |
| --- | ---: | --- | --- |
| Baseline | 19 | yes | yes |
| Explicit k update | 22 | yes | yes |
| Explicit i update | 29 | yes | yes |
| Both updates; equivalent if/do control | 25 each | yes | yes |
| Both updates and explicit final decrement | 44 | yes | no |
| Consumed hardware-store result | 25 | yes | yes |
| Consumed store and explicit final decrement | 21 | yes | no |
| Final assignment consumed as subscript | 25 | yes | yes |
| Expanded final load/store | 37 | yes | yes |
| Named short/long final index | 25 each | yes | yes |
| Postdecrement final index | 41 | yes | no |
| Initial stores with postincrements | 75 | no | no |
| Initial stores with ordinary assignments | 25 | yes | yes |
| Initial stores with preincrements | 76 | no | no |
| Row-scoped counter | 60 | no | no |

Full-context equality is verified for explicit-both/if-do, consumed-store/
consumed-final-assignment/ordinary-initialization, and named-short/named-long.
These exact forms are closed; do not resweep them without new compiler evidence.

## Validation and next step

The case contains17 native object comparisons,34 exact normal/debug middle-pass
replays,140 score checks,18 hardware priority replays reconstructing36 constant
and37 variable totals from99 and149 contributions, plus two hardware sort replays
verifying16 positions. Every instrumented replay preserves original .reg/.ilb.
Both nuada checkouts still verify all33 production pins, ROM hashes, and report
metrics. The latest full build/test gates remain checkpoint84's234 enabled tests
and independent clean builds; private negative controls did not trigger a rerun.

Evidence is frozen in `tools/shc_probe/cases/session_obj_allocator100`, including
`ANALYSIS.json`, `PRIORITY_ANALYSIS.json`, `MODEL.json`, and `ALL_RESULTS.json`.
The best allocation/loop source is `consumed_store_final.c`; the contrasting
82-byte source is `final_assignment_index.c`.

Next compare those two compiler intermediate trees to locate the stage that
removes the consumed k assignment, and trace final-decrement lowering. Preserve
the current allocation and genuine data flow; neither padding nor a dead use is
an acceptable way to recover the missing instruction. The full100% goal remains
active and incomplete. Continue solo without brainstorming.
