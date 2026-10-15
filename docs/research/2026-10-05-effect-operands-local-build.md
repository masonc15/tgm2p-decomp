# Operand planning and native Mac build — continuation242

Production stays135594/200698 bytes (67.56121%). Two new source controls produce
no new matching bytes. The literal100% goal remains active.

Two read-only hardware replays capture290 events:122 operand-planner entries
and168 last-scratch writes. Every observed write agrees with frozen241; ordinary
and watched SUA/ASA remain equal. The unique target request before emission20
comes from42207b into422290, with mode0xc00, node opcode32/type16 and load type8.
Its source operand is signed-short memory at R6+2; destination is registerR0.
That identifies d->y as the cause of reset255 at42260e, followed by R0 selection.
Baseline excludes mask240; product-first excludes248, additionally reservingR3.
The product requests themselves originate in recipe scratch planning (20900,
call return421159), rather than this operand-planner path.

Two unsigned first-sum controls preserve the arithmetic bit pattern before
conversion to target signed32 int. Height is0..255 and y is signed16; the signed
sum is bounded. This relies on SHC's target two-complement unsigned-to-signed
conversion and is not presented as portable ISO C for every implementation.
Both ordinary Mac compiles exactly equal their corresponding frozen signed
controls:384 bytes,13 differences for y-first and30 for product-first. Nuada
independently recompiles both with identical full bytes and function roots.
All248 protected prefix bytes remain exact; default admission rejects both.
There is no reason to repeat these unsigned boundary variants.

## Local build validation

An isolated APFS clone at local-toolchain/validation-decomp was cleaned, split
and rebuilt with the unchanged production scripts and authenticated original
SHC executables. Native SH binutils2.45 and Wibo1.2.0/Rosetta run on this Mac.
The first split attempt exposed macOS objcopy's inability to write seekable
output to /dev/stdout. A host adapter directs that output to a temporary file
and copies its exact bytes to stdout. A second adapter supplies realpath -m.
Neither alters compiler or object bytes. Failed and passing logs are preserved.

The clean rebuild passed the project check, including EPROM checks. Original
and rebuilt1MiB program SHA256 are both
 d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7.
All64 production pins and67 build/tool scripts equal canonical source. All20
compiler executable hashes are authenticated. local-toolchain/run-build.py
reuses this isolated checkout and verifies original compiler/Wibo hashes before
running the original make commands. Full tests and objdiff report generation
were not run locally; checkpoint225 remains the last266-test full-suite gate.

verify.py checks these receipts and pins, frozen240/241, all168 write
correspondences, both traced output pairs, actual operand requests, both native
object comparisons and default negatives. Next pivot to another affected
unmatched drawing caller with proven array-argument skipping, or inspect recipe
scratch planning20900's result/preferred-register choices with a specific new
prediction. The y-load reset itself and signed/unsigned sum boundaries are now
explained and should not be reswept. Keep the13-difference reference unchanged.
