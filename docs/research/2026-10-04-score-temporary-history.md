# Score temporary registers and labeled guards — continuation170

A genuine control-flow rewrite improves the score candidate from19 to13
mismatched target bytes, with unchanged1456-byte context length and nominal
7658/7956 function roots. It is not yet a matching function and receives no C
credit. Production remains128398/200698 (63.9757247%).

## Improved candidate

`guard_label_positive.c` spells the first two guard conditions as positive jumps
to the next check, with failures going to the shared end of the bonus guard.
The original outer `if (n)`, short-circuit read order, bonus body and subsequent
`f_11cc6(p)` call are preserved. There are no added state reads, dummy consumers,
physical-register overrides or padding. `guard_label_mod2.c` expresses the
unsigned global low-bit test with `%2` and produces the identical whole context.

The30 bytes at7788–77A6 now match the ROM exactly: threshold addressR3, threshold
valueR2, player levelR1, byte testR1, and capR2. All bytes in the following
function also equal the baseline context. Remaining differences are:

- Nine earlier constant2/FFFF allocation and scheduling bytes at768E–7711.
- Four bytes at7780,7783,7784,7785: the global address usesR1 instead ofR2,
  and the bit test uses `tst r12,r0` instead of `tst #1,r0`.

The fresh native capture matches the normally compiled object in every ELF
section except `.comment`. Traced `.sua`/`.asa` equal saved untraced controls,
and `.sua` equals the captured native intermediate. Strict verification still
rejects the candidate at768E; no shifted or partial code receives credit.

## What the temporary trace proves

The new trace observes the scratch call at4239F4, its result at4239F9, and the
return of4234C0 to406513. It records both the original source operand and node
metadata, plus all four scratch-cache records. It modifies only hardware debug
registers. The selector model predicts all42 original observations across the
baseline, reduced guard and int-mask control.

For the36 direct-register operands, the complete state transition is verified:
node metadata word18 gains the selected-register bit, byte38 stores that
register, and the last-register global becomes that register. All other128-byte
node metadata, scratch cache, recent mask and tracked configuration remain
unchanged. The six memory operands can perform additional cache operations;
their full conversion paths are not modeled here. The low-level emitter counter
is unchanged within these calls; that does not establish absence of later code.

In particular, the baseline's two mode/global bit-test nodes each selectR1 and
retain the source operand and cache. The completed global-test snapshot exactly
matches the later threshold-result allocator input from169 in all tracked cache
and history fields. Equivalent links are checked for the reduced guard and the
int-mask control. This closes the unexplained origin of the last-register state.

## How the labeled candidate differs

A second trace verifies19 temporary selections/17 direct-register transitions in
the improved candidate. Its mode, global and later byte-test planning occurs at
low-level emitter counts124,130,134, rather than one124 batch. At global-test
planning, R1 holds the global-address cache entry; the other scratch slots are
empty. The temporary selector choosesR3 and records it as last-used.

Five independently traced address selections also match the selector model.
Before both the global-address and threshold-address requests, all four scratch
cache entries are empty. The global request excludesR0, has last0/recent12,
and choosesR1 because R2/R3 have recent-use penalties. The threshold request
excludes its resultR2, has last2/recent3, and choosesR3. Thus the improvement
comes with a measured change in planning boundaries/cache history, while the
remaining global-address choice is still explained by current history.

The global bit-test metadata byte7 is0x21 in the baseline and0x01 in the improved
candidate. That bit difference is an observation, not proof of why SHC chose a
register test. Original disassembly of potential writers/readers is saved for
the next experiment; notably413D00 and414774 can set bit0x20.

## Native source controls

All variants preserve the following function's body. The unsigned-int global
alternative changes its common declaration; generated code is nevertheless
identical. The inherited following `f_17834` prototype remains unresolved.

| Control | Target differences | Context bytes |
|---|---:|---:|
| `baseline` | 19 | 1456 |
| `mode_literal_u` | 19 | 1456 |
| `mode_literal_l` | 19 | 1456 |
| `mode_literal_ul` | 19 | 1456 |
| `global_uint` | 19 | 1456 |
| `global_eq_one` | 464 | 1460 |
| `global_not_complement` | 459 | 1460 |
| `guard_goto_flags` | 20 | 1456 |
| `guard_goto_all` | 23 | 1456 |
| `guard_label_entry` | 24 | 1456 |
| `guard_label_positive` | 13 | 1456 |
| `guard_label_mode` | 24 | 1456 |
| `guard_label_eq_one` | 458 | 1460 |
| `guard_label_not_complement` | 458 | 1460 |
| `guard_label_mod2` | 13 | 1456 |
| `global_mod2` | 19 | 1456 |

Typed mask literalsU/L/UL and an unsigned-int global declaration reproduce the
complete baseline. Equality-to-one/complement forms add4 bytes and fail roots.
Early exits and labeled forms retain context size but select different guard
instructions. Equal diagnostics are not used to infer equal code; the verifier
compares actual bytes and original ELF roots.

## Verification and next action

`tools/shc_probe/cases/session_score_temporary170/verify.py` checks42 original
and19 labeled temporary selections,53 exact direct-register transitions, three
links to the169 threshold snapshots, five labeled address selections, five
unchanged-output replays, native capture equivalence, and all16 source controls.
The new best difference list and exact7788–77A6 comparison are in `RESULTS.json`.

The next focused question is how bit0x20 in the global-test node is established
and consumed. Compare the baseline, the improved positive-label candidate, and
the combined-entry label control, which preserves the immediate global test but
has worse downstream allocation. Trace those original flag writers/recipe
selection paths; do not alter the flag. A successful source must retain the
improved threshold sequence while recovering the original global-address and
immediate-test instructions, then resolve the independent early constant tie.

All46 production pins and56 prior frozen cases remain unchanged in both Nuada
checkouts. ROM hashes and128398/200698 coverage still match checkpoint161. The
last complete build/test gates remain checkpoint161's245 enabled tests and
independent clean builds.72300 bytes remain, including1638 runtime assembly
bytes. The literal100% goal remains active, solo, without brainstorming.
