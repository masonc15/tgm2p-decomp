# Secret menu: unsigned absolute-value expression and genuine storage

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%).

Packet: `research/2026-10-17-secret-menu-semantics/`.
The declaration review led to f_d394's RNG result. The old whole-unit draft
uses a short-return RNG declaration, an independent zero-valued `neg` local,
a discarded RNG call, and another RNG call selected by that zero flag.

## Original control flow and source inference

A worklist follows 590 reachable halfwords in d394..d8e0, treating actual delay
slots explicitly and excluding inline pools. There is no indirect dispatch in
this function. R14 receives a global pointer before the initial wait, becomes
zero at d426, and is restored only at the return. Under the ordinary preserved-R14
call convention, d866 tests zero. The path calls RNG at d862, skips the negative
arm d86c, and calls it again at d876. The original RNG literal is d8d8=0602b74a.
The accepted RNG implementation returns unsigned remainder modulo its argument;
argument 5 therefore returns 0..4.

This suggests an unsigned absolute-value macro expansion:

```c
w4 = f_2beca(5) < 0 ? -f_2beca(5) : f_2beca(5);
```

The inference is about a plausible source producer, not recovered original text.
It preserves two runtime RNG events on the reached path, including the otherwise
discarded first evaluation. The compiler may fold the unsigned comparison while
retaining both syntactic arms. That exact prediction was registered before an
ordinary eight-stage full-context compile with the actual unsigned RNG contract.

**The native output reproduces the constant-zero R14 test and three syntactic
RNG call sites.** Thus an independent dummy sign flag is unnecessary to explain
this unusual ROM sequence. The result still keeps w4 in R8 and adds a result
copy; it does not match the original stack storage or layout.

## Earlier evidence and one producer control

The source search found 505 definition-bearing paths, grouped into 12 body forms
(excluding this packet). The historical `agent_d/STUCK.md` already identifies
five explicit reset stores and 32-bit display x. Its closest archived form uses
five inert g_dummy statements, which are excluded. Most newer whole-unit copies
instead carry a reset loop and short x. No explicit unsigned RNG absolute-value
expression appeared in the recorded expression searches; this does not prove
absence of every possible equivalent spelling.

One further preregistered eight-stage control combines the new genuine unsigned
expression with the target-backed producer operations: initialize run before the
array copy, use 32-bit display x, and clear tab[0] through tab[4] explicitly in
ascending order. No dummy effects, code padding, forced register choices or
rebasing are introduced. The original prologue stores run before copying the
array and saves five table addresses; these are the concrete predictions.

Five address slots are recovered, but the frame reaches **48 bytes**, not the
required 52. Their slots are 8,24,12,16,20, rather than 12,16,20,24,28. Run is at 32,
the array at 36, and w4 remains R8. Original run/array are 36/40, with w4 at 4 and
its promoted copy at 0. The required constant 1/R8 ownership is not recovered.
This producer hypothesis therefore stops at the observed storage mismatch.

## Raw results and admission limits

All three contexts start at 9c0c; all target roots remain **d2d8**,188 bytes before
the required d394. Full original rodata is retained and linked; initializer
addresses are not rewritten to the ROM's 6032e40. Pool/address ownership still
needs reconstruction along with the earlier source-unit layout.

| Measurement | Archived baseline | Unsigned expression | Recovered producer |
| --- | ---: | ---: | ---: |
| Complete text bytes | 19684 | 19696 | 19724 |
| Actual following d8e0 root | d810 | d81c | d838 |
| Target raw differences d394..d8e0 | 1327 | 1324 | 1315 |
| Retained b2f4..b39a differences | 0 | 0 | 0 |
| Nominal following d8e0..d962 differences | 129 | 127 | 119 |
| Nominal e3b8..e3e0 differences | 40 | 40 | 39 |

Only the 166-byte retained interval inside these research contexts remains exact.
The separately accepted palette owner uses its own canonical context; the table
above does not claim preservation of that owner by these candidates. Changes in
raw difference counts are diagnostic and earn no matched bytes.

There were 16 compiler executable stages across the two separately registered
questions. `cfg.py`, `measure.py` and `verify.py` reproduce the control-flow,
original-object linking, raw measurements and bounded-stage assertions without
launching SHC. No candidate passes strict admission; required tests, candidate
whole-ROM checks, cold replay and dashboard publication are not claimed.

Next work needs genuine constant1 lifetime and w4 storage evidence alongside
original producer roots. Do not repeat the unsigned expression, reset loop or
explicit-store contrast, or the historical dead-statement controls.
