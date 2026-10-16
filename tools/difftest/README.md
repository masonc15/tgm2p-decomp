# SH-2 differential smoke tests

This experimental harness runs an SH-2 ROM function and a compiled candidate
from the same generated state. Use it to find counterexamples and prioritize
matching work, not to approve a final. A passing run does not prove equivalence,
correct C, register-only differences, or byte identity.

Run on nuada, from the repository root:

```sh
/drive2/tgm2p/venv/bin/python tools/difftest/difftest.py CASE.c f_1050e --trials 200 --time 30
/drive2/tgm2p/venv/bin/python tools/difftest/sh2emu.py --selftest
/drive2/tgm2p/venv/bin/python -m unittest discover -s tools/difftest -v
DIFFTEST_COMPILE_TESTS=1 /drive2/tgm2p/venv/bin/python -m unittest discover -s tools/difftest -v
```

`MATCHED_TRIALS` (exit 0) means every executed trial completed without an
observed mismatch. `DIFFERENT` (exit 1) reports a divergence under the harness;
replay it with the printed input and inspect the disassembly and input validity.
`INCONCLUSIVE` (exit 2) means at least one trial was unsupported or truncated,
or none completed. Unresolved symbols or unsupported relocations also exit 2; the harness never
substitutes expected ROM pool values for missing candidate symbols.
Compile/setup failures exit 3; invalid CLI arguments exit 2.
A time budget can stop before the requested count. It is checked between trials
after at least 20 trials, not a hard wall-clock timeout; it excludes compilation
and callee-arity discovery. Use an external timeout for a hard execution cap.

`--trace INPUT` prints both traces for a seed or seed/overrides string.
`--input INPUT` (repeatable) runs exactly the given inputs as the trials,
compared as any trial is, for example inputs chosen so a function's loops end. `--all`
checks every mapped function in the case. `--at name=0xADDR` supplies a ROM
address for a differently named function. `--ret none|8|16|32` overrides the
source-based return-type guess.

## Function boundaries

The default ROM end comes from `fn.py`, whose heuristic can stop at an internal
indirect jump. That can report a false tail call and a false mismatch even for
byte-identical code. For a separately verified boundary, use `--rom-end`:

```sh
/drive2/tgm2p/venv/bin/python tools/difftest/difftest.py src/board_28334.c f_296b6 --rom-end 0x29bf8
```

The linked ELF gives `_f_296b6` size `0x542`, ending at ROM `0x29bf8`; the
heuristic incorrectly ends it at `0x297fe`. The override is exclusive and applies
to one function, not `--all`. Never pick an end just to make a test pass.
Instruction coverage remains an estimate: heuristic pool detection and extents
can include data or omit code. Even 100% instruction coverage is not exhaustive
input or path coverage.

## Limits of the model

Calls are stubbed, except the SHC runtime helpers. Their arguments are selected
by sampled callee-arity discovery, which can miss conditional argument uses,
together with a static guess (r4-r7 whose first appearance is a read) over the
instructions control flow reaches from the callee's entry, following SHC switch
tables; literal pools are never reached, so a pool word fn.py's pool detection
misses cannot pass for an argument read (it made f_2e5aa(char) look like four
arguments). A callee with another indirect jump falls back to a linear pass
over its non-pool words. Unknown callees default to comparing only r4. Stubs do not dereference pointer
arguments or reproduce memory side effects. Frame-pointer offsets are normalized
away, so passing the wrong local buffer can escape detection. Unwritten frame
slots receive synthetic values, not real game state. Lazy random memory depends
on access widths and order and can produce false divergences. Generated pointers
and other inputs need not satisfy a function's preconditions.

Writes outside the frame are compared by final byte values between calls,
ignoring store order. This is not a model for volatile MMIO, interrupts or DMA.
Exceptions, address errors, caches, timing, RTE, TRAPA and SLEEP are not modeled.
Return types and relocated addresses are inferred; check those assumptions when
a result looks wrong. Any use of this tool still ends with funcscore, objdiff,
probe and whole-ROM byte verification before linking.

The interpreter's delay-slot PC-relative loads follow the Hitachi SH-1/SH-2
Programming Manual, MOV immediate-data notes (printed p. 117) and MOVA notes:
“When this MOV instruction is placed immediately after a delayed branch
instruction, the PC points to an address specified by (the starting address of
the branch destination) + 2.” The reference is `refs/h12p0.pdf`.

## Observed sampling misses

In a 40-trial run, changing `a + b` to `a - b` in f_1050e was not detected,
even at reported 100% instruction coverage and all seven comparisons seen both
ways. Changing `sh > 223` to `sh >= 223` in f_2808a also escaped that run.
A changed draw argument in f_1050e and a changed comparison in f_1eb1c were
detected. These are concrete reasons not to interpret MATCHED_TRIALS as a
correctness verdict. Construct boundary inputs as well as sampling.
