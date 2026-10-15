# Board-clear context match — continuation183

Integrated the complete656-byte f278C0–27B50 through the unchanged ordinary
c-range verifier. Coverage is130,980/200,698 bytes (65.2622348005%), +656.
69,718 bytes remain:68,080 non-runtime and all1,638 runtime assembly bytes.
The literal100% goal remains active. Work was performed solely by the primary agent.

## Why the standalone near-match failed

A fresh standalone compile exactly reproduces the historical656-byte object and
its22 differing bytes. The loop, literal pool and return already match; setup
registers and scheduling differ. All10 middle-pass candidates reproduce under
the recovered profit/sort/positive-allocation model. The real historical C
register qualifiers are preserved; no physical binding or compiler flag is added.

The first global-address operand request has four free scratch registers, zero
preference and recent mask0. The descending tie break selectsR3. In genuine
preceding context, the scratch caches are likewise empty and the last-register
field has reset to255, but recent mask0xC remains. This penalizesR2/R3 and selects
R1, exactly as required by ROM. Hardware entry snapshots recordmask0xC before the
new function starts; its first request still sees0xC after initialization.
This is observable compiler state crossing function boundaries, not a patched
register mask or a changed selector. The read-only selector model reproduces
all6 observed choices, including an early-reset control that also selectsR1 but
fails instruction/literal order. This control is excluded from production.

Eight successful output-preserving replays capture230 events: one SHCGEN emitter
trace, one SHCMDL graph trace and six address-selector traces. All compare against
fresh untraced controls. Native ELF sections also authenticate the captured source.
GEN SUA equals native capture; ASA is compared to its fresh control and its older
cleanup-position differences are recorded. A separate temporary-value hook
received zero calls, completed the stage, and was rejected by its own assertion;
it is preserved but not counted as a verified replay.

## Genuine context restores the boundary

The single preceding cache-update routine initially puts the clear body8 bytes
late, despite an exact656-byte physical body. Two bytes come from its input-section
alignment. The initial diagnostic omitted those2 bytes when labeling original
object roots; the saved linked ELF exposed it. The corrected receipt and the
initial diagnostic are both retained. No acceptance relied on the bad label.

Using the real context276E8 onward gives11 actual functions. It preserves the
already-retained seven setters and places the exact board-clear body4 bytes late.
The preceding cache-update code computesidx*4 twice. The real ROM computes that
byte index once. Changing that source toidx*=4 and consuming it at both existing
accesses removes4 compiled bytes (including layout effects) and placesf278C0 at
its true nominal root. The before/after-player-read forms both pass all656 bytes;
a separate named-offset form shifts the function early and is rejected.

Onlyf278C0–27B50 is integrated. All472 preceding context bytes are excluded,
including207 differing bytes. The excluded cache-update function still emits
at27860 rather than nominal27862 and uses an external call sequence; it receives
no credit. No preceding body, alignment gap, moved range, relaxed comparison,
or synthetic helper is substituted for matching source. Production uses the
before-player-read compound-index form, with genuine context and ordinary C.

## Scope and validation

The target clears seven globalshorts and exactly224 elements in each of two
long and two short board arrays. Board strideE50, signed-short offset wrapping,
field offsets50/750/AD0/C90 and the untouchedX array remain as in the source and
original instructions. All15 native sources are independently resolved from their
original ELF relocations:2 strict positives and13 negatives. The new proof retains
only_f_278c0,656 bytes, no relocations, no outside literal dependencies, and no
special tail/switch mode. The full1128-byte production context equals the accepted
research context. Native regression tests cover exact scope and rejection when
the preceding repeated index calculation shifts the target boundary.

Isolated clean split/check/report and251 enabled tests pass with zero skips.
An independent canonical clean split/check/report passes. Both ROMs reproduce:

- SHA1 c84cf538962ddbb377290301d76d75d8b542e5a2
- SHA256 d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7

52 production file pins agree in both checkouts. Report:210 units,165 complete,
603 matched report functions. Research and integration evidence are frozen in
session_board_setup183 and session_checkpoint183_board. The primary agent reviewed
source, extraction scope, original objects and clean-build receipts; no subagent
or brainstorming skill was used.

## Next action

Apply this proven context-state mechanism to other standalone near-matches,
starting with score7658. First locate/authenticate its genuine preceding source
and measure the entry recent mask. Preserve the now-matched690-byte speed routine
and this656-byte clear. Do not replay the earlier score expression sweeps or force
a mask; only a real source context and complete nominal-byte proof can earn credit.
