# Complete board-scroll match: 800 new C bytes

The entire function27B50–27E70 now compiles to the original800 bytes, including
its literal pools, under the existing SHC toolchain and unchanged default
verifier. Production source is `src/board_scroll_27b50.c`; `splits.txt` retains
its complete range. No preceding or following source context is needed.

Current verified coverage is **131,780 / 200,698 bytes (65.6608436556%)**.
The gain is800 bytes;68,918 remain, including all1,638 runtime assembly bytes.
The literal100% goal remains active. The primary agent performed this work
alone, following the user's solo-work instruction.

## What broke the plateau

The important distinction was between the raw inputs and their converted values.
A single long variable for both the loaded high word and its packed successor
made an ordinary register hint affect both phases. Separating `raw_hi` from
`hi`, and applying the hint only to `raw_hi`, preserves the raw value inR12 while
the packed value spills to the original stack slot. The unhinted split alone
produces byte-identical output to the single-variable baseline, so the useful
change is the phase-specific hint rather than a variable rename.

With that lifetime fixed, the original ABC loop statement order restores all
eight main variable homes. The earlier BCA ordering was only appropriate for
the previous conflict graph. The offline tie-order prediction was followed by a
native compile and complete allocator replay; a predicted home never earned
matching-byte credit.

Packing as `(raw_hi & 0x3ff) << 16` also matters. Compared with shifting first
and applying0x3FF0000, it adds a pre-loop0x3FF use conflicting with the hinted
raw value. Its priority rises16→17, but its profit falls enough to lose a retained
register. This freesR8 for the flag address, while3F00 and1FF occupyR10/R9.
A larger raw priority therefore does not imply a more favorable allocation.

The remaining differences came from concrete source types and update order:

- The state globals are scalar signed shorts. Their one-member-struct wrappers
  introduced an extra reload after the increment.
- The middle attribute component is a separate hinted **unsigned short**, loaded
  before combining the fields. Widening it immediately to an unsigned long loses
  the original move/conversion sequence. With the short temporary, the entire
  prologue and first part of the loop match through27C54.
- The loop counts224 down to0. Its decrement follows the five pointer increments
  in the update expression. Moving that decrement from first to last preserves
  the complete26-candidate allocation graph but changes instruction scheduling,
  closing the final146 differing bytes and producing exactly800 bytes.

All hints are ordinary C `register` qualifiers. The source does not name physical
registers, emit assembly, add dummy work, patch compiler output, or change data
for matching credit. The scalar short's original comparison against0x1FF00 is
preserved with its original type.

## Compiler investigation

The suspected40E580 shared-expression extraction pass was not responsible for
the high-word split. Three fresh read-only traces cover4,827 events and2,148
entry/exit pairs. Each creates exactly one temporary for the shared `k * 4`
address calculation. The high-word packing expressions have count1 and do not
pass the repeated-expression extraction threshold. This negative observation
redirected work toward explicit source lifetimes.

The case contains15 native controls:12 new and3 carried from earlier work.
Fourteen remain strict negatives; `counter_last` is the complete positive match.
Seven fresh26-candidate allocator models reproduce priorities, conflicts, sort
order, eligibility and assigned/unallocated homes. Together with the extraction
traces,10 completed fresh replays cover5,513 events. Every replay rebuilds the
frontend, runs a fresh untraced SHCMDL control, and verifies that traced REG/ILB
outputs equal both that control and the corresponding native capture.

Evidence: `tools/shc_probe/cases/session_scroll_hi_extract192/`.
`RESULTS.json` records all controls. `EXTRACTION_ANALYSIS.json` authenticates the
extraction and mask findings. `COUNTER_GRAPH_EQUALITY.json` proves identical
allocation before and after the last scheduling change. The archived positive
candidate retains its inherited draft comment; the production source has the
current explanation and required build metadata.

## Integration and verification

Both the isolated coordinator checkout and canonical Nuada checkout completed
independent `make clean` and `make split check report` runs. Both full suites
passed **253 tests with zero skips**, with `PREFIX_COMPILE_TESTS=1` and
`PERM_SETUP_TESTS=1`. The new native regression checks prove the full function,
its single owned root, no excluded context, no external literal dependencies,
and no remaining packaged relocations. A negative control moves the decrement
back to the front and must fail without publishing a retained object or proof.

Both builds reproduce the original1MiB program and split EPROM checks:

- SHA-1: `c84cf538962ddbb377290301d76d75d8b542e5a2`
- SHA-256: `d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7`

The fixed byte denominator remains200,698. The current report lists604 matched
functions out of781 report functions and166 complete units out of210; those
report function/unit counts are not the completion denominator.

The first isolated integration build rejected my missing `context-rom` header
for the `c-range` entry. I added the explicit same-origin header and reran the
clean build and full suite. That failed metadata attempt is retained as
`isolated-build-missing-header.log`; it was not a source-byte mismatch and is
not counted as a passing build.

Production validation, original and packaged objects, full reports, logs and54
source/tool pins are frozen in `session_checkpoint192_scroll/`. The previous51
pins other than the intentionally updated split map remain unchanged; the new
source and native regression test add two pins. The compiler, verifier, runtime
rules and original target ROM remain unchanged.

Next inspect the existing near-match for the following wave/draw function2808A,
including its prior rejected controls, before selecting a fresh source change.
The proven method here is to recover the actual types and distinct consumed
value lifetimes, then verify complete native bytes; it is not a justification
for repeating broad hint or declaration sweeps.
