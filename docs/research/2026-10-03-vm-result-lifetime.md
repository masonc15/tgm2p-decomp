# F200 result conversions and lifetime separation

Continuation159 applies the cast/copy investigation to the146-byte F200–F292
VM handler. The genuine23-function F200–F5CC context from case98 is pinned by
source SHA256 e55a6b158e637e6e5cd2e4b3f17763e123ad25fa7a35906fd32925e788891d56.
All experiments use the production compiler and flags. No production source,
split, verifier, target ROM or denominator changes.

## Fresh baseline and bounded controls

The original five-byte mismatch reproduces: sprite resultR6/zeroR5 instead of
R5/R6. Both retained ranges F292–F29A and F4C6–F5CC pass their existing checks.

| Source | F200 body bytes | Differing nominal bytes | Both retained ranges pass |
| --- | ---: | ---: | --- |
| baseline | 146 | 5 | yes |
| long_result | 144 | 31 | no |
| ushort_result, signed cast at use | 146 | 5 | yes |
| ulong_result, signed short cast at use | 144 | 31 | no |
| conditional_result | 148 | 108 | no |
| reversed_branch | 146 | 69 | yes |
| split_long | 144 | 33 | no |
| split_ushort | 146 | 10 | yes |
| split_destination | 146 | 10 | yes |
| chained_base | 146 | 47 | yes |
| chained_split | 146 | 54 | yes |

Counts compare original nominal addresses; shifted roots remain failures.
The unsigned-short full context is byte-identical to baseline. Both wide-result
contexts are byte-identical to each other. For the first four controls the
entire normalized allocation graph and serialized lifetimes are equal, except
for spr's serialized type. Widening changes final conversion/move generation,
not that allocation graph; it removes two bytes and displaces later entries.

## A real graph change

A conditional expression gives spr only the address/zero conflicts, removing
v0 and v3 from its conflict list. However v0 loses call-argument preassignment
and goes toR14, introducing a saved register and disturbing the whole function.
Reversing the statement branches retains the original conservative conflicts.

The consumed split form preserves the earlier code:

```c
short spr;
unsigned short result;
/* Original calls and branch structure assign result. */
spr = (short)result;
g_606006c[spr].x18 = 0;
g_606006c[spr].x22 = 0;
```

The return-value record has priority3 and conflicts with v0/v3. The signed index
has priority2 and conflicts only with the final common address and zero.
Both return value and signed index receiveR5 without overlapping each other.
The first difference is nowF274: zeroR4/addressR6 instead of zeroR6/addressR4.
All earlier instructions and all23 nominal roots remain exact; the function is
still rejected for its ten incorrect bytes.

An explicit consumed destination pointer winsR4, but removes the index/address
conflict and allows zero to takeR5. Zero materialization also moves later. This
is another ten-byte failure, not an equivalent native result. Chaining the two
real field assignments preserves C store order but changes the graph again and
regresses to47/54 differences. These tested forms are closed.

## Direct allocator verification

Three hardware traces reproduce the stock compiler outputs exactly and record
41 candidates and89 eligibility calls. The model reconstructs every score,
sort position and physical allocation. Pointers are joined only within each
compiler invocation. On the new14-candidate split_ushort graph, all512 ordinary
register-keyword combinations over nine existing locals fail to retain every
earlier assignment while placing spr/resultR5, addressR4 and zeroR6. This model
holds interference, preassignment and eligibility fixed; it is not a proof
about all source forms. No numeric compiler state was changed.

Verification includes11 native/capture comparisons,22 exact middle replays,
145 independently recomputed scores and the three allocator traces. Source
reproduction and all raw original-position comparisons pass their audit.
Every F200 control still fails the unmodified strict prefix checker.

The initial model assertion used empty anonymous names as labels; symbolic IDs
now distinguish those records. INITIAL_LABEL_ASSERTION.log preserves the failure.
One chained compile was launched before its transfer completed and failed with
a missing source; INITIAL_TRANSFER_RACE.log preserves it. The transfer was
confirmed terminal before the successful rerun. Neither failed run is treated
as compilation evidence.

Both Nuada checkouts retain44 production pins and44 prior frozen cases, original
ROM hashes and128316/200698 matched bytes. The last full validation remains
checkpoint122's independent clean builds and243 enabled tests. No full suite
was rerun for these rejected private controls.72382 bytes remain; the literal
100% goal is active. VERIFICATION.json and PRODUCTION_RECHECK.json hold receipts.

## Next action

Keep split_ushort as a genuine new lifetime baseline. Its change fixes the
sprite result without disturbing the preceding instruction sequence. Avoid
more result-width, branch-order, chained-store or fixed-graph hint sweeps.
Next test a mixed named/direct destination: compute a consumed pointer for the
first real field store and retain signed indexing for the second. Observe
whether common-address recognition preserves the three-value interference
while changing address creation or priority. Compare complete native sections
and the retained contexts before treating a local allocation improvement as
progress toward a source match. If this collapses to an already tested graph,
close it immediately rather than tuning the same priorities again.
