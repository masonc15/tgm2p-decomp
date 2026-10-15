# Board-scroll parameter assignment — continuation186

Continuation185 made evidence progress by locating an input assignment before the
main SHCMDL allocator. This continuation authenticates its writer and eligibility,
and tests real source copies. It adds **zero matching C bytes**.

## Causal write and eligibility

Two fresh read-only hardware watchpoint replays observe the input candidate after
sorting (405ED1), at 405220 and 405EE0, and before general allocation (414D48).
The original instruction **40613F writes logical home 1**; the data watchpoint
stops at the following instruction 406142. Both baseline and consumed-input sources
start unassigned, receive exactly this write, and enter general allocation at 1.

Two further replays observe the eligibility decisions:

- Symbol 55 is a pointer parameter: symbol kind 7, node type 0x40.
- The call-interference list is empty and 405750 returns 0.
- 406170 returns incoming argument home 1.
- Configuration byte 37 is 0, selecting the integer/pointer path.
- Home 1 is within the four argument homes and no clash already owns it.

The read-only model predicts the observed assignment in both sources. The baseline
input clashes with 10 candidates; the consumed input clashes with only 1. No compiler
instruction, input node, configuration byte or ordinary compiler memory was changed.
Only hardware debug registers are written by the tracer.

## Correction to the earlier interpretation

Parameter preassignment does **not** itself prevent a source copy from freeing R4.
The consumed-input source shortens symbol55's native REG range from 1–11 to 1–2.
Its working board pointer, symbol 58, occupies 2–11 and gets **R12**. Native machine
code copies R4 to R12; the original ROM copies R4 to R10. Thus the new source already
performs the required kind of copy, but still has the wrong working allocation.

Two full graph replays reproduce all 24 baseline and 25 consumed-input candidates,
including profits, sorting and positive assignments. The consumed copy has real
priority 9, and its input has priority 1. The next investigation should focus on
working-value/constant promotion rather than trying to suppress a normal parameter
assignment or repeating simple aliases.

## Source controls

Two previous native controls are carried forward with authenticated checkpoint185 hashes.
Seven new normal compiles test copy placement inside real x/b/c array-address
calculations, consumed input plus the real named clamp limit, and equivalent
adjacent comparison bounds. None matches; no source or proof is integrated.

| Source | Compiled bytes | Nominal differing or missing bytes |
| --- | --- | --- |
| baseline | 792 | 534 |
| input_consumed | 804 | 643 |
| copy_at_x | 804 | 638 |
| copy_at_b | 804 | 637 |
| copy_at_c | 804 | 651 |
| copy_with_limit | 808 | 711 |
| inclusive_mask | 804 | 705 |
| exclusive_cap | 808 | 707 |
| adjacent_bounds | 818 | 722 |

For all 65,536 unsigned-short values, the verifier checks both equivalences:
`(pc & 0x3f00) < 0x3f00` versus `<= 0x3eff`, and `pc >= 0x3f00` versus
`pc > 0x3eff`. These alternatives were tested because distinct source constants
can produce the same machine comparison boundary while changing earlier promotion.
They remain strict negatives; semantic equivalence earns no matching-byte credit.

## Validation and next action

Six completed replays contain **199 events**, preserve REG/ILB against fresh
untraced controls and native snapshots, and authenticate captured/native ELF
sections except source-date comments. The initial tracer build failed on C
misleading-indentation warnings before any replay; formatting was corrected and
the failed build log is preserved separately. It is not counted as a trace.

`verify.py` independently resolves all original ELF relocations, validates source
hashes, original/linked roots, nominal differences and all nine strict rejections.
It verifies both parameter models, complete allocation graphs, boundary equivalence,
and all 52 current production pins. Both remote reports and original ROM hashes
remain unchanged. No new full suite was run for excluded diagnostics; checkpoint183's
251-test result remains the last full-suite validation.

Current coverage: **130,980 / 200,698 bytes (65.2622348005%)**. **69,718 remain**,
including all 1,638 runtime assembly bytes. The literal 100% goal remains active.
The primary agent continues solo without brainstorming.

Next trace individual 0x3F00/0x3FF literal membership, weighted contributions and
literal-to-register rewrites in this board-scroll source. The ROM reloads some
mask occurrences while using resident registers for others. Continuation185's
baseline instead gives 0x3F00 priority 48. Compare baseline, consumed input and the
already compiled named-limit control using the read-only mechanisms in
`session_score_flag171/trace_constant.c` and `trace_promotion.c`. Identify which
real expression-node relationship accounts for selective promotion; do not patch
flags, force registers, or repeat the recorded copy/bound/source families.
