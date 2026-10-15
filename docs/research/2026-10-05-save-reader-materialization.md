# Save-reader materialization — continuation251

No new C coverage. The best complete reader13380–13484 still differs by16 bytes.
Production remains checkpoint249:135,910 /200,698 bytes (67.71866%). This study
locates the remaining decisions before GEN emission and closes additional type
and helper forms; it does not claim that a hypothetical priority change is a
source solution.

## The two remaining swaps are already in the middle-pass output

Fresh original-driver captures authenticate both the canonical baseline34 and
the best candidate16 from continuation250. GEN's reader is emitted function4,
symbol78; the inline diff helper exists in the middle-pass function list but
has no emitted function. Reader identity is checked explicitly.

The MDL diagnostic allocation numbers map to serialized physical registers as
n+3 for incoming bank1..4 and19−n for saved bank5..11. All38 positive records
across the two controls agree with this map, symbol aliases and serialized REG
ordering. The initial GEN live-range table exactly matches serialized REG,
including ranges and signed physical values. No later saved-to-scratch mappings
are emitted in either reader: all six recorded remap snapshots have empty maps.

For the best source, the relevant dataflow is:

| Role | Native/GEN | Original ROM | MDL internal before → required |
|---|---|---|---|
| First-loop buffer | R11 | R10 | 8 →9 |
| First-loop offset | R10 | R11 | 9 →8 |
| Third-loop counter | R11 | R10 | 8 →9 |
| Third-loop offset | R10 | R11 | 9 →8 |

The source's first counter and second loop already have the required roles.
The emitter confirms the wrong first buffer copy R12→R11, first offset copy
R11→R10, third counter initialization R9→R11 and third offset copy R9→R10.
These are not late remapping substitutions or final scheduler choices.

## Fixed-graph sensitivity and actual source results

Using the authenticated27-candidate graph from250, the independent model
reproduces the observed allocation. Four desired WEB changes have no graph
conflict. None of800 single-priority trials produces the complete target.
Four of294 bounded paired trials do, including first-buffer priority8→6 with
third-offset6→7, or third-counter7→5 with first-offset6→8. These are offline
models only; compiler inputs, executable code and output objects are never
modified to implement those changes.

A repeated all-assigned-pointer control genuinely lowers first-buffer priority
8→6, but its output remains the same16-byte near-match. A wide-return helper
lowers the third offset's priority6→4, rather than the required7, and adds code.
A short third-loop counter retains priority7 and also adds code. These observed
responses rule out those proposed mechanisms for realizing the model solution.

| Source control | Context bytes | Reader differing bytes |
|---|---:|---:|
| All consumed pointer assignments (prior control) |668|16|
| Typed short buffer pointer |668|16|
| Wide offset carrier |664|223|
| Wide carrier plus all assignments |664|224|
| Third wide-return helper |672|101|
| Wide-return helper plus all assignments |672|92|
| Short third counter |672|81|
| Reversed helper parameters, first loop |668|16|
| Reversed helper parameters, third or both loops |668|23|
| Unsigned-short third helper |672|92|
| Buffer derived from consumed short or wide offset |672|87|

There are13 compiled controls in the table (two rows each combine two equal
results), plus fresh baseline and best-source controls. Both pointer-derived
buffer forms consume the actual byte offset within the same state object;
there are no added dummy uses. Long/unsigned-short offsets fit the actual0..250
range; the short counter only reaches3. All reader proofs reject unchanged.
The shrinking wide-carrier controls also break retained setter dependencies.
Do not repeat this type/helper/increment family without a distinct, measured
source mechanism. The useful next work must produce the paired priority change
while preserving instruction shape, or move to another genuine unmatched target.

## Evidence and limits

Five native object comparisons, ten exact normal/diagnostic middle replays,
four original GEN hardware replays,162 reader emission events and six remap
snapshots pass verification. Traced GEN SUA/ASA equal their freshly regenerated
untraced controls. SUA also equals the original driver capture; the cleanup ASA
snapshot differs at76 metadata bytes in each replay, which is recorded explicitly
and is not claimed equal. Compiler SHA-256 pins cover all20 executables.

`tools/shc_probe/cases/session_save_materialization251/verify.py` authenticates
source/object hashes, original roots/relocations, all13 strict rejections,
positive physical bridges, emitter roles, unchanged replays and the offline
model. `PRIORITY_TARGETS.json` contains the precise paired hypotheses. Existing
production remains unchanged; the latest full production gates are249's two
clean ROM builds and266 tests per checkout, not a newly run suite here.
