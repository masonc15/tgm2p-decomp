# Bounded follow-up to the accepted drawing match

2026-10-10. Previous goal turn: progress, with 202 bytes accepted and published.
This continuation: verified compiler evidence and bounded negatives; zero new C
bytes and zero percentage-point gain. Coverage remains 137,292 / 200,698
(68.407258667%). The goal is active, with 61,768 non-exempt bytes unresolved.

## Baseline and experiment results

All 229 inputs frozen by the accepted two-run cold replay were rehashed against
the canonical tree. The rebuilt program and report still have the accepted
identity and byte count. The frontier check passed.

| Investigation | Work | Result |
| --- | --- | --- |
| Producer signed link addresses, Sol | One ordinary source control, native capture and diagnostic replay; 18 traced compiler executable launches total | Next-value and previous-destination complete addresses separate. Only scaled signed index remains common. No fixed-address match. |
| Node splice, Sol | Three ordinary source compiles; internal launches not traced | All emit identical 84 bytes, with 41 differences; unchanged-output stop rule triggered. |
| Producer counter ownership, parent | Two ordinary source compiles | Removing comparator sign extensions is independent of first-loop addresses and leaves the extra eight-byte stack frame. Neither source preserves the drawing's fixed address. |

The signed-address candidate grows the comparable first-loop span from 142 to
150 bytes, against ROM's 156 bytes. Its complete object is 1556 bytes, moving the
drawing entry eight bytes forward to 23e46. Exact bytes at a displaced entry earn
no coverage. The candidate still has indexed zero-byte stores, a narrow counter
extension, differing allocation/scheduling and stack spills. The six-byte span
deficit is not assigned to one unproven cause.

Uniform int counters remove all four comparator sign extensions and produce
1524 bytes, drawing at 23e26. Keeping the first short counter and using a separate
int counter for later loops removes three extensions and produces 1528 bytes,
drawing at 23e2a. The first 134 loop bytes remain identical in these controls.
No further counter spelling sweep is justified.

The node audit found an older 41-difference baseline that was stronger than the
more recent 58-difference draft. Its first ten and final ten bytes are exact;
its complete function is still rejected. Independent instruction simulation
agrees on all five ordered numeric stores for 16,384 offset residues and eight
32-bit boundary cases. This supports width/truncation recovery, not portable C
memory validity, device behavior or byte-match credit.

## Parent review and preservation

The parent read and ran both agent verifiers. The rank verifier independently
GNU-links ordinary and captured objects, verifies native/debug REG and ILB
equality, reparses the definition-use graph, and checks unchanged inputs.
The node verifier checks raw ELF relocation/root/pool, strict rejection and
numeric simulation; the parent additionally GNU-linked all three original
objects independently. The parent counter packet also passed independent GNU
linking, root/byte/extension checks and canonical/tool hash checks.

Packets:
- research/2026-10-10-rank-block-recovery/REPORT.md
- research/2026-10-10-node-splice-typed-addresses/REPORT.md
- research/2026-10-10-node-splice-typed-addresses/PARENT_LINK_VERIFICATION.json
- research/2026-10-10-rank-loop-counter-ownership/REPORT.md

No source, split, shared proof tool, build input or ROM changed. No full test
suite or new cold replay was warranted for rejected research candidates.
No new dashboard progress entry is warranted. Routing and the goal checkpoint
were updated only after verification; rank packet's historical canonical hash
set pins the prior routing JSON, so a later routing-hash mismatch is expected
and is not evidence that its frozen compiler outputs changed.

## Next falsifiable questions

For f238fc, inspect GEN's zero-byte effective-address selection under the
authenticated graph: why does it choose @(R0,index), where the ROM builds an
explicit base-plus-stride destination? Audit archive overlap before choosing
one genuine lvalue representation and predicting an instruction-selection
change. Preserve counter width and signed-address ownership as separately
measured discrepancies; do not blindly combine all controls.

For f238a8, keep mixed unsigned integer address spelling parked. First map the
frozen baseline's base-copy definitions and lifetimes in native SHCMDL/REG to
explain ROM's R6 base/R7 offsets versus source R2 base/R6 offsets. No source
sweep is authorized by an unchanged output alone.
