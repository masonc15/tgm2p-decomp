# Score guard literal promotion — continuation171

Production remains checkpoint161: **128,398 / 200,698 bytes (63.9757247%)**.
The best score candidate remains continuation170's positive-label form, with
13 differing target bytes and an unchanged 1,456-byte context. This continuation
explains its global-test mismatch across two original compiler stages. It adds
no verified C bytes. Continue solo toward literal 100% with the fixed denominator.

## What the backend flag actually records

Hardware execution breakpoints observe SHCGEN at function entry4269A0, the
literal check413E00, immediately after the flag write413E20, and the temporary
selector call4239F4. The tracer records the AND node, both operands, the second
operand's child, and their metadata. It changes only hardware debug registers.

Across baseline, positive labels, and the combined-entry label control:

| Source | AND-family inputs | Flag writes | Linked temporary nodes |
|---|---:|---:|---:|
| baseline | 11 | 7 | 11 |
| positive labels | 11 | 6 | 11 |
| combined-entry label | 11 | 7 | 11 |

The original dispatch takes AND opcode4C to413E00. Bit20hex is set exactly when
either direct operand is literal opcode71. All20 observed writes change only
metadata byte7 by OR20; all33 later temporary snapshots retain the predicted bit.
This is a model of the observed integer AND-family path, not every use of bit20
elsewhere in SHCGEN.

For the first global guard, baseline and the combined-entry label contain an
immediate unsigned-long1 operand. The positive-label form instead contains a
conversion opcode20/type1C around identifier opcode70/type10, symbolFFF7 (-9).
The flag is therefore already determined by the incoming expression tree.
Changing backend cache flags would not explain this source-level difference.

All three fresh codegen replays reproduce their untraced SUA and ASA outputs.
SUA also matches the original native capture. The known cleanup-snapshot ASA
differences remain recorded explicitly. All three captured native ELF objects
match normal compilation in every section except compiler comment metadata.

## Earlier literal membership and priority

Three unchanged SHCMDL replays observe original constant-priority construction.
The independent verifier reconstructs all18 completed priority totals from
their factor/use products and running sums. The constant1 priority is6 for
baseline and combined-entry labels, and7 for positive labels.

The extra contribution is tied to the actual global guard: its literal node
has type1C, value1 and generated line0; its parent is AND4C at source line96,
whose parent is branch opcode12. Full parent chains terminate at f7658's
function node. The other generated line0 use belongs to the later mode-bit
test and occurs in all three sources. Thus the extra contribution is not
inferred merely from the final priority difference.

## Original literal-to-register rewrite

A fourth middle-stage replay arms data watchpoints after identifying that exact
guard literal in the priority pass. It observes the literal's header, child
pointer and sibling-list link. Only debug registers are written by the tracer;
the observed node changes are made by the original compiler.

1. Instruction407402 changes opcode71 to identifier70.
2. The original rewrite installs symbolFFF7. Instruction4074D0 changes its type
   from1C to10 while constructing a type1C conversion wrapper.
3. Instruction40A9A3 relinks the AND's operand to the new opcode20 wrapper.
4. A later40A48A write belongs to the node free-list routine and is retained as
   cleanup evidence, not counted as another semantic conversion.

The node's word30 records logical register4; the saved REG table maps logical4
to physicalR12. Word58 is25 and is a different field. The codegen trace then
observes precisely the conversion/FFF7 tree described above. The native bytes
contain `tst r12,r0` where the original ROM has `tst #1,r0`.

All four middle-stage replays preserve REG and ILB byte for byte against their
native captures. Together with the three codegen replays, seven observed runs
leave compiler output unchanged. No compiler or configuration patch is used.

## Native source controls

Every control uses the production compiler and flags. All retain both nominal
roots7658/7956, context size1456, and the following function's exact baseline
bytes. The strict verifier rejects every candidate.

| Source | Target byte differences |
|---|---:|
| baseline | 19 |
| positive labels | 13 |
| combined-entry label | 24 |
| positive labels with1U,1L,1UL | 13 each |
| positive labels with unsigned-int or signed-long flag cast | 13 each |
| positive labels with explicit `!= 0` | 13 |
| positive mode label, negative global exit | 19 |
| negative mode exit, positive global label | 14 |
| positive mode label, nested global condition | 17 |
| nested mode condition, positive global label | 29 |

The six literal/type/comparison controls have whole contexts identical to the
positive-label candidate. No new game reads, dummy consumers, forced physical
registers, volatile accesses, dead stores or padding were added. Evaluation
order and existing effects are preserved. Preparation verifies unchanged f7956.

## Next investigation

Trace how the literal becomes eligible for the constant-member list before
4159E0. The original allocation driver calls the node traversal414E10 at414D07;
recursive worker414E90 calls candidate processing4155C0 at41523D. Observe the
guard literal and its parent chain in the same three controls, especially the
compound logical condition versus direct branch. These are static entry-point
leads; this continuation does not yet prove which eligibility branch differs.

The desired source must keep the immediate global test while preserving the
positive-label candidate's already exact7788–77A6 threshold/level/cap sequence.
Nine earlier constant2/FFFF allocation bytes remain independently unresolved.
Do not repeat the six now-identical literal/cast/comparison controls or edit
the compiler's metadata to manufacture a match.

Evidence is in `tools/shc_probe/cases/session_score_flag171/`, including the
tracers, native objects, unchanged-output controls, original disassembly,
compiler hashes, `RESULTS.json`, and rerunnable verification. Production checks
retain all46 pins,57 earlier frozen cases, original ROM hashes and the fixed
report in both Nuada checkouts. All23 reference ROM files still match the
recorded MacBook source. Full build/test gates remain checkpoint161's clean
builds and245 enabled tests; this research-only continuation adds no new suite
claim and earns no C credit.
