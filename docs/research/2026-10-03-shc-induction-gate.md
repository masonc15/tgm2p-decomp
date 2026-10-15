# SHC induction recognition and recurrence cap — continuation95

Two original-compiler rules now explain the failed explicit-counter experiment:
SHC scans top-level update nodes without descending into a comma expression, and
it constructs at most three recurrences per loop. Nine new source controls are
rejected; production remains **127,276 / 200,698 bytes (63.41668%)**. No production
source or verification rules changed.

## Direct induction-recognition evidence

At401665 both the baseline and explicit-rank loop pass the entry condition. At
4016DC neither has the disqualifying loop flag0x80. Both reach candidate processing
at401714. Therefore the earlier broad loop gate is not the cause.

The scan at401830 visits these top-level nodes:

| Source | Mode1 loop line | Nodes | Recognized induction variables |
|---|---:|---|---:|
| baseline |282| lt, call, pri (i++) |1|
| explicit rank in comma increment (case94) |284| lt, call, comma |0|
| separate rank update in body |284| lt, call, assign, pri |2|

The original x86 at401880–401889 subtracts0x38 from the opcode and rejects values
above0x27. Opcode0x6C (comma) is outside that range. The scanner advances to the
next top-level statement at401BE4; it does not recursively find i++ inside the
comma node. In contrast, baseline i++ passes401C30 (return value1 observed at
4018D7), and the induction table cursor advances1→2. Separating the actual rank
update makes the cursor advance1→3. The raw trace field `induction_count` is this
one-based next-slot cursor, so the recognized count is cursor minus one.

All three loops pass the broader gate. Their recurrence transformation counts
are3,0,3 respectively. These observations isolate the cause without modifying
compiler state or relying on a hypothetical register-graph perturbation.

## Direct three-recurrence cap evidence

At40233D the original compiler compares global433030 against3 and branches out
when the value is at least3. Hardware observations record each pending candidate
before that comparison. Mode1 baseline visits:

1. Row pointer g6079474+i*8 at count0.
2. Coordinate i*22+170 at count1.
3. Rank i+1 at count2.
4. Wait i*12+36 at count3; this candidate is skipped.

With a separately updated explicit rank, the mode1 candidate list is row pointer,
coordinate, then wait. The wait expression is selected at count2. Constructor
observations identify synthetic−6 as i*12+36, not the baseline's i+1. The
coordinate and rank use the same priority19, but an additional live wait value
changes the interference graph. The loop index i has priority9 and spills in all
four separated-update controls. This explains why fixing recognition alone does
not restore the original machine code.

Mode3 remains a separate loop: its first three selected recurrences are index
scale, coordinate, and rank, with wait again rejected at count3. Synthetic IDs
must always be interpreted with their observed expression and loop, not assumed
to preserve their meaning across source variants.

## Source experiments

Every variant preserves three calls with the original argument values and uses
only real consumed computations. The short/unsigned-char wait intermediates
remain exact over i=0,1,2; neither overflows. The shift expression is likewise
exact for this nonnegative bounded index. No artificial consumers or padding
were introduced.

| Control | Positioned differing bytes | Following root |
|---|---:|---|
| Rank updated separately in body |1139|12FA8|
| Coordinate updated separately in body |1134|12FA8|
| Both updated separately in body |1131|12FA8|
| Rank update in header, i++ in body |1140|12FA8|
| Rank body plus short wait product |1139|12FA8|
| Rank body plus unsigned-char wait product |614|12FCC|
| Rank body plus wait expressed with shifts |1098|13018|
| Rank expression named within each iteration |707|12F98|
| Coordinate expression named within each iteration |633|12F9C|

The short-product variant reproduces the entire rank-body compiled context
exactly. All nine fail the default verifier; none improve the original nominal
32-difference baseline. The two named-expression controls require further IR
inspection before proposing any related change. Their lower byte counts are
not valid improvements over that baseline.

## Reproduction and verification

Case: `tools/shc_probe/cases/session_rank_gate95`. `prepare.py` deterministically
regenerates all nine sources from `input.c.txt`. The existing normal compiler,
flags, ROM, roots, and contextual prefix verifier remain authoritative.

`analyze.py` verifies nine native capture comparisons,18 exact normal/debug
middle-pass replays,300 priority/profit calculations, source regeneration and
hashes, and nine exact hardware replays. The six compact gate/scan logs contain
218 records. Three AST logs contain28,815 node observations. Every hardware
replay reproduces the captured.reg/.ilb byte-for-byte with unchanged input
configuration. Only hardware debug registers are written by tracing.

`GATE_ANALYSIS.json` contains actual candidate expressions and table counts.
`ALL_RESULTS.json`, `ANALYSIS.json`, and `REGISTERS.json` aggregate all sources.
Batch WATCH/REPLAY summaries cover the final batch only; validation uses every
per-source output. Gate and scan controls reference frozen case94's baseline and
explicit-rank captures; new sources use case95 captures. Replay receipts pin
source and compiler hashes. Compiler C listings are inferred decompiler output;
original instruction disassembly is saved alongside them.

Both nuada checkouts freshly pass33 checkpoint84 production pins, original ROM
hashes, and the fixed-denominator progress report. No full production rebuild was
needed for these rejected private experiments. Checkpoint84 remains the latest
full build/test gate with234 enabled tests.

## Next action

Inspect candidate order, reuse, and subsequent copy elimination in the two named
per-iteration expression controls. They preserve a standalone induction update
and move a real expression ahead of the call, so they test candidate ordering
without adding an independent counter. Use the observed AST and allocation graph
to explain their shorter bodies before attempting another source form. Keep the
original32-difference baseline; do not repeat the nine closed variants or credit
position-shifted comparisons. The independent mode0 pointer tie remains open.
