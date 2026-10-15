# SHC's late live-range remapping explains the object-list register choice

Continuation102 identifies the R1 substitution in the original compiler. It
adds no matching production bytes. The best 251CC candidate remains 9 raw byte
differences in an 82-byte function, with both retained ranges intact. Verified
production remains 127,276 / 200,698 bytes (63.41668%).

## Mechanism established by hardware tracing

The initial SHCGEN instruction emitter still uses the physical registers in
SHCMDL's `.reg`: R14 for the named next-table pointer, R13 for literal127, R12
for the bucket pointer. The scratch selector at418900 is not the source of the
persistent R1 substitution.

At function completion, SHCGEN calls41A850 from41A480. Its helper41AC70 merges
ranges sharing physical registers, computes their inclusive total lengths, and
orders the unflagged records by increasing length. This is an exchange sort,
not a stable sort guarantee. The observed flags are all zero; the recovered
model deliberately covers this observed path, not every special compiler mode.

41A850 scans that order, attempting to fit each eligible saved-register value
into R0, R1, R2, then R3. Every live interval must fit within the selected
scratch register's free intervals. The first successful value consumes its
intervals, reducing what remains available for later values. The compiler
appends a source-register/destination-register/interval mapping to the function
metadata. SHCPEP later applies that metadata.

| Control | Next-table range | Bound127 range | First eligible mapping |
|---|---|---|---|
| Canonical baseline100 | R14:6–13, length8 | R13:6–13, length8 | R14 → R1 |
| condition_outer101 | R12:6–12, length7 | R13:6–12, length7 | R13 → R1 |
| named_next101 | R14:5–13, length9 | R13:7–13, length7 | R13 → R1 |

These coordinates are compiler live-range positions, not ROM addresses.
R1 is free throughout each function before remapping. The competing R0/R2/R3
intervals do not contain these complete ranges. Baseline's address record wins
the equal-length ordering; condition_outer's bound wins its tie; named_next's
bound is strictly shorter. This explains why merely obtaining the correct
middle-pass physical register numbers was insufficient.

Hardware breakpoints at41A850,41A8B0,41AA55 and41AC61 record pre-normalization,
normalized candidates/free intervals, appended mapping and final intervals.
The model reproduces all24 table positions, all three mappings and all final
scratch free lists. No executable instructions or compiler data were patched;
the tracer writes only CPU debug registers.

The separate emitter trace at41B3D0 records3,502 instructions over the three
contexts. SHCPEP's output writer414330 records114 target instructions: baseline
loads through `(R0,R1)` and compares R13, while named_next loads through
`(R0,R14)` and compares R1. This corroborates the actual native mismatch.

## Exact replay and an input-capture limitation resolved

Direct SHCGEN replay from cleanup snapshots produced matching traced/control
outputs but differed from the original capture in symbol references. Direct
SHCPEP replay from those snapshots failed, and regenerating only SHCGEN still
left inconsistent metadata lengths. Those exploratory runs are not the basis
for original-output equality claims.

The final replay reconstructs preceding stages using the unchanged original
SHCFRT and SHCMDL executables, source and captured configuration. Nine final
SHCGEN hardware replays reproduce the originally captured `.sua` byte for byte;
all traced/control `.sua` and `.asa` outputs agree. `.asa` cleanup metadata
still has recorded position differences. Three final SHCPEP replays, after
reconstructing SHCFRT/SHCMDL/SHCGEN, reproduce both original `.sub` and `.asb`
exactly. The preserved exploratory `GEN_REPLAY_RESULTS.json` explicitly records
its unequal cleanup comparisons; use FRESH_GEN/REMAP/EMIT/PEP results for final
claims. The scratch model independently predicts all1,676 selections in the
fresh original-output replays.

The code-generator export is an unchanged re-export of the existing analysis.
SHCPEP was newly imported and decompiled for inspecting the consuming stage.
Decompiler output is inferred C, not recovered original source.

## Seven source controls closed

Starting from named_next101, all of these compile to the exact same full
2,532-byte context as that best candidate:

- A real `short limit=127` initialized before k, before l, or before next and
  consumed by the outer comparison.
- A real `long limit` initialized before next and used in that comparison.
- Moving next initialization into the outer condition, each row body, or the
  nonempty-row branch before the actual traversal.

The compiler normalizes these changes. Naming a bound or moving this invariant
assignment does not change the emitted result. Seven native capture comparisons,
14 exact normal/debug middle replays and56 priority-score checks pass. Every
raw target proof rejects at9 bytes, and both retained-range proofs pass. No
whole-project build is claimed for these rejected research candidates.

## Next action

Model both allocation stages together for source experiments. The object target
needs k/R4, s/R5, i/R6, hardwarebase/R7, bucket pointer/R12, bound/R13 and next
address/R14, then next-address→R1. Anonymous next-address construction can share
the bound's range, but the current condition_outer allocation gives it the wrong
physical register and tie order. Named next fixes physical allocation but makes
its range longer. Seek a genuine store/index expression that preserves the
anonymous address and changes its allocation order; do not repeat declaration,
qualifier, named-bound or initialization-placement sweeps. Existing separate
final-decrement source remains rejected for its shorter function.

Evidence is frozen in `tools/shc_probe/cases/session_obj_backend102`. Production
recheck confirms all33 checkpoint84 pins, original ROM hashes and report metrics
in canonical and coordinator checkouts. The full100% goal remains active.
