# Save-reader consumed-operation closure

Production remains checkpoint 249: 135,910 / 200,698 verified bytes.

The final bounded follow-up tested a consumed third-loop increment, reuse of
the first buffer expression, and their combination against the 16-byte near-match.
Reader differences were 156, 74, and 169 respectively; context lengths were
668, 668, and 676 bytes. All fail the unchanged admission proof. The fused
increment and combined controls also invalidate the retained setter.

The fused increment lowers one counter priority from 7 to 5, as the offline
model suggested, but changes other live ranges and generated instructions.
This demonstrates why a priority-only counterfactual is insufficient evidence
for a source-level match. No compiler or intermediate representation was changed.

All four Mac objects reproduce on Nuada with identical resolved bytes, roots,
and relocations. Eight unchanged middle-stage replays reproduce driver REG/ILB.
The verifier records strict rejection reasons and actual allocator records.

Park the save-reader source family. Do not expand these spelling, helper, type,
or consumed-expression matrices. Next work should target a different genuine
unmatched function. No production source or verification gate changed; the full
production suite was not rerun for these negative research controls.
