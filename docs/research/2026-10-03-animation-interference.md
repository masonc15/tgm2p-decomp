# Animation argument/address interference — continuation153

Production remains checkpoint122: **128,316 /200,698 bytes (63.93487%)**.
Continuation152 was progress because its same-run argument joins isolated the
binding difference. This continuation explains the difference with actual
interference graphs. No production source changes or new matching-byte credit.

Case: `tools/shc_probe/cases/session_animation_interference153/`.
Inputs are frozen152 `id_int` and `array_int` captures.

## One compiler process per complete graph

`trace_graph.c` observes pre-sort405ECC, post-sort405ED1, pre-allocation414D48
and post-allocation414D4D. At pre-allocation it temporarily repurposes already
consumed hardware breakpoint slots to observe the eligibility gate406584,
then restores them after allocation. Only hardware debug registers are written.
All tables and gate decisions for a given candidate come from the same process;
heap addresses are never joined across replays.

Both unmodified SHCMDL runs reproduce the captured REG and ILB byte for byte.
The model reconstructs profits, partition-sort order and all original candidate
assignments:89 candidates/303 eligibility events for id_int,88/305 for array_int.
Each allocation appends three addressed-memory records with negative locations,
zero priority/profit and no clashes. The initial model incorrectly expected no
appended records; the final verification checks them separately and requires
all177 original candidates to agree with observed allocation.

## The two graphs have different constraints

In `id_int`, the ID argument, scaled offset and sprite-pointer records do not
interfere with one another. All three currently share internal1 (hardwareR4).
They each interfere with both brightness arguments. Offset and sprite pointer
have priority6/profit-3; the three argument records have priority4/profit-5.
The current helper assignments are bri1R6, bri0R5, ID/offset/addressR4.
The original target bri1R4, bri0R5, ID/offset/addressR6 is graph-compatible.

In `array_int`, the ID, generated common address and both brightness arguments
all interfere pairwise: a four-vertex clique. ID and common address cannot both
occupyR6 in this graph, regardless of sort priority. The observed assignment is
addressR4, bri0R5, IDR6 and bri1R7. This is a structural obstacle, not merely
an unfortunate tie order. The graph must change before this form can reproduce
the original three-register value/address layout.

## Offline sensitivity, not compiler modification

The model changes copied Python graph values only. It never changes source,
compiler memory, executable bytes or intermediate inputs. Desired assignments
include the six already recovered loop/setup roles as well as all helper values.

- 90 single-record trials span helper priority deltas-4 through+6, excluding0.
  None meets the full target assignment.
- For graph-compatible id_int,250 paired trials use deltas-4,-2,+2,+4,+6.
  Exactly two tested combinations meet the complete target: lowering both
  offset and address priorities6->2, or raising both brightness priorities4->10.
- No paired priority sweep is run on array_int: its explicit interference edge
  already contradicts sharingR6.

These are bounded sensitivity results, not a proof about every numeric priority
or source program. The successful numbers are not instructions to insert dummy
uses, fabricated padding, forced registers or compiler patches. No genuine C
rewrite producing those changes has been established. They show that id_int
preserves a usable graph while array_int does not.

## Verification and next action

`verify.py` checks capture identity, source hashes, gate-event placement between
the same-process allocation snapshots, all177 model assignments,608 gate events,
six appended memory records and the bounded sensitivity results. Both checkouts
retain44 production pins,38 prior frozen cases, original ROM hashes and all23
reference files. Last complete production gates remain checkpoint122's independent
clean builds and243 tests. No fresh native-source compile is claimed this turn.

Retain152 id_int as the viable interference control. Retire priority-only tuning
of array_int: its graph cannot fit the target. The next source question is how to
represent the consumed address calculation without introducing the current two
high-priority temporaries, while retaining ID/address noninterference. Investigate
that expression/binding boundary before another parameter-order or qualifier
sweep. The spr1 home36 versus16 and A604 versusA608 boundary remain unresolved;
this register model does not repair them or establish a native match.
