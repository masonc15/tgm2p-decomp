# Palette initializer lifetime improvement — continuation201

The genuine1504-byte initializer candidate improves from227 to119 differing
body bytes. Its first402 bytes (201 complete instruction/data words) agree with
ROM when compared from the actual function start. This is diagnostic progress,
not matching credit: the candidate begins at2A56A, twelve bytes before nominal
2A576, and still differs in90 words. All target retention attempts fail the
unchanged strict checker. Production remains131780/200698 bytes (65.66084%).

The previous goal turn made progress by excluding gradient source families and
selecting this distinct target. This turn adds a measured source improvement and
explains its allocation mechanism. There is no external blocker; the literal100%
goal remains active, solo, without brainstorming.

Evidence: `tools/shc_probe/cases/session_gfx_graph201/`. The best source is
`phase_pointer_in_loop.c`, with its original object and resolved context kept
beside it. No production source, proof tool, split map, compiler, or denominator
was modified. The existing246-byte canonical suffix was freshly strict-verified.

## Reproduction and source change

Canonical `src/gfx_2ab56.c` compiles to2936 bytes. Its initializer starts2A57A,
four bytes late, and occupies1500 bytes; the retained suffix starts correctly at
2AB56. The archived `phase_01.c` context reproduces2924 bytes and a1504-byte
initializer at2A56A, with227 raw body differences. Its source and output hashes
match the preserved historical evidence. The two initializer allocation graphs
are identical after normalizing heap addresses while preserving binding-group
membership. Their different layout is not a different allocator decision.

The improving change moves the existing `o = &PFX(n)` calculation into the body
of the final color loop, ahead of its actual uses. The two palette copies retain
their original PFX destinations. The pointer calculation has no side effects;
n is unchanged, and its bounded index-times336 offset fits signed short. This
is ordinary C describing the same accesses. No manufactured use, padding, fixed
register, IR edit, or extra external context was added to obtain the improvement.
The compiler output still has four expanded color-loop iterations,12 long
multiplies and72 arithmetic right shifts. No repeated runtime pointer-computation
claim is inferred from the source alone.

## Measured mechanism

The original assumption of a base/zero tie was wrong. In the archived baseline:

| Candidate | Priority | Neighbor count | Neighbor priority sum | Profit | Home |
| --- | ---: | ---: | ---: | ---: | --- |
| Global array address |3|17|196848|-11576|R12|
| Zero |5|18|262414|-14573|R11|

Excluding their mutual conflict, zero's sole additional neighbor is the final
color-loop index, symbol79, priority65568. The original ROM instead requires
zero inR12 and the global address inR11.

The improved source removes that extra zero/index edge; it does not add the
hypothesized address/index edge. Both constants now have17 neighbors. Zero's
profit becomes-11574, ahead of the address's unchanged-11576, and their homes
become exactly R12/R11. The constants' actual priorities remain5 and3. Three
read-only constant-constructor replays authenticate their contributions and
running sums, including the unchanged five unit-weight zero uses and three
unit-weight address uses.

The improvement also raises the real pointer variable's priority146 to153 and
changes the complete candidate count71 to76. Additional binding-group candidates
must not be described as five new source variables. All observed profits, sort
orders, and final register assignments are reproduced by the graph model.

`PREDICTIONS.json` records earlier offline hypotheses separately: adding only
an address/index conflict predicted the desired pair with two changed homes;
removing the final index's register bonus predicted the pair but changed13 homes.
Neither hypothetical graph was injected into the compiler. The observed source
mechanism is the removed zero/index conflict described above.

## Controlled alternatives

Twenty native controls cover the two reproduced contexts, named-zero lifetimes,
pointer sequencing, index initialization, and a separate final loop counter.
Naming zero only for the actual stores folds back to byte-identical baseline,
even with a register hint. Naming it for all five uses disrupts loop expansion:
the hinted archived version has39 candidates, one loop index increment and three
long multiplies rather than71 candidates, four increments and twelve multiplies.
It produces764 initializer bytes and is not a matching substitute.

Moving the pointer calculation before or after i=0 inside the for initializer
produces identical230-difference outputs. A separate i=0 after the palette copies,
followed by the pointer assignment and `for (;...)`, produces189 differences and
the right base/zero pair. Initializing i before the copies produces490 differences;
a separate unhinted final counter produces279. All remain excluded.

Thirty-six distinct archived genuine predecessor bodies were then paired with
the improved initializer. Their only changes are ordinary declarations; k/i stay
within0..32, so the tested integer widths retain their values. No combination
places the initializer at2A576. Snapshot sources and hashes are in `archive_inputs`
and `CONTEXT_INPUTS.json`; this avoids depending on old case directories missing
from the isolated checkout. This was a composition check of the new initializer,
not a new claim about the earlier failed type search.

Two further controls join the actual preceding palette routines from29BF8.
Their shared structures use the known compatible pal/mode layout at offsets4/6;
no fake predecessor or padding is inserted. All19 actual function roots are
recorded. Joining leaves the archived improved initializer's root and119 body
differences unchanged; the canonical predecessor version still starts four bytes
late. This larger genuine context does not resolve the boundary problem.

## Remaining discrepancies and next action

`BEST_DIFF.json` independently records all90 differing words/119 bytes. The first
different word is at body offset402 (nominal2A708). One remaining problem is a
three-way constant tie: all color masks have priority32/profit-41991.

| Mask | Current home | ROM home |
| --- | --- | --- |
| FC000000, red |R11|R13|
| 00FC0000, green |R13|R11|
| 0000FC00, blue |R12|R12|

Their native constructor positions are71/72/73 in red/green/blue order. Later
expanded color calculations also swap f/t temporary homes; correcting just the
mask pair would not establish a full match. Continue with the constant creation
and sorting evidence, and preserve the improved pointer lifetime. Avoid another
broad declaration or named-zero sweep.

The preceding update/fade function still needs a genuine reconstruction that
places this initializer at its true boundary. The36 archived forms and joined
palette context are now verified negative boundary controls; do not re-run them
unchanged or shift object bytes to manufacture a valid entry.

## Verification

58 strict negatives,14 output-preserving compiler replays/1728 events,11 full
allocator graphs, and11 independently assembled native captures pass the case
verifier. Traced REG/ILB output equals fresh untraced controls and native captures.
Original native objects independently reproduce all resolved bodies, roots,
source/output hashes, raw scores, and protected suffix comparisons. The402-byte
agreement is explicitly a shifted-body comparison, never a retention proof.

The initial graph equality check was corrected to normalize heap pointers while
preserving group memberships. The word-level first-difference assertion checks
the full two-byte instruction, whose differing byte is its second byte. These
were verifier assumptions, not changed compiler results. Previous frozen evidence
is unchanged. No full build/test rerun is warranted for excluded diagnostics;
checkpoint192's clean builds,54 production pins and253-test gates remain the
production evidence. Final checkpoint verification rechecks both Nuada copies.
