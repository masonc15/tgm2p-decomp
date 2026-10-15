# Save dirty-bit allocation — continuation123

Production checkpoint122 remains **128,316 / 200,698 bytes (63.934867%)**.
This investigation adds no verified C bytes. All twelve bounded source controls
fail the unchanged writer and reader proofs. No production source, compiler,
split map, denominator, or verifier was changed.

Evidence is in `tools/shc_probe/cases/session_save_dirty123/`. `analyze.py`
checks the sources and resolved native objects, three fresh native captures,
six exact middle-stage replays, six GEN hardware-breakpoint replays, 248 scratch
predictions, 219 logical-register profit records, and 289 writer GEN events.
Normal and traced stage outputs agree; the native cleanup comparison establishes
SUA equality, not equality of cleanup ASA metadata. Tracers do not modify compiler
instructions or data.

## What the trace establishes

The original writer loads the shift seed 1 into R5, passes the index from R14
to R1, copies R5 to R0 in the helper-call delay slot, and returns the mask in R4.
The baseline GEN stream instead assigns both seed and mask to R4. A named seed
does not correct that allocation: the seed and mask are **distinct logical WEBs
with nonoverlapping lifetimes**, sharing physical R4. This is not evidence that
the compiler merged them into one logical value. The named seed has priority4,
profit−7; the mask priority6, profit−5. Neither appears in the other's clash list.

An actual inline shift helper with seed and index parameters gets the seed into
R5, but adds an R14-to-R4 index copy and puts the result in R10. It increases the
context from668 to672 bytes and shifts later roots. It therefore demonstrates
how parameter dataflow affects allocation, without providing an acceptable match.

The GEN baseline emits seed-to-R0 copying that is absent from the final native
sequence. This places the simplification after the observed GEN emission.
No fresh PEP trace was made here, so its exact later stage is not established.

## Bounded source controls

| Source form | Context bytes | Writer differing bytes |
| --- | ---: | ---: |
| Current production baseline | 668 | 88 |
| Mask equality against zero | 668 | 88 |
| Inline shift helper with seed argument | 672 | 154 |
| Named seed | 668 | 90 |
| Shift assignment in the condition | 668 | 92 |
| Inline dirty-bit helper accessing global | 684 | 217 |
| Inline dirty-bit helper taking pointer | 680 | 202 |
| Four signed/unsigned char/short seed forms | 668 each | 90 each |
| Seed also supplies the real loop increment | 668 | 90 |

All four seed-width forms and the loop-increment form produce exactly the named
seed's linked bytes. They preserve the required shift width but do not extend
the seed's effective allocation lifetime. The equality form is byte-identical
to baseline. Do not repeat these forms or interpret their variable names/types
as new progress. A further experiment needs a measured change in real lifetime
or interference, rather than another equivalent seed spelling.

## ROM transfer and production recheck

The requested MacBook trilogy directory was copied and analyzed in continuation121:
23 files, 61,822,311 bytes, 40 duplicate TGM2/Plus payload occurrences, and new
TGM1/board-firmware inputs. The exact shared448-byte tetromino block authenticates
all28 rotations but adds no new SHC sample or C coverage. See
`2026-10-03-trilogy-rom-reference.md` for offsets and identification evidence.

The current MacBook SSH refresh timed out. Existing local and Nuada copies are
checked against the previously captured source manifest; this is not a claim
that the now-unreachable source directory has remained unchanged.

The fresh read-only production check confirms44 pins, original ROM/report, and
eight prior evidence freezes in both Nuada checkouts. The last complete build
and243-test gates remain checkpoint122's; they were not rerun for these rejected
research-only controls. The literal100% goal remains active and incomplete.
