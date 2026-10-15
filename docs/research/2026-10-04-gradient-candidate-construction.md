# Gradient match from SHCMDL candidate construction

Continuation 214 matches the complete 248-byte palette gradient at 0x29F94–0x2A08C.
The selected source is `session_gradient_construct214/paired_gb.c`; production
retains the entire 1,244-byte context at 0x29BF8–0x2A0D4, including all 13 functions.
No outside literal dependency is needed when that complete context is retained.

## Why the previous controls plateaued

Read-only hardware-breakpoint tracing of original SHCMDL at 0x415920 records the
candidate's triggering IR node in EBP at the call returning to 0x415134. Candidate
construction follows first reads in the delta calculations, not declaration order
or the order in which the base components were initialized. Swapping red/green
initializations leaves creation in semantic red/green/blue order. Swapping their
declarations only exchanges symbol IDs. Reordering the delta computations does
change creation order, but also changes native instruction order.

A consumed divisor copy fixes the old ar/db allocation tie but exchanges the
red/green base registers (nine operand-byte differences). A consumed green copy
fixes those component homes, but exchanges the induction variable and an internal
candidate, changing the function layout. A second consumed blue copy restores that
induction decision while preserving the desired component and accumulator homes.
The selected case has 25 candidates, including three inactive entries after
coalescing. The allocation model reproduces native profitability, unstable sorting,
and allocated homes. Allocation indices in model records are not SH register numbers.

## Ordinary C change

Copy `g0` into `g_base` and `b0` into `b_base` before the delta calculations, then
consume each copy in its delta and output-color expressions. Copy `n` to `divisor`
before the loop and consume it in all three divisions. Original types and values
are preserved; the base components and n are not subsequently changed. The aliases
are used by the computation, with no dummy reads, padding, forced registers,
compiler edits, intermediate-file edits, or object patches.

The isolated gradient proof references literals in its adjacent retained function.
The integrated proof covers the whole source, so its literal dependency list is
empty. Existing reviewed task-cleanup tail proofs remain required and unchanged.

## Evidence

- 61 strict native controls (57 new sources and four freshly rebuilt controls).
- Five exact controls: paired_gb, paired_br, paired_bg, paired_grb, paired_bgr.
- 12 allocator models; 18 construction replays (four initial and 14 final).
- All 30 replays preserve actual REG and ILB bytes against fresh untraced controls
  and native driver captures, totaling 1,242 events.
- 14 native assembly captures independently assembled and resolved; all 61 original
  context objects independently resolved and compared.
- Research model, analyzer and verifier exit zero before freezing 1,176 files.
- New native regression coverage checks the complete context and rejects removal
  of the consumed blue or divisor copy.

See `session_checkpoint214_gradient` for both clean production build gates, native
suite logs, exact retained object/proof, original ROM hashes and deployment pins.
The final canonical report remains authoritative for progress.
