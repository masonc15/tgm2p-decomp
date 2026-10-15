# SHC volatile-read scheduling investigation

Checkpoint81 remains **126,886 / 200,698 bytes (63.22235%)**. This investigation
adds compiler evidence and four rejected diagnostic compiles, no production bytes.
The preceding compiler investigations already enabled210 medal bytes and1,412
board-update bytes. Original compiler and default matching rules remain intact.

## Finding

For f26264, making the discarded short read volatile preserves the operation but
introduces an all-register scheduling barrier in SHCASM. At41C2D0, instruction
code flag0x80 forces both write-mask words toFFFFFFFF. The live observed volatile
MOV.W has code flags0x81 and resulting flags0xC000. Its dependency successors
include the later record-address multiply and zero materialization, so neither
can move ahead of that read in this candidate.

The original ROM orders these operations differently:

| Address | Original instruction |
|---|---|
|262B6|MOV #0,R6|
|262C4|MULS.W R14,R5|
|262CC|MOV.W @R3,R3 — discarded field word|
|262CE|STS MACL,R14|

The multiply and its result retrieval straddle the original read. The observed
volatile candidate places both after it. This explains a concrete failure of
that source form; it does not prove all conceivable volatile forms impossible.
The ordinary form remains subject to the precisely observed SHCMDL deletion
reported in the preceding dead-read investigation.

## Independent observations

Hardware execution breakpoints observe41C2D0 before write-mask classification,
41BF92 after dependency construction, and41BE7F after selection. Compiler code,
memory and configuration are unmodified. Fourteen buffers contain182 nodes and
414 directed dependency edges. Every node is selected exactly once and every
edge is respected by the observed order. The volatile read is buffer162/node9;
its successors include multiply12 and zero18. Baseline has90 nodes, volatile92.

The target's FLABEL record contains source line450, independently identifying
f26264 in both sources, and occurs as function14. Internal label IDs are not
assumed equal to assembler symbol IDs. Original x86 instructions and inferred
Ghidra C for the relevant scheduler routines are saved alongside the traces.

Two normal and two scheduler-debug assembler replays, plus two hardware replays,
reproduce all original native ELF sections except compiler comment metadata.
Debug runs change only file offset318 (common raw offset13C), enabling bit8;
the scheduling-disable bit1 stays clear. The analyzer independently performs
eight native-section comparisons, validates the graph and checks original ROM
instructions. It does not infer behavior merely from diagnostic logs.

## Historical revision control

Four bounded compiles test the same ordinary and volatile sources under SHC5.0
r10 andr31. All four fail the unchanged target verifier. Their target bytes
26264–2631A are exactly the same asr32 for the corresponding source. r31 also
reproduces the entire context; r10 changes earlier context. Thus these two
revision changes do not resolve the discarded-read mismatch. This evidence
closes these specific controls, not all compiler versions. Production staysr32.

## Capture corrections and limits

Replaying cleanup-time common files initially crashed the assembler. Fresh
snapshots at the assembler's Linux exec boundary replay successfully. This
establishes that stage-entry inputs work; the specific stale field responsible
for the cleanup replay failure has not been isolated.

The successful capture used capture_exec_initial.c. A subsequent formatting-only
fix in capture_exec.c removes two indentation warnings and passes compilation
with-Wall-Wextra-Werror; that revised capture executable was not rerun. The
original source and capture logs remain saved. Initial scheduler tracing also
mistook metadata FLABEL operands for pointers and failed; its source and partial
log are retained. The corrected tracer treats metadata operands as non-pointers;
both successful traces pass independent unchanged-output checks.

## Next action

Stop repeating the unused-result/storage and tested volatile controls. If pursuing
this read further, identify a genuine use eliminated only after SHCMDL, using
existing source and compiler stage evidence; do not invent dead consumers or
branches. Otherwise apply the already productive priority/copy analysis to a
different unresolved function. No compiler modifications, object edits or relaxed
verification are justified by these observations.

Evidence: tools/shc_probe/cases/session_read_schedule83. Run analyze.py with the
nuada project venv to revalidate saved artifacts. The production recheck verifies
31 pins, original ROM hashes and reports in both checkouts. Checkpoint81's232-test
pass and two independent clean builds remain the latest full validation; they
were not rerun for these private rejected experiments.
