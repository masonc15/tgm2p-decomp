# Resumed source recovery, 2026-10-13

Coverage remains **137,332 / 200,698 bytes (68.42718911000608%)**: **zero newly
accepted bytes and zero percentage-point gain**. The objdiff report stores
68.427185% at its numeric precision. There are **61,728 unresolved non-exempt
bytes**, plus the existing 1,638 runtime assembly bytes. No source or split was
integrated, and no matching completion is claimed.

## Baseline and preserved state

Canonical `make split check report` ran under the existing import lock. Both
original EPROM SHA-1s and the full rebuilt program SHA-256 pass. The exact current
sources and tools reauthenticate against the most recent successful two-run cold
receipt: 231 accepted inputs, 30 compiler inputs and nine host tools, no drift.
The earlier 40-byte text-palette gain is already part of this baseline; it is not
a gain from this round.

Evidence is frozen in `research/2026-10-13-continuation-review/`: `baseline.log`,
`baseline.json`, `reauthentication.json`, and the previous GOAL, routes, splits,
frontier and report under `before/`. Current source, compiler, verifier and ROM
inputs were preserved. Metadata changes only add the reviewed checkpoint and
three routes, with the prior history retained.

## Two bounded research assignments

Both workers used GPT-6.1 Sol in separate directories, with no nested delegation.
Only the parent reviewed integration eligibility. Seven research compiler
launches occurred: two for the spawn/cell context and five for graphics, each
below its conservative 24-launch cap. Canonical baseline build compilation is
separate from those research counts.

### Spawn and cell context

[The worker report](../../research/2026-10-13-resume-61d0/REPORT.md) preserves the
complete inherited predecessor context and three real function bodies. The
October 11 source-only inventory missed older archived drafts and failed frame,
ID/cap and retry-helper experiments. Those were read and excluded from repetition.

A proposed change from whole-cell assignment to explicit word copies had a false
premise: the existing compiler output already used three word transfers. Explicit
cached pointers changed allocation, improving a positional diagnostic score but
worsening the actual fixed-ROM residual. The two controls were stopped. All three
independently linked contexts preserve the accepted 438 bytes at601a–61d0.

For the entire 3,636-byte unresolved owner, the inherited draft has 3,054 differing
or missing bytes; the two controls have 3,147 and 3,140. The667c and67f4 roots
remain16 and22 bytes early. No smaller retained candidate was claimed.

### Graphics unit

[The worker report](../../research/2026-10-13-resume-2657c/REPORT.md) and
[best.c](../../research/2026-10-13-resume-2657c/best.c) recover seven genuine functions
in original order for2657c–27310. Older apparent drafts used synthetic `g_dummy`
stores for several bodies and are not admissible source.

Parent instruction review recovered the row writer's real stack arguments:
`px, row, py, x`, all consumed as shorts. The draft had swapped the first two and
used wider types for the final two. Actual callers and callee stack reads agree.
The recovery also separates two tail wrappers from the large redraw routine,
replaces the decompiler's unknown private-ABI remainder expressions with ordinary
C remainder operations, and recovers redraw do/while loops that avoid unwanted
fourfold compiler unrolling.

The final context is3,320 bytes versus3,476 in the ROM. It has3,330 differing or
missing bytes at fixed target addresses, including156 missing bytes. All helper
roots remain displaced. Correct structure and sampled semantics are research
progress; neither supplies matching coverage.

## Independent parent verification

`relink.py` independently GNU-links all eight original objects from the two
packets. Every context reproduces the worker's saved bytes; all actual roots and
fixed-address residual counts are retained in `PARENT_RELINK.json`. The short
remainder helper's address is independently authenticated from ROM pool272d8.
No source compiler was launched by these parent research checks.

`ABI_REVIEW.md` and `abi_guards.json` record22 authenticated ROM instruction words
supporting the stack and signedness review. The linear literal scans remain
heuristic inventories; they are not literal ownership or reachable-CFG proofs.

`verify_row_writer.py` executes the original row writer and each of two ordinary
compiled variants with the real ROM remainder helpers and no call stubs. Both
variants pass the same96 controlled cases. An independent model agrees with
ordered writes to the tile destination and the record flag, and callee-saved
registers/MACL are preserved. The cases vary formats, mode flags, banks, widths
and coordinates. Deliberately wrong models for negative wrapping, column offset
and flags are rejected in72,96 and48 cases respectively. Receipts:

- `ROW_WRITER_RESULTS.json`: initial corrected ABI context.
- `ROW_WRITER_narrow_steps_RESULTS.json`: final source context.

This is bounded synchronous helper evidence, not whole-game equivalence or a
semantic proof for the other six routines. No strict c-range extraction, new full
suite, cold whole-ROM replay, or dashboard gain publication was performed because
no exact candidate was found. Existing acceptance checks were not weakened.

## Next work

For graphics, recover the first body's genuine storage lifetimes: ROM uses a
28-byte private frame versus24 in the draft, initially spills its field pointer,
then reuses that slot for a coordinate. Separately, explain the wrapper's original
12-byte parameter-spill frame from producer and call-site evidence before changing
prototypes or optimization controls. The narrow step-counter declaration control
was byte-identical and must not be repeated as a new discovery.

For67f4, first authenticate the queued-row loop's unsigned-byte destination offset
at6edc and signed-short source offset at6eea. A subsequent typed-offset control
must change those actual extension sites and improve raw comparison without
moving accepted neighbors. Do not compensate for earlier root displacement with
padding or rebasing.

The updated frontier routes link both genuine drafts and the older negatives.
`source_candidates` searches canonical `src` only; zero entries does not establish
that research is absent.
