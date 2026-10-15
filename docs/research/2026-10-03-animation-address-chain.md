# Animation address expression chain — continuation154

Production checkpoint122 remains **128,316 / 200,698 bytes (63.93487%)**.
All four genuine consumed-source controls fail the unchanged strict verifier.
No production source, compiler, denominator, or comparison rule was changed.

The preceding response only reverified the completed ROM transfer; it was no
progress toward matching C. This turn resumed the pending native experiments,
completed their analysis, and established which simplifications are equivalent.

## Native controls

All controls derive from frozen152 `id_int.c`. They change only the brightness
helper's consumed address calculation. The compiler and native flags are unchanged.

| Form | A608 body bytes | Frame | spr1 home | Outcome |
|---|---:|---:|---:|---|
| Initialize scaled offset directly |1750|108|36|45 text bytes differ from152; constant registers swap|
| Initialize sprite pointer from ID expression |1750|108|36|Allocated text/data exactly equal152|
| Initialize sprite pointer with array address |1750|108|36|Allocated text/data exactly equal152|
| Compute address in caller and pass pointer |1744|104|32|Total enters a register; loop allocation regresses|

Every candidate still begins atA604 instead ofA608, has four static spr1 reads,
and retains the wrong brightness mapping: bri1R6, bri0R5, ID/addressR4. The
original requires R4/R5/R6. No extra sign extension occurs in these int/pointer
forms. All36 expected functions remain, with no extra exported or anonymous
local helper calls. Each has six loops and no inline-return gotos.

Equal lengths and raw difference counts did not imply identical code: the first
three controls share both, but only the latter two have byte-identical allocated
sections to152. `SECTION_COMPARISONS.json` records exact comparisons. The pointer
argument form changes the complete text size19672->19640 and frame108->104; its
shorter local body is not a matching gain.

## Same-process graph evidence

The pointer-initializer replay preserves captured REG/ILB byte for byte. Hardware
breakpoints read compiler data without changing it. All88 original candidates,
302 eligibility events and three appended addressed-memory records are checked.
Sort order, computed profits and positive register assignments reproduce the
observed allocator exactly. Pointers are joined only within the same process.

The offset temporary is gone. ID priority4/profit-5 and pointer priority6/profit-3
can still share a register, while both conflict with brightness arguments. The
target is graph-compatible, but the pointer sorts first and takes internal1/R4.
Both brightness arguments remain priority4/profit-5 and receive internal2/3.

Forty bounded single-priority model trials produce no full target assignment.
Of150 paired trials, only raising both brightness argument priorities4->10
reproduces all target helper and six loop roles. These are offline sensitivity
calculations, not modified compiler inputs or source fixes. Removing the offset
alone does not solve allocation, and the prior offset-plus-pointer reduction
hypothesis no longer applies to this smaller graph.

## Validation and next action

Four native builds and four captures agree in all non-comment sections. Eight
middle replays preserve native intermediates. Four stack/CFG checks pass, and
four home traces/models verify107 records with538 frequency contributions.
The graph replay adds the88 candidate/302 gate checks above. The production
recheck confirms44 pins,39 earlier frozen cases, original ROM/report hashes and
all23 reference files in both Nuada checkouts. Prior independent clean builds
and243 enabled tests remain the applicable production gates; no new integration
or full-suite pass is claimed.

Preserve the simpler pointer initializer as an exact native-equivalent control.
Close further offset-removal/array-address spelling experiments. Next test real
address expressions for the existing brightness locals at their consumed call
arguments. Their current storage and overlap differ from the original late
stack loads; addressing those real values can test SHC's memory classification
without padding, dummy consumers or forced registers. Any changed graph needs
fresh native and same-process evidence. spr1 home36 versus16 and the A604 entry
boundary remain unresolved. The literal100% goal stays active.
