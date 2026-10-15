# Rank allocation order observed; seven source controls rejected

Checkpoint84 remains **127,276 / 200,698 bytes (63.41668%)**. No source from
this investigation is integrated. The previous continuation established a clean
rank-renderer baseline; this continuation observes its original allocator and
narrows the remaining source mechanisms.

## Original compiler observations

Seven hardware-breakpoint replays of unmodified shcmdl.exe stop before and after
profit sorting (405ECC/405ED1) and physical allocation (414D48/414D4D), filtered
to actual function symbol151. No compiler instruction, data or configuration is
patched. Every replay reproduces the captured .reg and .ilb exactly.

The recovered partition-sort model predicts all **224 candidate positions**
across seven 32-entry tables. All224 live priority-minus-average-conflict scores
also agree. Hardware records are joined to diagnostic records only after their
address, priority, profit, conflict list and final physical assignment agree.
The two pointer symbols are unambiguous; this does not identify the earlier WEB
construction algorithm or disambiguate all repeated loop-counter lifetimes.

The clean baseline places q at input slot6 and p at slot7. Both score priority13,
profit-10. Sorting puts q at slot19 and p at22; they receive R12 and R10, opposite
the original roles. The prior ordinary-assignment control moves p to priority19,
profit-4, sort slot11 and R13. This explains its excessive priority change.

## Bounded source controls

| Control | p priority / register | q priority / register | Aligned diagnostic differences |
|---|---|---|---:|
| Clean baseline (90) | 13 / R10 | 13 / R12 | 32 |
| Reverse pointer declarations | 13 / R10 | 13 / R12 | 32 |
| Reverse independent pointer increments | 13 / R10 | 13 / R12 | 32 |
| Existing p increment as p += 1 | 13 / R10 | 13 / R12 | 32 |
| Both pointer increments in existing call | 7 / R8 | 7 / R10 | 48 |
| Only q increment in existing call | 13 / R12 | 7 / R8 | 67 |

Reversing declarations and increments leaves their input-table and sorted order
unchanged. Declaration order changes literal order; increment order changes the
emitted independent additions. Equal difference counts do not mean equal bytes.
Only the compound-increment form is byte-identical to the entire clean native
context. These particular order/spelling controls are closed.

Moving the existing local pointer increments into call arguments still passes
the same old pointer values and advances each exactly once per iteration.
The nonescaping local pointers are not read by the callee. Each reaches the same
one-past end after the third iteration. These are real consumed operations, with
no added reads, dummy consumers or padding. They lower measured pointer priority
13 to7 but alter lifetime/scheduling enough to worsen the complete match. The
q-only form gets p into R12 but pushes q into R8. Neither is acceptable.

All five functions retain their actual four-byte-early roots12AD0–12FA4 and
1,236-byte extent. Aligned-by-start comparisons are diagnostics only: all default
positioned-range proofs reject them. Five native captures equal original compile
sections outside comments; ten normal/debug middle-stage replays are exact.

## Prefix deficit is also an allocation problem

The saved instruction windows show that original f12608 copies x/R11 into R8
at126AE and adds2 in the branch delay slot126CE. The clean build computes x+2
into R0 at126BA–126BC and spills it. It instead retains constant110 in R10;
the original materializes110 at individual call sites in this region. This is
more than a four-byte padding deficit: allocations and scheduling differ.

Two genuine controls name the nine consumed x+2 expressions in a local computed
before the switch. A long local preserves the promoted expression. A short local
performs the same signed-word conversion already required at every corresponding
short parameter; SHC's target conversion behavior is the same at those calls.
Both preserve the real input loads and external calls.

The long form shrinks f12608 to1,084 bytes and moves rank to12AC0. The short form
keeps1,100 bytes and rank12AD0 but changes56 native context bytes relative to the
clean baseline. Both preserve the same1,236-byte rank body with32 aligned
diagnostic differences. Neither restores the original1,104-byte predecessor.
All seven fresh source controls fail default verification and add zero coverage.

An initial source-preparation assertion counted eight x+2 uses instead of nine;
it stopped before producing inputs. An initial analysis assumption that the
short form was byte-identical was disproved and corrected to exact target-body
equality. The final analyzer verifies the actual context hashes and regenerated
sources; neither failed assertion is counted as a successful validation.

## Next action and retained evidence

Inspect f12608's allocation diagnostics for the synthetic x+2 value and constant
110, then use the already recovered priority/score machinery to select a narrow
source control. Do not restore historical dead statements, pad the predecessor,
credit shifted comparisons or repeat the seven rejected controls. The rank
renderer still also needs two allocation windows repaired.

Evidence: tools/shc_probe/cases/session_rank_sort91. prepare.py reproduces all
seven fresh inputs. analyze.py verifies source hashes, roots, rejected default
proofs, native captures, normal/debug output equality, full sort ordering, scores
and assignments. PREFIX_LAYOUT.json records the original/clean instructions.
Per-source outputs are authoritative; capture/middle-replay summary files cover
their last batch. HARDWARE_RESULTS.json covers all seven hardware replays.

Both production checkouts freshly retain all33 checkpoint84 pins, original ROM
hashes and unchanged reports. The234-test full-suite pass is from checkpoint84,
not newly run for these rejected private experiments. The100% fixed-denominator
goal remains active, with73,422 bytes remaining. Continue solo, no brainstorming.
