# VM row-address generation and scratch cache — checkpoint233

Production remains checkpoint225: **135,594 / 200,698 bytes (67.56121%)**.
All eight context probes fail the original-address EC0C–EF04 gate. No new C
credit;65,104 bytes remain, including1,638 runtime bytes.

## The trace corrects the previous scratch hypothesis

The baseline is checkpoint232's predicted_chain_0231 source. GEN emits the
row-address addition as ADD R1,R3, stores line from R3, and then chooses R2 for
the row*29 product. At that selector call, **last=R0 and recent_mask=0**.
The earlier conjecture that R3 was merely penalized as recently used is false.
R3 and R1 are occupied; R2 is empty. The recovered native selector assigns
ranks R3=4, R2=2, R1=4, R0=3 and selects R2. R3 has just held the completed
line address. The rank result, occupancy state and selected register are directly
observed, not inferred from the assembly alone.

The existing scratch selector at0x418900 is observed with hardware execution
breakpoints and modeled independently. Every one of1,253 selections in eight
controls matches the model. Separate emitter tracing correlates each relevant
selection with the next row load and MULS, using the emitter count recorded by
the scratch observer. The native compiler and all inputs remain unmodified.

## Complementary source controls

| Form | Row-address addition | Next product | Context bytes |
| --- | --- | --- | ---: |
| baseline / array indexing | R1 into R3 | R2 |744|
| seed line, then += offset | R2 into R1 | R2 |748|
| seed line, expanded assignment | R1 into R2 | R1 |748|
| named char offset | R2 into R4 | R1 |744|
| named base, ordinary addition | R4 into R3 | R1 |744|
| named base, += offset | R3 into R4 | R3 |744|

Seeding line introduces a real intermediate stack store/reload. Removing the
explicit address-taking of line leaves the same native bytes. Naming an offset
or a base for ordinary addition adds an allocator candidate and changes homes.
The expanded and compound seeded forms have different native bytes, including
line/col placement, despite similar source semantics.

`named_base_accumulate.c` restores the R3 multiplication result and direct
SUB R3,R9 without a copy-back. At its selector call, R3 and R1 are empty with
rank2, R2 is occupied with rank4, and R0 has rank3. Descending scratch scan order
chooses R3. The row base, however, is a named variable assigned R4, and literal29
uses R1 instead of original R0. Its44-byte frame survives but letter0/letter3
homes are exchanged. It remains a complementary diagnostic, not a replacement
for the baseline's fully recovered stack layout. EE9C still lands18 bytes early.

The original row-address addition is ADD R3,R1 and the following product stays
in R3. A successful source must produce the base accumulator in scratch R1
without the seeded line's extra memory accesses or the named base's allocated
R4. The measured controls isolate this choice more narrowly than generic
register or declaration permutations.

## Validation and limitations

Six validation commands pass. Eight allocation/negative-coloring models, complete
layout models and home models cover82 records. Twenty-four unchanged middle-stage
replays contain4,960 events (graph1,031; layout3,823; homes106). Eight captures
independently assemble to their strict compiler object bytes.

Sixteen GEN replays record2,380 emitter events and1,253 scratch selections.
Every traced SUA/ASA output equals its fresh untraced control. SUA also equals
the native cleanup capture; recorded cleanup ASA metadata differences are
independently checked. Emitter and scratch runs agree with each other. This
establishes output-preserving observation, not matching acceptance.

Only baseline/array_index and line_seeded/line_unaddressed_seed are recorded
byte-identical pairs. All context objects, source hashes, original boundaries,
strict rejections, compiler hashes and replay identities are rechecked.
Production source is unchanged, so full suites were not rerun. Checkpoint225's
64 pins, ROM hashes and two266-test clean validations remain authoritative.

## Next action

Keep baseline and named_base_accumulate as a measured pair. Inspect the GEN
addition recipes and middle-stage expression classification that distinguish
an unnamed base accumulator from the allocated row_base variable. Preserve the
original source semantics, saved roles and whole-context gate. Per-row width
reloads, initialization order, shared color paths and tail boundary also remain
unresolved. Case: tools/shc_probe/cases/session_vm_rowaddr233. Goal active, solo.
