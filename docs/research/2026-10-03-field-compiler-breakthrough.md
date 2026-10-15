# Field scan: zero-test history resolves address registers

Checkpoint118 adds the complete328-byte field scan at81CC–8314. The authoritative
matched total is128316/200698 (63.93487%),599/784 functions and161/205 units.
72382 bytes remain, including all1638 runtime bytes. Literal100% remains active.

The unchanged SHC compiler produces an exact844-byte context. Only328 newly
retained bytes count; the neighboring516 bytes were already retained. The new
production source is `src/field_hole_81cc.c`. The existing retained-suffix source
`src/field_81cc.c` is unchanged. Both source contexts describe genuine functions.

## Experiment and result

The original candidate has two next-row address sequences with28 differing
bytes. Historical operand commutation alone leaves42 differences. Fresh GEN
traces showed the cell-base calculation occupied a scratch register before the
offset calculation, while the preceding zero comparison also changed hidden
scratch history. The next experiment therefore varied that preceding condition.

| Source control | Context bytes | Target differing bytes | Result |
|---|---:|---:|---|
| Existing baseline |844|28|Rejected|
| Historical offset-first address |844|42|Rejected|
| Current-cell logical NOT only |844|26|Rejected|
| Offset-first address and current-cell logical NOT |844|0|Accepted|
| Inverted current-cell branches |820|291|Rejected: boundaries also move|
| Named current-cell flag value |848|214|Rejected: boundaries also move|

Each of the first four preserves all516 following bytes and all four nominal
function roots. The logical-NOT change is applied to the two current-row cell
checks, not the later next-row cell checks. The positive source uses:

```c
if (!f->cells[y * f->width + y].flags) {
    if (((struct cell *)(((((y+1)*f->width)+y)*6)
        + (long)&f->cells->flags))->flags != 0) n++;
}
```

The analogous second branch substitutes `k` for the final column index. These
are actual consumed conditions and address expressions, with no dummy consumers,
forced physical registers, added padding, compiler patches or verification changes.
Logical NOT and equality to zero have the same C truth value here; address
operands have no side effects. This is a recovered matching representation, not
proof of the original spelling of the source.

## Observed compiler mechanism

For the two offset-first controls, the first67 GEN emissions have identical
operations and operands. The test at event65 has size metadata2 for equality and1
for logical NOT; the resulting native instructions before8260 are identical.

At the first next-row scratch allocation, returning to421159, the equality form
has last-register3 and selectsR2. The logical-NOT form has last-register255 and
selectsR3. Exclusions, preferences, ordering, recent mask, used mask, mode and all
four scratch value/flag/age records otherwise match. The recovered selector
model ranksR3 at3 in the equality state and2 in the NOT state; R2 andR1 rank2 in
both. Descending selection consequently choosesR2 versusR3. A fixed-state model
counterfactual changing only `last` predicts the observedR3 choice.

This rotates the later width scratch toR2 and keeps the offset inR3. Loading the
base later selectsR1; adding baseR1 into offsetR3, then loading the cell word into
R2, reproduces the original sequence. The base-first NOT control gets part of
the history right but still uses the wrong address destination. Both changes are
necessary, as the two independent negative regression tests demonstrate.

The live history trace also distinguishes the zero-test recipes: equality has
an additional recipe and scratch selection per modified check. Thus an emitted
instruction sequence can match while leaving different internal history that
changes the next expression. That observation is the transferable result.

## Verification

`session_field_recipe118/analyze.py` binds the six native controls to source
hashes, linked objects, raw differences, function boundaries and default prefix
validation. Four separately captured compiler objects match their native probe
objects outside comment metadata. Eight normal/debug middle replays reproduce
captured `.reg` and `.ilb` exactly. Twelve hardware replays preserve `.sua` and
`.asa` against untraced controls; `.sua` also matches native capture. Cleanup
snapshot `.asa` metadata differences are recorded and are not called equal.

The recovered selector predicts all616 observed scratch choices; the recipe
mask model predicts all308 observed history updates. The four target streams
contain580 emitter events. Tracing writes CPU debug registers only and does not
patch compiler bytes or compiler input data. The middle debug control changes
only its documented diagnostic-output flag.

The production prefix passes ordinary verification with two outside literal
loads, both referencing the original `00de` value at83CE. It uses no optional
tail/switch proof. Its retained object exports only `_f_81cc`, contains exactly328
ROM-matching bytes and has no unresolved relocations. Full native context roots
remain81CC,8314,837A,8430.

Two independent clean `make split check report` builds pass. All243 enabled tests
pass with zero skips in the isolated checkout. Canonical and isolated41 production
pins, reports, linked native contexts and retained objects match. Original
compiler objects differ in5 bytes, all within `.comment` metadata. Both original
EPROM SHA1s and combined ROM SHA1/SHA256 remain unchanged:

- Combined SHA1: `c84cf538962ddbb377290301d76d75d8b542e5a2`
- Combined SHA256: `d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7`
- `2b.u21`: `783e8413b11f1fa08d331b09ef4ed63f62b87ead`
- `1b.u22`: `3f7e81756470c173cc17a7e7dee91437571fd0c3`

Integration receipts and build/test logs are in
`tools/shc_probe/cases/session_checkpoint118_field_recipe/`. The preceding117
compiler research and115 integration checkpoint remain immutable. No new ROM
was required. Next, use condition-recipe history as an explicit diagnostic for
other scratch-register near-matches, while retaining the closed-source controls.
