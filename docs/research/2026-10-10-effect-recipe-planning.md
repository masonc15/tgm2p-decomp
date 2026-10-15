# Effect drawing helper: recipe planning and expression boundaries

Target: `[0x20a9c,0x20b24)`; genuine context starts at `0x209a4`.
The preceding 248 bytes are protected. No source or split was integrated.
Verified linked C remains 137,090 / 200,698 bytes (68.306610%).

## What was learned

Fresh ordinary controls reproduce the previous 13-byte residual. The first
argument retains the pointer cast already justified by the earlier SHC argument
binding investigation. This packet does not claim that discovery again.

Read-only code-generator breakpoints at 421137/421159/421168 show the two product
results being assigned scratch slots before emission 20. They use the same
recipe, 0x4510a0. This is advance planning of expression results, not selection
interleaved with the eventual product instructions.

For the baseline at emission 20:

| Request | Exclusion low 16 bits | Preference | Last / recent | Selected |
| --- | --- | --- | --- | --- |
| y load, call 26 | f0 | R0 | 0 / 1 | R0 |
| first product, call 27 | f1 | none | 0 / 1 | R3 |
| second product, call 28 | f1 | none | 3 / 1 | R2 |

The second request ranks R3 below R2 because R3 was the last selection. Reordering
the product before the y load does not solve this: the later product excludes
R3 (low mask f8). The complete masks, node/child metadata, recipe addresses,
cache states and slot writes are retained in each `events.jsonl`.

## Bounded source controls

Packet: `research/2026-10-10-effect-expression-boundaries/`.

| Form | Context bytes | Target raw differences | Protected differences |
| --- | ---: | ---: | ---: |
| baseline | 384 | 13 | 0 |
| split_sum | 384 | 71 | 0 |
| sum_comma | 388 | 72 | 12 |
| subtotal_local | 384 | 55 | 6 |
| subtotal_register | 384 | 55 | 6 |
| accumulate_steps | 388 | 115 | 12 |

All fail the existing strict proof. Changed output size or register allocation
does not count as progress.

The two traced controls do change planning: split_sum plans its first product
with preference R14 at emission 20, and its later operation at emission 32.
subtotal_local prefers R4 for the first product and plans the later operation at
emission 33. Both therefore lose the desired accumulator allocation. The simple
statement/subtotal boundary hypothesis is exhausted for these recorded forms.

## Verification and replay

Run from the canonical project:

```sh
/drive2/tgm2p/venv/bin/python research/2026-10-10-effect-recipe-planning/verify.py
```

The verifier independently resolves original object relocations, checks full
context bytes, function roots, source/tool hashes and every reported residual.
It assembles four captured assembly outputs and confirms equality with the
ordinary strict-compile contexts. Fresh traced SUA/ASA files equal fresh
untraced controls. Cleanup-snapshot ASA differences are explicitly retained;
they are not silently normalized into equality.

The existing independent scratch-selector model agrees with all 26 observed
requests across four captures. `VERIFICATION.json` also authenticates the rank
packets described in the companion research note. Compiler observations explain
the tested output; they do not establish that no other valid source exists.

## Next action

Park these expression-boundary controls. Reopen only with a concrete IR or
producer mechanism that predicts an R0 accumulator without reserving both
products simultaneously. Do not repeat the already-explained y-load reset,
callee binding, source signedness or subtotal matrices.

No shared build/proof tooling changed. Research verification passed; the full
production test suite and cold integration replay were not required or claimed.
Zero matched bytes were added.
