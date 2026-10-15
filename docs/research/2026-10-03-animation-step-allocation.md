# Animation step allocation — continuation130

Production stays at checkpoint122: **128,316 / 200,698 bytes (63.93487%)**.
Nine native source controls remain unmatched. The new result is a verified
allocator model and a narrower source target for the remaining register roles.

## Verified allocator behavior

The baseline is continuation129's genuine `count_short_brightness.c` unit.
Seven independent native captures reproduce their ordinary native objects,
excluding only the build comment. Fourteen ordinary/debug middle replays and
fourteen hardware sort/eligibility replays reproduce `.reg` and `.ilb` exactly.
Hardware breakpoints read compiler state without modifying its instructions.

Across these seven cases, the recovered partition sort, profit calculation and
positive register allocation reproduce **611 preallocation candidate records**.
Deleted candidates have no eligibility query. Records appended for stack
handling after allocation are identified separately and excluded from the model.
This distinction avoids applying the ordinary profit formula to those records.

In the baseline, constants5,15, `total`, and constant2 occupy sort positions5–8.
Their profits all equal7. Constants5 and15 take internal physical registers9
and10 (R10 and R9); `total` takes11 (R8). Their conflict lists overlap constant2,
which receives no register. This explains the draft's allocation through
observed ordering, eligibility and conflicts, rather than priorities alone.

The original keeps constant2 in R8 (load A7EE), mask15 in R9 (load A7F8),
and the later hold counter in R10. It stores `total` on the stack. These are
partial role targets, not a full-function match criterion.

## Source controls

The real step takes only values1,2,5. Signed int, signed long, signed short and
unsigned short controls preserve these values and the unsigned-long loop index.
For all65,536 signed-short totals, all three step values and both arithmetic
directions, the final low16 bits equal the baseline's unsigned arithmetic.
Signed32-bit arithmetic never overflows in these checks.

| Form | Body bytes | Local frame | Result |
| --- | ---: | ---: | --- |
| Baseline unsigned-long step |1676|112|Unmatched |
| Signed int or long step |1676|112|Every native section equals baseline |
| Signed or unsigned short step |1712|112|Same selected allocations; added conversions |
| Full conditional step expression |1664|108|Removes constant5 and halves constant2 priority |
| Ordinary total assignments |1676|112|Raises total priority15→19 |
| Full expression plus ordinary totals |1664|108|Combines those effects |
| Conditional expression for final step5 choice only |1676|112|Removes constant5, preserves constant2 priority16 |

All nine functions begin at A604, four bytes before original A608. The original
is1748 bytes with a108-byte local frame. Equal frame size alone establishes no
matching code. All candidates fail the unchanged strict verifier.

The full expression is the direct conditional equivalent of the existing
if/else ladder. The narrower final choice is:

```c
if (hold / 240 == 0)
    step = 1;
else if (hold / 360 == 0)
    step = 2;
else
    step = hold / 480 == 0 ? 5 : 5;
```

The final comparison already exists in the baseline and original instructions;
it was not added as an allocation hint. A control-flow audit follows reachable
instructions and resolves six `__divls` call sites in the original and each
of the nine candidates. These expression controls retain all six native calls.

The full expression suppresses the constant2 uses in the two step selections:
only its input-mask uses remain allocation candidates, lowering priority16→8.
The final-choice-only form retains all those constant2 uses and removes
constant5 from the candidate list. Constant2 then takes R10, while mask15 stays
in R9 and `total` stays in R8. The long-lived hold value still spills.

## What to investigate next

A bounded diagnostic model on the final-choice-only graph tests320 combinations
of total/hold weights. It finds30 combinations reproducing the four observed
role targets. The nearest changes only `total`'s priority **15→12**, keeping
hold at15. It predicts holdR10, mask15R9, constant2R8 and a spilled total.

This is a fixed-graph counterfactual, not a proposed compiler modification or
a promise about source output. The ordinary-assignment control moved total's
priority in the wrong direction, and the full conditional expression changed
other priorities and the conflict graph. Both are closed leads in these forms.

Next measure the actual per-use contributions to `total`'s priority in
`tail_selection.c`, then look for an ordinary source representation that
preserves its real updates, wrap tests and display use. Verify the graph again
after any source change. Do not force registers, fabricate uses, remove real
divisions, or change the verifier to realize the model's target.

Evidence is in `tools/shc_probe/cases/session_animation_step130/`, including
`ALLOCATION_ANALYSIS.json`, `TAIL_COUNTERFACTUALS.json`, `CALL_AUDIT.json`, native
objects and linked bodies. Both Nuada checkouts retain all44 production pins,
original ROM/report hashes and fifteen prior frozen cases. No production source,
split, target or denominator changed. The full goal remains active.
