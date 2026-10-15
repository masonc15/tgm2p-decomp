# Animation count/setup register pair — continuation146

Production remains checkpoint122: 128,316 / 200,698 bytes (63.93487%),
72,382 bytes remaining. No source was integrated and no moved-byte credit was
awarded. Every candidate starts fA608 at A604 instead of A608 and fails the
strict context verifier. The preceding fA226 still needs authentic source for
its different frame; this investigation does not solve that boundary.

## Source evidence

Starting from frozen145 `combined_late`, replacing `hold = last = n - 1;`
with `n = n - 1; hold = last = n;` restores header count R10 and setup sprite
R9. The latter now emits three word stores to the real sprite home at stack
offset zero. With the original short saved `cnt`, mask15 and constant2 occupy
the opposite registers from the ROM. Widening saved `cnt` to int or long
recovers all six target roles together: total on stack, hold R10, mask15 R9,
constant2 R8, header count R10 and setup sprite R9. The frame is 108 bytes;
first four homes are sprite_id0, sel4, tbl8 and total12. This is a genuine
consumed C change compiled by unchanged SHC, with no forced allocation.

The wide-count form tests the cached promoted count from stack44. Replacing
`if (n == 0)` with `if (!n)` restores `EXTS.W R10,R2; TST R2,R2`, but exchanges
mask15/constant2 again. Casting the equality operand to long is a native
no-op. Restoring either single hold update to the combined expression does
not repair that exchange. Remaining homes also differ: spr1 is52 versus the
original16, and the saved count is44 versus72. Those limitations prevent a
matching-C claim despite recovering the setup stores and register pair.

| Control | Body bytes | Result |
|---|---:|---|
| bound_from_header | 1756 | Count spills; constants exchanged |
| total_from_header | 1740 | Count/setup pair still exchanged |
| late_count_copy | 1756 | All allocated sections equal bound_from_header |
| decrement_count | 1748 | Correct count/setup pair; constants exchanged |
| predecrement_count | 1744 | All allocated sections equal145 combined_late |
| decrement_count_int | 1744 | All six roles recovered; cached zero test |
| decrement_count_long | 1744 | All allocated sections equal decrement_count_int |
| count_zero_not | 1744 | Original register zero test; constants exchanged |
| count_zero_long | 1744 | All allocated sections equal decrement_count_int |
| zero_not_hold_first | 1748 | Single split hold update does not fix constants |
| zero_not_hold_second | 1744 | Other split hold update does not fix constants |

There are eleven controls and eight distinct native text hashes. Equality
claims above compare every allocated section including flags and alignment.
All eleven retain six detected loops, no inline-return gotos,36 exported real
functions and no unnamed local call targets in the audited function.

## Why the count priority changes

Read-only hardware traces recover header priorities4,6,5 for combined_late,
decrement_count and decrement_count_int. The explicit decrement adds a real
assignment weight2; its input read weighs1. The wide count eliminates the
header read from the zero test by using the cached promoted count instead.
The model reproduces356 allocation candidates across four graphs. On the
baseline fixed graph, increasing count priority by one is the smallest
single-priority solution; actual source changes must also satisfy the other
code-generation constraints, so this sensitivity result alone is not credit.

An initial verifier incorrectly compared initial variable weights directly
with the later pre-sort table. It failed on wide saved cnt:2 initially,4
later. Inspection identified FUN_00410cf0's merge via FUN_00411010. A second
read-only hardware trace observes the actual merge arguments and priorities
and checks the resulting table after the pass:2+4-2=4. The baseline also
merges4+2-2=4, while the short explicit-decrement form has no merge. The
analyzer now verifies both distinct phases, rather than weakening the final
priority assertion. All153 initial variable priorities and662 weighted terms
reconcile, followed by the two observed merges and the final symbol table.
Heap identities and symbols are joined only within the same traced process.

## Validation and retained evidence

- Eleven native compiles and captures,22 middle-stage replays and11 CFG audits.
- Seventeen hardware replays, all with exact captured .reg/.ilb or applicable
  stock-output equality. Earlier variable traces remain in `_variable_premerge`;
  initial no-symbol-table trace remains `_variable_initial`.
- Two home models reproduce54 home records and271 frequency contributions.
- `verify.py` passes including source regeneration, exact native capture checks,
  REG round trips, strict rejection checks and the constraint analyses.
- `recheck_production.py` confirms44 production pins and31 prior frozen cases
  in both Nuada checkouts, unchanged original ROM/report hashes and all23
  reference ROM files. Full production build/test gates were not rerun for
  private probes; the latest full gates remain checkpoint122.

Evidence: `tools/shc_probe/cases/session_animation_registerpair146/`.
Next test standalone pre/post-decrement statements under the logical-not
condition: they may change assignment representation while retaining the
required count read. If they canonicalize identically, retain that negative
result and inspect a different actual source dependency rather than repeat
syntax-only searches.
