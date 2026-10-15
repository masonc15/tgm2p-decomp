# Wave allocator investigation193

Production remains checkpoint192, 131,780 /200,698 bytes. Six strict controls
produce no retained object and no new matching C credit.

The genuine preceding800-byte board-scroll function preserves all prior context
but leaves the target's10 differences unchanged. Baseline and extended-context
SHCMDL traces each contain110 events and identical37-candidate graph structure,
priorities, profits, clashes, ordering and assignments after normalizing addresses.
Fresh traced .reg/.ilb files equal untraced controls and native cleanup snapshots.
The saved assembly also assembles through the project's ordinary inline-assembly
path to exactly the independently resolved strict-context bytes.

The R5/R6 tail difference is already present in the middle-stage .reg allocation:
logical10/address speed R4, logical11/address direction R5, logical12/value speed R6.
The ROM instead uses direction R6 and value speed R5. The direction address
candidate and generated value temporary(-10) each have priority3/profit-1.
The unstable sort puts the address first(position15), temporary second(position16).
The speed address has priority4/profit0 and sorts at14. This is a middle-stage
allocation tie, not evidence of the final generator's recent scratch mask.
Two final-generator operand replays preserve SUA/ASA versus fresh controls,
with21 total events, but their observed requests are phase loads/truncation,
not the tail address allocation. Cleanup ASA positional differences are recorded.

Controls: baseline10 differences, scroll_context10, j_after_phase16,
speed_switch90 including4 missing bytes, speed_int10 byte-identical to baseline,
j_loop_init16 byte-identical to j_after_phase. All preserve the preceding538
matched bytes; extended context additionally preserves800 scroll bytes.
Changing the32-bit speed declaration int/long does not alter output. Moving
j initialization later does not fix setup scheduling. No production change.

The initial diagnostic capture used machine-object mode and failed because the
existing inline_asm helper requires assembly output. Its compiler.log is retained
in baseline_machine_watched. Corrected capture uses the actual production
assembly path. The first .reg replay count assertion omitted the inline helper's
record; corrected expected counts are5/6 versus4/5 emitted functions. Neither
harness error affected a candidate source or production output.

The inherited fixmul inline-assembly primitive and historical inert if(0) context
are unchanged; no new assembly, fixed-register binding or fake use was introduced.
Next investigate construction/order of the tied value/address candidates using
read-only SHCMDL IR tracing before further source changes. Full build/test gates
remain checkpoint192's independent253-test runs; excluded diagnostics do not
justify repeating them.
