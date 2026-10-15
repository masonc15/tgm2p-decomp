# Controller constant allocation — continuation182

The previous turn made diagnostic progress by recovering the initializer and
pointer homes. This continuation tests the resulting constant-allocation hypothesis.
Production remains130,324/200,698 bytes (64.9353755394%), with70,374 remaining.
No production source, split, proof, compiler flag or denominator changed.

## Native controls

All26 controls reject the unchanged strict verifier. Independent original-ELF
relocation resolution reproduces every linked byte. All13 roots remain nominal
and all1236 protected renderer bytes remain exact. Context lengths vary, so root
identity alone does not establish the required end boundary.

| Source family | Differing bytes / context bytes |
|---|---|
| Consumed short initializer baseline |146 /4392|
| Shared right frame named as external object |455 /4380|
| All five frame addresses named |489 /4392|
| Short coordinate local, or same local reused for label coordinate |526 /4404|
| Long coordinate local; branch coordinate assignment |146 /4392, identical baseline|
| Shared frame pointer local |516 /4396|
| Coordinate local initialized each iteration |336 /4400|
| Unsigned short or unsigned long timer, explicit signed16 comparison |35 /4392|
| Unsigned flag; reversed declarations |146 /4392, identical baseline|
| Four individual left-frame symbols, then all four together |146 /4392, identical baseline|
| Increment flag; Boolean inverse flag |157 /4392;486 /4396|
| Early exit around timer block; explicit outer-loop label |523 /4364;529 /4368|
| Two-side drawing loop in case0/1/3; all three cases |535/4432;545/4436;530/4432;484/4524|

The35-difference timer controls are not byte-identical to the previous best.
They restore the same17-byte initialization mismatch and wrong input homes, so
this is no advance. The increment/Boolean flag forms preserve behavior on the
zero-flag path: the preceding nonzero-flag switch always returns. Neither matches.
The side-loop controls preserve call order, coordinates and frame addresses;
SHC retains a loop (e.g. BF/S1315C to13120 in side_loop_0), instead of reproducing
the required drawing sequence. No extra drawing calls or fake consumers were added.

## Four output-preserving allocation traces

Fresh SHCMDL traces capture408 events. REG/ILB match fresh untraced controls and
native intermediate captures. The native ELF sections match the separately
compiled source objects. The read-only model reproduces all candidate profits,
partition order and positive register assignments for33/33/28/29 candidates.
Negative spill-slot numbering is observed, not independently modeled here.

- Baseline: the shared frame0xA767C and coordinate134 have equal priority12 and
  profit5; frame getsR11 and134 getsR10, opposite ROM. Coordinate155 getsR9.
- Short coordinate local: numeric literal134 is replaced by a real variable at
  priority7/profit0. It spills; timer moves toR9 and zero occupiesR8. This is why
  replacing the literal by a local cannot simply repair the tie.
- All frame symbols: five numeric-address candidates disappear. The shared frame
  no longer participates in this saved-register allocation, causing broader changes.
- Left-only symbols: four one-use numeric-address candidates disappear, leaving29,
  but the full emitted context is identical to baseline. Changing candidate count
  alone does not resolve the relevant tie.

No compiler memory/instructions or backend tables were changed. No physical
register directives, padding, dead consumers or relaxed comparisons were used.

## Next target

Park these controller families. The authoritative report contains apparent small
functions that earlier original-object audits identify as interior labels; do not
count them as fresh helpers. A concrete remaining complete body is board clear
f278C0–27B50 (656bytes), still ASM inside277D0–27E70. Its exact historical source
and original-object receipt report22 differing bytes, concentrated in setup, with
matched loop structure. NEXT_TARGET.json pins that source and the follow-up:
fresh compilation, independent relocation resolution, then setup operand/result
tracing using the current compiler-reconstruction tools. Preserve the source's
historical standard C register qualifiers; do not introduce physical bindings.

Evidence: tools/shc_probe/cases/session_controller_constants182/RESULTS.json,
GRAPH_REPLAY_RESULTS.json, GRAPH_MODEL_RESULTS.json, verify.py and FREEZE.json.
All50 production pins and original ROM/report hashes remain exact in both Nuada
checkouts. No full build/test rerun was needed for these excluded research sources;
checkpoint179's249-test run remains the last full suite. Literal100% stays active.
