# Board scroll: constant substitution through conversions

Continuation191 resumes compiler investigation after the ROM recheck, which added
no new decompilation evidence. Production remains **130,980 / 200,698 bytes
(65.2622348005%)**, with 69,718 incomplete bytes, including all 1,638 runtime
assembly bytes. This investigation adds no matching C bytes. The goal remains
active; work is performed by the primary agent alone.

## Observed cause of the duplicate clamp limit

Read-only hardware-breakpoint traces of the original SHCMDL executable locate
the replacement inside pass40DF30, called at40F8EB. The store still contains a
conversion from variable72 at40F8EB; it contains a literal at40F8F0. Earlier
cleanup420400 does not cause this replacement.

Within40E0C0, the branch at40E3BF accepts an expression only when all its operands
are literal constants or resolve to constants through40E500. At40E3DA, helper
40E4C0 substitutes the defining constant into the operand. At40E41A,414460 folds
the conversion;40E41F returns the resulting literal. The trace authenticates the
actual lookup result, gate outcome, rewritten operand, and returned expression.
Mixed AND/comparison expressions encounter a live nonconstant operand and fail
the all-constant gate. This is observed compiler behavior, not a general claim
about every constant-propagation pass.

For `long limit`, the narrowing conversion in `*pc = limit` folds from type0x18
to0x0C, while wide arithmetic uses retain the variable. The prior allocator graph
therefore has a priority33 variable plus a priority17 literal for0x3F00.

For `unsigned short limit`, the store needs no conversion and retains the
variable. Four promotions at the AND/comparison/use sites fold from0x0C to0x10
instead. The fresh graph has a priority9 variable plus a priority33 literal.
Changing the declaration moves the duplication; it does not remove it.
The 100 final evaluator events prove five limit conversions and40 mixed-expression
gate rejections across the two controls.

## Source controls and allocator evidence

All source changes are ordinary C, compiled by the supported unmodified toolchain.
Every complete nominal800-byte function is checked against the original ROM.

| Control | Provenance | Compiled bytes | Differing target bytes | Result |
|---|---|---:|---:|---|
| named_masks_flag | carried from190 | 816 | 730 | rejected |
| named_flag | carried from190 | 812 | 687 | rejected |
| short_limit | new declaration test | 828 | 741 | rejected |
| late_flag | move flag definition after state update | 814 | 717 | rejected |
| late_flag_pack | also move high-word packing after update | 800 | 645 | rejected |

The two placement controls follow the original ROM's late flag-pointer load
at27C00 and high-word masking/packing at27BFE–27C0A. In `late_flag`, the flag
variable loses its earlier lifetime's favorable conflict average and spills;
3F00/1FF/3FF occupy the final three homes. Moving high-word packing as well
lets the two state-address constants win homes9/10, with3F00 in11; flag still
spills. Neither matches the original eight main variable homes: attr/counter
remain reversed. Instruction placement alone is insufficient evidence for a
matching source lifetime.

Three fresh graphs contain27,26,26 candidates. The profit, unstable sort, native
eligibility decisions, and all positive/unallocated register assignments are
reproduced by the model. In addition,768 hypothetical hint sets add an ordinary
`register` hint to the named flag while varying the eight existing hints, across
three graphs. Each graph's unchanged native allocation is separately reproduced.
None predicts all11 desired main/flag/mask homes; these are offline models,
not768 native compiles. No source was accepted based on a model.

## Verification and limitations

`tools/shc_probe/cases/session_scroll_limit_type191/verify.py` authenticates five
strict negatives (three new, two carried), source hashes, native ELF sections,
relocations, linked ranges,12 completed fresh replays/9,489 events, the conversion
analysis, three allocator models,52 unchanged production pins, the authoritative
report, and the original ROM SHA-256. Every completed replay regenerates the
frontend, runs a fresh untraced middle-stage control, and proves both REG and ILB
outputs equal that control and the native capture. No intermediate or instruction
is modified.

The first evaluator tracer attempt inspected the old expression after folding
had released it; the traversal guard terminated that diagnostic. Its logs are
retained in `failed_eval_old_node`. The tracer was corrected to inspect only the
returned expression at40E41F. Both fresh corrected replays pass output equality;
the failed attempt is not counted among the12 completed replays.

The last complete build/test gates remain checkpoint183's independent clean
builds and251 enabled tests. They were not rerun for excluded diagnostics.
No production source, compiler, verifier, split map, denominator or ROM target
changed. A fresh cross-check authenticates this case and production on both
Nuada checkouts.

## Next action

Do not repeat these named-limit type or late-definition controls. Inspect the
adjacent40E580 expression-extraction path, including its shared-reference count
and conversion handling at40E623–40E66A, for the early raw `hi` value and its
packed successor. That is a newly located possible source of the remaining
lifetime differences; it is not yet a proven cause. Observe it before choosing
another source expression. The original ROM requires raw hi inR12 before the
state update and its packed value on the stack afterward, while the mask and
flag lifetimes must still allow3F00/R10,1FF/R9,a6/R8.
