# Animation stack-home initialization — continuation145

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**. No new C bytes were accepted. The preceding continuation144 made progress by closing the handoff/literal-base hypotheses with verified compiler evidence. This continuation preserves an additional original operation sequence through genuine source value flow.

## Initialize the memory-backed sprite ID first

The original second sprite return is stored to home0 at A744. It is loaded into R9 at A750, after the first initialization call. Three later R9 word stores update home0 at A760/A77A/A7AC. Earlier explicit-copy controls instead initialized the temporary spr2 directly and copied it into sprite_id later.

Four candidates initialize `sprite_id = f_2528c()` and then assign `spr2 = sprite_id`. The early versions copy before the first initialization call; the late versions copy after it. The copy feeds the second initialization call and the existing actual field operations. The combined pair starts from143 common_brightness; the loop pair starts from139 shared_home_loop_legacy. Source generation verifies frozen input hashes and exact replacement counts.

| Source | Bytes / frame | Sprite home | Result |
| --- | --- | --- | --- |
| combined_early | 1744 / 108 | 0 | Saves the return to home0, then copies R0 directly into R10. |
| combined_late | 1744 / 108 | 0 | Saves the return to home0, then reloads R10 after the first call. |
| loop_early | 1724 / 104 | 8 | Temporary remains R8; original constraints not recovered. |
| loop_late | 1728 / 104 | 8 | Explicit reload survives, but temporary remains R8. |
| brightness_ref | 1732 / 104 | 0 | Addressed first-sprite brightness access changes its home ordering, but loses frame and loop-register constraints. |

The combined late candidate has these actual native instructions:

```
A738  mov.w r0,@r15
A744  jsr @r3             ; first initialization call
A746  mov.w @(r0,r15),r4  ; delay slot
A748  mov.w @r15,r10
A756  jsr @r2             ; second initialization call
A758  exts.w r10,r4       ; delay slot
A75E  mov.w r10,@r15
A76E  mov.w r10,@r15
A7A2  mov.w r10,@r15
```

This restores the initial home-store/reload relationship while preserving the108-byte frame, six loops, original main-loop hold/mask15/constant2 roles, total on stack, and homes0/4/8/12. It is not a byte match: setup usesR10 and the header count usesR9, other homes differ, and the function still beginsA604 rather thanA608. The initial/repeated stores are explained by consumed source assignments; no automatic writeback mechanism is inferred from them.

## First sprite home frequency

The fifth source changes only the existing brightness address calculation from `long sprite_offset = spr1` to a consumed local pointer `short *sprite_id1 = &spr1; long sprite_offset = *sprite_id1`. No address escapes, dummy reads, volatile qualifier, aggregate, or extra helper is introduced.

Read-only SHCMDL hardware traces show spr1 changes from an ordinary home with frequency4 at offset44 to a memory-group home with frequency7 at offset32. The original needs offset16. This source also moves total intoR8, constant2 intoR9, and leaves mask15 unallocated; the frame falls to104. The frequency increase therefore does not solve the joint constraints. All five still have six detected loops and no inline-return gotos.

## Correction and validation

An initial analysis incorrectly inferred identity from equal body lengths and equal mismatch counts. The exact allocated-section comparison rejected that assertion. The early/late combined candidates differ from143 common_brightness in15/11 full-unit text bytes respectively; their read-only data is identical. They also differ from each other. The failed assertion and explanation are preserved, the user-facing claim was corrected, and no source changed to repair this analysis error. Equal mismatch counts are not evidence of identical code.

Five native compiles and independent captures agree outside comment metadata. Ten normal/debug middle replays reproduce captured REG/ILB output exactly; all REG records round-trip. Five native CFG audits resolve stack accesses, restore the stack on returns, and find no anonymous local calls. All emit36 exported game functions. Five hardware home-allocation traces preserve REG/ILB output; the home and frequency models reproduce132 records and644 contributions. The existing strict contextual verifier rejects all five candidates.

Both Nuada checkouts retain44 checkpoint122 production pins,30 earlier frozen cases, original ROM hashes, and unchanged coverage. All23 reference ROM files match the recorded source. The last complete production build/test gates remain checkpoint122; private probes do not constitute a new production suite.

## Next action

Retain145 combined_late alongside143 as the new setup value-flow control. The immediate allocation question is now concrete: why does the real setup temporary takeR10 while the header count takesR9? Study their interference and candidate ordering together, then test only a consumed source transformation predicted to exchange those roles without losing the recovered loops or homes. An inline void operation with a by-value setup argument and a separate pointer to the real home is a distinct potential control; do not take an inline by-value argument's own address, a family already rejected by138.

A read-only review of the shortest remaining report entries found familiar misleading seeds: some tiny entries are literal/interior labels, while E490 and16938 are truncated views of larger bodies. Existing lookup8b7c and renderer investigations already reject the obvious independent forms. This review supplies no new matching credit and does not justify changing the denominator or extracting arbitrary byte fragments. Preserve their existing negative results rather than repeating declaration sweeps.
