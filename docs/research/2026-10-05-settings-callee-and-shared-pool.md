# Settings helpers: corrected EEPROM callee and shared literal pool

Production remains checkpoint 249: 135,910 / 200,698 verified bytes.
No new C bytes are admitted.

A fresh compile of the genuine B2F4/B352/B36E context found an actual source
error: f_b36e calls f_2f36e, whereas the original word at B3BC is 0602FBEE.
Subtracting the authenticated code relocation delta 05FFF880 yields 3036E,
the retained eeprom_write_all implementation in src/eeprom.c. The corrected
source changes only that callee name and its declaration. It remains a research
candidate; no frozen source or production mapping was changed.

Across all 166 instruction bytes B2F4–B39A, exactly six bytes differ. They are
the displacements of six MOV.L literal loads. Every instruction/register and
all three function roots otherwise match. After correcting the callee, all six
loaded values agree with ROM, but each native slot is 20 bytes early.

A ROM-only control-flow walk of AF8C–B2F4 establishes nine reachable reads of
five distinct four-byte literals at B39C–B3AC. They are the Y/N strings, glyph
renderer, and player-input globals. These real predecessor dependencies occupy
the 20 bytes absent from the three-helper context. This is structural ROM
evidence, explicitly not a proof that compiled bytes match.

A bounded genuine-context control keeps the actual AF8C input-test body and
all following menu/renderer bodies from frozen125, replacing eight preceding
bodies with their real prototypes. It compiles without warnings but moves all
three helper roots 74 bytes late. Its extra allocated data is not proposed for
production. This closes that simple suffix-context hypothesis. Do not repeat
prior renderer-prototype or section-placement sweeps from124.

All three Mac objects reproduce on Nuada: allocated sections, function offsets,
and symbolic relocations agree. Original compiler hashes and source provenance
are verified. Existing strict admission rejects every helper range. No compiler,
object, ROM, verifier, or denominator was patched. Full production tests were
not rerun for this research-only change.

Next action: audit genuine draft callee literals against the original ROM before
more allocation experiments; this inherited wrong callee survived otherwise
exact instruction matching. For these helpers, only a genuine predecessor
context that preserves original roots and all shared literal slots can qualify.
Artificial pool padding or repositioning the retained functions cannot.
