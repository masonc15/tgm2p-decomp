# Animation cached-short materialization — continuation138

Production remains checkpoint122: 128,316 / 200,698 bytes (63.93487%). No
production source, compiler, verifier, denominator, or runtime rule changed.
The preceding goal turn repeated the completed ROM verification and was no
progress toward matching C. This continuation adds causal compiler evidence and
rejects new source hypotheses; it does not earn matching-byte credit.

## Generator evidence

Case: `tools/shc_probe/cases/session_animation_codegen138/`.

Two independent read-only hardware-breakpoint passes replay SHCGEN for the135
baseline and137 signed_three / signed_three_legacy_call sources. Both record
identical emission sequences. Each traced replay's SUA and ASA outputs equal an
untraced replay from identical input. SUA also equals the full driver's captured
output. ASA does not equal the cleanup snapshot: symbol positions have advanced
in that snapshot. Exact differences are retained in the receipts, and no native
assembly identity claim is based on silently normalizing ASA.

Function binding checks both the raw symbol and function sequence. The initial
symbol check confused the raw index with SHCGEN's table index (+0xB6); the failed
attempt is retained. The tracer's original `line` field was actually the file
index at0x460628. It now records `file` and the source line at0x4605BA separately;
all three recipe traces were rerun with that correction.

Breakpoints: function entry4269A0, emission41B9D0, recipe selection420900;
a second pass replaces recipe selection with transfer423B40 and temporary-stack
allocation419010. Compiler executable bytes and configuration are unchanged.
Only debugger hardware registers are written. Target traces contain:

| Source | Emissions | Recipe events | Transfer events | Stack temporary allocations |
|---|---:|---:|---:|---:|
| baseline |562|220|106|0|
| signed_three |568|222|107|0|
| signed_three_legacy_call |568|226|108|0|

None emits an R9-to-memory transfer. Therefore the missing short home writebacks
are absent before the peephole pass; these candidates are not generating them
and subsequently optimizing them away. Zero calls to the temporary-stack
allocator also rules out that allocator as the source of their setup behavior.
This does not prove that every possible source form avoids stack materialization.

## Genuine helper and local-copy controls

Ten full-unit source builds and ten captured native builds cover eight distinct
sources. Two initial `nested_all` controls accidentally left the already-staged
loop expressions unchanged and are exact source duplicates of `nested_setup`;
the generator and verifier explicitly document and check that fact. They are not
counted as independent evidence. Subsequent `local_pointer_all` controls really
do replace both loop expressions, with occurrence assertions.

The nested helper takes a short by value and passes its address to a second
pointer-taking helper. The single-helper control uses a local pointer to its own
short argument. SHC reports warning1400 and does not inline the value helper.
Reachable direct calls to its anonymous emitted body remain in the native code.
Counting exported ELF functions alone would miss this: there are36 exported game
functions even when anonymous helper code exists. Verification now checks the
reachable call targets against known function roots as well.

The frontend exits5 in these successful full driver runs, rather than the usual1.
The first replay rejected that status and its failure logs are retained. The
replay now derives the expected frontend status from the corresponding captured
process trace, requires that exact status, and still requires byte-identical
REG/ILB outputs. All20 normal/debug middle replays pass.

The scoped-reference form is different: each of the three setup operations makes
a real short copy of spr2 and passes its address to the previously supported
pointer helper. It inlines without warnings or extra calls. Its emitted stores
and reloads target three different local homes, and spr2 is not cached inR9.

| Source | ROM entry | Body bytes | Frame bytes |
|---|---:|---:|---:|
| nested_setup_typed / duplicate nested_all_typed |A624|1694|104|
| nested_setup_legacy / duplicate nested_all_legacy |A624|1692|104|
| local_pointer_setup_typed |A624|1694|104|
| local_pointer_setup_legacy |A624|1692|104|
| local_pointer_all_typed |A624|1676|104|
| local_pointer_all_legacy |A624|1674|104|
| scoped_ref_typed |A604|1750|116|
| scoped_ref_legacy |A604|1790|120|
| Original |A608|1748|108|

All fail the unmodified strict contextual verifier. No moved or relative byte
comparison is credited. Native CFG stack audits verify all ten candidates.

For scoped_ref_typed, spr2 is at28 and the three copies at56/60/64. For the legacy
variant, they are32 and60/64/68. The legacy variant retains holdR10/mask15R9/
constant2R8/total-stack, but its120-byte frame and home stores remain wrong.
Two additional SHCMDL hardware traces show four spr2 candidates with use counts
2,1,2,1 and no cache allocation. The minimum-three-use gate explains why the
scoped copies do not preserve the desired R9 cache.

## Next action and validation

Retain134 as the register-layout control and137 signed_three_legacy_call as the
cached-conversion control. Do not repeat address-taking of inline value arguments:
that family has explicit compiler rejection evidence. The next causal question
is why the indirect scoped-copy operations split spr2's use chain into four
candidates, compared with137's successful three-use chain. Examine those chain
boundaries and alias invalidation before proposing another source transformation.

Evidence: `GENERATOR_EVIDENCE.json`, `BLOCK_EVIDENCE.json`,
`NATIVE_EVIDENCE.json`, `REGISTER_ROLES.json`, `SLOT_MAPS.json`,
`CANDIDATE_STACKS.json`, and the raw trace/capture files. Verification covers10
compiles/captures (8 unique sources),20 exact middle replays,8 hardware replays,
and10 CFG stack audits. All44 production pins,23 prior frozen cases, original ROM
hashes and unchanged progress verify in both Nuada checkouts. All23 copied ROM
files still match their recorded source. No full build/test rerun was needed for
these private rejected probes; checkpoint122 remains the complete production gate.
