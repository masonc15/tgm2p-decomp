# Board-call ABI and music-clamp controls

These experiments add **0 matched bytes and 0.000000 percentage points**.
Canonical coverage remains **138,670 / 200,698 linked C bytes (69.093862%)**.
No source owner, split, exception, compiler, or proof rule changed.

## Reconcile the newer board-scroll source

The October 16 functional-track source `src/nonmatching/board_2657c.c` contains
newer argument-width and branch-local coordinate recovery than the older
seven-function matching draft. Its functional qualification is useful evidence,
but does not confer matching credit.

A complete seven-function control combines those first three bodies with the
genuine following redraw and selector bodies. Non-prototype declarations and
identifier-list definitions express the full-width caller arguments and narrow
callee parameters without incompatible function-pointer casts. This is the
same language mechanism already investigated for the EEPROM interface.

Ordinary output still has a 24-byte frame instead of 28. Its first successor is
0x26c38 instead of 0x26cb0, 120 bytes early; context size is 3,316 rather than
3,476 bytes. The first target interval has 1,758 fixed-address differing bytes.
The displaced wrappers cannot replace the canonical exact 140-byte range.

Difftest using the already compiled object passes 100 complete sampled trials,
with no limited or unsupported trials and 98% estimated instruction coverage.
The older draft also passes 50 complete sampled trials at 92% estimated coverage.
Neither sample proves equivalence, exactness, or that the older draft is wrong.
The coherent ABI control is closed without further source permutations.

Evidence: `research/2026-10-17-board2657c-semantic/REPORT.md`. One ordinary
capture plus authenticated frontend/normal/debug replay used 11 compiler stages.

## Music routine: a clamp changes the allocation tie

The latest f96ac snapshot-alias baseline has volume spilled and timer in R12;
the ROM has volume in R12 and timer at SP+0. Both webs have priority 3 and
profit -2. A read-only sort model reproduces all 91 native sort events.
Predicted state-address and volume-address priority increases do not reverse
their order, so those source controls were not compiled.

A fixed-graph model predicts that increasing the second player's clamp-value
priority from 4 to 6 would rank volume before timer. One source control changes
that clamp from `if (v > 500) v = 500;` to `v = v > 500 ? 500 : v;`.

The ordinary result recovers volume in R12 and timer at SP+0, but falsifies the
predicted priority mechanism: the value splits into two priority-2 webs rather
than becoming one priority-6 web. The new graph's 95 sort events also replay
exactly. The extra copy, branch, and literal-pool placement change extend the
body from 1,248 to 1,260 bytes versus the ROM's 1,252. Within the original extent
there are 1,051 raw differing bytes, plus eight excess bytes. The baseline had
982 differences including four missing bytes. Other address assignments remain
wrong. This is not an exact candidate.

All entries remain unchanged. The unchanged strict validator passes both
protected intervals, 0x881a–0x88fe and 0x95b8–0x96ac, totaling 472 bytes; it rejects
the target. Close this conditional-clamp control. Any new hypothesis must
recover the original clamp sequence and address roles as well as the timer slot.

Evidence: `research/2026-10-17-field96ac-state-priority/REPORT.md`. Two ordinary
captures and authenticated frontend/normal/debug replays used 22 compiler
stages within the recorded 24-stage budget. Diagnostic outputs were not linked.

## Frontier evidence reconciliation

Read-only review found that the VM ranges and checksum already have extensive
negative evidence despite missing focused frontier routes. Their new routes
point to those records instead of treating them as undrafted.

The current canonical f10788 context has six raw differing bytes: 0x1082e,
0x10830–0x10831, 0x108b4, and 0x108b6–0x108b7. The archived 219/221 diagnostic
source additionally uses inert source statements to perturb compiler history.
It is preserved as diagnostic history, not promoted as a genuine source solution.
No compiler or canonical source mutation was needed for this inventory.

No candidate qualified for integration, whole-ROM candidate replay, or coverage
publication. No new full test-suite result is claimed for research-only work.
