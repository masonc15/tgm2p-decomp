# Wave staged lifetimes and next target — continuation198

Production remains checkpoint192: **131,780 / 200,698 matching bytes
(65.6608436556%)**, with **68,918 bytes remaining**, including all1,638 runtime
bytes. No matching C was added. The previous turn supplied progress by explaining
the setup store-order constraint and the phase/k allocation cost reversal.

## Experiments derived from those constraints

All new forms preserve the phase-store-before-j-store source order from197.
The first experiments stage actual consumed loop initialization values, seeking
a low-priority live value across the phase conversion without fake uses:

- `start = half; sh = phase; ang = (unsigned char)sh; j = start;`
- `mirror = half - 1; sh = phase; ang = (unsigned char)sh; j = half; m = mirror;`
- A separate `long phase` local for initialization, leaving loop-local sh unchanged.

The first and third forms compile to precisely the same1220 bytes as
`j_after_phase`, including its16 differing target bytes. The graph shows why the
first does not create the desired independent conflict: SHC coalesces start
(symbol94) with half(symbol97). `.reg` assigns both symbols to R12 in one entry;
the combined candidate has priority65539. It is inaccurate to say the new local
simply vanishes before allocation: the graph has38 candidates rather than37.
Phase profit remains-32772 and its home remains R5. The separate-phase graph
has37 candidates and likewise preserves the relevant profit/home and native bytes.

The staged mirror remains a distinct priority2 candidate and takes R4. The
phase profit improves to-29131, but k also improves to-28090 and takes R5;
the phase takes R6. Thus improving one profit in isolation is insufficient.
This candidate differs41 target bytes and changes previously matched506-byte
wave context, despite preserving all nominal roots and1220-byte total length.
It remains excluded.

Three additional controls test the mirror variable itself with and without its
ordinary C register hint, early or late in initialization. These do not create a
match. Removing the hint changes code size and protected context. No compiler,
IR, physical-register assignment, or object byte was patched.

## Native results

| Form | Target differing bytes | Missing bytes | Previous538 bytes exact |
|---|---:|---:|---|
|baseline (carried)|10|0|yes|
|j_after_phase (carried)|16|0|yes|
|staged_start|16|0|yes|
|staged_mirror|41|0|no|
|separate_phase|16|0|yes|
|m_plain_late|370|4|no|
|m_hinted_early|53|0|yes|
|m_plain_early|367|4|no|

All eight were freshly compiled and strictly rejected. No retained `.kept.o`
exists. All four nominal function roots remain correct. The two shorter cases
have1216 context bytes; the other six have1220. Raw differences and context checks
are independently recomputed rather than inferred from normalized scores.

## Verification and coverage review

Case: `tools/shc_probe/cases/session_wave_lifetimes198/`.
Five full native SHCMDL graph models and553 read-only hardware trace events are
verified. Every `.reg` and `.ilb` equals a fresh untraced control and native
capture. Five captured assembly outputs were independently assembled and resolved
to their strict compile bytes. Source/byte hashes, ELF relocations, actual roots,
protected context, coalesced symbol homes, graph sorting and mismatch counts are
checked by `verify.py` and `model_graph.py`.

A fresh canonical report review finds44 incomplete units totaling68,918 bytes.
The next target is the248-byte palette-gradient function29F94–2A08C. Prior
`session_gradient_step42/REVIEW.md` reports six byte differences from a blue-delta/
red-accumulator home swap, while surrounding924-byte prefix and72-byte tail match.
That older evidence is a target-selection lead, not a fresh matching claim.
Its136 prior source forms,945-iteration permuter, consumed addition helpers and
zero-initialization placements should not be repeated. The current complete
allocator tracing infrastructure was not used in that older study.

Next: freshly reproduce that gradient baseline, capture its complete allocator
graph, and determine whether the two values conflict, differ in profit, or are
constrained by bounds/eligibility. Choose a source experiment from that evidence.
Park the tested wave staging/hint forms. Wave's store-order and allocation
constraints remain documented for a later return. Checkpoint192's independent
clean builds and253 enabled tests remain the last production gates. The literal
100% goal stays active, solo, with the fixed denominator unchanged.
