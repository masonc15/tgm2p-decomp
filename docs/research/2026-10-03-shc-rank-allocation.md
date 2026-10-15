# Clean rank-renderer baseline and measured pointer allocation

Verified coverage remains **127,276 / 200,698 bytes (63.41668%)**. This round
adds no production bytes. It closes three tile-address controls and establishes
a reproducible, genuine-source rank renderer for the next compiler experiment.

## Tile lifetime controls

All three forms calculate the same consumed destination address inside the
clipping block. They introduce no dummy consumers or synthetic padding.

| Source | Frame bytes | Following entry | Positioned differing bytes |
|---|---:|---|---:|
| Split decode, copy then mask | 28 | 27690 | 412 |
| Split decode, mask then scale | 36 | 276A2 | 404 |
| Compound decode, copy then mask | 24 | 27688 | 453 |

All preserve the 432 preceding bytes and fail default full-function verification.
Their reachable instruction counts are 220, 229 and 216 respectively, versus
227 in the original. Three native captures match their compile sections except
comments; six middle-stage replays preserve .reg/.ilb exactly; 89 WEB score
checks pass. These staging forms are closed. Smaller frames alone are not a
matching improvement.

## Rank renderer: fresh provenance and context limitation

The selected target is f12AD4, 1,236 bytes through 12FA8. Historical near-matches
with false-write dependencies are excluded. Four inputs regenerate from the
saved historical source, with every compiled definition free of its historical
constant-false statements. The historical input remains evidence only.

A target-only translation unit and one retaining its genuine helper both fail
layout and byte checks. The full genuine context removes eleven constant-false
statements from preceding f12608. That function compiles to 1,100 rather than
1,104 bytes. Consequently the rank renderer starts at **12AD0**, four bytes
early, and the next function starts at 12FA4. Its actual extent is 1,236 bytes.

Comparing at the respective function starts gives 32 diagnostic byte differences
and reproduces the historical cleaned target body exactly. This comparison is
not coverage: default verification rejects the real address mismatch and 1,166
positioned byte differences. The preceding layout must be repaired through
genuine source before any target can be accepted.

## Source experiment from the allocator evidence

In the first mismatching loop, the original uses p/R12 and q/R10. The clean
candidate has p/R10 and q/R12. SHCMDL reports both pointers at priority 13,
profit -10. The ordinary pointer-assignment form that helped an earlier target
provides a bounded control here:

```c
/* existing loop increment */
i++, p = p + 1, q++, y += 22
```

Only the spelling of p's existing increment changes. Its measured priority rises
13 to 19 and profit -10 to -4. It receives R13, while q receives the required
R10. This overshoots p's required R12: the aligned diagnostic differences rise
32 to 57, with 1,167 positioned differences. Both actual function boundaries
remain four bytes early. The experiment is rejected, with no credited bytes.

The two captured native compiles match sections except comments. Four normal
and diagnostic middle-stage replays reproduce .reg/.ilb exactly. All 42 WEB
profit calculations agree with the recovered formula. Symbol aliases identify
p/q/y; repeated i/j lifetimes remain ambiguous rather than being joined by
unstable logical-register numbers. Capture/replay summary files describe the
last batch; analyze.py checks both per-source sets independently.

## Next experiment and validation

Trace the original profit-sort input/output for full_clean's tied p/q candidates,
using the recovered sort model from session_tile_sort87 and this function's
actual symbol ID. Establish the ordering cause before another source control.
The second mismatching loop and the genuine four-byte prefix deficit remain
separate requirements. Do not restore dead statements, add padding, relocate
comparison bytes for credit, or repeat the closed pointer-assignment control.

Evidence is frozen in session_tile_lifetime90 and session_rank_compiler90 under
tools/shc_probe/cases. prepare.py and the analyzers reproduce their input and
measurement checks. Both production checkouts retain all 33 checkpoint84 pins,
original ROM hashes, and unchanged reports. The 234-test clean-build validation
belongs to checkpoint84; no production change required a new full suite here.
The fixed-denominator 100% goal remains active, with 73,422 bytes remaining.
