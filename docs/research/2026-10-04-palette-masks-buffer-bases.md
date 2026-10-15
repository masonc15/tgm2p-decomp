# Palette masks and buffer-base allocation — continuation202

No additional C bytes qualify. Two separate candidates recover useful pieces
of the original output: the initializer's three color-mask homes, and the
preceding function's68-byte local stack area. Neither candidate matches its
complete function. The best initializer remains201's119-difference body at
2A56A, twelve bytes early. Production stays131780/200698 bytes (65.66084%).

The previous goal turn was progress: it improved the initializer and identified
specific remaining decisions. This continuation measures those decisions and
identifies the next extraction/binding question. There is no external blocker;
the full100% goal remains active, solo, without brainstorming.

Evidence: `tools/shc_probe/cases/session_gfx_masks202/`. Its baseline source and
resolved context are byte-identical to201's`phase_pointer_in_loop`.

## Mask candidates

All four reversed-AND operand controls emit the baseline's exact bytes. Moving
the actual pointer declaration into the loop, with or without const qualification
of the pointer, also leaves the output unchanged. These forms do not change the
remaining mask problem.

Naming the actual masks as consumed unsigned-long locals before the final loop
produces different allocation candidates. Read-only weight replays authenticate
priority17 for the green-only mask and for the red/green variables in the
all-mask variant. The initializer contributes its ordinary assignment weight;
loop uses account for the rest. These are not the original constant candidates,
whose priority is32. Their conflict averages change too, so lower priority does
not imply later allocation.

| Variant | Mask allocation evidence | Initializer body differences |
| --- | --- | ---: |
| Baseline | RedR11, greenR13, blueR12 |119|
| Named red | Variable priority17/profit-40956 inR13 |128|
| Named green | Variable priority17/profit-40956 inR13 |132|
| Named blue | Byte-identical to baseline |119|
| All named | RedR13, greenR11, blueR12 |155|

The all-mask variant has two variable candidates (symbols93/94), both priority17
and profit-40955; blue remains a constant candidate, priority32/profit-41990.
Original native assembly independently confirms literal loads of FC000000 into
R13,00FC0000 intoR11 and0000FC00 intoR12. Those are the ROM's mask homes, but other
instruction differences increase. No claim of a matching initializer follows.
Const-qualified versions produce exactly the same bytes as their plain versions.
All these contexts remain2924 bytes, with a1504-byte initializer at2A56A.

Two early generated operand controls were malformed because a substring
replacement consumed the prefix of a longer hexadecimal token. The generator
now requires a token boundary. The malformed sources are retained under
`generation_errors` and explicitly excluded; both corrected controls were
compiled and verified. They are not evidence of a compiler defect or two extra
valid source experiments.

## Preceding function and the boundary

The original2A0D4 prologue reserves68 local bytes (`7FBC` at2A0E6), plus32 bytes
of saved registers/MACL. The reconstructed baseline reserves76 local bytes,
frame108. It computes two buffer addresses into separate stack slots.

Changing only the hardware-copy read from
`*(short *)((char *)buf + i * 2)` to
`*(short *)((int)buf + i * 2)` makes both buffer address calculations use the same
integer arithmetic form. On this32-bit target, bounded indices retain the same
addresses. The resulting native frame is100 with68 local bytes, matching that
one original property. However, the predecessor grows from1174 to1192 bytes,
placing the initializer six bytes late at2A57C. It differs in1108 predecessor
bytes and is not a source replacement.

The full46-candidate graph explains why stack-size recovery is insufficient.
Three generated values remain mutually conflicting:

| Candidate | Baseline priority/home | Integer-address priority/home |
| --- | --- | --- |
| -13 |7/spilled|7/R11|
| -14 |7/spilled|7/R10|
| -15 |6/R14|7/R9|

The native integer-address sequence computes R15+4 inR6 and copies it separately
toR11,R10,R9. Thus the two spilled base values become register-resident; the source
does not collapse all buffer bases into the ROM's single retained R10 base.
One counter spill remains. Graph homes and the actual stack setup are verified
independently; an identical frame size alone is not an allocation match.

Eight buffer forms cover consistent char/integer arithmetic, ordinary subscripts,
named char/integer bases, standard register hints, and a real union buffer.
Four targeted counter/hardware-pointer hint controls follow the observed
allocation priorities. Four direct hardware-address controls remove the real
local pointer, and four equivalent do-loop forms test the ROM's lack of an
initial outer-loop test. The do condition performs both original increments,
including after continue, and bounded counters do not overflow.

None gives a nominal2A576 initializer root. The direct hardware/integer-buffer
candidate begins2A574 (two bytes early) and has a1506-byte initializer; its205
nominal-position differences must not be confused with its1270 differing body
bytes. It is not a near-complete body. Broad char/union/named-base changes also
regress the code and do not justify adopting them.

## Verification and next action

35 valid target controls fail unchanged strict retention. Twelve compiler
replays preserve REG/ILB output against fresh controls and native captures:
four initializer graphs, six predecessor graphs and two variable-weight traces,
3689 events in total. Twelve captured assembly files independently assemble and
resolve to their strict-compile contexts. Sources, original objects, actual roots,
body and nominal-position differences, stack fields, and mask-load homes are
checked by `analyze.py` and `verify.py`.

No production source, compiler input format, proof, split map or denominator
changed. No full build/test rerun is claimed for these excluded diagnostics;
checkpoint192's independent clean builds and253 enabled tests remain the last
production gates. Final verification rechecks54 production pins, report and ROM
hashes on both Nuada checkouts, and preserves all prior frozen evidence.

Next trace the extraction/hoisting and binding of the three equal stack-buffer
addresses in SHCMDL, comparing the mixed-address baseline and integer-address
control. The relevant first function is symbol53, REG record0; the initializer
is symbol77, record1. Determine why separate loop regions retain mutually
interfering buffer-base values, then choose an ordinary C representation or
lifetime change supported by that evidence. Do not repeat the closed named-mask,
operand-order, hint and buffer-type controls, or add padding to repair the root.
The original119-difference initializer remains the best preserved body; the
155-difference all-mask candidate is a separate mechanism control.
