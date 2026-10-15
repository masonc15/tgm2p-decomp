# Score comparison register preferences — continuation174

Production remains checkpoint161: **128,398 / 200,698 bytes (63.9757247207%)**.
No native candidate in this continuation passes the strict prefix verifier.
The goal remains literal 100%, including all1,638 runtime assembly bytes.

## Preference provenance

Four SHCGEN runs watch the threshold load's metadata preference word `0x12`
and inherited exclusion word `0x16`: baseline, positive labels, joint exit,
and the previously rejected reversed comparison. The tracer arms when the
expression is available to original planner `405fe0`; it checks that the
selected load, symbol57 child, and metadata allocation remain identical through
the actual result request at `421137`. It disables the data watches there,
before the compiler later frees and reuses the expression storage.

Initial attempts at function entry and before statement emission found no
threshold tree, because SHC loads expressions incrementally. An overlong watch
also reached released metadata after the result request. Those attempts were
corrected and are not included among the eight completed, output-preserving
replays. The accepted trace starts with an initialized zero preference; it
does not claim to observe the metadata allocator's initial zeroing.

For baseline, positive labels and joint exit, the trace proves this sequence:

1. The threshold preference and inherited words start at zero.
2. Original `42cd39` copies comparison recipe word `0x2c` (zero) to preference.
3. Original `41c904` copies the parent's inherited mask `0x7cf0` to the load.
4. Original `41c948` copies the parent's zero preference, because the load's
   preference and paired preference word `0x20` are still zero.
5. Original `41c972` removes recipe byte `0x31` from the inherited mask; this
   recipe byte is zero, so the mask remains `0x7cf0`.
6. The result request has preferred=0 and no low-register exclusion bit.

The watchpoint PCs are after those writing instructions: `42cd3d`, `41c908`,
`41c94c`, and `41c976`. The captured comparison recipe is at `45c0c8`, with
both operand preference words `0x2c/0x2e` zero. These are the same recipe bytes
in all four controls. Register histories and cache occupancy can differ despite
the common preferences, as continuations169–173 already establish.

Reversing the C comparison puts the threshold in the second child. It copies
recipe word `0x2e` through `42cd65`, still zero. At `41c9a6`, it inherits the
first child's updated mask `0x7cf2`: the already allocated level occupies R1.
Original `41ca15` copies the parent's zero preference. This explains a real
ordering difference without inventing a preferred register. Native output
loads level first, then threshold, and uses `cmp/ge` plus `bf`; it differs from
the target's threshold-first `cmp/gt` plus `bt` sequence and remains rejected.

Across the four runs, the verifier checks all **15 preference/inherited writes**
and stable node identities through the requests.

## Corrected meaning of the result-slot byte

Earlier trace schemas called stack byte `esp+0x30+slot` `use_count`. The name
was too broad: this byte is a **result-slot marker**, not the number of source
uses. The separate recipe operand-occurrence array is at `esp+0x20`.

Four additional replays observe original `420b97` before and `420b9c` after the
marker write, plus subsequent result requests. In this corpus, recipe byte1=3
selects slot0. Original `420b90` sets the preference-selection flag to1 and
`420b97` sets that slot marker to2. All209 observed transitions are from
`[0,0,0,0,0]` to `[2,0,0,0,0]`, with node metadata and recipe unchanged.

The verifier links designated-slot requests back to those writes across
**229 result requests**. For each threshold load, the actual operand-occurrence
array is `[255,255,255,255,255]`, the recipe's counting-disabled sentinel;
the fixed-register value is `0xffff`, and the destination mask is zero.
Thus marker2 does not establish two source uses. The older mathematical
preference-selection checks remain valid, but source-use arguments based on
that field would be incorrect. No frozen historical files were rewritten.

## Native controls and limits

Thirteen new controls use the joint-exit candidate as their source baseline:

| Source change | Differences | Context bytes |
| --- | ---: | ---: |
| Assign threshold inside comparison to existing q/r/d/s | 15 each | 1456 |
| Assign threshold to q/r/d/s in a preceding statement | 33 each | 1456 |
| Assign level to existing int s before comparison | 32 | 1456 |
| Convert global flag to char or short before testing bit0 | 256 each | 1460 |
| Convert global flag to unsigned char or unsigned short | 456 each | 1464 |

All assignments are consumed by the comparison, and the existing locals are
overwritten before their later uses. The four in-expression assignments produce
the entire joint candidate context byte for byte. The narrowing variants grow
the context and shift the following function, so they cannot be integrated.
All1456-byte controls preserve nominal function roots and the following
function byte for byte. The older reversed-comparison control remains at24
differences; its fresh native capture matches the prior object in every ELF
section except `.comment`.

All eight completed tracer runs preserve both SUA and ASA versus fresh untraced
controls, and SUA versus the native captures. ASA snapshot differences are
recorded separately, as in earlier replays. Tracers write only hardware debug
registers, not compiler instructions or data. Evidence, source diffs, original
disassembly, captures, and verification code are in
`tools/shc_probe/cases/session_score_preference174/`.

Both Nuada checkouts retain46 production pins,60 previous frozen cases, the
original ROM hashes, and the fixed coverage report. All23 reference ROM files
match their recorded source. The last full build/test gates remain
checkpoint161's independent clean builds and245 enabled tests; this continuation
does not claim a new suite run.

## Next action

The threshold preference is now explained for both operand positions. Preserve
the best13-difference positive-label and15-difference joint-exit candidates.
Next inspect the parent comparison's child planning and instruction scheduling:
what determines child order before `41c984`, and whether a genuine source
expression can alter allocation order while retaining the target load order.
The reversed control provides a concrete comparison for that investigation.
Do not repeat named-local, equivalent-promotion, narrowing-conversion, or
literal-suffix variants. Do not reinterpret the role marker as a source-use
count or force a physical register. The72,300 unmatched bytes remain incomplete.
