# SHC effect parameter binding — continuation117

Reusing the exhausted x parameter for height, with its type changed to int,
reduces the unmatched136-byte f_20a9c from26 to21 differing bytes. The complete
384-byte context retains all three nominal roots and its248-byte protected
prefix remains exact. This is a better research reference, not a completed
function: no source or split change was integrated and no C bytes were credited.
Production remains127988/200698 (63.77144%), with72710 bytes remaining.

## Compiler mechanism

Continuation116's named height takes R4. Reusing short x instead preserves R5,
but SHC creates an anonymous promoted int value in R4. Widening the parameter
to int removes that extra logical value. The incoming x and later height occupy
two distinct logical WEBs, both already assigned internal physical2/R5 before
the general allocator at406450 runs. GEN now forms the second height product in
R3, matching the original, but still uses R4 for the first product and sum.

Reusing x for the final coordinate does not inherit R5. It creates a third WEB,
which remains unbound before allocation and subsequently spills. GEN forms the
sum and subtraction in R0, but stores the result to a stack slot and reloads it
for the call. The context grows four bytes. Delaying minus6 until the call
argument preserves the same allocation failure. Two short-sy controls also
spill and make the context eight bytes too long.

Hardware observations before/after sorting and before/after general allocation
verify this order directly. This establishes the observed bindings and spills;
it does not claim a complete model of every eligibility rule. The inferred
405220 and405EE0 listings are supporting evidence. In particular,405EE0 checks
conflicting register assignments before binding an incoming-parameter WEB.

| Source control | Context bytes | Target differing bytes | Prefix differences |
|---|---:|---:|---:|
| production baseline |384|26|0|
| named short height |388|85|12|
| reuse short x for height |388|82|12|
| reuse int x for height |384|21|0|
| reuse int x, short sy |392|83|12|
| named int height, short sy |392|86|12|
| reuse int x for height and result |388|68|12|
| same, minus6 in call argument |388|69|12|

All controls fail default byte/boundary verification. The21-byte control remains
unlinked. Existing callers in item_1d1bc.c and item_1d640.c declare the coordinate
parameter as int and pass signed-short player values. Their multiplication by8
fits a32-bit int. Assigned height is an unsigned byte and fits both tested types.
The short-sy controls deliberately test narrower result storage: they can change
the full32-bit argument outside the short range. The accepted drawing path uses
short coordinates and masks them to10 bits, but these controls do not establish
full argument equivalence and were rejected by native matching in any event.
No positive integration rests on that narrowing assumption.

## Verification

Case `session_effect_binding117/analyze.py` checks eight normal/native object
comparisons,16 exact normal/debug middle replays,138 priority/profit calculations,
22 hardware replays,481 modeled scratch selections,338 target emitter events,
and38 final allocations. Twenty values are already bound before general
allocation across the six observed middle-stage controls. GEN traces equal
untraced controls and captured SUA. Cleanup-snapshot ASA metadata differences
are recorded; compiler instructions and data remain unchanged.

Read-only production checks pass all39 checkpoint115 pins, original ROM hashes,
and fixed-denominator reports in both nuada checkouts. No full build was needed
for these rejected experiments; the240-test full-suite result remains the
checkpoint115 validation. Frozen116 and115 evidence stays immutable.

## Next independent target

`field_next` refreshes the current source for f_81cc–f_8314, a328-byte gap. The
full844-byte context has four nominal roots and all516 following retained bytes
match ROM. Its28 differing bytes are confined to the next-row address sequences
at8260–827D and82B0–82CD. The first original sequence loads width into R2 and
base into R1; the candidate reverses those roles. Both original pointer additions
leave the address in R3, while the candidate leaves it in R2 or R1.

Historical `session_field/STATUS.md` already closes broad arithmetic order,
stride, staging, type and pointer-representation sweeps. Do not repeat them.
Next capture the four-function context and trace the two GEN address recipes,
including operand resolution and scratch history, to explain this specific
base-versus-offset destination choice. Retain the new21-difference effect
control for future distinct hypotheses; park the eight source forms above.
The optional original TGM2 ROM is still absent and is not a blocker.
