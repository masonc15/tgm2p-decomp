# Rank-code drawing: 202-byte accepted match

## Result and scope

Accepted source: src/rankcode_draw_23e3e.c, ROM [0x23e3e,0x23f08).
Coverage increases from 137,090 to 137,292 / 200,698 bytes:
68.306609931% to 68.407258667%, +202 bytes / +0.100648736 percentage points.
61,768 non-exempt bytes remain, plus the unchanged 1,638-byte runtime exception.

The 182 instruction bytes include the complete return delay slot. All 20 shared
literal-pool bytes remain at their original addresses; several serve the remaining
assembly predecessor. The 1,346-byte producer context is excluded and earns no
coverage. The unchanged strict c-range proof exports only f_23e3e and has no
external literal dependencies.

## Evidence that changed the search

Two GPT-6.1 Sol agents investigated distinct routines under bounded assignments.
The initial score lifetime control produced identical bytes and was stopped.
The rank source audit showed the early drawing entry was a sum of differences
across genuine predecessor regions, not evidence for a missing 32-byte operation.
After review, a focused rank capture mapped the first-loop shared address in
SHCMDL and the final register graph. Its array and pointer typed ADD expressions
gave a new falsifiable source question.

The parent ran four ordinary address-expression controls. Keeping real accesses
but expressing the byte destination as a byte pointer, the next pointer as an
array expression, and the previous pointer value through target-width unsigned
integer arithmetic restored f_23e3e to its actual 0x23e3e entry. The unsigned byte
offset spans only 0..168, so no truncation occurs for the actual loop domain.

Two drawing-loop controls introduced a consumed X offset without naming the Y
expression. Incrementing X after the original character-counter increment fixed
the instruction schedule but left the X/zero register homes swapped. One final
ordinary source control, x = x + 10 rather than x += 10, made all 202 bytes exact.
Prior accepted SHC priority evidence motivated this change. The raw causal effect
is measured; an internal priority-graph change for this last variant was not
captured and remains an inference.

No patched compiler output, generated assembly, forced registers, arbitrary
padding, dead liveness, new exception, or proof-tool change was used.

## Acceptance evidence

The integration packet is research/2026-10-10-draw-integration/:
- candidate.kept.prefix.json: unchanged strict proof.
- stage-build.log: isolated make split check report.
- stage-tests.log: 278 tests, no failures or skips, both native test opt-ins set.
- cold/receipt.json: two independent source/tool-frozen full-ROM replays.
- canonical-build.log and ACCEPTED.json: serialized canonical integration.
- tools/test_rankcode_draw_prefix.py: exact positive control and a semantically
  equivalent x += 10 near-match that must be rejected without publishing output.
- research/2026-10-10-sol-rank-cse-capture/CANDIDATE_REVIEW.md: independent
  semantic, boundary, pool and ordinary-object review, without further compiles.

The first isolated full-suite attempt omitted existing difftest and probe support
modules from the lean build snapshot. It ran 271 tests with two import/setup
errors. The missing original modules were copied in unchanged, and all 278 tests
then passed. The failed log is retained as stage-tests-missing-dependencies.log;
no test was weakened. An earlier malformed comment-packaging attempt also failed
before proof; its source is preserved, and the accepted source body matches the
frozen exact experiment.

Mapped-helper ABI caveat: the inherited caller declaration uses int X, whereas
src/text.c defines draw_text with short X. The ROM ABI passes the integer register
and the callee consumes its low signed word. These exact target bytes agree, but
the declarations are not a claim of portable unified ISO C prototype compatibility.
Narrowing the caller again would repeat already-negative source controls.

## Reusable workflow and next question

Use authentic compiler evidence to select a small source experiment, and stop
equivalent-output families quickly. The bounded agents supplied useful causal
evidence; parent synthesis, independent review, strict proof, cold replay and
publication turn that evidence into accepted progress. Compilation volume alone
does not measure success.

The producer f_238fc remains unresolved. Its entry and drawing/shared pool are
now correctly positioned, so the next experiment should map its first divergent
basic block to the authenticated address-expression graph, predicting which real
node operation changes before compiling. Preserve the exact drawing suffix and
all existing protected ranges. Do not resume broad coordinate or type sweeps.
The HUD review authenticated an already-known scratch-order mechanism and earned
zero bytes; keep that source family parked until new evidence predicts a change.
