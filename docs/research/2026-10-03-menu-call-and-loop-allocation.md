# Menu call interfaces and allocation — continuation126

Production checkpoint122 remains **128,316 / 200,698 bytes (63.934867%)**.
Five consumed-source controls add no verified C bytes. No production source,
compiler, source map, proof, runtime policy, or denominator changed.

The previous turn is classified as progress: its genuine full menu context
explained shifted internal function pointers and identified A226 as the first
remaining allocation/frame discrepancy. This turn tests actual call interfaces
and control flow in that same context, rather than restoring padding or stubs.

## Source controls

All sources are reproduced by `session_menu_abi126/prepare.py` from the frozen125
source. Native SHC5.0r32 flags remain `-macsave=1 -optimize=1 -speed`.

| Control | Native result | A226 local stack reservation |
|---|---|---:|
| Baseline from125 |990 bytes; four short of original|12|
| Unspecified menu callback parameter lists |Identical native sections|12|
| Unspecified restart-callback parameter list |Identical native sections|12|
| Both interface changes |Identical native sections|12|
| Explicit guarded do/while busy wait |Identical native sections|12|
| Named pointer used for both real busy reads |Different output; still990 bytes|16|

The original reserves44 local stack bytes. These controls do not explain it.
The prototype changes preserve zero-argument call behavior. The guarded loop
performs the same initial read, calls and repeated reads as the original while.
The pointer control reads the same global at the same points; it adds no unused
local, physical register assignment or artificial padding.

Every linked A226 interval fails the unchanged verifier because the next real
root is A604 rather than A608. The nonempty data section also remains
outside this diagnostic's production proof scope: an ordinary diagnostic link with the real
rodata address does not expand supported production packaging. No code is accepted.
The first302 data bytes remain original. Raw differing-byte counts are857 for the
baseline and307 for the named pointer; the latter also shifts stack offsets and
register allocation. Those counts do not establish additional matching code.

## What the compiler actually chooses

Three fresh driver captures match the native ELF sections exactly, excluding only
comment metadata. Six normal/debug middle-stage replays reproduce the captured
REG and ILB files byte for byte for the entire36-function unit. Lossless REG
encoding/decoding checks pass. The allocator profit formula passes129 records
across the three A226 versions.

Two read-only hardware-breakpoint replays inspect constant-priority calculation
in the baseline and guarded-loop versions. No instruction or data memory is
patched. Both replay outputs equal the native/control intermediate outputs.
The recorded136 weighted contributions reproduce62 final priority totals.

- The `0x200` literal gets4 +4 +1 =9 and internal physical register11, SH-2 R8.
- The address of `g_6064750` gets4 +3 =7 and no physical register.
- The two source loop forms have exactly the same complete priority/member
  summaries. Rewriting while as guarded do/while cannot solve this mismatch.
- Naming the genuinely consumed busy pointer produces a variable with priority8;
  it spills instead of winning a register and grows the local reservation to16.

The debug printer calls the busy-address record NUMBER CONST with value33, but
the hardware trace identifies descriptor type4 and leaf33 `g_6064750`. It is a
symbol address, not the integer33. This distinction prevents a misleading
interpretation of the allocator log.

The first analysis attempted to apply the ordinary profit formula across all36
functions and hit a fixed-register constant with zero profit in another routine.
That failed expectation is saved in `INITIAL_ANALYSIS_FAILURE.log`. The129 profit
checks reported here are explicitly scoped to A226; they are not a general proof
of every SHC allocation path. Whole-unit intermediate equality still passes.

## Validation and next action

`ANALYSIS.json`, `LINK_ANALYSIS.json`, `REGISTER_ANALYSIS.json` and
`PRIORITY_ANALYSIS.json` bind the conclusions to source/object hashes, roots,
strict rejection and observed compiler events. `ALL_RESULTS.json` covers six
native compiles; the aggregate capture and middle receipts cover3 and6 runs.

A fresh production recheck verifies all44 pinned production files, unchanged ROM
hashes/report counts and eleven prior frozen cases in both nuada checkouts.
Both23-file trilogy ROM copies had already been reverified in125; the nuada copy
is rechecked here. The previous independent clean builds and243 enabled tests
remain the applicable production gates; no new full-suite run is claimed for
these unintegrated diagnostics.

Close these prototype and loop-spelling hypotheses. A226 still requires authentic
frame or variable-lifetime evidence; the unused32-byte padding remains excluded.
The neighboring ACDC helper has a concrete, separate discrepancy worth testing
next: the ROM preserves its actual palette ID and assembled value in callee-saved
registers, while the current draft passes them directly in argument registers.
Its ROM also uses a pointer to the count member and a different sequence-index
address expression. Investigate those consumed locals/member expressions in an
isolated genuine context, without interpreting equal lengths as matched code.
The literal100% goal stays active with72,382 bytes remaining.
