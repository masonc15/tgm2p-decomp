# Blend upload invariant: authenticated archived controls

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%). No canonical source or coverage
map changed.

This completes the specific compiler-stage comparison requested by the
`0x2ac4c` route and `research/2026-10-10-blend-counter-lifetime/REPORT.md`.
The existing `lifetime_07.c` and `lifetime_08.c` controls were copied without
edits from `tools/shc_probe/cases/session_gfxblend_next/`. Both retain the
whole real blend function and following setter, real declarations and ABI,
and the setter's three DIR32 references. They were already failed source
experiments; this work authenticates their mechanisms, not a new source search.

Packet: `research/2026-10-17-blend-upload-invariant/`. The recorded prediction
was that the destination-pointer form would move the hardware base outside
the upload loop but introduce an extra pointer spill. The hoisting prediction
holds; the pointer-spill explanation is falsified.

## Native evidence and fixed-address measurements

There are **22 compiler executable stages**: two original eight-stage machine
captures and, for each, a fresh frontend plus normal and diagnostic MDL replay.
Both REG and ILB files agree byte-for-byte across native capture and both
replays. Decoded REG also round-trips exactly. Diagnostic CMN settings are
used only for explanations, never admission objects. The frontend's exit5
is authenticated against its successful native driver trace; the generic
ledger status label does not turn it into a compiler failure.

The fresh objects reproduce each archived allocated section, function root,
and relocation tuple exactly. Independent ordinary GNU linking reproduces
every archived resolved context byte. No displaced or normalized comparison
is counted as progress.

| Control | Context bytes | Actual setter root | Blend differences / 786 | Retained setter differences / 98 |
| --- | ---: | --- | ---: | ---: |
| Integer row, lifetime_07 | 880 | 2af5c (-2) | 231 | 68 |
| Destination pointer, lifetime_08 | 888 | 2af6a (+12) | 714 | 78 |

The first context ends four bytes before the protected interval ends. Those
four missing bytes count as failures in the setter residual above. The second
context extends four bytes beyond the original full extent. Both fail actual
entry placement and protected-neighbor retention.

## What changes

The integer-row source computes `row = i * 64 + pal * 64`. Its final trees
strength-reduce that value across the outer loop: synthetic symbol -3 is
initialized before the loop and incremented by 64 at its end. The hardware
constant `0x24040000` remains in both unrolled upload-store address trees.
Its upload setup is six bytes and its repeated upload body is 48 bytes.

The destination source computes a real `short *dest` including the hardware
base. Its synthetic outer-loop induction value now includes `0x24040000`.
The upload stores no longer add that constant; setup is six bytes and the
repeated body is 42 bytes. In contrast, the ROM recomputes the row from the
short counter immediately before each upload, then adds the hardware base:
20-byte setup and 42-byte body. Neither archived control has that induction
boundary.

The short-lived source row/destination is **R6 in both controls**, final
logical12, range73..81. It does not spill. The significant allocation change
is the long-lived constant-zero value versus blend coefficient `u`:

| Control | Zero diagnostic ID | u diagnostic ID | Both priorities | Both profits | Final allocation |
| --- | ---: | ---: | ---: | ---: | --- |
| Integer row | 46 | 45 | 37 | -7123 | u R8; zero immediate |
| Destination pointer | 41 | 42 | 37 | -7256 | zero R8; u SP+0 |

The diagnostic IDs are from the initial allocation report, not blindly equated
to final REG IDs. Symbol58 identifies `u`; the constant record identifies zero.
All recorded graph profits recompute from the native priorities and clash
lists. In the destination control, final logical41 is zero in R8 over10..85;
final logical42 is u at spill -1 over9..85. The extra spill increases the local
frame from76 to80 bytes and moves the buffer from SP+12 to SP+16. The source
counter then occupies SP+4, the outer destination induction value SP+8, and
count n SP+12. In the integer-row control, i is SP+0, row SP+4, n SP+8.

This is evidence of an equal-score allocation-order change, not a complete
proof of every internal allocator tie-break. It is sufficient to reject the
proposed extra short-lived pointer-spill explanation and prevent repeating
that hypothesis.

## Reproduction and next question

Run `analyze.py`, `priorities.py`, then `verify_conclusions.py` with the project
Python. They perform no SHC compilation. ANALYSIS.json retains typed trees,
actual roots, relocations and raw residuals. PRIORITIES.json retains the native
allocation records; RESULT.json records the bounded conclusion. Raw disassembly
and ROM upload bytes are saved alongside them.

Before another source edit, recover a genuine expression/lifetime boundary
that keeps the hardware base invariant within the upload loop without turning
the complete row into an outer-loop induction variable. It must also preserve
u in R8, the 76-byte frame, and nominal setter root. Do not repeat the row or
destination spellings, zero/reset placement, or width sweeps without that
new evidence. The archived lifetime_11 reset-placement control already exists.

No source variation, accepted receipt, whole-ROM candidate qualification,
required-suite run, cold replay, or dashboard publication is claimed for this
negative packet. All existing research is preserved.
