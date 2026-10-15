# Literal screen of archived genuine drafts

Production remains checkpoint249: 135,910/200,698 bytes, 67.71866%. No source,
split, symbol, proof, compiler, or denominator changes were admitted. No full
production test suite was rerun for these research-only negatives.

The unfinished screen was completed with 19 original-compiler contexts covering
37 currently unmatched bodies, including no-call bodies omitted by the earlier
callee-name screen. Every context was freshly compiled on Mac and Nuada using
all 20 authenticated original SHC executables and the normal supported flags.
All allocated section contents, original function definitions, and original
relocations agree between machines in all 19 comparisons.

Of the 37 selected bodies, 23 retain their nominal entry, ten have shifted
entries and are explicitly excluded from instruction comparison, and four menu
bodies remain unsupported by this screen because their object also allocates
`.rodata`. The menu literal obstacle was separately investigated in253. Three
of the 23 nominal-root CFGs stop at an unresolved indirect transfer. This is
neither exhaustive coverage nor a complete call graph.

The private diagnostic `__quick_mvn` address was recovered from existing
ROM-backed evidence: ROM22618 contains060308ec and the original helper at
3106c–310a4 has SHA256
b001c5c96a4f3f179cad64688c1a241ead0f0fada80365b8611e0b3478bcc307.
The screen checks those bytes before resolving that compiler-generated call.
Production symbols are unchanged.

## The apparent9098 match is still rejected

The genuine context produces exactly the known4180-byte resolved binary, SHA256
47faef523976500611d7fbfcfce90e68778f67e34e19baff9f37b418535043de.
All80 reachable instructions of9098 match, including every literal load's
register, displacement, and value. There are no unresolved indirect transfers
in this body. Nevertheless the complete9098–9180 range differs in ten bytes,
90c6–90cf. Three preceding-function literals share this pool:

| Slot | Original | Native draft |
| --- | --- | --- |
|90c4|06065750|06065698|
|90c8|000fffff|06065750|
|90cc|06065698|000fffff|

Both full9098–9180 and enlarged907c–9180 fail the unchanged default strict
verifier at90c6. The already-retained28-byte907c–9098 range and its external
literal dependencies still pass. No synthetic cuts or normalized comparisons
can turn the ten different bytes into a match. Previous pointer, array,
consumed temporary, and inline-copy permutations remain closed.

## Interpretation and next action

The other22 nominal-root bodies all have actual instruction differences.
Across those bodies the screen finds22 same-PC literal-value differences, but
those are not22 source errors: reordered or shifted sequences may put unrelated
loads at the same address. A change to a callee or constant requires an
independent semantic/control-flow correspondence, not these positional pairs.
This screen confirms no additional wrong callee or constant.

The selection intentionally copies archived drafts; it is not a latest-best
candidate registry. For example, the selected save reader predates250 and the
selected field-helper context does not represent every later context recorded
in its STATUS.md. Therefore do not infer that these are the nearest current
matches or restart their old declaration sweeps. Before the next targeted
experiment, reconcile genuine current candidates with their latest evidence
and select a concrete untested discrepancy. Keep save-reader/ranking families
parked absent new structural evidence.

Evidence: tools/shc_probe/cases/session_literal_audit257/{ANALYSIS.json,
SELECTION.json,NATIVE_RESULTS.json,POOL_REJECTION.json,VERIFICATION.json}.
The initial Nuada conversion attempt failed because a local archive path was
absent; the existing installed converter was then hash-verified as byte-identical
and all19 compiles completed successfully. No process remains running.
