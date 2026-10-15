# SHC investigation: the discarded board-word read

Continuation 82 investigates the 182-byte unmatched function at 26264–2631A.
Checkpoint 81 remains authoritative: 126,886/200,698 bytes. This investigation
adds no matching bytes and changes no production source or verification rule.

## Original operation

The ROM recomputes the selected board address using the input word saved on the
stack, stride 3664 from 26352, and base 060AD6A0 from 26358. It adds 24, then executes
`MOV.W @R3,R3` at 262CC. The loaded word is discarded: 262CE writes MACL to R14,
and 262D0 overwrites R3 with the animation pointer. Thus the unused read is an
actual original operation, not a fabricated source liveness requirement.
The address calculation sign-extends the low 16 bits of the word product;
the audit records that detail rather than assuming unrestricted 32-bit indexing.

The historical draft retains a similar load through an `if (0)` store, but still
does not match. That hint is absent from every source tested here. The ordinary
clean draft is six bytes short; a narrow volatile read preserves the operation
but changes scheduling and remains two bytes short. This evidence does not
establish the spelling or qualifiers of the original source.

## Pinpointed compiler decisions

The original SHCMDL read-tree dump contains `x = *p`. Four read-only hardware
snapshots show it survives the first expression pass, DAG construction and
pattern-matching pass. It also survives the initial common-expression pass.

The first iteration through 418D50 changes it in two steps:

1. The dead-assignment routine 418E10 sees a definition record with an empty
   use list and no reference chain. Its branch at 41939B removes the assignment
   to `x`, leaving the actual dereference as an expression statement.
2. The next expression pass calls 4140D0. At 41419B, the ordinary dereference
   has type 8, no required-value flag and no side-effect flag. It is replaced
   by an empty expression. The pointer definition is removed in a later pass.

The narrow volatile control instead has type 10 (type 8 plus bit 2). The read
survives all three later snapshots, reaches the output tree, and never enters
either observed deletion branch for this source line. The native compiler output
still mismatches. Retaining the read is necessary but not sufficient for matching.

This rules out merely changing the dead result's spelling, storage class or
assignment target. It also explains why the earlier allocation-priority technique
cannot fix this particular missing operation: the operation has already gone
before register allocation begins.

## Bounded source controls

All 16 source controls fail the unchanged target gate. Seven alternatives produce
the exact clean baseline bytes: bare pointer read, bare field read, void-cast
read, register result, overwritten input result, register pointer and local
initializer. Assigning the volatile read to the unused result produces the same
bytes as discarding it directly.

Additional negative controls test the volatile member form, register field pointer,
an actual record pointer computed before the read, and a zero local consumed by
the three original zero stores. These change allocation or scheduling but do not
match. The register-field control preserves all 15 roots and protected ranges,
yet has 117 target byte differences. The zero/record combination still has 92
nominal target differences and shifts the following root. None is integrated.

Do not repeat these controls. A future attempt must account for the original read's
retention and interleaved address calculation using genuine source behavior.
Artificial consumers, dead branches, padding and object edits remain excluded.

## Evidence

`tools/shc_probe/cases/session_board_read82` contains all 16 sources, raw objects,
linked contexts, boundary checks and negative results. `analyze.py` checks the
ROM instructions and literals, source hashes, byte-identical controls, native
compiler sections, and both observed removal decisions. `check_sources.py`
reconstructs the controls from the pinned checkpoint 81 research source.

Four normal/debug middle-pass replays and five distinct hardware replays preserve
`.reg` and `.ilb` exactly. Two native stage captures match the ordinary compiler
objects outside comment metadata; the baseline reuses the byte-identical capture
from case 81. Fourteen target-tree snapshots support the pass boundaries. Hardware
breakpoints only alter debug registers; compiler instructions and data are not
patched. Private logging bits add read/write-tree dumps without changing outputs.

The tracer's function symbol 174 and source line 465 are specific to these pinned
sources and this compiler. Recovered C in `optimizer_sources` is inferred
decompiler output, corroborated here by the original x86 and live observations.

No whole build or whole test suite was repeated for these rejected private
sources. Fresh production-pin and ROM checks preserve checkpoint 81; its 232-test
pass and independent clean builds remain the latest full validation.
