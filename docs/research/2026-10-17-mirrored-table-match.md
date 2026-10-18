# Mirrored video table: 82 accepted bytes

Solo work on Nephele recovered `f_bd8a`, ROM `[0xbd8a,0xbddc)`, using
ordinary SHC v5.0r32 (`-macsave=1 -optimize=1 -speed`). Coverage rises from
138,670 to **138,752 / 200,698 bytes (69.134720%)**, a gain of **82 bytes and
0.040857 percentage points**. The agreed goal remains 199,060 linked C bytes;
60,308 non-exempt bytes remain. No exceptions or proof rules changed.

## Source operation and prediction

The ROM writes 224 video-table entries as mirrored pairs `i` and `223-i`,
each with value `(0x1c00 + i * 0x100) | 0x7f`. All 504 distinct archived
candidate bodies explicitly declared a descending pointer. Previous pointer,
register and counter sweeps were already exhausted.

The new candidate expresses the two indexed stores directly, allowing SHC to
construct its own descending-address and value recurrences. Before compiling,
the packet predicted the exact 82-byte routine, two-fold unrolling, original
register assignment, roots, four literal dependencies and 166 protected bytes.
The first candidate met all those predictions: one driver invocation, eight
compiler stages out of the shared 24-stage budget. The full ordered sequence
of 224 stores was also checked against the original operation.

The genuine settings translation unit begins at `0x9c0c`. Its preceding
`f_bc02` coordinate is reconstructed as `short`, consistent with its sprite
field and callee argument; the earlier `int` draft displaced the following
root. That predecessor earns no coverage. The new source differs from the
existing settings context only in this coordinate, the mirrored-loop body,
and the retained-range header/comment. Other accepted source owners are unchanged.

The emitted recurrence behavior is observed. The attribution to different
source-defined versus compiler-created allocation records is an inference
supported by earlier SHCMDL research, not a new instrumented trace. See
[the compiler recurrence study](2026-10-03-shc-rank-recurrence.md).

## Admission and replay

- Existing function-code-range proof accepts all 41 instruction/delay-slot
  halfwords at the actual `0xbd8a` entry; successor remains `0xbe04`.
- Four literal dependencies match at their original addresses; the shared
  trailing pool `[0xbddc,0xbe04)` remains assembly.
- All 166 protected bytes at `[0xb2f4,0xb39a)` remain exact in the new context.
- Isolated and canonical `make split check report` pass. Two independent cold
  replays reproduce the original program and both original EPROM hashes.
- The full configured test suite passes: 336 executed, five optional shiftable
  build tests skipped. The first run had one `sh-elf-nm` subprocess failure in
  an existing HUD test; an unchanged complete rerun passed. Both logs survive;
  the initial subprocess failure's cause is not established.

Private replay/evidence packet: `research/2026-10-17-nephele-mirrored-table/`.
`INTENT.json`, `ledger.json`, `RESULT.json`, `ACCEPTED.json`, and the acceptance
source review, tests and cold receipt preserve prediction and verification.
`PUBLICATION.json` records live dashboard verification after push.

Next investigate whether another authentic indexed operation explains a
compiler-created recurrence currently misrepresented as explicit C locals.
Qualify against the archived negatives before compiling; this match does not
reopen the exhausted pointer/register spelling families.
