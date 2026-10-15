# Piece predicate lifetime and branch-boundary experiments

The current baseline was revalidated with serialized make split check report:
137,090 / 200,698 linked-C bytes (68.306610%). Both EPROM SHA-1 values and the
program SHA-256 remain original. This research gains zero bytes.

## Authenticated compiler observations

research/2026-10-10-piece-first-turn-liveness completes the two previously
pending captures. Assembling each captured native source with the original
assembler/converter and resolving relocations at 0x2f68 reproduces its complete
ordinary compiled context: baseline 5,468 bytes; ordered-input 5,464 bytes.
Both normal and diagnostic middle-pass replays preserve native REG and ILB.

The first promoted pressed-input common value in f4044 is final logical register
10, physical R4, with one live interval (expression33, expression33), no named
symbol, and integer type. The independently decoded REG stream round-trips
losslessly. The final expression tree consumes it twice inside a logical OR.
Earlier held-input ordering does not change this allocation.

The original ROM instead saves the promoted byte to stack+4, reloads R0 for the
second test, and constructs separate R3 addresses for the first rotation's
rotation/row/x loads. Current source uses R4 for the common byte and indexed R0
field loads. The capture identifies the difference; it does not prove that
changing the spill alone causes all subsequent address choices.

## Three bounded ordinary-source controls

research/2026-10-10-piece-predicate-lifetime recorded its budget before running.
Each source preserves the two input tests' short-circuit order and introduces
no side effects or artificial consumers. Existing block entries bypass no
declaration initializer.

| Form | Context bytes | Present differences | Missing target bytes |
| --- | ---: | ---: | ---: |
| Separate tests with ordinary gotos | 5,472 | 936 | 12 |
| Group first two tests before goto | 5,464 | 927 | 20 |
| Right-associated logical OR | 5,464 | 927 | 20 |

All original f4044 entries remain nominal and all 3,894 protected bytes match.
Every strict target proof rejects. Right-associated OR is byte-identical to
the ordered baseline; the grouped control is not, despite equal discrepancy
counts. Separate tests duplicate the input load instead of preserving one
common value in the ROM's stack home. No control achieves the predicted spill.

Stop this branch-boundary family and switch owners. Reopen only with evidence
for a genuine storage lifetime or producer context that predicts the target
stack home without duplicate loads. Do not repeat these controls, previously
tested named-input/helper forms, or unchanged integer field-address spellings.

## Verification

piece-first-turn-liveness/verify.py authenticates compiler/source hashes,
two native assembly reproductions, four middle-pass replays, the decoded common
input binding, all three original ELF resolutions, roots and raw range residuals.
VERIFICATION.json and VALIDATION.json contain the receipts.
No production source, split, proof tool or compiler changed; no cold replay or
full test-suite run is claimed for these rejected research candidates.
