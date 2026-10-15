# Complete board-wave match — continuation215

The complete682-byte wave function at2808A–28334 now matches the original ROM.
Move j initialization after phase conversion and give ang the ordinary C register
storage-class hint. A separate control hinting j produces identical1220-byte
context. Neither source binds a hardware register or changes an algorithm.

## Allocation cause

The former late-j control put the phase inR5 and k inR4. Phase priority2 and
profit−32772 lost to k priority3/profit−30251. Hinted ang priority becomes65540;
it remains spilled, but its interference raises k's average conflicting priority.
The actual native model now gives k profit−35292. Phase consequently precedes k
and takesR4; k takesR5. Hinted j(priority65544) has the same corrective effect.
This is a conflict-cost change, not a request to place ang in a particular register.
The new sort also resolves the speed/direction tie in the114-byte tail. Both
candidate objects reproduce every byte, including stack offsets and literal pools.

## Proof coverage needed for integration

The initial compile helper reported zero differing bytes but rejected the literal
backedge at2828C. Enabling the existing local-loop proof still rejected it: that
proof supported only R1 and also required a genuine following function root.
Neither initial rejection was counted as matching C.

Added the genuine following f28334 as excluded compiler context, using its real
board fields; it begins at the original28334 boundary. The context is2168 bytes;
only1188 bytes27E90–28334 are retained, including506 previously matched bytes.
The980 context-only bytes receive no credit. The existing separate32-byte helper
prefix remains separately retained. The preceding wave's R1 proof is preserved.

Extended the local-loop proof narrowly to the equivalent R2 form:
MOV.L @(disp,PC),R2; JMP @R2; NOP. The defining register must agree, and only R1/R2
are accepted. All original section-relocation identity, embedded-addend, linked ROM
pointer, actual following-root ownership, backward target, ordinary reachability,
unique defining load, delay-slot, foreign-entry and literal-data checks remain.
New fixtures reject mismatched registers, indirect calls, non-NOP slots, bad target
positions, wrong relocation facts, alternate jump/slot entries, missing following
roots and all other matched register pairs. Byte equality remains mandatory.
The real R2 proof is load2828A/jump2828C/slot2828E, target2810C, owner end28334.
No compiler, intermediate, object or ROM bytes were edited. Existing fixmul inline
assembly and prior context are unchanged; no new assembly was introduced.

## Evidence and excluded controls

69 native source controls, including two fresh baselines; the larger contexts are
independently compiled. All67 initial controls and two larger-context controls were
rejected without the applicable loop proof. Four have zero byte differences; only
the two larger contexts pass the extended strict proof. Eight allocator models,
16 output-preserving graph/construction replays and1116 events are authenticated.
Ten captured native assembly outputs independently assemble/resolve to the strict
context objects. All69 original objects are independently resolved as well.

The loop alias experiments were negative. Aliases of attr/n/ang/w were byte-identical;
aliases of k, half, q and bd perturbed allocation or layout. Tail pointer aliases
and most phase movements also failed. Explicit phase-local hints produced43
mismatches; unhinted phase-local type variants produced16. Hinted k still had10.
One source-generator attempt changed the union member .w while renaming local w;
the compiler rejected it. Its source/log are retained separately and excluded from
the69 valid controls. The corrected source was compiled normally.

Production changes: wave source, one removed assembly split, R2 proof support and
its native/fixture tests. Both independent clean builds, full native suites and
ROM hashes are recorded in session_checkpoint215_wave. That checkpoint's final
report is authoritative; the full100% goal remains active.
