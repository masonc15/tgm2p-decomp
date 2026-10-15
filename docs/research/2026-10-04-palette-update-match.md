# Complete palette updater match — continuation212

The 1,186-byte function at 0x2A0D4–0x2A576 now matches the original ROM exactly.
Together with the initializer and four existing tail functions, its ordinary C
source reproduces the complete 2,936-byte range through 0x2AC4C. Native compiler
flags, verifier, fixed denominator and runtime accounting are unchanged.

Three source changes explain the match:

1. A structured do loop with `while (k++, o++, k < 32)` removes the surplus entry
   branch while preserving inner-loop unrolling. Both early continue paths still
   advance the object pointer and the 32-object counter. Earlier do/goto trials
   lost unrolling; that result did not apply to this structured form.
2. Combining the two return guards with `||` preserves short-circuit evaluation
   and recovers the four required scratch-register operands. All other bytes of
   the 2,932-byte intermediate candidate are unchanged by this guard edit.
3. Ordering the shared case labels as `case 0x44: case 0x40:` preserves behavior
   but changes the compiler's last switch conditional. That recovers the four
   missing bytes and the exact word-literal-pool location.

## Why the case order matters

Read-only hardware breakpoints in the unmodified SHCPEP observe its actual pool
planning routine at 0x40F680 and its distance decisions at 0x40FF16/0x410077.
The word-pool threshold is 0x1CE (462). The original-label control reaches a
natural unconditional-branch boundary and flushes the pending pool there.
The reversed-label source instead reaches a forced-pool decision while planning
`MOV #-1,R0`, after the preceding mode-zero store: estimate 464 exceeds 462.
The two word constants are consequently emitted at 0x2A3C8 and 0x2A3CA, behind
`BRA 0x2A3CC; NOP` at 0x2A3C4. The complete result equals the ROM; no pool,
branch, object, compiler or register allocation is patched.

Immediately after the comparison with 68, the native trace changes from opcodes
0x25/0x24 (conditional plus unconditional) to 0x26 (one conditional). This
supports the distinction between an expanded far conditional and separate
conditional/unconditional branches that happen to look alike after expansion.
A switch-label alias influences the earlier decision even though both labels
lead to the same C statements.

## Evidence and closed controls

`session_palette_update212/analyze.py` checks 28 full native source controls and
one strict full-range match. `verify.py` independently resolves every original
native object and checks six complete retained roots, no retained relocations
and no external literal dependencies. Both exits are checked in VALIDATION.json.
Three SHCPEP traces contain 9,452 events; each traced SUB/ASB/LIT output equals
both a fresh untraced replay and its native capture. Four independently assembled
captures reproduce their strict-compile contexts. The existing three SHCMDL
origin traces retain 429 output-preserving observations.

The negative controls cover structured outer loops, return guard expressions,
equivalent switch exits, explicit dispatch ladders and shared/duplicated case
layouts. Explicit goto ladders lose unrolling and fail; duplicating reset case
bodies shifts boundaries. A regression test reverses the winning shared labels
and requires strict rejection. The unaliased-initializer negative remains.

The first pool tracer tried to decode a metadata record as an instruction operand;
it was corrected to inspect operands only for instruction opcodes. The failed
attempt is recorded in replay_pools_initial.log. Its successful replacements
verify unchanged outputs. The first integration build caught a missing required
context-rom header; its logs are retained. The header was corrected and both
production gates require new clean builds. See session_checkpoint212_palette_update
for final whole-ROM, test-suite, report and publication evidence.
