# SHC temporary-register selection at the 2E4C near-match

Continuation77, checkpoint73 unchanged: 125,264 / 200,698 matching bytes.

The two differing bytes in the genuine 240-byte 2E4C–2F3C context come from
`EXTS.W R14,R2; CMP/GT R4,R2` at 2ED2/2ED4. The ROM uses R1. The neighboring
2EFC function and shared pools already match. This investigation identifies
where R2 is selected and reproduces the selection rule; it does not yet recover
the source form that selects R1 while preserving the rest of the output.

## Evidence

Case `tools/shc_probe/cases/session_danger_stages77` captures the known baseline
and the previously rejected promoted-comparison control using the unchanged
SHC v5.0r32 compiler and standard flags. These are calibration cases, not new
source improvements. Normal-driver audits retain original roots and reject
baseline at 2ED2 (two differing bytes) and promoted at seven differing bytes.

Four private middle-pass replays reproduce captured .reg/.ilb exactly. The
baseline .reg has no logical value assigned to R2. `count` is assigned R14.
The disputed scratch register is introduced later by shcgen.

Original x86 and the prior Ghidra exports locate the selector at 0x418900 and
its ranking helper at 0x418ea0. An external Linux ptrace tracer observes their
inputs and return values using hardware execution breakpoints. A third hardware
breakpoint observes instruction emission at 0x41b9d0. There are no software
breakpoints, executable patches, configuration changes, or writes to compiler
memory. Debug registers are the only traced-process state changed.

Two generator runs under observation and two ordinary control replays use
identical captured inputs. Traced and control .sua/.asa outputs agree exactly;
both .sua instruction streams also equal the original driver captures. The
cleanup snapshot is not a pristine pre-generator snapshot: its .asa replay
label positions differ from captured .asa by a uniform 240 bytes. The full
byte differences are retained in GEN_REPLAY_RESULTS.json. The reason for that
saved-state offset is not proven. We do not claim complete captured .asa
identity or a separately rebuilt native object from this replay. The matching
.sua and traced/control comparisons establish that observation preserved the
instruction stream used for this diagnosis.

## Recovered selection rule

The selector considers R4–R14, then R0–R3 in an order chosen by its caller.
The normal scratch order in this example is R3, R2, R1, R0. Exclusion masks
remove unavailable candidates. Remaining candidates receive lower-is-better
ranks:

| Candidate state | Rank |
| --- | ---: |
| Preferred and empty | 0 |
| Preferred and occupied | 1 |
| Empty scratch register | 2 |
| Empty scratch, last selected or in recent-use mask | 3 |
| Occupied scratch register | 4 |
| Occupied scratch, last selected or in recent-use mask | 5 |
| Nonpreferred register above R3 | 6 |

For ranks 1, 4 and 5, a stored usage counter breaks ties. Other ties retain
scan order. A special mode changes R0 treatment. The model includes those
branches and predicts **all 63 observed selections**: 32 baseline, 31 control.
This is empirical coverage of these cases, not proof for every compiler input.
See analyze_scratch.py, SCRATCH_ANALYSIS.json and scratch_selector.x86.

At baseline allocation call24, all four scratch slots are empty, the recent
mask is zero, and the last selection was R3. Thus R3 ranks3; R2, R1 and R0
rank2. Descending scan order chooses R2. Call25 selects another temporary;
the emitter then records a signed word extension from R14 to R2. The final
native audit identifies the corresponding two-byte mismatch at 2ED2/2ED4.
The R2 choice already exists in shcgen, before peephole optimization and
assembly scheduling.

The model predicts that also penalizing R2 would make R1 win. That is a model
counterfactual only: no compiler memory, emitted instruction, or object was
changed. A genuine source construct must create the needed state naturally.
Promoting the comparison instead gives it a middle-pass allocation in R4 and
produces seven differences, so that known workaround remains rejected.

## Next work

Trace the generation order immediately before allocation24 and identify a
semantically required expression/type/lifetime difference that changes scratch
state without changing prior instructions. Preserve the true two-function
context and default verifier. Do not retry generic declaration permutations,
dummy register consumption, forced physical registers, or already rejected
promotion/helper variants. Use this tracer to distinguish changes to the
middle-pass allocation from changes to temporary selection.

No production source or matching metric changed. All 27 checkpoint73 production
pins and both built ROM hashes are rechecked by freeze.py. The last full build
and suite remain checkpoint73's independent clean builds and 228 enabled tests;
private diagnostics do not justify rerunning that entire suite.
