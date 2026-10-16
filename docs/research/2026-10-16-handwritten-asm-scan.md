# Hand-written assembly scan

2026-10-16. `tools/asm_scan.py` asks, for every unresolved assembly range and
every function in it: does this look like SHC v5.0r32 output, or like
hand-written assembly? The answer is a routing hypothesis, not proof of
authorship, and it changes nothing about exemptions or coverage. The two
runtime ranges in `RUNTIME_EXCEPTIONS.md` stay exactly as approved.

Result: the scanner separates its controls without error (precision 1.000,
recall 1.000 under five-fold cross-validation), and it finds hand-written
evidence in exactly one unresolved range, the ROM-resident boot block at
`0x400`–`0x780`. All 40 other unresolved ranges are SHC-shaped.

## Method

Each unresolved range from `splits.txt` (an `asm` segment minus the exempt
bytes) is cut into functions at entry points with code evidence: `bsr`
targets, `jsr`/`jmp` targets loaded from a literal pool, RAM code pointers in
pools and in the data after the code, and the exception vector table. Unlike
`fn.call_targets()`, halfwords inside literal pools never seed an entry; the
old heuristic's ~240 pool-only seeds (`0x14ba`, `0x17a4a`, ...) otherwise
create fake "shared code". Each function is decoded by recursive descent with
the new table-driven decoder `tools/sh2isa.py`, which records register
definitions and uses for every SH-2 instruction. Descent follows SHC's
`mova; mov.w @(r0,rI); braf` switch tables (length from the `cmp/hs` bound),
stops at a fall-through into the next function's entry (SHC drops a tail call
to the following function), and never decodes pools or table words. Code no
entry reaches becomes an orphan fragment; one that does not decode as at
least three valid instructions ending in a transfer is reported as data.

The rules were fixed before looking at the unresolved ranges. Strong evidence
(any one makes the verdict "hand-written"):

| Rule | What it detects |
|---|---|
| `scratch-input` | r0–r3, T, MACH or MACL read before any definition on some path from the entry (a private register ABI) |
| `callee-clobber` | r8–r14 written without being saved |
| `scratch-save` | r0–r3 saved by the prologue and restored before return, outside an `rte` handler |
| `pr-unsaved-call` | a call followed by `rts` with PR never saved |
| `sp-imbalance` | r15 offset differs between paths or is nonzero at return |
| `sp-write` | r15 loaded or copied from a general register |
| `shared-code` | a branch into another function's body, or a conditional branch to another entry |
| `zero-idiom` | `xor`/`sub` of a register with itself (SHC uses `mov #0`) |
| `system-op` | SR/GBR/VBR control, `rte`, `trapa`, `sleep`, division steps, rotate through carry, `mac`, `tas`, `clrmac`, `sett`, unless the matched C controls contain the same operand form |
| `prologue-order` | saves out of SHC's order (callee-saved descending, then PR, then MACH/MACL) |

Weak evidence is reported but never decides: `callee-input` (r8–r14 read before
definition, which SHC does for an uninitialized local such as `n` in
`f_2c878`) and `unseen-form` (an operand form no matched C function uses).

Fragments that no entry reaches start in an unknown state, so only the
context-free rules (`system-op`, `zero-idiom`, `sp-write`) apply to them.

## Controls

Negatives are the 507 functions the linked ELF defines inside `c`,
`c-prefix` and `c-range` segments: SHC output by construction. Positives are
the 9 runtime routines with entries in the two exempt ranges (`_divls`,
`_divlu`, `_divws`, `_modls`, `_modlu`, `_slow_mvn`, `_sftl`, `_sftra`,
`_modws`): hand-written SHC library assembly. The learned vocabularies (which
operand forms and system operations SHC emits) come from four fifths of the
negatives and judge the held-out fifth, so no negative vouches for itself.

```
$ python tools/asm_scan.py
controls: 507 matched C functions (negatives, 5-fold), 9 runtime functions (positives)
  hand-written verdicts: 9 true, 0 false; precision 1.000, recall 1.000
feature counts (negatives / positives):
  scratch-input       0 /   9
  callee-clobber      0 /   0
  scratch-save        0 /   6
  pr-unsaved-call     0 /   0
  sp-imbalance        0 /   0
  sp-write            0 /   0
  shared-code         0 /   0
  zero-idiom          0 /   0
  system-op           0 /   6
  prologue-order      0 /   0
  callee-input        1 /   0
  unseen-form        14 /   5
```

