# Cache-update helper: genuine local call and source operations

Coverage remains **138,670 / 200,698 bytes (69.09386242015367%)**.
This research adds **0 verified bytes and 0 percentage points**. The agreed
completion threshold remains 199,060 C bytes with 1,638 existing ASM exceptions.
No canonical source, split, compiler, or proof was changed.

## Evidence before the experiment

The unresolved helper occupies `[0x27862,0x278c0)`, including its literal pool.
The original call at 0x2789c is a BSR to 0x26f64. R4 carries the player word,
R5 is explicitly zero, and R6 carries a pointer into the game's cache array.
The accepted redraw callee consumes only the first argument. The existing
helper-only contexts declare that callee external and emit an indirect call.

The earlier toggle work restored the helper's entry but still measured 48 raw
helper differences. The board-clear work changed repeated index multiplication
to a consumed scaled index, preserving the clear but leaving this helper
unmatched. An explicit ignored-file-inclusive search found 228 source paths
and five distinct helper bodies, including the new joined draft; they are
archived in BODY_GROUPS.json. Most copies are repeated context or cold snapshots,
not independent attempts. No new sweep repeats these old index forms.

## Three bounded questions

The packet is `research/2026-10-17-cache-update-unit/` on Nuada. Each question
records its prediction and source hash before compilation. All output comes from
the ordinary unmodified SHC v5.0r32 driver and the established flags.

1. **Whole producer context.** Join complete real bodies from the accepted
   redraw, callback/queue, setters, toggle, and board-clear contexts, starting
   at 0x26f64. The two unused formal parameters of the redraw callee preserve
   the three arguments observed in the ROM caller. The independently recovered
   full-width allocator return is declared int. The predicted direct BSR is
   emitted. Both already-accepted redraw wrappers and their pool remain exact,
   140 bytes. Earlier unresolved bodies still displace later entries.
   An initial source-composition error mixed an older integer callback member
   assignment with the actual function-pointer member type. That driver stopped
   after four stages without an object. Replacing that predecessor with its
   current accepted body was recorded before an eight-stage retry; the amended
   construction budget was 12, not an unrecorded source search.
2. **Typed game-record access.** ROM loads player at byte82 with indexed word
   addressing and reuses a scaled cache index at byte128. A struct member view
   recovers the indexed player load and the separate R6 scaled index. The helper
   extent becomes 94 bytes. An explicit final short cast alone does not recover
   the original EXTS.W after SHLR16. One eight-stage driver launch.
3. **Signed cache high word.** Removing only the inherited unsigned-long
   conversion makes the long cache element undergo a signed arithmetic shift.
   This emits the original SHLR16/EXTS.W pair. It does not recover instruction
   scheduling or the missing predecessor extent. One eight-stage launch.

The complete packet therefore records **28 stage launches: 4 failed construction
stages plus three successful eight-stage drivers**, across three preregistered
questions (12, 8, and 8 stage limits). No instrumented compiler output is used.
Read-only launch capture retains native objects and pre-stage snapshots.

The typed-record draft uses a one-element trailing cache member only to express
its observed base and stride. The actual array extent has not been recovered;
this draft is research and is not established as an admission-ready C layout.
Future integration must recover that extent or an appropriate genuine view.

## Raw results and rejection

All roots and differences below use the actual compiler object at the real
context origin, without rebasing, padding, or branch normalization.

| Control | Context bytes | Helper root | Next root | Helper extent | Fixed target differing bytes |
| --- | ---: | --- | --- | ---: | ---: |
| Joined context | 3000 | 0x27832 | 0x2788c | 90 | 94 |
| Typed record, unsigned shift | 3004 | 0x27832 | 0x27890 | 94 | 92 |
| Typed record, signed shift | 3004 | 0x27832 | 0x27890 | 94 | 92 |

The helper is **48 bytes early** in all three controls. The first unresolved
redraw body places 0x2722c at 0x271fe; the following helper places 0x27310 at
0x272e0. The signed control has 421 fixed-address differences in the retained
wrapper/queue interval, 74 in the callback dispatcher, 95 in the callback setter,
133 in the seven setters, and 610 in the board clear. These are rejected research
regressions; the canonical versions remain untouched.

The unchanged strict proof rejects the target because its requested boundaries
are not compiler function boundaries. It independently accepts the existing
140-byte redraw prefix in all three objects. The signed draft still loads the
lock-global address into R0 before the high-word expression, unlike the original
later load; subsequent source-pointer scratch allocation and scheduling differ.
Correct helper extent and a direct call do not establish exactness.

## Verification and next route

`verify.py` checks original source pins, ordinary object identities, linked text,
all actual roots, fixed-address residuals, stage counts, the three observed
mechanisms, prefix preservation, and strict target rejection. Its saved
VERIFICATION.json reports passed. Re-run only the verifier against the frozen
packet; copy the packet to a new directory before regenerating experiments.

Stop these exact whole-context/member/shift controls. The next useful evidence
must address the real 48-byte predecessor deficit, the helper's lock-address and
pointer lifetimes, or the actual game-cache array extent. Do not add filler,
rebase the helper, alter the BSR, discard intervening bodies, or repeat index/cast
spellings without a new compiler prediction. No full suite, cold integration
replay, or dashboard publication was warranted for these rejected sources.
