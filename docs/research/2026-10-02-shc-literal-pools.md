# SHC investigation: literal slots precede instruction scheduling

First bounded compiler investigation, continuation75. Colin explicitly selected
the compiler-inspection proposal. Work remains solo, without brainstorming.
Production checkpoint73 remains125264/200698 bytes. This investigation earns
no additional C coverage.

## Result

For the medal example, literal order follows the first references presented to
the assembler's literal-lowering path, before its instruction scheduling. The
assembler keeps separate word and long literal lists, deduplicates entries,
appends new entries, and emits each list in insertion order. Consequently the
first visible MOV.L in final assembly need not refer to the first pool entry.

This is supported by native experiments, retained intermediate files and static
inspection of the actual v5.0r32 executable. It is not inferred solely from a
normalized matching score. The full scheduler, earlier register allocator, and
all literal-expression cases have not yet been reconstructed.

## Reproducible observations

Case directory: `tools/shc_probe/cases/session_compiler_stages75`.
All runs use the production CPU/endian/optimization settings and unchanged
compiler executables. `trace.py` records process/file activity with strace.
It captures both ordinary machine output and the supported assembly-output
mode. A host-side observer takes temporary-file snapshots while strace delays
unlink entry by100ms; no syscall error or compiler-byte patch is injected.
`analyze.py` verifies capture sizes against the final compiler stat observations,
identical native ELF sections except.comment, and identical assembly text with
and without observation. Identical stage files through.sua/.sub/.asa/.asb/.lit
connect the two output modes.

The initial LD_PRELOAD unlink observer captured nothing because wibo is static.
That attempt and its logs are retained. The working observer uses syscall timing,
not interposition. The first symbol-record parser assumed two extra zero bytes;
its failed assertion is preserved in analysis_initial.log and corrected in the
script. Capture evidence is bounded to these runs, not a universal race-free
temporary-file acquisition library.

| Source | First two table references in.sua and.sub | Final pool order | Native context |
|---|---|---|---|
| old | icon, frame | icon, frame |572bytes,4 differing ROM bytes|
| reordered | frame, icon | frame, icon |572bytes,126 differing ROM bytes|
| direct | frame, icon | frame, icon |572bytes,150 differing ROM bytes|
| named | frame, icon | frame, icon |580bytes,shifted roots|
| reduced_old | icon, frame | icon, frame |228bytes,diagnostic only|
| reduced_reordered | frame, icon | frame, icon |228bytes,diagnostic only|
| declswap_old | icon, frame | icon, frame |identical linked bytes to old|
| declswap_reordered | frame, icon | frame, icon |identical linked bytes to reordered|

In old, the final frame-table load precedes the icon-table load despite the
opposite pool order. All four full sources reproduce their prior native objects
outside.comment. The two reductions preserve the actual function body and remove
neighboring functions and unused type declarations. The behavior survives that
reduction: those neighbors are not necessary for this ordering mechanism.
Reduction binaries are never used as production context or coverage evidence.

The declaration-swap prediction was written in PREDICTION.md before compilation:
swap the external declarations, thereby swapping internal symbol IDs, without
changing executable bodies. Both controls change those IDs and retain the exact
linked bytes and pool order. This discriminates insertion order from sorting by
symbol number or hash-bucket order. The assertion is part of analyze.py.

## Actual compiler stages and implementation

Observed driver sequence:
shcprm -> shcfrt -> shcmdl -> shcgen -> shcpep -> shcasm -> shctil.
shcmdl writes.ilb and.reg; shcgen writes.sua and.asa; shcpep reads those and
writes.sub,.asb and.lit; shcasm reads the latter files and produces the selected
output. shctil cleans up temporary files.

Ghidra12.1.4 imported shcasm.exe into a separate analysis project and exported452
functions with no decompiler failure. These exports are inferred pseudocode,
not recovered original source. Compiler SHA256:
469d3445770dd8b52ed0ec9059d6f3c1c4be099986b8db8715ce182c13183459.
The project is `/drive2/tgm2p/ghidra/proj-shc75/shcasm75`; readable exports and
headless logs are frozen in the case directory.

Concrete locations in that executable:

| Address | Observed responsibility |
|---|---|
|0x414990|Reads/lowers intermediate records; opcode0x2a invokes0x405690. Also selects the scheduling path.|
|0x405690|Chooses immediate/word-pool/long-pool lowering; looks up or inserts a literal, gets its list index and constructs the reference.|
|0x420440|Literal membership lookup through127 hash buckets.|
|0x4205e0|Adds the expression to its hash chain and appends a separate ordered-list node.|
|0x41a790|Returns the one-based position by traversing that ordered list.|
|0x41a7f0|Emits word entries, alignment as needed, then long entries; follows each ordered list and clears it.|
|0x41bfc0|Fills the SH scheduling buffer by calling0x414990 with bypass parameter1, performing lowering before selecting scheduled output.|
|0x41bed0/0x41be10|Build and drain the scheduled instruction buffer.|

The append behavior is checked against original x86 disassembly in
pool_append.x86.txt:0x4206a8 loads the previous tail,0x4206ad links the new node,
0x4206af updates the tail, and0x4206b2 increments the entry count. This corroborates
the key data-structure claim independently of Ghidra's inferred types.

## Consequence for the target and next experiment

The ROM needs frame then icon in the pool, with the register assignment found
in old.c. Moving the final assembly loads cannot fix this: their order already
looks correct. A candidate must present frame before icon to the literal-lowering
path while preserving the earlier allocator's choices and the complete context.
The reordered source gets the pool order but changes those allocations.

Next inspect shcmdl's.reg production and shcgen's hoisted-address placement for
these two reduced examples. Derive which live ranges and use counts cause the
R14 frame/R8 icon allocation, then test a genuine source change against both
intermediate order and exact native bytes. Keep source changes that fix one
component but damage the other out of production. The same stage-capture workflow
can then classify the2E4C and259EA register-choice benchmarks.

No compiler executable, production source, verifier, split, denominator or ROM
was changed. Last full checkpoint validation remains228 enabled tests and two
independent clean builds; these diagnostic runs do not substitute for that gate.
