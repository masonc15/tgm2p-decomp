# Animation total priority — continuation131

The original compiler's per-use trace explains `total`'s priority15 exactly.
Four native controls remain unmatched; production is still checkpoint122,
**128,316 / 200,698 bytes (63.93487%)**.

## Measured contributions

The baseline is continuation130's `tail_selection.c`. Hardware execution
breakpoints observe the original SHCMDL base weight, weighted contribution and
final priority. The trace adds the compiler's IR point number to correlate each
event with the final printed tree and the actual source line.

| Source operation | Contribution |
| --- | ---: |
| Initial `total = 0` |1|
| Display argument |2|
| Forward update's read |2|
| Forward compound update node |0|
| Forward wrap comparison |2|
| Forward reset to zero |2|
| Reverse update's read |2|
| Reverse compound update node |0|
| Reverse negative comparison |2|
| Restore from last index |2|
| **Total** |**15**|

The three controls combine genuinely consumed assignments while preserving
their order and values:

- Initial `total = 0; sel = 0;` becomes `sel = total = 0;`.
- Forward-wrap `total = 0; pos = 0; hold = 0;` becomes
  `hold = pos = total = 0;`.
- The third control combines both changes.

All four cases have the same ten contributions **[1,2,2,0,2,2,2,0,2,2]**.
`total` remains priority15/profit7 in R8. These grouping forms do not realize
the earlier fixed-graph priority12 counterfactual and should not be repeated.

Each body is1676 bytes with a112-byte local frame, starting at A604 rather than
A608. The original is1748 bytes with a108-byte local frame. Each diagnostic
relative comparison has1525 differing bytes; their complete native outputs
are distinct despite identical aggregate metrics. None is accepted.

## Validation

Four independent captures reproduce their ordinary native objects, excluding
only the build comment. Eight normal/debug middle replays and four hardware
priority replays reproduce captured `.reg` and `.ilb` byte for byte. Register
tables for all36 functions round-trip in each case. The analyzer verifies200
complete variable-priority groups and correlates all40 `total` contributions
to source lines. Six reachable division calls remain in every candidate.

The previous case's reused call-audit script initially printed a hardcoded
"nine candidates" although its JSON contained only these four. Its message now
uses the actual count and the audit was rerun. No verification assertion or
scope changed.

Both Nuada checkouts retain all44 production pins, original ROM/report hashes,
and sixteen prior frozen cases. No source map, retained C, proof rule, target
ROM or denominator was changed. Full-suite tests were not rerun for these
private negative experiments; checkpoint122 remains the last complete gate.

## Next investigation

The priority12 result was a hypothetical weight change on a fixed graph. It is
not an original-source finding. With the ten observed members and these control
weights unchanged, total necessarily remains15. Further work must establish a
real change in value membership, interference or use weighting, rather than
assuming assignment grouping will supply the missing three points.

Audit the original stack accesses and value lifetimes next. `total` occupies
the local-frame slot at offset12: initialization at A62E; forward read/write
at AAB8/AAC0; comparison/reset at AAE4/AAF4; reverse read/write at AB38/AB40;
negative comparison/restore at AB64/AB72. Its display read at A9E0 uses
`@(24,r15)` after three four-byte pushes, referring to the same frame slot.
Track temporary argument pushes and delayed cleanup before assigning every
SP-relative access to a local. Three calls to ACDC explicitly pass SP+4 for
the selection variable; do not infer an address escape for total from those.

Use that audit to compare the current compiler's actual value lifetimes with
the ROM. Do not add local volatility, forced registers, aggregates, padding or
dummy uses merely to make total spill. Evidence is in
`tools/shc_probe/cases/session_animation_total131/`, particularly
`PRIORITY_ANALYSIS.json`, `CALL_AUDIT.json` and the linked disassemblies.
The literal100% goal remains active and incomplete.
