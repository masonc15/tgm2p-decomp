# Source-form experiments following the SHC scratch trace

Continuation78. Production checkpoint73 remains 125,264 / 200,698 (62.41417%).
Nine genuine source variants were compiled with the standard unmodified SHC
compiler and default verifier in the actual 2E4C–2F3C two-function context.
None is accepted. No production source or matching credit changed.

| Source change | Context bytes | Differing bytes in nominal context | Result |
| --- | ---: | ---: | --- |
| Consume `count /= 10L` inside the comparison | 240 | 15 | Original roots; following function and pools exact |
| Ordinary assignment with input/result casts | 240 | 27 | Original roots; following function and pools exact |
| Explicit long comparison | 240 | 2 | Exactly the baseline linked bytes |
| Explicit short comparison constant | 240 | 2 | Exactly the baseline linked bytes |
| Shared divisor used in threshold and division | 248 | 213 | Shifted root; boundary rejection; eight extra bytes |
| Shared maximum used in comparison and clamp | 244 | 103 | Shifted root; boundary rejection; four extra bytes |
| Ordinary assignment without casts | 240 | 27 | Identical to the cast assignment |
| Ordinary assignment with only result cast | 240 | 27 | Identical to the cast assignment |
| Ordinary assignment with only input cast | 240 | 27 | Identical to the cast assignment |

The first three representative forms have full captured stage outputs, six
exact middle-pass replays, three generator hardware traces with ordinary replay
controls, and lossless .reg decoding. The middle-pass score formula also checks
on all 35 logical records. The unchanged temporary-selector model from case77
predicts all **94 new observed selections**. Traced/control .sua/.asa agree;
.sua also agrees with original driver capture. The cleanup-snapshot .asa offset
limitation from case77 remains explicitly recorded, not treated as equality.

The plain long comparison has exactly the baseline's complete recorded scratch
input/output sequence, not merely the same final register. Adding type casts to
that comparison therefore does not reach the selector state we need.

The explicit assignment initially appears to implicate casts, but its three
ablations show otherwise: all four assignment variants produce identical linked
bytes. The middle pass splits `count` into two logical values assigned R13 and
R14. Compound assignment keeps one logical `count` in R14. This explains the
spread of differences into the loop and prologue. It also leaves the clamp's
R14-to-R2 extension, so it damages existing matches without addressing the
scratch mismatch.

Consuming the compound assignment in the condition keeps `count` in R14 but
uses the return value in R0 for comparison. It moves the bound into R5 and
changes instruction order. That result distinguishes expression-result reuse
from the separate scratch-copy problem in the baseline.

Evidence is frozen in `tools/shc_probe/cases/session_danger_source78`:
ALL_RESULTS.json holds unchanged-verifier audits; ANALYSIS.json validates the
ablations and scratch predictions; REGISTERS.json and generator traces preserve
the intermediate mechanisms. Source generation is retained in prepare.py.

Next retain compound assignment and its current logical allocation. Inspect
the comparison instruction recipe and preferred-register mask construction in
shcgen (0x420900), using the hardware trace. Further generic type or declaration
permutations are unsupported by these results. The next useful change needs to
alter the scratch preference/exclusion state while preserving earlier code.

All 27 production pins and both ROM hashes are checked again by freeze.py.
Last full integration validation remains checkpoint73: 228 enabled tests and
two independent clean builds. These rejected private experiments do not warrant
rerunning the full production suite.
