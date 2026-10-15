# ROM-checksum allocation: tied accumulators and use weights

Continuation106 refreshes the genuine canonical checksum_30c2e source context.
All three original roots remain30880/30A44/30C2E, with1216 bytes total. The first
function differs in125 bytes, the second in54, and the retained274-byte suffix
is exact. These are positioned raw comparisons, not normalized scores.

## Allocation evidence

Unchanged SHCMDL diagnostics show both unsigned-long accumulators have priority9
and profit2. The high sum takes internal register4 (R7); the low sum takes6
(R13). The original uses the opposite assignment. Both unrolled input values
have priority9/profit3 and takeR6. The inner counter has priority11/profit5 and
R5. The bank counter has priority5/profit-2 andR10; its unsigned-short source
causes an unwanted extension. The bank count has priority4/profit-3 and spills.

Live read-only hardware traces reconstruct these weighted contributions:

- Baseline high and low priorities are1+3+3+0+2=9. Changing the low update
  from compound to ordinary assignment gives1+3+6+3+2=15, promoting it toR4
  and disrupting the input/inner-counter allocation.
- Ordinary bank-counter increment increases its priority5→7. Widening its
  source removes the extension, but these tested combined contexts shrink by
  four bytes and move the following roots; neither is eligible.
- A consumed input assignment in the high update reduces each unrolled input
  value's priority9→6. It moves the input toR11 and expands the context by4.
- Narrowing the high sum when consumed by the final checksum store splits it
  into priority8 and priority2 values, disturbing other registers. The cast
  preserves final low16 checksum observations, but does not match the ROM.

These weights are observations, not a claim that every assignment of the same
spelling has the same cost. Loop depth, unrolling and consumed results matter.

## Eleven native controls

| Source control | First function differences | Context bytes | Suffix exact |
|---|---:|---:|---|
| Baseline |125|1216|Yes|
| Ordinary low update |173|1216|Yes|
| Ordinary high update |131|1216|Yes|
| Both ordinary |174|1216|Yes|
| Ordinary low, wide bank counter |293|1212|No|
| Ordinary low, wide explicit bank increment |224|1212|No|
| Commuted final sum |123|1216|Yes|
| Original final store order |125|1216|Yes|
| Commuted sum and original store order |123|1216|Yes|
| High narrowing at final store |165|1216|Yes|
| Consumed input load |270|1220|No|

Changing final store order changes four context bytes even when total mismatch
counts are equal. The commuted sum changes the sum's instruction operands but
leaves accumulator allocation unchanged. The best genuine control retained for
future comparison is best_context.c (sum_and_stores),123 differences; both
whole-function proofs still reject. No source or assembly is integrated.

## Verification

Eleven captured compiler objects agree with ordinary native objects in every
ELF section except comments.22 normal/debug middle replays reproduce captured
.reg/.ilb exactly. Lossless .reg decode/encode and all826 priority-minus-average
conflict score records pass across the three functions. Ten hardware replays
reconstruct244 constant contributions/80 totals and277 variable contributions/
66 totals, preserving original middle outputs. Compiler binaries are unchanged;
only CPU debug registers are written by tracers.

A read-only diagnostic scan of582 local archived contexts excludes36 known
inert-control sources and records source/context hashes. It is not an
eligibility proof and gives no matching credit. The archived sweeps should not
be repeated. Canonical source, split map, verifier and denominator remain exact
checkpoint104. Both remote checkouts revalidate35 production pins, original ROM
hashes and127326/200698-byte reports. The last full test run remains104's236
passing tests; private rejected candidates do not require a new production build.

## Next action

Trace first candidate append and unstable sort ordering for hi/lo in the best
context, preserving inputR6 and inner-counterR5. Their priorities are tied;
large priority changes are now directly shown to be unsuitable. Compare genuine
changes in first-use/lifetime order and inspect the fixed conflict graph before
further native trials. The separate54-byte second-function mismatch remains
unresolved. The literal100% goal stays active with no external blocker.

Frozen sources, native outputs, intermediate streams, diagnostics, trace events
and independent analysis are in tools/shc_probe/cases/session_rom_checksum106.
