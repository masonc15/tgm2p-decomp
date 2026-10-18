# DAS native GEN state and promotion-recipe control

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%). No canonical C, proof, split,
compiler, or exception changed.

The October10 DAS packet could authenticate MDL but not GEN. Its replay read
all files from the final monitor snapshot. Reviewing the native syscall trace
showed GEN reads its CMN and later truncates and rewrites that same file. This
was concrete new state evidence, rather than another blind replay.

## Authentic native-stage inputs

Packet: `research/2026-10-17-das-native-gen/`. The read-only observer stops at
Linux exec events before GEN and before PEP, copying the actual files while
the child is stopped. It never patches compiler instructions or input bytes.
Two original machine-mode captures preserve both stage boundaries and the
ordinary original objects. Their allocated sections, roots, relocations, and
independently linked context bytes reproduce the archived signed/unsigned
controls exactly.

Replaying unmodified GEN from each before-GEN snapshot reproduces its actual
before-PEP SUA, ASA and SUD **byte-for-byte**. Hardware-breakpoint observations
also preserve all three streams exactly. Signed SUA is37,409 bytes; unsigned
SUA37,425; ASA3,529 in both; SUD is empty. This removes the earlier GEN
provenance limitation for these captures.

Among retained pre-existing input files, only CMN changes during GEN. The
raw little-endian DWORD at offset174 changes1 to649; offset274 changes0 to17;
offset1014 changes0 to7354/7356. These are measured fields, not a claimed full
CMN-format decoder. STATE_TRANSITION.json preserves every changed byte.

A preregistered causal control replaces only the starting CMN with the native
before-PEP CMN. All other starting files remain identical. The signed replay
then differs from native by804 SUA bytes and814 ASA bytes. This demonstrates
that stale CMN alone breaks replay; these counts are not asserted to reproduce
the older final-driver snapshot's598/506 differences.

The packet used **22 executable stages**: two eight-stage drivers, two normal
GEN replays, two hardware emitter traces, one stale-CMN control, and one
additional conversion-node observation. Normal GEN's exit1 is its expected
successful stage exit. No C source variations belong to this packet.

## Conversion ownership

The authenticated signed emitter sequence has EXTU.B R4,R4 at emission67 and
MOV R4,R9 at68. The unsigned control adds index EXTU.B R7,R7 at67 and retains
the same delay pair at68/69. Thus the pair already exists in GEN before PEP
scheduling. The signed source is known wrong for high-bit indices and remains
ineligible even though its nominal residual is only15 bytes.

The conversion-node observer identifies **the same parent node for both
instructions**, with final operandR9 and child operandR4. Its selected recipe
is0x44e3d8. The emitted narrowing recipe words are `[768,0,169,169,250,0]`,
followed by the move recipe. This is not evidence that two independent GEN
nodes or a later scheduling pass introduced the copy.

## Bounded source hypothesis and rejection

Packet: `research/2026-10-17-das-promotion-recipe/`. The new prediction was that
an unsigned-long outer promotion would select the direct-extension recipe
observed on the unsigned index conversion. Only the two comparison RHSs change
from `(unsigned char)delay` to `(unsigned long)(unsigned char)delay`.
`f->das` is unsigned char and delay is6..14, so both compared values remain
nonnegative bytes. Existing conversion/staging controls were reviewed; older
referenced /tmp generators are missing, which is not proof of novelty. The
native recipe observation is the evidence for reopening.

The prediction is **falsified**. The common threshold's final REG logical10
changes type0x10 to0x1c, but retains R9 and ranges22..30 and33. The parent's
conversion recipe remains0x44e3d8, with the same in-place extension and copy.
Only two resolved bytes change from the unsigned baseline:3d5d and3d8f,
93 to92, selecting unsigned instead of signed comparison conditions.

| Full context | Bytes | Actual gravity root | DAS differences /422 | Protected gravity differences /534 |
| --- | ---: | --- | ---: | ---: |
| Signed diagnostic | 4316 | 3e2e | 15 | 0 |
| Existing unsigned control | 4320 | 3e32 | 240 | 500 |
| Unsigned-long comparison | 4320 | 3e32 | 240 | 500 |

All three preserve the3,360-byte predecessor exactly. The new source uses
**10 compiler stages**: one native eight-stage driver, normal GEN, and one
hardware conversion observation. An initial observer spawn failed because a
copied observer lacked execute permission; no SHC process launched. That failure
is preserved separately with zero actual compiler stages, then corrected.

The independent `measure.py` and `verify.py` in the native packet recompute
raw placement, replay equality, conversion assertions and launch totals without
SHC calls. The observer packets and all rejected outputs remain preserved.

## Route decision

Do not repeat post-GEN CMN replay or the unsigned-long promotion experiment.
A new source control needs a genuine consumed conversion/binding mechanism
that changes the register-source recipe itself while retaining the unsigned
index, actual roots, and both protected neighbors. Changing the outer promotion
type alone does not do that. Switch owners until such evidence exists.

No acceptance receipt, full candidate ROM qualification, required-suite result,
cold replay, or dashboard publication is claimed for either rejected packet.
