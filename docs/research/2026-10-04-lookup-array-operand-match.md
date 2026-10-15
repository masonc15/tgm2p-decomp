# Lookup: a typed array operand recovers the ten-byte function

Continuation 218 matches f_8b7c at 8B7C–8B86. The preceding 92 readiness bytes
remain unchanged. Production can retain the combined102-byte prefix from a
374-byte context, excluding272 bytes of the following preparation function.
No compiler, proof, object or ROM patch is involved.

Previous lookup experiments removed an unused parameter-address expression in
the following function and consequently had wrong literal-pool displacements.
Checkpoint217's consumed parameter-address expression fixes that layout. The
refreshed scalar lookup now has exactly two differences: loading its index
address intoR3 and reading the word viaR3 instead ofR0.

The matching source declares the index storage as `extern short g_6060036[1]`
and reads its element zero. This is a two-byte view at the same external symbol,
with the same signed16-bit read and index, not a wider access, new storage,
forced register, dummy use or changed address. Struct and union scalar views
still have two differences. The native captures authenticate both symbol values
and the resulting five-instruction leaf; table addressing and return are unchanged.

Read-only native traces explain the result. The scalar source enters the parent
recipe0x180080 as symbolic absolute memory(kind13), destinationR0, mask0. The
child planner adds the destination dependency bitR0 and calls the scratch
selector with exclusion1. That selectsR3 for the address. Its lookup selections
areR0,R3,R1. The array source materializes its address intoR0 first; the same
parent recipe now sees a register-based memory operand(kind8/R0) and mask1.
It bypasses that additional child address-allocation call. Its selections are
R0,R0,R1, matching the ROM exactly. The independent dependency model checks both
parent masks and the scalar child reservation; the selector model reproduces
all98 choices across both complete three-function compiler traces.

Fifteen source attempts include14 compiled controls and one compiler rejection.
The supported compiler rejects the standard-C reversed subscript spelling with
'Index not integer' and 'Illegal array reference'; that diagnostic is preserved.
One of the14 compiled controls is strict-exact. All preserve the readiness bytes.
The genuine forms cover operand order, consumed index/result locals, direct
addresses, array/struct/union storage views and value casts.

Six native replays preserve fresh untraced SUA/ASA output byte for byte across
117 events. Two native assembly captures independently reproduce their objects.
The original operand tracer expected at least one allocation call; the matching
array source correctly makes zero such calls. Its initial harness failure is
preserved. The tracer now accepts zero calls only after normal compiler exit,
all three function entries and no pending trace phase; verification explicitly
requires the absence of the child request and the independent parent/scratch
records proving the alternate path. This changes diagnostic tracing, not matching
acceptance or compiler behavior. Snapshot ASA metadata differences remain explicit.

Production uses the existing strict prefix and reviewed readiness tail proofs.
Fifteen external literal loads must match the ROM; only the complete readiness
and lookup functions are exported and credited. Full integration and dashboard
receipts are in tools/shc_probe/cases/session_checkpoint218_lookup.

Next apply scalar-versus-array operand construction to a remaining global-load
near-match where native tracing shows a separate address register is the problem.
Do not repeat the closed lookup locals, casts or aggregate-scalar views. The full
literal100% objective and its fixed200698-byte denominator remain active.
