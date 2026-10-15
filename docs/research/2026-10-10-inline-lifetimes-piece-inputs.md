# Inline parameter lifetimes and piece input operand order

No bytes were admitted. These are compiler observations and rejected source
controls, not increased coverage. The last verified baseline remains
137,090 / 200,698 linked C bytes (68.306610%).

## Ranking: what happens to the inline parameters

Two fresh full-context captures in
`research/2026-10-10-rank-inline-lifetimes/` reproduce the previously verified
local-pointer and repeated-input candidates. Their captured assembly was
assembled and independently relocated, and equals the ordinary compiler output.

An initial optimizer replay from the end-of-driver snapshot changed twelve
symbol references by +20. This was rejected. The snapshot already contained
optimizer-generated inline symbols. Regenerating frontend state before running
SHCMDL restores equality: both ordinary and diagnostic runs produce REG and ILB
files byte-identical to the original native capture. Diagnostic common-file bits
are changed only in isolated runs; neither production flags nor compiler binaries
are changed. The normal/debug artifacts and exact equality checks are retained.

The middle-pass write trees contradict the hypothesis that the four scalar
parameters were eliminated. All four survive as explicit assignments from tbl.

| Source | Clone symbols | Allocator bindings |
| --- | --- | --- |
| one computed-pointer local | 300, 305, 310, 315 | R4, R4, R4, R4 |
| repeated base expression | 298, 302, 306, 310 | -9, -1, -8, -8 |

Negative values here are the compiler's storage-binding identifiers, **not byte
offsets**. The final two repeated-input clones share a binding. REG decoding
round-trips losslessly; source names are mapped through the final write tree
rather than assuming early logical-register numbers remain fixed.

Thus the next question was storage allocation, not simply making clones exist.
Four controls in `research/2026-10-10-rank-aggregate-parameter/` tested array
formal declarations and a one-pointer aggregate passed by value. Every aggregate
field is consumed; no filler, volatile effects, dead stores or artificial calls
were added.

| Form | Context bytes | Raw target differences | Protected differences |
| --- | ---: | ---: | ---: |
| local_array | 5480 | 1287 | 0 |
| local_aggregate | 5488 | 1330 | 0 |
| repeated_array | 5488 | 1454 | 0 |
| repeated_aggregate | 5488 | 1325 | 0 |

The two array declarations are byte-identical to their pointer controls.
The aggregate forms use 44-byte frames, not the original 60-byte frame.
All nine function roots stay nominal and all 924 protected bytes stay exact,
but every candidate fails strict extraction.

Park these parameter-form families. A reopening should bring new target-backed
storage evidence explaining original homes +56/+52/+48/+44 and the shared
computed pointer at +4. Merely repeating source spelling or generating four
more scalar clones does not address the observed cause.

## Piece update: transferring an established compiler mechanism

Target `[0x4044,0x44d4)` is 1,168 bytes, with genuine context beginning at 2f68.
Protected ranges are `[0x2f68,0x3c88)` and `[0x3e2e,0x4044)`, totaling 3,894
bytes. The intervening DAS body is unretained and has a documented high-index
semantic limitation; serving as producer context does not grant it credit.

Read `tools/shc_probe/cases/session_piece_update29/REVIEW.md` before this packet.
Earlier plain base+index and index+base forms were identical. The new hypothesis
comes from `2026-10-04-vm-operand-order.md`: SHCMDL's observed term-size ordering
can preserve opposite orders when both operands are equal-rank cast nodes.

The three controls in `research/2026-10-10-piece-input-term-order/` change only
four original input loads in f4044. They use actual mapped 32-bit addresses and
an unsigned byte index; no address wraps and no input is cached across writes.

| Form | Context bytes | Raw differing or missing target bytes |
| --- | ---: | ---: |
| baseline | 5468 | 878 |
| address_base_first | 5464 | 941 |
| address_index_first | 5464 | 945 |

Both cast controls remove the extra MOV R3,R0 before the first comparison.
Base-first emits exactly the original bytes at `[0x40c6,0x40d2)`:

```text
mov r11,r0
mov.b @(r0,r12),r0
extu.b r0,r0
and r13,r0
cmp/eq #32,r0
bf 0x40da
```

Index-first reverses which input becomes R0 in the indexed load. This confirms
the predicted instruction-selection effect in this new context. It does not
prove all the same intermediate rewrites occurred: no piece MDL dump is claimed.

The owner remains incomplete. Changed body length shifts later literal pools,
and subsequent stack/field-address choices still differ. The 12-byte local
instruction match receives **zero** coverage credit.

## Localized follow-up and next action

Two controls in `research/2026-10-10-piece-first-turn-addresses/` restrict the
input change to the first two comparisons, then additionally spell the first
rotation call's real byte/short member addresses through unsigned-long offsets
362/364/368. Both output exactly the same binary: 5,464 context bytes, 947 raw
differing/missing target bytes, nominal roots, all protected bytes exact.
The field-address rewrite is therefore a verified negative, not another avenue
for pointer-spelling permutations.

The original first rotation path has two still-unexplained features:

- It stores the promoted pressed-input byte at stack+4 before the second bit
  test. The draft keeps the common value in R4.
- It independently forms the rotation and coordinate addresses with ADD into
  R3. The draft uses R0 indexed member loads and reuses one coordinate offset.

Next inspect these live ranges and address nodes in the first rotation call.
Use the correct first-load sequence as a mechanistic control, not as a claim
that a worse whole-owner residual should be integrated. Stop if the proposed
mechanism remains unchanged; preserve the existing source-family negatives.

## Verification

From the canonical project:

```sh
/drive2/tgm2p/venv/bin/python research/2026-10-10-rank-inline-lifetimes/verify.py
```

The verifier checks two capture/context equalities, four optimizer replays,
eight clone mappings, nine ordinary source controls, independent original-object
relocations, nominal roots, protected ranges, every reported raw residual, and
the exact original 12-byte input-load sequence. All source/tool hashes and
strict rejection reasons are preserved in experiment receipts.

No production source, split, shared build/proof tool or compiler changed.
No candidate passed admission; no cold integration replay or full tool test
suite is claimed. Dashboard coverage is unchanged.
