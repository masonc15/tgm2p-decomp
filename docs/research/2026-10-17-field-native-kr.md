# Field reset: native K&R compilation and MDL state control

Verified gain: **0 bytes, +0.000000 percentage points**. Canonical linked C
remains 138,670 / 200,698 bytes (69.093862%). This corrects a research inference,
not coverage or acceptance policy.

The October13 field-reset packet tested the K&R byte-parameter definition only
through isolated frontend/MDL replays. Both normal and diagnostic MDL crashed;
no ordinary K&R driver was launched. The later DAS investigation established
that CMN must be captured at the actual stage boundary. The recorded K&R
replay also began with final ANSI driver intermediates, providing a specific
reason to check the native path once.

Packet: `research/2026-10-17-field-native-kr/`. The archived K&R full-context
source is copied byte-for-byte. It preserves the genuine85d8 translation-unit
context, function bodies, declarations, calls, and ABI operations. No source
variant, padding, unused address expression, or invented call was introduced.

## Native result and causal control

The original, unchanged eight-stage compiler driver **succeeds** on this K&R
source. A read-only exec-event observer saves files before MDL, GEN and PEP.
The native before-MDL ILA, SYM, INT and SWI files are byte-for-byte identical to
the earlier failed K&R frontend output. This rules out the earlier assumption
that those frontend trees inherently fail in the pinned compiler.

Normal and diagnostic MDL replays from the native before-MDL snapshot reproduce
native REG (4,774 bytes) and ILB (38,841 bytes) exactly. The diagnostic flag is
used only for observation; those outputs are not linked as an admission object.

The preregistered causal control supplies an identical union of both historical
and native filename sets. Input manifests prove that **only selected.cmn**
changes between the two controls:

| Selected CMN | MDL result | REG/ILB result |
| --- | --- | --- |
| Earlier failed replay CMN | SIGSEGV, exit-11 | no valid result |
| Native before-MDL CMN | expected stage exit1 | both byte-identical to native |

This establishes a stale/configuration-state failure for the earlier replay,
not lack of K&R support. It does not assert a complete interpretation of the
CMN fields. The old failed outputs remain preserved, and their historical
failure is not relabeled as a successful native compile.

Total: **12 executable stages**, comprising eight native compiler stages and
four MDL observations/controls. The native and causal questions were recorded
before execution in EXPERIMENT.json and MDL_STATE_CONTROL.json.

## Actual target and neighbor result

The K&R change does **not** recover the ROM's parameter and pointer stack
storage. Function154/f_8b86 has exactly the same final REG data as the ANSI
baseline. The byte id is logical12/R4 over expressions1..2; selected f is
logical11/R13 over2..4. The entire actual generated body8b7e..8c84 is identical
between both objects:262 bytes. This is diagnostic equality at their real
positions, not admission of the original272-byte range8b86..8c96.

| Measurement | ANSI | K&R |
| --- | --- | --- |
| Actual f_8b86 root | 8b7e | 8b7e |
| Actual f_8c96 root | 8c84 | 8c84 |
| Whole context bytes | 5524 | 5516 |
| Raw target differences,8b86..8c96 | 254 | 254 |
| Retained881a..88fe differences | 0 | 0 |
| Actual f_95b8 root | 95b8 | 95b0 |
| Retained95b8..96ac differences | 0 | 232 |

The following caller's allocation/output changes under the different function
declaration semantics, shortening the unit by eight bytes and moving later
protected functions. No nominal root is restored and no new range is accepted.
Independent GNU linking retains original object roots and complete context;
no object or instruction is patched.

Run `measure.py` followed by `verify.py` in the packet for the read-only link,
raw-byte, native-stage and input-manifest assertions. These checks launch no
SHC stage. All original and failed research remains preserved.

## Route consequence

K&R byte-formal syntax is now a verified **negative homing control**, not an
untested source rejected on an unauthenticated compiler crash. Do not repeat
that syntax or the old-CMN MDL failure. A new field-reset source hypothesis
still needs a genuine consumed storage/lifetime mechanism for id atSP0 and f
atSP4, with original roots and retained neighbors.

For subsequent compiler research, capture inputs at the actual exec boundary.
A replay failure from final driver state cannot establish failure of ordinary
source compilation. Positive replay evidence requires exact comparison to the
corresponding native stage outputs. This adds no alternative acceptance path.

No accepted receipt, full candidate ROM qualification, required-suite result,
cold replay, or dashboard publication is claimed for this rejected source.
