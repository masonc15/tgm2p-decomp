# Solo matching resumption, 2026-10-17

The live nuada baseline at bd7cbfe is **138,670 / 200,698 linked C bytes
(69.093862%)**, leaving **60,390 non-exempt bytes** and the unchanged **1,638
approved assembly bytes**. Compared with the dated 137,090-byte baseline,
existing progress is +1,580 bytes, +0.787252 percentage points. This session
has accepted **0 bytes, +0.000000 percentage points**.

Completion remains 199,060 / 200,698 linked C bytes (99.183848%), reported as
100% of the agreed goal only when every non-exempt byte is proven.
No denominator, exceptions, source owners, or acceptance rules changed.
Work is solo on nuada under the current user instruction.

## Verification

- Locked make split check report passed; make status regenerated the
  frontier and tools/frontier.py --check passed.
- Both original chip SHA-1s passed. Program SHA-256 remains
  d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7.
- research/cold-2026-10-17-solo-baseline/receipt.json has passed: true.
  Both independent cold builds report 138,670 / 200,698 and the expected
  program hash.
- Live https://nuada.hale-tint.ts.net/tgm2p/ reports the same byte count;
  evidence is research/2026-10-17-solo-completion/dashboard-check.json.
  No gain means no new coverage publication was requested.
- No full unit-test suite is claimed: changes so far are research and routing
  documentation, with no build/proof-tool or canonical C changes.

## Evidence recovery and bounded experiments

Missing October 16 cloud evidence was restored from private companion
commit 50ac96084cc0eeb7db7498495ad2877ef8a926cb, preserving existing
directories. Full recovered paths are indexed in
research/2026-10-17-solo-completion/recovered-evidence.json.
The private checkout and all experiment outputs stay gitignored.

1. research/2026-10-17-f57d4-local-web/REPORT.md: consumed early flag
   reuse splits into separate logical webs; complete 4,068-byte context
   is identical. 705 raw target-byte differences, 1,504 exact predecessor
   bytes. 18 compiler stages; hypothesis closed.
2. research/2026-10-17-video-producer/REPORT.md: one inline row-fill
   abstraction gives 356 bytes versus ROM 344, with 238 raw differing
   bytes. Native output recomputes row shifts instead of incrementing a
   row offset. Diagnostic REG equality fails for the inline candidate;
   those diagnostics are not authenticated allocation evidence.
   18 compiler stages; hypothesis closed.
3. research/2026-10-17-f57d4-shortcircuit/REPORT.md: logical call
   expression recovers the all_clear stack slot but keeps distinct R2/R3
   call tails. Next function eight bytes late, 545 nominal raw differences,
   1,504 exact predecessor bytes. Eight compiler stages; hypothesis closed.

Each packet records its hypothesis and prediction before compiling, source
and tool identity, bounded budget, actual entries, raw differences, and
preserved neighboring ranges. None qualifies for source integration.

## Producer follow-up and later bounded controls

The genuine f780 draft is preserved in research/2026-10-17-startup780-draft.
Its frame is36 bytes versus ROM292, and f1108 starts10d8 versus1108. No
invented local buffer or alignment filler was added. Restricted differential
runs pass32 controlled and21 directed-switch trials, with46% and80% estimated
instruction coverage; they do not establish equivalence or exactness.

Register loop counters and a flat indexed store recovered the video initializer
register homes. The separate research/2026-10-17-video-byte-offset packet then
reproduced all344 function bytes at the displaced10d8 root. That is diagnostic
only and earns no credit. Independent nominal compilation and a genuine
header-only control each emit356 bytes with143 raw differences. The preceding
body affects compiler history; its actual layout must be recovered. Each packet
used24 compiler stages. No extraction, rebasing or padding was integrated.

research/2026-10-17-menu-palette-abi tests the genuine unsigned-byte palette
callee contract in the recovered1278-2f3c unit. The corrected interface changes
code but retains x in R8 instead of restoring the desired mask allocation.
f29ea remains two bytes late; protected following bytes fail. Two native
captures plus diagnostics used18 stages. Diagnostic REG equality fails, so
only original native objects support conclusions. No further ABI grid follows.

All follow-up work still earns **0 bytes, +0.000000 percentage points**.
Routes preserve these negatives and require new native/context evidence.
The frontier is regenerated and checked; no canonical source or split changed.

Read-only f9180 review also preserved its target listing. Its apparently
redundant flags &= ~0xc6; flags = 1 corresponds to actual ROM AND at
0x94c2 followed by store of R13 at0x94c4; do not change it to OR merely
because the source looks suspicious. Prior palette/field ABI controls are
already byte-identical negatives.

The subsequent [menu native-allocation report](2026-10-17-menu-native-allocation.md)
authenticates fresh frontend/MDL replays and records three additional bounded
source controls. They restore allocation in two cases but fail instruction
layout, so coverage remains unchanged.

The [native storage trials](2026-10-17-native-storage-trials.md) identify the
f9180 promoted-result spill and test a dimensional lifetime split in f2657c.
Both bounded packets preserve ordinary native outputs and authenticated
diagnostics; neither yields an admissible interval. Session gain remains
**0 bytes, +0.000000 percentage points**.

The [ABI and clamp controls](2026-10-17-abi-and-clamp-controls.md) reconcile the
newer board source and recover the music routine volume/timer homes through
a rejected conditional-clamp control. Both remain raw-negative. VM/checksum
routes now reference existing negatives; no fresh coverage is claimed.

## Additional staged evidence and title context

The canonical-context audit found no exact unresolved function among63 contexts/107 pairs. A second private branch supplied the staged functional-source packet; both snapshots are preserved. The title-screen24byte stride correction recovers the original address recipe but leaves795 raw target differences. A separate authenticated flag-width control falsifies the proposed extra-home mechanism and fails protected bytes and roots. See [staged rank context](2026-10-17-staged-rank-context.md) for hashes, budgets, negative results and the newly identified text-pointer lifetime discrepancy. Coverage change: **0 bytes, +0.000000 percentage points**.