Every positive carries `scratch-input` (the private r0/r1 ABI), six carry
division-step or rotate `system-op`s, and six save r2/r3 in the prologue. No
strong rule fires on any negative. The calibration took three revisions, each
fixing a measured false positive on the negatives before any range verdict
was read: SHC pushes r0–r3 mid-function as outgoing stack arguments (so
`scratch-save` only counts prologue saves restored before return), the old
entry heuristic seeds functions from pool words, and SHC's fall-through tail
call into the next function is legitimate.

Limits of the calibration: the positive set is small and homogeneous (all
private-ABI arithmetic helpers), so the recall figure says little about
hand-written code that follows the C ABI. The boot block below is an
independent sanity check rather than a control: it is hand-written by
inspection (RAM test loops, VBR and SR setup, a stack pointer loaded from
memory), and the scanner flags it. A function with no evidence can still be
hand-written; the scanner only finds shapes SHC never emits.

## Verdicts

`python tools/asm_scan.py --functions` lists every function;
`build/asm_scan.json` has the evidence. `python tools/asm_scan.py --routes`
records the per-range verdicts under the `asm_scan` key of
`config/frontier_routes.json` (`tools/frontier.py` reads only `targets`, so
`make status` is unchanged).

| Range | Bytes | Verdict | Functions | Hand-written evidence | Weak only |
|---|---:|---|---:|---|---|
| `000400`–`000780` | 896 | mixed | 9 | `400` callee-clobber, scratch-input, system-op, zero-idiom; `658` shared-code; `686` scratch-input, sp-write, system-op, zero-idiom; `6dc` zero-idiom; `710` system-op | — |
| `000780`–`001260` | 2,784 | shc-shaped | 2 | — | unseen-form |
| `0012e2`–`001896` | 1,460 | shc-shaped | 2 | — | — |
| `0020be`–`002e4c` | 3,470 | shc-shaped | 6 | — | — |
| `003c88`–`003e2e` | 422 | shc-shaped | 1 | — | — |
| `004044`–`0044d4` | 1,168 | shc-shaped | 1 | — | — |
| `0057d4`–`00601a` | 2,118 | shc-shaped | 2 | — | — |
| `0061d0`–`007004` | 3,636 | shc-shaped | 3 | — | callee-input |
| `007324`–`007956` | 1,586 | shc-shaped | 2 | — | — |
| `007c08`–`007f38` | 816 | shc-shaped | 1 | — | — |
| `007fa0`–`008156` | 438 | shc-shaped | 1 | — | — |
| `0085d8`–`0087b8` | 480 | shc-shaped | 1 | — | — |
| `008b86`–`00907c` | 1,270 | shc-shaped | 2 | — | unseen-form |
| `009180`–`0095b8` | 1,080 | shc-shaped | 1 | — | — |
| `0096ac`–`009b90` | 1,252 | shc-shaped | 1 | — | — |
| `00a226`–`00b2f4` | 4,302 | shc-shaped | 5 | — | unseen-form |
| `00b39a`–`00be04` | 2,666 | shc-shaped | 4 | — | — |
| `00be28`–`00d28c` | 5,220 | shc-shaped | 5 | — | unseen-form |
| `00d394`–`00e3b8` | 4,132 | shc-shaped | 6 | — | unseen-form |
| `00e3e0`–`00e520` | 320 | shc-shaped | 3 | — | — |
| `00ec0c`–`00ee9c` | 656 | shc-shaped | 1 | — | unseen-form |
| `00f29a`–`00f4c6` | 556 | shc-shaped | 2 | — | — |
| `01050e`–`0105ba` | 172 | shc-shaped | 1 | — | — |
| `010788`–`010942` | 442 | shc-shaped | 1 | — | — |
| `012fa8`–`0131e8` | 576 | shc-shaped | 1 | — | — |
| `01326c`–`013378` | 268 | shc-shaped | 1 | — | — |
| `013380`–`013484` | 260 | shc-shaped | 1 | — | — |
| `014d50`–`015f10` | 4,544 | shc-shaped | 5 | — | unseen-form |
| `015fae`–`01715c` | 4,526 | shc-shaped | 3 | — | unseen-form |
| `02090c`–`0209a4` | 152 | shc-shaped | 1 | — | — |
| `020a9c`–`020b24` | 136 | shc-shaped | 1 | — | — |
| `021d94`–`0225e4` | 2,128 | shc-shaped | 1 | — | unseen-form |
| `0225fa`–`02261c` | 34 | pool data | 0 | — | — |
| `0238a8`–`023e3e` | 1,430 | shc-shaped | 2 | — | — |
| `026264`–`02631a` | 182 | shc-shaped | 1 | — | — |
| `02657c`–`026f64` | 2,536 | shc-shaped | 3 | — | — |
| `026ff0`–`027310` | 800 | shc-shaped | 2 | — | unseen-form |
| `0274c0`–`02769e` | 478 | shc-shaped | 1 | — | unseen-form |
| `0277d0`–`0278c0` | 240 | shc-shaped | 2 | — | — |
| `02ac4c`–`02af5e` | 786 | shc-shaped | 1 | — | — |
| `030880`–`030a44` | 452 | shc-shaped | 1 | — | — |

