# Save-reader loop priorities — continuation250

Production stays at checkpoint249, 135,910 / 200,698 bytes (67.71866%). The
260-byte reader13380–13484 remains assembly. New genuine source controls reduce
its differing bytes from34 to16 while preserving all four original roots,
the668-byte context, and the140 already retained checksum/setter bytes with
their dependencies. All35 source controls still fail unchanged reader admission.

## Source and bounded controls

Baseline is the current canonical src/save_checksum_131e8.c, including the
checkpoint122 identifier-list EEPROM calling contract. The older reader archive
used narrower prototypes; its conclusions are historical and were not used as
a substitute for the current source. No callee contract changes occur here.

The playfield result249 motivated real loop increment controls. An ordinary
`i = i + 1` in the first read loop reduces34 differences to30. The same source
form in the second/third loops worsens the match; compound increments reproduce
the baseline. An actual short offset used by the EEPROM call independently
reduces34 to30; combining it with the first ordinary increment gives26.

Consuming the buffer assignment inside the offset expression, for example
`offset = OFS(p = E.tdc[i]);`, preserves evaluation before the following EEPROM
call and adds no artificial uses or externally visible effects. The best source
uses this shape in the second and third loops. Together with the first ordinary
increment and real short offset temporary it gives16 differences. Applying it
also to the first loop gives exactly the same complete668-byte compiler output.
All offsets are within the real state object; loop counters cover0..18 or0..2.

The complete set contains baseline plus34 bounded controls: all combinations
of ordinary increments with/without consumed offsets, assignment-result
consumption, and four direct-address/early-step probes. Direct address arithmetic
changes helper inlining/context and worsens the result to209/212 reader bytes,
shifting retained dependencies; the early pointer step worsens to50. These
rejected controls never modify production. The later increment combinations
on the best consumed-assignment source do not improve16. Do not repeat them.

## Remaining exact locations

The best candidate is assigned_pointer_12.c. Its first-loop pointer/offset
roles still exchangeR10/R11 at133BC–133D0. Its third-loop counter/offset roles
exchangeR10/R11 at13402 and13426–1343A. The second loop matches. There are16
byte differences across15 instructions, with exact roots131E8/1326C/13378/13380.
The writer remains the unchanged88-byte near-match. No partial instruction or
loop match is counted as C coverage.

A linear disassembler initially stopped at an embedded literal pool; complete
byte-coordinate checks and per-instruction decoding were used to correct the
initial interpretation that the third loop matched. Every reported difference
is stored in SOURCE/OFFSET/ADDRESS/COMBINED/ASSIGNMENT/CONSUMED_CONTROLS.json.

## Compiler evidence

Five fresh Mac objects reproduce on Nuada with equal resolved bytes, roots and
relocations. Original driver, normal/diagnostic middle replays, sort tracing and
eligibility tracing preserve REG/ILB outputs. The initial hardware trace driver
selected internal index3, accidentally the neighboring setter: the inline diff
helper occupies an extra REG function entry. The verifier caught the incorrect
function count before acceptance. Those traces are explicitly excluded under
excluded_setter_traces/. Corrected traces select the final reader entry and are
checked against the reader's logical records and observed assignments.

The first ordinary increment raises that reader counter's priority7→9; changing
all three raises all three from7→9. Source/result traces isolate the improvement
without claiming that altered priorities alone solve the final physical roles.
The next useful investigation is the best candidate's pointer/offset allocation
and subsequent GEN materialization, not another loop spelling matrix. No full
production suite is rerun for rejected research controls; checkpoint249's clean
ROM builds and266-test gates per checkout remain the latest production evidence.

The corrected model reproduces126 incoming allocation candidates across the
five reader traces. Each compiler run appends one category2 record with
physical−2, zero priority/profit and no clashes after the observed allocation
phase. Those five records are checked separately; they are not counted as
predicted allocations. Reader symbol identity and exact record-address sets
are checked explicitly. All35 strict reader attempts reject. Five cross-host
object comparisons and20 unchanged original middle-pass replays pass.
