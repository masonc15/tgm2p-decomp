# VM division shared-pool context, continuation 222

No production gain: 135,502 / 200,698 bytes remain verified (67.51537%).

The complete division handler EEBC–EF04 shares its final literal pool with the
preceding EC0C text routine. The authenticated six-function context spans
EC0C–EF04. EE9C–EF04 is a possible 104-byte retained interval, including twelve
already-C bytes for three empty functions; the potential net gain is 92 bytes.
A standalone division function cannot reproduce this shared pool.

Thirty-four ordinary-source controls were compiled with unmodified SHC 5.0r32,
-macsave=1 -optimize=1 -speed. All fail the strict acceptance gate. Replacing
byte-pointer row arithmetic with g_607930c[row].c restores all six original
function starts. This candidate is 764 bytes against the original 760, with
21 differing bytes in the nominal tail. Thirty nonliteral instruction words
are exact. All seven PC-relative loads resolve to the original values, but
five displacements differ. This is diagnostic evidence only, not byte credit.

The original final long pool contains drawing-function, VM-pointer, row-base,
length, mask and signed-division-helper addresses. The candidate omits the
late drawing-function address and adds row-base+30 and row-base+60. The pool
therefore grows by four bytes. It keeps the drawing function in R8 for all
seven calls; the original reloads it for each final call. Native allocation
models give that function-address candidate priority 19 and profit 10.
Explicit shared row-base expressions remove the extra addresses but also
move the function starts; short y and consumed aliases do not fix the layout.
The original also saves MACL and has a 44-byte local frame; the best candidate
has a 52-byte frame without that MACL save. The whole context remains unmatched.

Three independently assembled compiler captures and three output-preserving
MDL replays (413 events) agree with the original compiler outputs. Independent
models verify 46, 45 and 46 candidates. model_graph.py, analyze_tail.py and
verify.py all exit zero. The first verifier run failed because its historical
source path was absent on the isolated checkout; its failure log and validation
record are preserved. SOURCE_PROVENANCE.json now pins the historical source
hash, checked locally against legacy_u4.c, without a sibling-directory dependency.
No acceptance condition was weakened.

Evidence: tools/shc_probe/cases/session_vm_division_context222/. The case freeze
pins controls, native captures, models, validation and initial failure evidence.
No production, proof, compiler, ROM, denominator or dashboard changes. The 62
production pins and checkpoint 218 clean builds / 263 tests per checkout remain
authoritative; full suites were not rerun for this research-only continuation.

Next inspect the original signed-word multiply operations for row*30 and row*29
and the live buffer pointer. Test source expressions that explain MACL use and
the original register pressure before broader allocation searches.
