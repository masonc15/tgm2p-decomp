# Animation brightness storage — continuation155

Production checkpoint122 remains **128,316 / 200,698 bytes (63.93487%)**.
This turn classifies154 as progress: its exact native comparisons closed the
offset-removal hypothesis. Four new consumed-source controls test real brightness
local addresses. All fail the unchanged strict verifier; no C bytes are added.

## Native results

Sources derive from frozen154 `pointer_initializer.c`. Three controls pass
`*(&bri0)`, `*(&bri1)`, or both, retaining value arguments. The fourth passes
both actual pointers and dereferences them in the existing two field stores.
There are no dummy reads, unused storage, forced registers or compiler edits.

| Form | Body bytes | Frame | bri0 home | bri1 home |
|---|---:|---:|---:|---:|
|154 value baseline|1750|108|88|84|
|Address bri0 only|1744|104|44|84|
|Address bri1 only|1744|104|84|44|
|Address both, pass values|1756|104|48|44|
|Pass both pointers|1760|104|48|44|

The original body is1748 bytes with frame108 and brightness homes96/100.
Addressed locals change from ordinary kind1/frequency2 to kind2/frequency5;
the weighted frequency adds initialization and assignment occurrences. All four
controls still read spr1 at four static sites, but move its home36->32. Their
entry remainsA604, four bytes before the requiredA608. All keep36 exports,
six loops, zero inline-return gotos and no anonymous local calls.

The value controls retain bri1R6/bri0R5/IDR4, against the originalR4/R5/R6.
In the pointer control R6/R5 hold addresses, not brightness bytes; its extra
loads are recorded in `CONTROL_ANALYSIS.json`. All four move total intoR8,
retain mask15 inR9 and lose the constant2 register. The two single-address
controls share size and raw mismatch count but differ by six native text bytes.
Exact allocated-section comparisons are saved, not inferred from those scores.

## Why the frame shrinks

One fresh read-only hardware replay traces the address-both graph. Captured REG
and ILB remain byte-identical. The same-process model reproduces86 candidates,
294 eligibility events and five appended memory records. Forty single-priority
and150 paired model trials find no assignment meeting all helper and loop targets.
These trials are diagnostics only, not changes to compiler inputs or C credit.

Comparing symbolic records with154 identifies exactly two removed ordinary
candidates: bri0 and bri1, both priority5. All remaining candidate priorities and
symbolic neighbor multisets are unchanged after excluding those two records.
Each process's heap pointers are resolved locally before symbolic comparison.
The initial unique-symbol assumption failed because k and step have two separate
live ranges. The corrected comparison preserves multiplicity and the initial
failure log; it does not silently join duplicate ranges by symbol or heap address.

| Candidate | Before clash sum/count | After clash sum/count | Floor average | Profit |
|---|---|---|---|---|
|constant2|689/77|679/75|8 ->9|8 ->7|
|constant15|689/77|679/75|8 ->9|8 ->7|
|total|726/87|716/85|8 ->8|7 ->7|

Removing low-priority records raises the constants' mean clash priority across
an integer boundary. Their profits drop into a tie with total. The verified
sort/allocation model then gives total a physical register; its stack home
disappears and the frame loses four bytes. Thus the shorter frame is a global
allocation consequence, not evidence of approaching the original frame.

## Validation and next action

Four native builds/captures, eight middle replays, four CFG/stack audits and four
home traces agree with uninstrumented output. The home models verify104 records
and508 weighted contributions. The graph model and symbolic comparison establish
the arithmetic above. Production recheck verifies44 pins and40 prior frozen
cases in both Nuada checkouts, unchanged ROM/report hashes, and23 reference files.
The last full integration gates remain122's independent clean builds and243
enabled tests; these unintegrated experiments do not claim another suite run.

Close brightness-local address-taking as a path to the original layout. Keep
the brightness locals ordinary. Next remove the brightness helper's argument
copies by expressing its consumed sprite pointer and two stores directly inside
the existing function, while retaining spr1's addressed load. Earlier direct
controls included separate ID/offset temporaries, duplicated offsets, or an
ordinary ID subject to hoisting. The next control removes those temporaries
and the helper argument copies in the current loop-preserving context. Compare exact native code and the late generator's scratch registers
before deciding whether another allocator experiment is warranted. spr1's home
and the precedingA226 boundary remain unresolved. The100% goal remains active.
