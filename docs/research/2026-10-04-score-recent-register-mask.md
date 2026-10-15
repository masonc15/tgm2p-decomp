# Score comparison: recent-register mask — continuation173

Production remains checkpoint161: **128,398 / 200,698 bytes (63.9757247207%)**.
This continuation adds compiler evidence and sixteen rejected native controls,
with no new matching C bytes. The full 100% goal remains active.

## What the trace proves

Three fresh SHCGEN replays use hardware execution breakpoints and a two-byte
write watchpoint at `0x460534`. They observe the baseline, the 13-difference
positive-label candidate, and the 15-difference joint-exit candidate from
continuation172. Only x86 debug registers are written by the tracer. Traced
SUA and ASA output equals an untraced control byte for byte; SUA also equals
the previous native capture. ASA differs from that cleanup snapshot as already
documented by the replay harness, so it is compared with the fresh control.

The verifier checks every one of **1,606 observed mask writes** against the
original instruction's operation and source register, and checks continuity
between writes and subsequent result-allocation requests. Separately, it
reconstructs **363 complete masks** from the node's result slots at metadata
`0x2c`, address slots at `0x34`, and output operand at `0x68`. All those complete
masks are below 15, so the all-four-register fallback is not exercised. This
model is scoped to the observed integer operands and slot values.

Original SHCGEN instructions perform the following sequence:

- `42007e`: clear the mask.
- `4200c1`: OR each allocated result register from metadata `0x2c`.
- `420124`: OR each address register from metadata `0x34`.
- `420152`: OR the output operand's dependency mask from `42c190`.
- `42015c`: retain only the four low scratch-register bits.

The watchpoint reports the PC after each write. For example, `420164` is the
observation immediately after the final AND, not the writing instruction.

Immediately before the threshold result allocation:

| Candidate | Global AND result slots | Address slots | Output operand | Recent mask |
| --- | --- | --- | --- | --- |
| joint exit | all unused | R2 | R0 | 5 (R0, R2) |
| positive labels | R0 | R1 | R0 | 3 (R0, R1) |

Both masks come from the global AND node, opcode `0x4c`. No intervening mask
write occurs before the threshold allocation. Fresh observations agree with
continuation172's independent result-selector traces: both requests have
`last=R3`, but the different recent mask makes threshold R1 win in the joint
candidate and R2 win in the positive-label candidate. The baseline also leaves
mask 5, but its threshold request occurs at emitter count124, with different
cache occupancy; the joint and positive candidates reach it at count134.

This closes the proposed recent-history trace. The correct immediate global
test naturally contributes R2 through its address slot. Changing that slot to
R1 would itself spoil the global load. A matching comparison therefore needs
a different allocation context or preference, rather than a manipulated mask.

## Native source controls

All controls retain the original following-function source and use ordinary,
consumed C values. Strict prefix verification rejects every control.

| Rewrite from joint exit | Target differences | Context bytes |
| --- | ---: | ---: |
| do/break block | 705 | 1460 |
| separate negative threshold exit | 22 | 1456 |
| conditional threshold expression | 15 | 1456 |
| threshold local: short, int, long | 33 each | 1456 |
| threshold and level locals: short/int/long threshold | 24 each | 1456 |
| comparison-result local | 462 | 1464 |
| entry-condition local | 478 | 1468 |
| five equivalent int/long operand-promotion variants | 15 each | 1456 |

The conditional expression and all five cast variants reproduce the entire
joint candidate context byte for byte. The three threshold-local types produce
identical contexts, as do the three paired-local types. Explicit locals put the
threshold in R4 and the paired level in R5, so they fail the original register
sequence despite preserving instruction count. Every 1456-byte control retains
nominal function roots and the baseline following function byte for byte.
The larger controls shift the following function and are rejected accordingly.

## Evidence and next step

`tools/shc_probe/cases/session_score_recent173/` contains the tracer, replay
harness, source diffs and provenance, native objects and strict rejection
receipts, original disassembly, and independently checked `RESULTS.json`.
The verifier checks the frozen continuation172 inputs before using them.

Both Nuada checkouts retain all46 production pins,59 prior frozen cases,
original ROM hashes and the fixed coverage report. All23 reference ROM files
match the recorded MacBook source. Last full build/test gates remain
checkpoint161's independent clean builds and245 enabled tests. No new full
build or suite is claimed for this research-only continuation.

Keep the 13-byte and15-byte candidates. Next inspect the construction of the
threshold node's register preference word at metadata `0x12`, together with
the result use count and inherited mask. Existing traces show preferred=0;
the next question is which genuine expression relationships give a memory
load a useful preferred scratch register or keep R1 unavailable while the
global test remains correct. Trace original writers and use small consumed
source experiments. Do not repeat literal suffixes, equivalent casts,
conditional syntax, or the named-local variants recorded here. Do not patch
the compiler, force physical registers, or credit partial matches.
