# Source-directed accumulator tie ordering

Continuation107 turns the tied checksum-register diagnosis into a verified
source mechanism. It does not yet produce a whole-function match or C gain.
The active checkpoint remains104:127326/200698 bytes,63.44159%.

## Measured allocator model

The unmodified compiler appends13 variable and16 constant candidates before
sorting29 entries. The baseline high sum is created from a real use at IR
point192; the low sum follows at197. Their priority9/profit2 tie is ordered high
then low by4074E0's unstable partition sort. The allocation is highR7/lowR13;
the original ROM requires highR13/lowR7.

A read-only breakpoint at406584 observes the return of405750's caller-register
eligibility check. Eight baseline constants cannot use internal registers1–4;
their values span calls. This evidence is required to model the other candidates
correctly. Sort, observed eligibility, conflict graph and greedy allocation
reproduce all29 original assignments, including the spilled bank count.

A fixed-graph search tests198 same-category pair swaps and156 variable moves.
It finds17 swaps and33 moves that exchange only high/low assignments. These are
counterfactual order targets, not source matches. All other registers must stay
unchanged in this search. Compiler inputs and executables are never patched.

## Source realizes the exact predicted allocation change

The genuine loop expression

```c
lo += (hi += v >> 16, v);
```

still adds the high half first and the full word second. Both updates contribute
to actual stored checksums. The outer low-sum reference is encountered first
when candidates are appended, exchanging the sums' order. Hardware creation,
sort and eligibility traces prove the following for baseline, ordinary low-first
statements, and this nested form:

- Every graph edge, priority, profit, bound and eligibility value is identical.
- Only lowR13→R7 and highR7→R13 change; all other assignments remain identical.
-87 creation positions,87 sorted positions,87 eligibility records and87
  predicted allocations agree with the original compiler.

The ordinary low-first form changes operation order and shrinks the context by4.
The nested form preserves calculation order,1216-byte context and the accepted
274-byte suffix. Its correct-register variant uses the original final sum/store
order and has122 positioned differing bytes. This is the structural candidate
for continuing work, even though a different118-difference candidate retains
the wrong accumulator assignment. No displaced instruction comparison earns
matching credit.

## Thirteen source controls

| Control | First-function differences | Context bytes | Suffix exact |
|---|---:|---:|---|
| Previous best baseline |123|1216|Yes|
| Low-first statements |278|1212|No|
| Low-first comma expression |120|1216|Yes|
| High division by65536 |123|1216|Yes; baseline identical|
| High word cast |271|1220|No|
| Outer low sequence |124|1216|Yes|
| Outer high sequence |118|1216|Yes|
| Combined load/update sequence |123|1216|Yes; baseline identical|
| Correct registers and final sum |122|1216|Yes|
| Correct registers, wide bank counter |149|1216|Yes|
| Named shifted bank-byte value |286|1212|No|
| Named bank bytes, wide counter |288|1212|No|
| Named bank bytes, wide counter, named audio byte |406|1208|No|

The wide counter removes the unwanted extension, but changes branch/pool and
store scheduling. A named pre-shifted value fails to reproduce the ROM's
R1-to-R4 bank-count computation and shifts following roots. These specific
forms are closed; the remaining setup/scheduling problem needs direct evidence.

All13 native capture objects agree with ordinary compiler objects except comment
metadata.26 normal/debug middle replays preserve original.reg/.ilb. All979
logical-register score records across the three functions verify. Nine hardware
replays (creation, sort and eligibility for three sources) also preserve original
middle outputs. All target full-function proofs reject with the unchanged
verifier. Both remote checkouts retain35 production pins and original ROM/report.
Latest broad validation remains104's two clean builds and236 passing tests;
no production build was rerun for private rejects.

Next retain correct_register_context.c as the controlled first-function starting
point. Inspect the bank-count expression's allocation and SHCGEN/PEP scheduling,
including the extra R1→R4 transfer and bank-loop setup expected in the ROM.
Keep best_raw_context.c as a separate118-difference diagnostic. The second
function remains54 differences; its whole proof and the literal100% goal remain
incomplete. Evidence is frozen in session_checksum_ties107.
