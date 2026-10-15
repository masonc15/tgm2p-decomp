# Genuine menu compilation context — continuation125

Production remains checkpoint122: **128,316 / 200,698 bytes (63.934867%)**.
This diagnostic adds no verified C bytes and changes no production source,
compiler, map, denominator, or proof rule.

## Reference ROM request

The entire MacBook trilogy ROM directory was copied during continuation121.
A fresh SHA-256 check of both destinations again verifies all23 files and
61,822,311 bytes against that recorded source manifest. A fresh MacBook SSH
attempt timed out, so this does not attest to subsequent source-directory changes.
The analysis in `2026-10-03-trilogy-rom-reference.md` remains applicable:
40 TGM2/Plus payload occurrences are duplicates; the new TGM1 content authenticates
448 shared piece-shape bytes across28 rotations. No additional TGM2 revision is
present. No source memory or new external download was needed for this recheck.

## Applying the compiler candidate gate

Continuation124 established that real function definitions exclude their addresses
from SHCMDL's external-function allocation candidates. The new `complete_unit.c`
combines36 real reconstructed bodies in original order from9C0C throughE750,
including intervening menus and the actual renderer definitions. It is an ordinary
single code section, with genuine data. It is a reconstruction experiment, not
proof that these were the original translation-unit boundaries.

`prepare.py` pins its five existing input files. It removes archived unused stack
padding and dead allocator hints, removes the unreachable arm of a constant-false
conditional, replaces an empty formatter draft with retained C, and uses the
retained E520/E63A renderers. Source composition declaration errors are preserved
in `rejected_initial.c` and its compiler log. The corrected source compiles without
warnings under unchanged SHC5.0r32, `-macsave=1 -optimize=1 -speed`.

Native output has19,608 text bytes and1,182 data bytes. The first302 data bytes
still exactly match the authenticated original AD80 data at RAM6032A3C. The full
data section is not exact:784 bytes differ. The diagnostic linker uses the real
data address and native symbol relocations; it does not rebase functions to earn
coverage or add production support for extra allocated sections.

| Routine | Original length | Native length | Native start displacement |
|---|---:|---:|---:|
| A226 |994|990|0|
| A608 |1748|1680|-4|
| ACDC |164|160|-72|
| AD80 |524|524|-76|
| AF8C |872|872|-76|
| B2F4 |94|94|-76|
| B352 |28|28|-76|

The four bytes differing in the previously retained9C0C–A226 prefix are explained
exactly by two ordinary internal function-pointer relocations: the words at9F1C
and9F44 now refer to the draft's shifted BD8A and E3B8 definitions. `link_audit.py`
asserts both relocated values and the complete difference list. The original
expectation that the prefix would remain byte-exact failed; its log is preserved.
These explained differences still fail the strict original-byte comparison.

A226 is now only four bytes shorter, compared with32 in continuation47, but its
register allocation and scheduling still differ. Its stack frame remains12 bytes
against44 in the original. The extra32 bytes have no authenticated source use;
adding unused padding would not be a valid repair. Matching function lengths
farther into the unit are diagnostic evidence only, not matching code claims.

## Validation and next step

`LINK_AUDIT.json` records roots, native hashes, data comparison, relocations and
unchanged verifier rejection. A fresh read-only check verifies all44 production
pins, original ROM hashes, exact report counts and ten previous frozen cases in
both nuada checkouts. Production was untouched, so the prior independent clean
builds and243 enabled tests remain the applicable build/test evidence; no new
full-suite run is claimed.

The next bounded question is the source of the earliest A226 size/frame/allocation
difference in this real compilation context. Its first four routine roots remain
at their original addresses, and full-unit context now supplies the actual far
callees. Use the existing compiler-stage tracing to distinguish a real local
object/type or source-lifetime difference from an unsupported padding hypothesis.
Keep the current diagnostic frozen before testing that question. The100% goal
remains active with72,382 bytes incomplete.
