# Score bit-field layout and consumed flags — continuation165

Checkpoint161 remains **128,398 / 200,698 bytes (63.9757247%)**. This continuation closes the bit-field hypothesis with native compiler evidence and tests seven source controls plus a fresh baseline. No additional C bytes are accepted.

## Native layout probe

`session_score_bitfield165/layout.c` defines an unsigned-short bit-field structure with upper three bits, mode bit, and lower twelve bits, overlaid with an unsigned-short word. Native SHC v5.0r32, SH2 big-endian, `-macsave=1 -optimize=1 -speed` emits a two-byte structure and union. In a containing structure preceded by one byte, the union starts at offset2 and the following byte at offset4. Three initialized objects encode `1000 e000 0fff`, establishing the exact mode and surrounding masks.

Both a consumed Boolean return and an actual conditional call compile the bit-field read as `mov.b @r0,r0; tst #16,r0`. Corresponding integer-word controls use `mov.w`, with the4096 mask. Original ROM7774–777E contains `mov.w @(r0,r14),r2`, the4096 literal load, extension and register test. Thus the bit-field's memory representation is right but its load width and instruction recipe are wrong. No bit-field game-source view was attempted or retained. These observations apply to this tested unsigned-short bit-field form, not every possible bit-field construction.

## Genuine consumed locals and equivalent predicates

All controls keep f7956 source unchanged, including its inherited unresolved f17834/RNG ABI assumption. All fail the ordinary prefix verifier.

| Control | Nominal differing bytes | Context bytes | Following-root displacement |
|---|---:|---:|---:|
| Baseline |19|1456|0|
| Mode assigned inside condition, unsigned short |19|1456|0|
| Mode assigned inside condition, int |19|1456|0|
| Mode assigned in preceding statement, unsigned short |37|1456|0|
| Mode assigned in preceding statement, int |37|1456|0|
| Global flags assigned inside condition |673|1460|6|
| Masked mode equals4096 |438|1456|2|
| Mode shifted right12 and masked with1 |482|1460|4|

Mode assignments inside the condition produce the entire baseline context byte for byte. The two preceding-statement controls also equal each other, but disturb other register homes and increase guard mismatches from10 to19. The global assignment introduces a four-byte stack allocation and shifts the next root. Nominal mismatch counts for shifted outputs are diagnostics, not matching credit.

Every new local is consumed by the original condition, with no dummy uses. The preceding mode assignment remains inside `if(n)` at the original mode-read point. The global read stays behind the mode short-circuit test. Since the mode word is unsigned16-bit, both single-bit predicates preserve the original truth value. No volatile changes, physical-register constraints, padding, compiler patches or relaxed comparisons are used.

## Verification

`verify.py` regenerates and hashes every score source, checks native layout bytes and function instruction sequences, authenticates the original guard load, compares the fresh baseline with frozen164, and verifies all eight strict-negative receipts against their resolved binaries. `NATIVE_ANALYSIS.json` preserves every mismatch and actual root. Both Nuada checkouts retain46 production pins,51 preceding frozen cases, original rebuilt ROM hashes and the checkpoint161 coverage report. All23 reference ROM files still match their recorded source manifest. Production is unchanged; latest full gates remain checkpoint161's245 enabled tests and independent clean builds.

The evidence is frozen and synchronized with the local mirror and both Nuada checkouts. The goal remains active;72,300 bytes remain, including1,638 runtime assembly bytes.

## Next mechanism

Static compiler inspection found a cache-suppression flag at460A78. The installer42AE20 returns early when it is nonzero; successful cache lookup42B2B0 sets it at42B3FB. It is saved/restored by several expression handlers and reset at413B15. The precise dynamic role in this guard is not yet proven. `CACHE_LOOKUP_DISASSEMBLY.txt` and `CACHE_SUPPRESSION_XREFS.txt` preserve the direct evidence.

Next observe writes to460A78 and entry/return of42B2B0 with read-only hardware breakpoints, requiring traced output to equal an untraced control. Determine whether a cache hit changes installation or expression state before the threshold address. This is a specific compiler mechanism to resolve before another source experiment. Do not repeat bit-field, flag-local, equality or shift variants from this case.
