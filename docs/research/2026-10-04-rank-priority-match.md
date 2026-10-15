# Complete rank-display match — continuation 179

The full 1,236-byte `f_12ad4` at ROM 0x12ad4–0x12fa8 matches with original
SHC v5.0r32 and unchanged strict validation. The C implementation spells out
three calls in the existing bounded loop. All pointer advances, coordinates,
rank arguments, and call ordering are preserved. Ordinary assignment and
postincrement forms reproduce the compiler's original allocation. There are
no dummy consumers, forced physical registers, patched compiler inputs, or
relaxed byte comparisons.

## Why the previous source missed

The original variable-priority constructor at SHCMDL 0x40ff00 starts with
use counts from each expression leader. At 0x40ff75, assignment opcode 0x5f
adds one use for a non-entry block. The observed loop factor is two. The trace
records this actual multiplier before 0x40ffba and verifies every running sum.
The initializer contributes one; the final unused assignment still contributes
two, while a final unused increment contributes zero.

| Pointer source form | Per-member contributions | Total |
| --- | --- | ---: |
| Loop postincrements | 1 + 4 + 4 + 4 + 0 | 13 |
| Increments consumed by calls | 1 + 2 + 2 + 2 + 0 | 7 |
| All explicit assignments | 1 + 4 + 6 + 6 + 2 | 19 |
| Matching p updates | 1 + 4 + 6 + 6 + 0 | 17 |
| Matching q updates | 1 + 4 + 4 + 6 + 0 | 15 |

The matching case0 second loop calls the same helper three times. It uses
assignment for p after the first and second call, assignment for q after the
second call, and postincrements for the other required advances. The original
loop's three iterations have the same observable arguments:
`p[k], q[k], y=170+22*k, ordinal=k+1, score_offset=36+12*k`, k=0,1,2.
Its final pointers are exactly one past their three-element arrays; y=236 and
i=3 remain representable. No access is added after the last call.

These priorities cause SHC's unchanged profit sort and allocator to choose
p=R12, q=R10, y=R8, ordinal=R10 and coordinate=R9 in the separate case1 loop.
The register-allocation model reproduces all 32 candidates, including distinct
webs that share the source symbol for i. Thus the earlier hypothetical 17/15
combination is now authenticated by a real native C compilation.

## Controls and independent proof

Four explicit three-call source controls have differences 32 (all increments),
57 (all assignments), 24 (only final updates changed back to increments), and
0 (the mixed updates). Every context has 4,404 bytes and all 13 nominal roots.
All 1,236 target bytes, including literals and returns, compare exactly with
ROM. The packaged object exports only `_f_12ad4`, contains no relocations and
has no external literal dependencies or tail/switch exceptions.

Four priority traces preserve fresh untraced REG/ILB outputs and native driver
intermediates. Each records 427 events and reconstructs all 21 variable totals
from 107 contributions: 84 totals and 428 contributions overall. A fifth trace
reproduces the complete graph allocation. Native driver and ordinary compilation
ELF sections agree, excluding only `.comment`. Hardware debug registers are the
only compiler-process state written by the tracers.

The initial analyzer incorrectly inferred loop weight from block kind. A failed
assertion exposed another block kind using factor two. The corrected tracer
observes the multiplier directly; all final checks use that evidence.

Preceding context still differs by 50 bytes; following context by 525 bytes.
Neither is retained or credited. The production split adds only
`0x012ad4 c-range src/rank_display_12ad4.c` through `0x012fa8 asm`.
Two native regression tests authenticate the complete function, original roots,
packaging, and rejection of the equivalent all-increment form.

## Evidence and continuation

- `tools/shc_probe/cases/session_rank_priority179/`: original objects, source
  controls, priority traces, native captures, independent verifier and freeze.
- `tools/shc_probe/cases/session_checkpoint179_rank/`: integration gates,
  production pins, independent build artifacts, and verification receipt.

The target remains literal 100% matching C, with the fixed 200,698-byte
denominator and all 1,638 runtime assembly bytes included as incomplete.
After this integration, 70,374 bytes remain: 68,736 non-runtime plus 1,638 runtime.
Next inspect another unmatched function's concrete compiler discrepancy; preserve
this now-matched rank renderer rather than repeating its allocation experiments.

Final validation: isolated clean split/check/report plus249 enabled tests pass
with zero skips; independent canonical clean split/check/report also passes.
Both reports confirm130324/200698 bytes,602 matched functions,208 units and164
complete units. All50 production pins agree across checkouts. Original ROM
SHA-1 c84cf538962ddbb377290301d76d75d8b542e5a2 and SHA-256
d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7 match.
The existing separately compiled120c0 production unit stays unchanged; its
differing context representation in the new source receives no second credit.
