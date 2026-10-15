# Effect callee and inline-return lifetimes — continuation238

Eight complete native contexts remain strict negatives. Seven are new source
controls and one reproduces frozen117 reuse_int_x byte-for-byte. Production
remains135594/200698 (67.56121%); no production source or split was changed.

## Source experiments and exact comparisons

The target is f_20a9c,136 bytes, following248 already-retained bytes. All eight
controls retain the three nominal function roots. Source changes are confined
to this target and meaningful inline helper definitions. Pointer types and
arguments are preserved. The int-return helper computes the same expression;
height0..255 and signed-short y introduce no overflow in the y calculation.

| Control | Context bytes | Target differing bytes | Protected differences |
|---|---:|---:|---:|
| frozen117 baseline |384|21|0|
| callee pointer at entry |388|122|12|
| callee after sx |388|103|12|
| callee after sy |388|78|12|
| direct argument, bound height first |388|124|12|
| direct argument, bound products first |388|123|12|
| full inline helper, named result |396|126|12|
| full inline helper, direct argument |384|29|0|

Counts cover fixed original addresses; extra context bytes are separately
recorded and rejected by the original boundary gate. Equal lengths never imply
matching. Every source fails the default validator, with no retained object.

The first three function-pointer controls all spill the drawing pointer and
add a four-byte stack slot, while retaining sy in R4. The preloaded-height
argument controls retain the height value in R5 but move other live values to
saved registers; removing named sy does not recover the original frame.
These results close the exact source families recorded in HYPOTHESES.md.

The full arithmetic helper differs from69's single-field inline accessors.
Its named-result consumer emits the desired ADD R3,R0 but spills sy and retains
extra saved-register traffic. Its direct-argument consumer recovers the original
four-byte frame and shared literal positions, but arithmetic still uses R4 and
its pointer uses R1 instead of original R6. The21-difference117 reference remains
better overall. The29-difference helper is a distinct diagnostic control, not
a decompilation gain.

## Native mechanism

Sixteen output-preserving SHCMDL replays expose the allocation records, with142
independently checked priority/profit calculations. Eight original SHCGEN emitter
replays preserve native SUA and untraced SUA/ASA, recording1275 emissions (339 in
the target). Cleanup-snapshot ASA position differences remain explicitly recorded.

Eight late-remapping traces capture25 events. The helper-argument control alone
maps R14 to R1 over compiler live positions3..8. This matches its emitted R14
pointer becoming R1 in final native assembly and removes the saved-register
cost. The other seven contexts append no mappings.

The102 normalization model originally covered only unflagged records; these
controls also contain parameter flag0x40. Native instructions and the inferred
41AC70/41AF40 listings corroborate signed positive-register merging, union of
overlapping/adjacent ranges, and propagation of0x40. The extended scoped model
predicts all51 sorted records, all eight mapping decisions and final free lists.
It excludes special flag0x80 paths. The first attempt to reuse the old model
correctly rejected this wider flag domain; no assertion was relaxed without
adding the observed merge behavior and checking original compiler instructions.

For the baseline, f and sy share R4 and their merged live range is1..7. Ordinary
late remapping considers saved registers8..15; it cannot turn this R4 result
into R0. The helper-argument alternative also leaves its result in R4. This
identifies the next question more narrowly: how inline-return/result WEB binding
chooses between the spilled helper result, allocated R4 result, and a scratch
value. Investigate that binding using the two fixed helper controls before
another source search. No register hints, artificial uses or patched compiler
state were introduced.

## Verification

Both validation commands pass. All eight assembly captures independently
reassemble to their ordinary strict-compile objects; relocations are independently
resolved, roots and fixed-ROM comparisons checked. Sixteen middle replays and
sixteen GEN tracing replays preserve their required native/control outputs.
Research files are frozen after validation and mirrored to both Nuada checkouts.
The last full production gates remain checkpoint225's two clean builds and266
passing tests per checkout. This research does not claim a new full-suite run.
