# SHC operand order and expression rebuilding — checkpoint236

Production remains checkpoint225: **135,594 / 200,698 bytes (67.56121%)**.
65,104 bytes remain, including 1,638 runtime assembly bytes. Two new native
source probes fail the full EC0C–EF04 match. No new C credit or production changes.

## Stage localization

Five output-preserving SHCMDL replays capture trees at four points: before initial
setup (0x40F5B0), before optimization (0x40F8B0), late optimization (0x40FC69), and
before allocation (0x405ECC). Three reuse frozen235 source/native captures; the
other two are new source probes. Each replay's REG and ILB bytes equal both an
independent untraced run and its normal native capture. REG also round-trips
through the independently authenticated format decoder.

The base-first integer-address source reaches the optimizer with base first.
At the late snapshot its offset is first. Baseline offset-first source retains
its order, and named-base compound assignment remains base first. This locates
the reversal inside the middle optimizer, before GEN's recipe selector.

## Actual writer and insertion rule

Two hardware data-watchpoint replays monitor the row-addition node's child field.
The 33 write events include bytewise clearing/reuse and node replacement; these
are not all operand swaps. The reconstructed addition is written at 0x40A654
(the watchpoint traps at 0x40A657), in node constructor 0x40A630. Each of three
rebuilds per control is called from 0x4046A7/return 0x4046AC in 0x404600. Its input
list is built by 0x404140, invoked by the expression flattener 0x403F10.

Four additional unchanged-output replays observe the term builder and its final
lists. The measured ordering key is node byte+0x5B: for the observed terms, it is
one plus the sum of child sizes. With no ordering-sensitive effects in these
additions, larger terms go first; bare-symbol ties additionally use symbol number.
The baseline has offset rank5 and address rank1. The integer-address form has
offset rank5 and address-cast rank2, so changing C operand spelling cannot retain
the base first through this pass. An independent model reproduces 63 recursive
size checks, 24 leaf insertions, and all12 reconstructed lists. Original x86
instructions and clearly labeled inferred decompiler output are saved.

## Predicted tie experiment

PREDICTION.md was written before compilation. A genuine char row_offset temporary
followed by unsigned-long conversion makes its term rank2, equal to the explicit
unsigned-long address conversion. Both are cast nodes, so the bare-symbol tie
rule does not apply. The prediction was that both source orders would survive
middle optimization. All four stage snapshots and all three rebuilding passes
per source confirm it:

| Probe | Middle/GEN child order | GEN ADD recipe | Native result |
| --- | --- | --- | --- |
| rank_tie_base_first | base, offset | [96,0,94,170,250,0] | ADD R2,R4 |
| rank_tie_offset_first | offset, base | [96,0,94,169,250,0] | ADD R2,R4 |

The two fresh contexts are byte-identical:744 bytes,677 differing/missing bytes.
Native captures independently assemble to each strict context. Both use the
allocated offset in R4 and load the base into scratch R2. GEN selects a different
recipe for each order but coalesces the result into R4 either way. Both emitted
operand pairs are independently decoded from original table descriptors and raw
metadata. Thus the source-order prediction succeeded while the original-register
and full-byte-match goals did not. Do not repeat operand commutation alone.

These are ordinary, target-specific 32-bit unsigned address conversions. The
row offset is0,30,60 and fits char; adding it to this mapped buffer does not
wrap. No compiler edits, IR edits, register directives, volatile qualifiers,
dummy uses, padding, or relaxed matching gates were introduced.

## Validation and next action

Two validation commands pass. Evidence includes five pipeline replays/11,276
records, two watchpoint replays/2,279 records, four term-list replays/161 records,
and two GEN replays/604 emissions. Of the watchpoint records,33 are child-field
writes. Only two sources were newly compiled; three calibration contexts were
reused and independently reauthenticated. No new full production suite was run.
Checkpoint225's two clean266-test gates remain historical production evidence;
a separate fresh dual-mirror receipt verifies64 pins and original/rebuilt hashes.

The previous goal turn made verified compiler-research progress. This continuation
locates the rewrite, models its ordering rule, and confirms a source prediction.
The remaining constraint is now explicit: preserve scratch offset R3 and base R1
without introducing a named offset allocated to R4 or additional stack traffic.
Next inspect GEN's choice to reuse an allocated child result versus allocate a
new accumulator, using the two equal-rank controls and the original unnamed
expression. Keep whole-context acceptance; initialization, width reload, color
paths, and boundary mismatches also remain. Full literal100% goal active; solo.
