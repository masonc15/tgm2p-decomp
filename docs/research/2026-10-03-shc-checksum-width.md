# Checksum accumulator width recovers 50 matching bytes

Continuation104 isolates the checksum21170–211A2 mismatch to the accumulator's
source width. Changing only `long sum` to `unsigned short sum` in the genuine
seven-function EEPROM context gives all50 original bytes. The signed-short
control also matches; unsigned short states the modulo65536 checksum directly
and is the production choice. No compiler binary, flag or verifier changes.

The initial canonical savestatus context is740 bytes with all seven original
function roots. Its checksum differs in seven positioned bytes. Existing reset
and status ranges are exact in this context; read/write context still differs
and is excluded. Their existing separate production providers are untouched.

## Why the source type changes the registers

The original usesR3,R1,R3,R1 for word fields38,3A,3C,3E. The wide-accumulator
candidate usesR3 for all four. Read-only hardware breakpoints in unchanged
SHCGEN establish this before the later scheduling pass:

- Temporary selector418900 chooses the arithmetic temporary before preparing
  the indexed word address. Address selection at42261F resets last toFF and
  selectsR0. The baseline next selection therefore sees lastR0.
- Recipe builder41F580 reconstructs the recent-register mask from its scratch
  fields at offsets2C/34 and operand68. The wide case creates separate word
  load and addition recipes. A word-load recipe leaves mask9 (R3 and R0), but
  the intervening addition recipe replaces it with zero.
- Narrow accumulation creates one combined recipe for each word addition.
  No intervening recipe clears its history. Masks alternate9 and3, penalizing
  the preceding word temporary; the next choice alternatesR1 andR3.
- The baseline has16 recipe returns for this function; the narrow case has12.
  Both emit24 instructions through41B3D0. SHCPEP produces26 recorded instructions
  including return handling; its narrow output matches the original schedule.

The initial suspicion that address preparation alone explained the mismatch
was incomplete: the intervening addition recipe clears the recent mask.
The type change removes that reset while keeping the address preparation.

The model independently reconstructs all60 observed recipe masks and all492
scratch selections across four genuine source controls. Four emitter, four
scratch, four recipe-history and four peephole replays pass control equality.
Every GEN replay reproduces the original captured.SUA. Every PEP replay
reproduces original.SUB/.ASB. Replayed.ASA metadata differences from cleanup
snapshots are recorded explicitly; traced and normal.ASA agree exactly.
Only CPU debug registers are written by the hardware tracers.

## Native controls

All ten native captures equal their corresponding ordinary compiler objects
in every ELF section except compiler comment metadata.

| Control | Checksum byte differences | Result |
|---|---:|---|
| Baseline long accumulator |7|Reject|
| Ordinary addition assignment |7|Whole context identical to baseline|
| Reversed addition operands |7|Whole context identical to baseline|
| Grouped word pairs |29|Reject|
| Middle pair grouped |10|Reject|
| Four-word expression chain |26|Reject; following roots shift|
| Overlapping real word loads |20|Reject|
| Unsigned long accumulator |7|Whole context identical to baseline|
| Signed short accumulator |0|Accept|
| Unsigned short accumulator |0|Accept; integrated|

The paired width controls isolate a one-line trigger. Earlier helper and final
result experiments did not test this successful genuine-context accumulator
change. Older results containing inert branches are unsuitable baselines.
No padding, dead consumers, forced physical registers or assembly replacements.

## Integration and validation

`src/savechecksum_21170.c` retains exactly50 bytes from740 bytes of genuine C
context. Only_f_21170 is exported. All seven original native roots are checked;
the retained object has no relocations. Two outside literal loads at21170 and
21174 resolve to original bytes06065650 and06065642. The other690 context bytes
are excluded from this unit's coverage. The existing source providers remain.

Two new compiler-backed tests verify roots, exports, retained bytes and literal
proofs, and reject the wide-accumulator regression at2118C. The isolated clean
build passes original ROM/EPROM hashes and236 enabled tests, zero skips.
Canonical preflight verifies all33 old production pins and exactly the intended
split change. The independent canonical clean build also passes original hashes. All35
production pins, reports, linked native context and retained bytes agree between
checkouts. Original compiler objects differ only in comment metadata. Evidence
is recorded in `session_checkpoint104_checksum/RESULTS.json`.

New checkpoint:127326/200698 bytes,63.44159%,596/784 functions and158/204 units.
Gain50 bytes;73372 bytes remain, including all1638 runtime bytes. The literal
100% goal remains active. Research sources, intermediate streams, hardware
traces, analysis and hashes are frozen in `session_checksum_codegen104`.

Next apply the recipe-history model to remaining narrow arithmetic/conversion
near-matches, beginning with the parked startup2E4C temporary conflict. Do not
resweep the closed checksum forms or the object-list initialization families.
