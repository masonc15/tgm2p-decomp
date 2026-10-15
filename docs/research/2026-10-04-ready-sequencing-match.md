# Readiness: cache aging and scratch history recover 92 bytes

Continuation 217 matches the complete f_8b20 at 8B20–8B7C using the unmodified
SHC toolchain and existing literal-tail proof. No proof extension is required.

Continuation 216 established that the original equivalent combined flag
expression left two constants at the same cache age; the resulting R3 eviction
caused eight differing register bytes. A new split global update, `flags |=
0x802; flags &= ~0x700;`, gives the second constant age 8 instead of 7. Native
traces show R1 is now evicted at emitter 30, while R3 remains cached. The final
machine instruction sequence still has the original single final store. This
recovers the first field clear and tail-call register, leaving five differences.

Changing the first field clear from explicit assignment to compound `&=` fixes
the remaining choices. With explicit assignment, additional scratch requests
select R0 and leave last=R0 at the second clear. With compound assignment, the
first result leaves last=R2. The observed selection sequence becomes R2 for the
first clear, R1 for the second, R2 for the call and R3 for the final tail. All
92 bytes now match, including the original PR-restoring tail delay slot.

Both changes are necessary in the tested baseline: compound assignment alone
has ten differences; splitting the global update alone has five. A temporary
holding the flag result changes cache ages but occupies R5 and removes the
required offset reload, so it remains rejected. The matching source adds no
forced registers, compiler/IR/object patches, assembly, padding or dummy uses.

The historical diagnostic source had 4,204 context bytes and an unused parameter
address expression in f_8b86. The production source reduces context to 374 bytes,
keeps only the following lookup and preparation functions, and replaces that
unused expression with the consumed input `g_6064898[*(&id)]`. SHC still homes
the parameter; normal named pointer aliases change the layout and fail. Only
92 bytes are retained; the other 282 bytes remain excluded. Thirteen external
literal loads are authenticated against the ROM. The existing reviewed f_20250
tail proof authenticates its relocation, target, load dominance and delay slot.

Evidence: 32 native controls, three strict matching variants, three independently
assembled native captures, and eight output-preserving read-only GEN replays.
The verifier checks 1,921 events, all 1,588 scratch selections and 18 cache
choices. SUA and ASA match fresh untraced controls. Snapshot ASA metadata
changes are explicitly recorded. An initial assembly-capture verification
failed because decode_reg.py had not been copied; that failure is preserved,
the helper was restored, and the complete capture verification then passed.

Production validation and publication evidence live in
`tools/shc_probe/cases/session_checkpoint217_ready`. No full-context or unverified
neighbor bytes are credited. The fixed 200,698-byte denominator and all 1,638
runtime assembly bytes remain part of the active literal 100% goal.

Next inspect the adjacent ten-byte lookup at 8B7C, or apply the observed compound
assignment scratch-history effect to another remaining near-match. Do not repeat
the closed readiness aliases and sequencing controls.
