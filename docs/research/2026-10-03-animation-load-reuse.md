# Animation load reuse and one-ID brightness helpers — continuation151

Production remains checkpoint122: **128,316 / 200,698 bytes (63.93487%)**.
No new matching bytes. Continuation150 was progress because it established
address-taking and loop-hoisting distinctions that changed the next experiment.
This continuation closes the immediate late-cache hypothesis and tests a source
shape suggested by the original brightness argument loads.

Evidence: `tools/shc_probe/cases/session_animation_loadreuse151/`.

## The second brightness load is present before peephole optimization

Four read-only SHCGEN replays of frozen150 controls record2,321 emissions and962
recipe decisions. Each traced SUA/ASA equals an independent untraced replay;
SUA also equals the original full-driver snapshot. ASA cleanup differences
remain explicitly recorded, as in148; no normalization is used to claim equality.

|150 control|Brightness word loads|Shift-by-two operations in brightness region|
|---|---:|---:|
|direct_address|2|6|
|direct_address_preload|1|3|
|shared_explicit|1|3|
|direct_value_preload|0|0|

The ordinary preloaded control has already hoisted its scale outside the loop.
The shared-pointer load uses transfer opcode64, while the direct variable load
uses opcode39. The first analysis incorrectly counted only39; the final audit
checks both opcodes, word size and memory-to-register operands, independently
of the native CFG audits.

## Late leaf-cache observations

`trace_cache.c` uses hardware breakpoints at function entry4269A0, leaf insertion
42AB50, lookup42B430 and each observed return address. It verifies exactly one
raw-symbol function binding and every return stack. It writes only debug
registers, reads the original compiler memory and preserves stock SUA/ASA output.
The generator trace additionally snapshots the four24-byte integer cache records
at460540 before each emission.

`model_cache.py` reproduces **all737 lookup results** from the captured symbol,
logical-register ID, type byte and flag64 comparison. It observes133 insertion
attempts. These traces contain no floating leaf query; the model does not claim
to cover floating cases. The corresponding inferred compiler routines are
copied under `compiler_evidence/`, explicitly labeled as decompiler output.

For direct_address, spr1 lookups at the two brightness expressions both return-1.
No spr1 insertion attempt occurs for either load. The sole spr1 insertion attempt
in each whole-function trace is its initial assignment at source line370.
Every cache snapshot across the brightness emissions lacks a reusable spr1 leaf.
The first field store therefore is not invalidating a successfully cached spr1
value on this observed path: it never became a reusable leaf there. This finding
does not rule out other generator paths or claim a universal cache rule.

## Source experiment suggested by original argument loads

The original loads both brightness values before loading spr1, then computes one
sprite address and uses it for both writes. A void inline helper taking one ID
and two consumed brightness values is a plausible source shape, not proof of the
original source. Four full-unit controls test ID by value or pointer, with either
two explicit field-address expressions or one consumed local sprite pointer.

|Control|Body bytes|Frame|spr1 frequency/home|Static spr1 reads|
|---|---:|---:|---|---:|
|brightness_value|1750|108|7 /36|4|
|brightness_pointer|1750|108|7 /36|4|
|brightness_value_sprite|1752|108|7 /36|4|
|brightness_pointer_sprite|1752|108|7 /36|4|

All four inline successfully with36 exported game functions, no anonymous local
calls, six loops and no inline-return gotos. The two `_sprite` forms compute one
sprite pointer and retain all six desired roles: holdR10, mask15R9, const2R8,
total on stack, header countR10 and setup spr2R9. Their two brightness stores use
the same R4 address. The original uses R6; spr1 must be home16 rather than36.
The two forms without that local pointer instead allow the global sprite base
to compete for a persistent register. The final native paths, not just body
lengths, are recorded in `HELPER_REUSE_RESULTS.json`.

All four still beginA604 rather thanA608 and fail the unchanged contextual prefix
verifier. A body being two or four bytes longer is not a measure of closeness to
full byte identity. No moved-byte, source-only or assembly credit is awarded.

## Validation and next action

Four stock native compiles/captures, eight exact middle replays, four CFG audits,
four home hardware traces plus eight generator/cache hardware traces pass.
Home models reproduce108 records and548 weighted contributions. Both Nuada
checkouts retain44 production pins,36 earlier frozen cases, original ROM hashes
and all23 copied reference files. Full production build/test gates remain122's
independent clean builds and243 tests; no production source changed.

Preserve `brightness_value_sprite` and `brightness_pointer_sprite` beside150's
home16 direct-address control. Do not continue assuming the first byte store
invalidates a cached sprite ID. The next useful source/compiler boundary is the
one-ID helper's argument binding and live ranges: it now preserves the desired
register roles and shared calculation, but assigns brightness/ID temporaries to
R6/R5/R4 instead of the original R4/R5/R6 and leaves spr1 at36. Inspect those
bindings and spill groups before changing source again. The separate preceding
function boundary mismatch remains required work.
