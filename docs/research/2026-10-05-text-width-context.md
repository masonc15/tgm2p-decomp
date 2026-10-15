# Minimal text-width context — checkpoint244

No production gain. Existing 135,594 / 200,698 matched bytes and all64 production
pins are preserved. Checkpoint243 was fully mirrored and verified before this work.

The small-source archive scan found no existing width-helper context using a
function-pointer callee. Prior broad contexts already document the incorrect
BF/S schedule and missing predecessor literal; those sweeps were not repeated.

The control reconstructs E39C width accumulation and the two genuine palette
uploads in E3B8 using the actual unsigned-char, unsigned-char, long-pointer ABI.
The experiment stores the real f2af5e callee once and uses it twice. This adds
no calls, fake operations, manually placed data or padding. It changes genuine
register save/restore code in the excluded following function.

| Source | Context bytes | E39C nominal differences | Width literal |
|---|---:|---:|---|
| direct calls |64|7|E3D0|
| cached callee |68|6|E3D4|
| cached callee plus explicit do-loop |68|16|E3D4|
| cached callee plus carried character |72|22|E3D8|

Cached callee has both original function roots; its first18 and last4 E39C bytes
and actual four-byte width literal match ROM. Only E3AE–E3B4 differs: original
ADD/TST/BF becomes TST/BF-S with ADD in the delay slot. The following function is
not matched or eligible. The carried-character followup also shifts E3B8 by2.

All four contexts were compiled independently on Mac and Nuada with the original
SHC and existing production flags. Independent relocation gives identical bytes
and roots on both hosts. The unchanged prefix validator rejects every candidate.
No downstream full dependency proof is claimed merely from correcting one literal.
The verifier authenticates source provenance, all64 pins, objects, roots, literal,
exact difference counts and rejection. Full suite was not rerun for rejected
research-only changes; checkpoint225 retains two266-test validations.

Stop spelling probes here. Further work on this helper requires a specific new
explanation of the scheduler decision, or should give way to another target.