The weak `unseen-form` hits in SHC-shaped functions are ordinary SHC frame
addressing the matched C happens not to use: of the 40 recorded (at most four
per function), 31 are `mov.b`/`mov.w rN,@(r0,r15)` stores to frame offsets
beyond the displacement range, six more are other `@(r0,r15)` or `add r15,r0`
frame forms, and one is `add r1,r15` for `f_780`'s large frame. `callee-input` in `f_61d0` is the same uninitialized-local pattern as
`f_2c878`.

### The boot block, `0x400`–`0x780`

This block runs from ROM before the copy to RAM, and the vector table at
`0x0` points into it. Five of its nine functions carry strong evidence:

- `0x400` (496 bytes, reset): `stc sr`/`ldc sr` to mask interrupts, RAM test
  loops that zero registers with `xor r2,r2`, read r2/r3 before defining them
  and clobber r8–r12 without saving, and the copy loops driven by the table at
  `0x313fc`. It ends in the `jsr` to RAM `0x06000000` and the idle loop at
  `0x658`, which the vector table also names (`shared-code`).
- `0x686`: `ldc r3,vbr` after `xor r3,r3`, then loads r15 from memory
  (`sp-write`) and branches back to reset.
- `0x6dc` and `0x710`: interrupt handlers using `xor r1,r1` and
  `stc sr,r0; and #0,r0; ldc r0,sr`.

Four fragments carry no strong evidence: the three exception spin loops at
`0x674`, `0x67a` and `0x680` (`nop; bra; nop`) and the vector-`0x6ac`
interrupt handler, which pushes r0/r1, does two byte stores to on-chip
registers with a discarded read in between, increments a counter and returns
with `rte`. Their route (`parked-duplicate-interrupt-controls`) records that
genuine `#pragma interrupt` controls did not reproduce the handler's scratch
registers or pools. The scanner cannot tell these 58 bytes apart from SHC
output, so the range verdict is "mixed": the five flagged functions are
likely hand-written, the four fragments remain C candidates. The table words
at `0x73e`, `0x742` and `0x760`–`0x780` are data (the copy-table addresses),
not functions.

### Pool-only range `0x225fa`–`0x2261c`

These 34 bytes are the shared literal pool of the `f_21d94`/`f_225e4`
translation unit (`f_225e4` is already retained C and loads `0x22614` and
`0x22618` from it). No code lives here; the range is covered when its owner
`f_21d94` is.

## Consequences

The scan supports the existing exception agreement: apart from the boot
block, nothing unresolved looks like anything but SHC output, so there is no
evidence-based case for new assembly exceptions, and none is proposed. For
the non-matching track, every function in the 40 SHC-shaped ranges and the
four unflagged boot fragments are C candidates; the five flagged boot
functions stay assembly.
