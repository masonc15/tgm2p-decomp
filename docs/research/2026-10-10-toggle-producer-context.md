# Toggle producer-context investigation

Follow-up: [the peephole investigation](2026-10-10-toggle-peephole.md)
locates the reload deletion after GEN and records the newer 18-byte residual.
The observations below remain the original experiment record.

Canonical baseline: 137,090 / 200,698 bytes (68.30660993133962%). Fresh
`make split check report` passed both EPROM identities and the whole program.
No game source, split, proof, compiler, or ROM was modified. New credited bytes: 0.

## Target and authentic context

The unresolved toggle occupies `[0x277d0,0x27862)`, 146 bytes including the
shared literal pool. The existing `src/board_setters_2774a.c` compiles genuine
context from `0x276e8`. Its seven retained setters `[0x2774a,0x277d0)` are exact.
The original draft places the cache helper at `0x27860`, two bytes early.

Do not interpret small assembly ranges as isolated functions: the separate
34-byte `[0x225fa,0x2261c)` range is a shared pool, and its apparent machine
instructions from linear disassembly are not function evidence.

## Three bounded hypotheses

Each packet freezes its baseline source hash and SHC executable hashes, records
the hypothesis/prediction and budget before execution, and retains all ordinary
compiler objects, linked contexts, actual roots, raw residuals, and proof errors.
Packets are under `research/2026-10-10-toggle-control-flow/` on Nuada and under
the same relative research path in the coordinating local workspace.

1. **Branch sharing, eight compiles.** Explicit shared allocation results,
   shared stores, inverted branches, single allocation and equivalent labels
   did not recover the original layout. Baseline has 44 differing bytes and
   preserves the setters; alternatives have 102–110 differences and disturb
   14 setter bytes. The simple common-store explanation is rejected.
2. **Return ABI and index locals, six compiles (`abi-index/`).** The actual
   allocator returns full-width `-1` or `1..10`: failure uses `MOV #-1,R0`, success
   returns the bounded full-width counter using `MOV R6,R0`. The target toggle
   consumes its low word without a sign-extension instruction. Its slot-address
   multiplication also copies the loaded index into another register. These
   observations motivated an `int` caller return declaration and a consumed
   slot-index local. That combination reduces the residual from **44 to 20
   bytes**, restores the successor root to **0x27862**, and preserves all **134
   setter bytes**. Both short and int index locals emit identical contexts.
   This supports an ABI hypothesis; it does not prove the original source type.
3. **Load/result spelling, seven compiles (`reload-result/`).** Genuine word
   pointer and integer-address loads, and a narrow result local, all preserve
   the 20-byte residual. Direct result stores coalesce the two allocation paths,
   yielding 92 differences and disturbing the setters. Stop these exact forms.

The best candidate is `abi-index/int_local_short.c`. It remains rejected by the
unchanged strict range proof. All genuine function roots, including `0x277d0`
and `0x27862`, now agree. No padding or context-origin adjustment was used.
The following 94-byte cache helper still has 48 raw differences; restoring its
root alone does not make it an integration candidate.

## Remaining mechanism and next action

All remaining toggle differences lie at `0x277f0`, `0x277f2`, `0x277fa`, and
`0x277fe` through `0x2780c` (11 halfwords, 20 differing bytes). The first call
uses R2 instead of R3. On the occupied-slot path, the compiler reuses the value
from the condition in R0, while the ROM reloads task.w8. The emitted allocator
result also receives an extra R0-to-R4 copy before the shared store.

Next inspect the middle-pass value reuse and the shared result's merge across
the two allocator calls, then the scratch selection for the first call. The
predicted useful source change must preserve the exact function size, shared
pool, seven setters, and disabled-slot path. More pointer casts, result-width
variants, or common-store rewrites without changed compiler evidence are not
justified by these results.

Before any future integration using a widened allocator declaration, reconcile
the allocator definition and affected caller declarations against the actual
ABI and replay their accepted ranges. No canonical declaration was changed here.

Replays:

```sh
/drive2/tgm2p/venv/bin/python research/2026-10-10-toggle-control-flow/run.py
/drive2/tgm2p/venv/bin/python research/2026-10-10-toggle-control-flow/abi-index/run.py
/drive2/tgm2p/venv/bin/python research/2026-10-10-toggle-control-flow/reload-result/run.py
```

These regenerate diagnostic artifacts and cannot promote coverage. Preserve an
existing packet before rerunning it if its original receipt needs to remain
immutable. No full test suite or cold integration replay was warranted for these
rejected research sources; the fresh canonical baseline is the production check.
