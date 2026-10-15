# Gradient alias binding — continuation213

Production remains checkpoint212: **134,470 / 200,698 bytes (67.00117%)**.
No C bytes were added. The preceding turn made verified progress by integrating
the complete 1,186-byte palette updater. This investigation follows the
initializer's successful alias/coalescing mechanism into gradient 29F94–2A08C.

## A new register-allocation route

The freshly compiled baseline still has six mismatched bytes in its 248-byte
body. A real divisor alias initialized before the loop, then consumed by all
three divisions, resolves that original accumulator/delta tie. It introduces
one inactive category-3 entry while preserving the divisor's priority65542.
The red accumulator, green accumulator and blue delta receive the desired homes.
The resulting native function differs in nine bytes, all R8/R9 component-register
operands. Its 924-byte prefix, 72-byte tail, actual function roots and total1244
context bytes remain unchanged and exact. This is an alternate near-match,
not an improvement in the authoritative percentage or an exact source.

The baseline ties ar/ag/db at priority65538 and profit41390. With the divisor
alias, the logical allocation IDs become ar8/ag7/db2 instead of ar2/ag7/db8.
These IDs are compiler allocation indices, not literal SH register numbers.
The new discrepancy is the physical R8/R9 assignment for red/green base colors.
Their observed priorities are both4. Read-only weighted-use observations give
three contributions for each:2,0,2. The first is assignment opcode0x5F with
common-node count1 plus assignment bonus1; the outside read is already counted
through that common node; the loop read has count1 and loop multiplier2.

An offline sensitivity model predicts that reducing red's priority by1, or
raising green's by1 or2, would recover all non-spill homes on this fixed graph.
These are diagnostic tables only, never supplied to the compiler. They do not
prove a source solution or stack-layout equivalence.

## What the native controls rule out

49 full native compiles (baseline plus48 new controls) cover consumed scalar,
address and accumulator aliases; standard register hints on aliases; assignment
scope/timing; component calculation and declaration orders; delta calculation
orders; direct color-word extraction; and widened shift-based extraction.
Every candidate fails the unchanged strict verifier; no kept object exists.
All49 retain the exact preceding924 bytes. Changed roots or tails remain excluded.

Inside-loop blue-delta aliases bind the live range to a group that forbids the
low allocation registers needed by the ROM, and raise priority to65540. Moving
that alias outside restores priority65538 but retains the binding restriction.
The real divisor alias avoids that obstruction because its desired home is
already in the high-register set. Moving its assignment among nine valid
pre-loop starts yields18,15 or9 differences; none is exact.

Component-order models predicted three successful array permutations, but none
of the tested C calculation or declaration permutations realizes that semantic
candidate order. Origin traces show red/green/blue candidate origins in shift
order24/16/8 in all three traced controls. Reordering calculations changes their
source lines to201/200/202; reordering declarations swaps symbol IDs115/114;
neither changes the semantic candidate order. Do not mistake symbol renaming or
IR statement order for actual candidate-array insertion order.

Accumulator-result aliases disappear before allocation (the red result control
has the same22-candidate graph and six native differences). Other aliases create
23-candidate graphs but alter binding or interference. One hypothetical inactive
entry at any of23 positions and six priorities gives no complete-home prediction
on the baseline graph. The successful initializer mechanism therefore cannot be
ported as a context-free insertion trick.

## Validation and next action

`session_gradient_alias213` contains11 complete allocator models, three IR-origin
traces, one weighted-use trace and11 independently assembled captures. All15
replays (1,188 events) reproduce both fresh untraced controls and native REG/ILB
captures exactly. The independent verifier resolves all49 original objects and
checks actual roots and surrounding bytes. All three model/analyzer/verifier
exit codes are checked in VALIDATION.json.

The copied assembly harness initially referenced old case names. Its failed
assembly.log is retained; the corrected harness enumerates this case's captures,
and assembly_final.log plus independent object checks cover all11 outputs.
No production source, split, compiler, verifier, denominator or runtime rule
changed. Checkpoint212's two clean builds and256-test suites remain the last
production gates; they were not rerun for these rejected diagnostic sources.
All56 production pins and both original/rebuilt ROM hashes were rechecked.

Next directly observe SHCMDL's candidate-construction traversal (0x415920) for
outside_divisor versus component_order_102. The origin snapshots prove that
statement/declaration permutation alone does not move the semantic entries;
identify the traversal or binding event that fixes their order before another
source search. Alternatively pursue a genuine component use/lifetime change
that realizes the measured one-point priority difference, with full native
validation. Do not repeat the closed alias, timing, order or extraction controls.
The full literal100% goal remains active, solo and without brainstorming.
