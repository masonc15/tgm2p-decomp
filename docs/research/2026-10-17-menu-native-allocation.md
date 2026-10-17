# Menu native allocation and bounded source controls

Coverage remains **138,670 / 200,698 linked C bytes (69.093862%)**.
This round accepts **0 bytes, +0.000000 percentage points**. The agreed
completion target and 1,638 assembly-byte exceptions are unchanged.

## Authenticating the evidence

The earlier direct MDL replay used post-driver intermediates and did not match
native REG. Fresh native captures followed by frontend regeneration and separate
normal/debug MDL replays reproduce both native REG and ILB byte for byte for the
original and corrected-palette-ABI sources. Independent GNU linking reproduces
the earlier ordinary output too. This used 22 compiler stages, within the 24-stage
packet budget. Evidence: research/2026-10-17-menu-native-auth/AUTHENTICATED.json.
The previous failed replay remains preserved, rather than retrospectively
labelled authenticated.

The recovered sort model matches all 123 native sort events. The original
palette draft has a mask web at priority 4/profit -6; narrowing its palette-call
contract removes a low-priority interfering web and lowers the mask profit
to -7. This explains why the ABI correction did not recover the ROM allocation.

Two offline predictions were translated into genuine source controls. These
predictions are diagnostics, not altered compiler output or accepted bytes.

| Ordinary source control | Native observation | Actual f29ea entry | Raw differing menu bytes |
| --- | --- | --- | ---: |
| Position derived as 100+20*i | Final counter priority 11→13; mask R8 and original stack roles recovered; position is recomputed instead of incremented | 29f0 (+6) |542|
| Initial four-element loop uses do-while latch | Counter priority 15→7; mask R8 recovered; required unrolling disappears |29cc (-30)|2053|
| Actual 12-byte sprite-record table type | Complete 7,352-byte context identical to baseline |29f0 (+6)|2197|

Each control used 11 compiler stages: one original native capture and the three
frontend/normal/debug stages. Both replay streams equal native REG/ILB for every
control. All raw measurements use actual nominal addresses; no displacement
normalization, rebasing, padding, patched object or modified compiler output is
used for progress. The required roots/layout do not match, so none was staged
for acceptance. All sources, diagnostics, objects, raw differences, roots and
neighbor measurements survive in the corresponding research packets:

- research/2026-10-17-menu-derived-position/
- research/2026-10-17-menu-initial-latch/
- research/2026-10-17-menu-frame-type/

The record type is supported independently by the matched drawing definition
in src/f_11518.c and ROM table 36334/36338, which points to pairs of single-piece
12-byte records. It is not an invented aggregate. The exact same native output
shows that restoring this type alone cannot repair the allocation.

ROM 28c8–28e4 also confirms the existing mixed bitwise/logical condition; a
wholesale short-circuit rewrite would discard observed operations. ROM 23f0–2400
consumes the width-helper result without EXTS.W, so copying the callee's narrow
source return type into every caller is not justified solely by that declaration.

## Decision

Close the three source families. The remaining requirement is simultaneous
recovery of the mask/palette allocation, original first-loop unrolling, and
position 100/+20 recurrence. The demonstrated allocation changes cannot substitute
for matching instructions and entry addresses. Reopen only with new producer or
native definition-graph evidence; otherwise select another owner.

Canonical C, splits, toolchain and proof code did not change. The refreshed
frontier check and documentation diff check pass. No full suite or candidate
cold replay is claimed for these rejected research-only sources. The earlier
canonical two-run cold baseline remains the coverage evidence; no new coverage
publication is warranted.
