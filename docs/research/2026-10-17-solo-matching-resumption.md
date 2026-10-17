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

## Next producer work

Recover the genuine f780 startup body before another isolated f1108 syntax
search. The archived ROM listing and m2c discovery output are in
research/2026-10-17-video-producer/; the latter is incomplete discovery
output, not a buildable or accepted candidate. Resolve its local storage,
call declarations, shared pools and complete CFG, including the large
startup switch, before the first bounded whole-unit drafting experiment.

Read-only f9180 review also preserved its target listing. Its apparently
redundant flags &= ~0xc6; flags = 1 corresponds to actual ROM AND at
0x94c2 followed by store of R13 at 0x94c4; do not change it to OR merely
because the source looks suspicious. Prior palette/field ABI controls are
already byte-identical negatives.
