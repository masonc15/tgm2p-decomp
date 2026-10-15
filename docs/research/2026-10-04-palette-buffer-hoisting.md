# Palette buffer hoisting — continuation203

No new production C bytes qualify. Production remains checkpoint192:
131,780 / 200,698 bytes (65.6608436556%), with 68,918 remaining including all
1,638 runtime bytes. The full goal stays active; work remains solo.

The preceding ROM-recheck turn added no new decompilation evidence, so its next
action was revalidated against the live Nuada report and continuation202 sources.
This turn advances the compiler investigation: it identifies and observes the
specific hoisting gate responsible for the three conflicting buffer addresses.

## Directly observed cause

Fresh baseline and integer-buffer native captures reproduce continuation202.
All tracing uses hardware execution breakpoints without modifying compiler
instructions, IR, allocation tables, output, or production flags. Fresh untraced
SHCMDL controls and saved native REG/ILB outputs equal every replay's output.
The SHCMDL SHA-256 remains
`181b111ac20a19871a04a3a29e24a679f4b4376648dc79d6a4fc17bf39f1bb60`.

The common-expression extractor at40E580 creates the first12 temporaries in
both sources. Baseline buffer writes at source lines63 and53 are instead hoisted
by41C180, which creates-13 and-14 through40AC50 with return address41C476.
The baseline short-copy buffer pointer becomes-15 later through the ordinary
common-expression extraction return40E788. In the integer-buffer source all
three are hoisted: line67->-13,63->-14,53->-15. Whole-function snapshots before
late optimization and before allocation preserve these distinct assignments.
The complete allocation graphs confirm their mutual conflicts.

The write expressions have a nested conversion: array-address type144 to signed
integer type16, then unsigned type28 for arithmetic with unsigned-long i. The
hoisting gate41C427 rejects a conversion whose direct child is a variable or
literal (op70/op71), but the outer conversion's direct child is another
conversion (op20). Native observations at41C471 and41C476 show it passing this
gate and creating the temporary. The raw loop-record field+0 is recorded as
loop_id; no claim is made that it represents nesting depth.

Direct `(unsigned long)buf` conversions have an op70 child. All six observed
gate visits reject hoisting. Three later common-expression temporaries still
exist, but their lifetimes no longer all conflict. They spill with priority6
rather than producing one shared retained address. The local stack rises to80
bytes and the predecessor expands to1312 bytes. Avoiding hoisting alone is not
an improvement. The ROM requires68 local bytes and1186 predecessor bytes.

## Source controls

All ten sources are genuine complete palette contexts, including the following
initializer and four already-C functions. None starts the initializer at2A576;
the unchanged strict checker rejects all ten. No retained object is published.

| Source | Predecessor bytes | Initializer root | Local stack bytes |
|---|---:|---:|---:|
| baseline |1174|2A56A|76|
| buffer_int_arithmetic |1192|2A57C|68|
| buffer_ulong_writes |1316|2A5F8|80|
| buffer_ulong_all |1312|2A5F4|80|
| base_after_guards |1302|2A5EA|76|
| base_after_guards_hinted |1302|2A5EA|76|
| core_hints_long |1174|2A56A|76|
| core_hardware_long |1178|2A56E|72|
| core_hints_short |1176|2A56C|76|
| core_hardware_short |1182|2A572|72|

Naming one unsigned buffer base after the two early-return guards removes all
three generated buffer candidates. There are43 allocator candidates instead of46,
and only one candidate for the actual base variable61. Its ordinary priority19
puts it inR8 in the baseline-hint context; hinting the base puts it inR4 and
perturbs the other homes. Neither is a native match. This is distinct from202's
named signed/char bases initialized before the guards.

Fixed-graph exploration of ordinary hint subsets covers32+32+64 combinations
for baseline, integer-buffer, and named-base contexts. None predicts every
requested home. These are diagnostic predictions only, not native code credit.

Four native core-variable controls keep the named base, hint o/k/i/v, and remove
the pals hint; i is tested as unsigned long and unsigned short. Two also use the
actual hardware constant directly rather than a pals local. All four put o inR6
and k inR7, as the ROM does. They still put v inR4 and the first two i lifetimes
inR5; later i lifetimes shareR4. The shared base spills. Thus a nearby initializer
root in one control does not imply a near-matching body.

## Verification and next action

`session_gfx_extract203/verify.py` authenticates10 strict negatives (8 new source
forms and2 carried controls),20 output-preserving replays/21,783 events,9 complete
allocator models and10 independently assembled native captures. Analysis also
verifies temporary creation provenance, the hoist gate, the changed interference,
and actual core-variable homes. Two harness issues are recorded in
HARNESS_CORRECTION.md: a C printf type warning corrected before tracing, and an
overstrict nonempty-event assertion for a source that legitimately hits no
watched hoist-expression hooks. Zero events are allowed only for that named
control; normal exit and all output equality checks remain required.

No production source, split map, verifier, target ROM, compiler, or denominator
changed. Checkpoint192's independent clean builds and253 enabled tests remain
the applicable full gates; no full suite was rerun for excluded diagnostics.
Fresh manifest,54 production-pin,report and ROM hash checks cover both Nuada
checkouts before the checkpoint is published.

Next trace the counted uses and conflicts of the first two i lifetimes versus
packed v in core_hardware_long/core_hardware_short. Their symbols are i57,v59,
o55,k56,base60; target function53 is REG record0. The existing register order is
already wrong in SHCMDL, so do not pursue assembler scheduling for that issue.
Do not repeat buffer casts or named-base placement without new evidence.
The best initializer remains201's119-byte difference at2A56A; no new source here
improves that result.
