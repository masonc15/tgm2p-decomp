# VM word multiplication and buffer lifetime, continuation 223

Production remains 135,502 / 200,698 verified bytes (67.51537%). No new C credit.

The important new control is named_base_short_y_buffer.c. It emits the complete
104-byte EE9C–EF04 tail byte-for-byte, including all instructions and the shared
literal pool, but starts it at EE88: twenty bytes early. All five tail function
starts have the same error. The existing strict gate rejects this candidate.
No object shifting, context rebasing, padding or changed acceptance rule is used.
The twelve already-C empty-function bytes are not new progress.

This ordinary-source control narrows y, names a char pointer to the genuine
initialized two-byte buffer, and uses one row base for the final three drawing
calls. The source remains a complete six-function context beginning at EC0C.
The named pointer restores seven loads of the drawing function through R3,
instead of keeping its address in a callee-saved register. This removes the
incorrect shared-pool composition seen in continuation 222. The y change also
restores two signed-word multiplies and the native MACL save. The 740-byte
context remains twenty bytes shorter than the original 760-byte context.

The frame is still 48 bytes rather than 44. Independent native allocator models
show y at priority 14, profit 4, spilled; constant 63 has priority 20, profit 10,
and occupies a register. An ordinary register hint raises y to priority 65,549
and profit 65,539, changing the conflicts and spilling different values. Its
frame becomes 40 bytes, also wrong. Do not repeat this hint as a local remedy.
The original places y in R9 and does not reserve a callee-saved home for 63.

Thirty-eight source controls cover twelve multiply/type/buffer combinations,
seven register-hint/initializer combinations, nine consumed sprite/width pointer
aliases, four byte-field representation controls and six loop/condition forms.
All strict-reject. Pointer aliases and bit-field forms do not fix the boundary.
For/while forms retain the twenty-byte error; top-break loops change pool splits
and also fail. Bit-field controls are experimental representations, not accepted
source or evidence of the original declaration. The best nominal-position raw
score in this batch is 88 differences; the previously aligned continuation 222
candidate remains at 21. Position-relative equality is only a diagnostic.

Five independently assembled captures reproduce their compiler object bytes.
Three output-preserving allocation replays produce 421 events, with 45/47/47
candidates independently modeled. The final model_graph.py, analyze_codegen.py
and verify.py runs exit zero. analyze_codegen.py checks original ROM instruction
bytes for the frame and signed-word multiplies, native frame/call patterns,
allocation priorities and the full 104-byte diagnostic equality at EE88.
verify.py independently resolves every compiler object, confirms source and
context hashes, checks all strict failures, and verifies replay equality.

Evidence: tools/shc_probe/cases/session_vm_wordmul223/. No production source,
compiler, proof, ROM or dashboard changes. Production still uses checkpoint 218
with 62 pins and successful clean builds / 263 tests per checkout. Full suites
were not rerun for research-only changes.

Next retain the exact-tail source as the boundary-reconstruction baseline. Trace
native MDL priority accumulation for y and the repeated 63 stores, then compare
its EC0C stack slots and instruction sequence with the original. A useful source
change must recover the original preceding-function boundary and pass the same
strict complete-function gate; shifted diagnostic equality alone earns nothing.
