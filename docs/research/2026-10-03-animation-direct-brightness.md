# Animation direct brightness expressions — continuation156

Production checkpoint122 remains **128,316 / 200,698 bytes (63.93487%)**.
Continuation155 made progress by explaining the brightness-address profit
threshold. This turn removes helper argument copies while leaving brightness
locals ordinary. Three native controls remain strict rejects; no C credit.

## Consumed direct source

`prepare.py` starts from frozen154 pointer_initializer, removes the brightness
helper definition/pragma, and replaces its one call with an ordinary local scope.

- `direct_pointer`: one sprite pointer initialized from the addressed spr1 load,
  followed by the existing b14 and b15 stores using bri0 and bri1 directly.
- `direct_values`: the same, with two consumed byte copies before the address.
- `direct_index`: the same as direct_pointer, with one consumed int ID copy.

All use the unchanged native compiler/flags and genuine36-function context.
No dummy consumers, padding, forced registers, compiler changes or relaxed
comparisons are involved. Each emits1744 bytes, frame104, beginsA604, and fails
at the unchanged contextual-boundary gate. The original requires1748/frame108,
with entryA608. Six loops, zero inline-return gotos and no anonymous calls remain.

All retain four static spr1 reads, addressed frequency7/home32. Brightness locals
remain ordinary frequency2; their homes are80/84, versus original96/100. Total
again receivesR8; constant2 getsR9 and constant15 loses its persistent register.

## Allocation and code generation

Three same-process hardware traces preserve captured REG/ILB and reconstruct
258 candidate assignments,894 eligibility events and9 appended memory records.
The graphs have85,87,86 candidates respectively. In every graph the explicit
sprite pointer has priority6/profit-3 and receives internal1, hardwareR4.
The direct index copy has priority4 and sharesR4 with its successor pointer.
In direct_values the byte copies have priority4/profit-5 and receiveR5/R6.

The two important constants remain priority16, but mean clash priorities are
floor(677/74), floor(685/76), and floor(681/75): all9. Their profit7 ties total,
consistent with the observed frame reduction. Removing argument copies therefore
does not preserve154's constant-register advantage.

Three independent generator replays reproduce their untraced SUA/ASA exactly;
SUA also matches the full-driver capture. ASA cleanup metadata differences remain
explicitly recorded. The traces contain1716 emissions and708 recipe events.
Each brightness region contains12 emitted operations: one word load, three
scale shifts, two brightness loads, and two field stores among address setup.

| Source | Byte load/store registers | Sprite address |
|---|---|---|
|direct_pointer|R2 thenR1|R4|
|direct_values|R5 thenR6|R4|
|direct_index|R2 thenR1|R4|
|Original ROM|R5 for b14, R4 for b15|R6|

The native CFG confirms the traced stores at offsets20/21. Removing byte copies
uses scratch registers, not the original byte-register mapping. Adding real byte
copies reproduces the prior wrong allocation. The address's early R4 binding is
already established in the middle pass; this is not a late scheduler surprise.

## Exact comparisons and validation

Direct values differs from direct pointer by73 allocated text bytes; direct index
by10. All rodata is equal. The initial expectation of complete direct-index
identity failed and is preserved in INITIAL_SECTION_IDENTITY_FAILURE.log. Its
brightness instructions agree, but wait/lastpos homes swap40/36 to36/40. The
corresponding ten offset bytes elsewhere in the function differ. Equal body sizes
and equal mismatch counts did not establish native identity.

Three native builds/captures, six middle replays, three CFG/stack audits, three
home traces, three allocation traces and three generator traces pass. Home models
verify78 records and381 frequency contributions. Production recheck verifies44
pins and41 earlier frozen cases in both Nuada checkouts, original ROM/report
hashes, and all23 reference files. Full production build/test gates remain122's
independent clean builds and243 tests; no new production integration is claimed.

Preserve direct_values as the two-byte-copy, no-ID-copy control. Close the simple
explicit-pointer/helper-removal family. Next replace the explicit pointer local
with ordinary array field expressions and inspect any compiler-created common
address. A common address had priority4 in152 but clashed with its separate ID
argument; the direct form can test that distinction without that argument copy.
Lowering an actual pointer candidate from6 to4 would cross the constants' integer
profit threshold in this87-candidate graph, but that is a diagnostic prediction,
not an authorized numeric edit or a source result. Require fresh native equality,
single-load evidence and the actual interference graph before drawing a conclusion.
The full100% goal remains active, with spr1 homes and the priorA226 boundary still
unresolved.
