# Animation common address and copy groups — continuation157

Production checkpoint122 remains **128,316 / 200,698 bytes (63.93487%)**.
Continuation156 made progress by separating explicit-pointer allocation from late
scratch-register choice. This turn tests actual common-address formation. Three
native controls add no matching C bytes and remain strict boundary rejects.

## What the source changes establish

The first two controls derive from frozen156 direct_values, retaining consumed
byte copies b0/b1 but replacing the explicit sprite pointer with two ordinary
array field expressions. Both read the real addressed spr1 value. One uses
separate statements, the other a comma expression. They produce exactly equal
allocated text and rodata, verified directly.

Their before-allocation trees retain two spr1 references and two multiplies;
no anonymous common-address assignment is present. Three-generator-trace analysis
confirms each of these two controls emits two word loads, six scale shifts and
two byte stores, through scratchR3 using b0R4/b1R5. The comma node remains visible
in the tree but does not change native code or merge the address expressions.

A third adaptive control computes two real destination pointers before either
field write. Both pointers feed their original distinct b14/b15 stores. This
creates an anonymous common-address assignment in the frontend tree, with one
spr1 load and one multiply. The generator emits one word load and three scale
shifts. Thus preloading changes common-expression formation, not merely spelling.

| Form | Body bytes | Frame | spr1 frequency/home | Static spr1 reads |
|---|---:|---:|---|---:|
|array_fields|1756|104|9 /12|5|
|array_comma|1756|104|9 /12|5|
|preloaded_addresses|1752|108|7 /36|4|

All beginA604 instead of originalA608. The original body is1748 bytes, frame108,
spr1 home16 and four static reads. Equal frame size in the third control does not
establish matching allocation: its new pointer spill displaces sprite_id to4,
sel to8, tbl to12 and total to16. The pointer group's home is0/frequency14.

## Why the common address spills

Three same-process hardware graphs verify261 original candidates,898 eligibility
events and9 appended memory records. In the preloaded form, dst1 is an ordinary
category1 root of priority8/profit-1 with its bound field pointing to itself.
Two category3 members each have priority4 and point to that same root. The
four ordinary eligibility checks return zero, but the separate nonzero-bound
condition excludes this group from internal registers1–4, hardwareR4–R7.
The verified sort/allocation model reproduces its eventual stack assignment.

This is a different constraint from the unmerged explicit pointer's priority6
advantage in156. A compiler-created common address did form, but its subsequent
copy group cannot receive the neededR6. The two byte values remainR4/R5 and the
generator uses scratchR3 for the spilled pointer, with stack materialization.
All claims about identities and links use data from the same compiler process.

The saved inferred SHCMDL decompilation in `merge_static/` gives the next bounded
question. 410CF0 recognizes candidate copy forms and sets these links;4111F0
requires exactly one defining event;411220 checks live-range overlap;411010
combines records and priorities. These static files guide investigation; they are
not original source or proof that every predicate was taken in this execution.

## Validation and next action

Three native builds/captures and six middle replays agree with stock output.
Three CFG/stack audits retain36 exports, six loops, zero inline-return gotos and
no anonymous calls. Three home traces/models verify80 records and399 weighted
contributions. The three graph traces cover261 candidates/898 gates. Three
independent generator controls agree in SUA/ASA with their traced replays;
SUA also matches capture. Cleanup ASA metadata differences are recorded explicitly.
The generator traces cover1735 emissions and715 recipes. Exact comparison shows
array_fields/array_comma text and rodata identity; preloading changes text.

Production recheck verifies44 pins and42 earlier frozen cases in both Nuada
checkouts, unchanged ROM/report hashes, and all23 reference files. Applicable
full integration gates remain122's independent clean builds and243 enabled tests.
No production source, compiler, proof rule, runtime policy or denominator changed.

Close comma/separate-statement spelling and the identical-pointer preload form.
Next inspect the copy-merge predicates with a genuinely consumed alternative
address type and a cast that survives the frontend. A byte pointer or integer
address view may distinguish a conversion from an identifier copy, but that
must be tested; it is not a proven workaround. Preserve common-address formation,
inspect the resulting group and native loads, and reject changes that only move
bytes or alter priorities artificially. The100% goal remains active, including
the unresolved spr1 layout and earlierA226 boundary.
