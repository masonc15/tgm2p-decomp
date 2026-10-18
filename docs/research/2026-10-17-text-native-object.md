# Text-helper native object and optimizer probe

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%). No canonical source, compiler,
converter, split, proof, or exception was changed.

After reviewing the authenticated startup-pointer negatives, no new startup
source control was justified. Work switched to the text-helper partition
question recorded in `2026-10-13-gain-textwrappers/REPORT.md`: can the real
pool flush and the local width-helper BSR be explained by information omitted
from ROF-to-ELF conversion or by the historical native linker workflow?

Packet: `research/2026-10-17-text-native-object/`. EXPERIMENT.json records
the prediction before the native capture. The source is the existing genuine
`producer_sections.c`, with real predecessor and wrapper sections. It was
captured once through the unchanged SHC v5.0r32 eight-stage driver. This is
not a new section-spelling search.

## Original ROF evidence

ROF.json inventories the actual original object blocks and raw relocation
attributes. There are 33 relocations, all attribute 0x04 with bit descriptor
0020 and four-byte expressions. The object contains header, unit, section,
external-symbol, section-header, object-content, relocation and trailer
records; no per-call optimization record was found. Its P section is 328
bytes and PPREFIX is 876 bytes. The current converter maps these actual
relocations to DIR32; it is not discarding an observed branch-relaxation
record. It does classify named PPREFIX as readonly data, which remains an
existing admission limitation rather than something this probe changes.

The initial inventory script accidentally shadowed Python's inspect module;
it was renamed inspect_rof.py before successful inspection. No compiler was
rerun for that scripting error.

## Historical native tool probe

Installed original tools include the H SERIES LINKAGE EDITOR Ver.6.0 and
SH SERIES INTER-MODULE OPTIMIZER Ver.1.1(Release05). Their displayed versions
and help strings are preserved. The optimizer exposes branch-size control.
The isolated native-tools directory contains copies of these existing tools;
production compiler installations are unchanged.

The bounded probe records parser/setup failures rather than treating them as
negative optimization results: command-line linker options were rejected by
the optimizer; FORM RELOCATE was rejected; plain filenames were needed to
reach the subcommand file reliably. A successful native optimizer run then
produced an output object and link script, but that script placed P at zero.
It could not test the original call distance. PLACEMENT_CORRECTION.json
records this new observation and one additional run before execution.

The corrected script explicitly places PPREFIX at 0x0600d8f4 (ROM e074) and
P at 0x0600dc60 (ROM e3e0). The generated native script confirms both addresses.
The optimizer completes and emits out3.rel. Both section byte arrays, all
relocations, and all defined-symbol section offsets are identical to the
original object. Only container metadata differs. NATIVE_RESULT.json retains
this comparison and both object hashes. No optimized output is an admission
candidate or a claimed gain.

There were eight ordinary compiler executable stages and nine native-tool
executable stages, including help/parser probes. The original native probe
bound was eight; the one-run placement correction was explicitly recorded
before execution. Total executable stages are 17, with no source variants.

The native input still reports undefined external symbols. Consequently this
is **not** a fully resolved native link or a positive control proving that
branch optimization was enabled for every function. It establishes that this
native optimizer reads the current ROF and, in this isolated corrected-layout
input, leaves the indirect cross-section call unchanged. Do not generalize it
to all native linker workflows or claim the whole partition question solved.

## Actual placement and ordinary bytes

measure.py independently links the original ordinary ELF with the actual
section placement and existing external-symbol resolution; it does not use
the native optimizer output. It retains both allocated sections and all real
function roots. The combined extent is 1,204 bytes versus the original 1,196.
Actual roots are e074, e39c, e3b8, e3e0, e414, e492. Thus e412 and e490 remain
two bytes late. Raw nominal differences are:

| Interval | Differing bytes |
| --- | ---: |
| e074-e3e0 | 727 |
| e3b8-e3e0, already retained palette | 0 / 40 |
| e3e0-e412 | 34 / 50 |
| e412-e490 | 103 / 126 |
| e490-e520 | 111 / 144 |

The retained palette remains byte exact; the whole predecessor does not.
Existing named-section admission restrictions and wrong roots still apply.
There is no extracted object, acceptance receipt, cold replay, suite result,
or dashboard publication for this research packet.

The next distinct question is whether native inter-module optimization needs
additional compiler-generated function/optimization metadata or a fully
resolved input. Recover that prerequisite and a meaningful positive control
before another source-partition attempt. Do not manufacture relocation
metadata, patch instructions, or infer correctness from relative addresses.
