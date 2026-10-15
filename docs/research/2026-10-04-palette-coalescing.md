# Palette initializer: coalescing explains allocation order

Continuation 209 reduces the aligned initializer at ROM `0x2A576` from 95
byte differences to **9**, with the exact 1,504-byte length and original
function address. This is diagnostic progress, not a matching function.
Production remains **131,780 / 200,698 bytes (65.66084%)**. The original
1,186-byte predecessor remains excluded, with 637 differences; its bytes
are unchanged across all 67 controls. The matched suffix is preserved by
the best candidate. No production source or compiler was changed.

Evidence lives in `tools/shc_probe/cases/session_gfx_bind209/`. The prior
user-directed dashboard recovery was completed and yielded a current artifact;
this continuation resumes the compiler investigation rather than treating the
retrieval as decompilation credit. There is no external blocker or live job left
at checkpoint completion.

## What the compiler actually does

The earlier description of category 3 as binding groups was incomplete.
Most such entries are old variable candidates left in the linked candidate
table after coalescing. Two category-3 entries already exist before this pass,
from low-use incoming parameters; they must not be counted as new merges.

Read-only hardware breakpoints surround `SHCMDL`'s coalescing pass
(`0x414D25`, `0x414D2A`) and merge helper (`0x411010`, return `0x410FF9`).
The helper changes the incoming category from 1 to 3, clears its conflicts,
merges definitions and conflicts into the root, and assigns the root priority
`root + incoming - 2`. The entries remain in the table later partition-sorted
for allocation. `SHCMDL_COALESCE.dis` records the native instructions.

The analyzer checks every merge's identity, category transition, priority,
conflict union, and definition union. It independently maps the resulting
linked table to the allocator trace by table position, normalizing process
addresses. All eleven binding traces pass these checks.

| Source | Merges | Active variables | Inactive entries | Constants |
| --- | ---: | ---: | ---: | ---: |
| baseline / invert_r | 18 | 49 | 20 | 7 |
| manual_four_red | 21 | 49 | 23 | 7 |
| baseline_direct_search | 17 | 49 | 19 | 7 |
| predicted_536 | 18 | 49 | 20 | 7 |

The explicit four-iteration source creates additional palette-pointer copies.
Removing the local search pointer and accessing the real palette global directly
removes a separate inactive search-pointer entry. This changes the ordering of
otherwise tied mask candidates. The desired red/blue/green allocation now gives
physical registers R13/R12/R11. With just direct search, the first 692 initializer
bytes match exactly; later color registers still differ.

The offline deletion model enumerated 717 one/two-entry counterfactuals. They
were never supplied to the compiler. Its baseline prediction singled out table
index 1; ordinary source removal of the search pointer then validated that
prediction with the unchanged compiler.

## All color and mask homes recovered

Starting from an explicit loop that reuses `o` on the fourth iteration and uses
direct search, the offline allocator model enumerates all 4,096 f/t pair-order
permutations. Thirty-two predict all 27 desired homes (24 color temporaries and
three masks). Four were tested as ordinary C branch inversions. The best is
`predicted_536.c`, with nine differences. Its independent native origin trace
confirms all 27 homes, rather than relying on the hypothetical model.

The only mismatching instructions in that initializer are:

| ROM address | Target | Candidate |
| --- | --- | --- |
| 0x2A850 | 8F05 (BF/S) | 8D05 (BT/S) |
| 0x2A85C | 2241 | 2251 |
| 0x2A864 | 2251 | 2241 |
| 0x2A8A0 | 8F07 (BF/S) | 8D07 (BT/S) |
| 0x2A8AC | 6043 | 6053 |
| 0x2A8B2 | 6053 | 6043 |
| 0x2A956 | 8F05 (BF/S) | 8D05 (BT/S) |
| 0x2A962 | 2241 | 2251 |
| 0x2A96A | 2251 | 2241 |

These are three equivalent reversed branch/store sequences: red and green in
the second expanded iteration, and red in the third. Each instruction differs
in one byte. No other byte in the 1,504-byte initializer differs. The strict
prefix verifier rejects the candidate at `0x2A850`; no retained object exists.
The full suffix through `0x2AC4C` matches in this candidate.

## Closed source experiments

There are 67 strictly rejected native source controls: three carried controls
and 64 new ones. Sources and scores are listed in nine `*cases.json` manifests
and `RESULTS.json`.

- Pointer reuse subsets improve allocation but do not independently solve it.
- Two-step and named-offset pointer construction lengthen the initializer.
- Named masks fix mask ordering but leave other differences.
- Direct search removes the causal inactive entry; moving search initialization
  alone does not achieve that change.
- Four predicted pair inversions produce 9/12/12/12 differences.
- Keeping positive branches with eight pointer-reuse subsets does not match.
- Six goto/selected-temporary/comparison variants worsen code or length.
- Eight complementary-if, boolean-switch, initial-assignment, and ternary forms
  also worsen code or length (1,492–1,526 bytes; at least 507 raw differences).
- Sixteen positive-branch controls that retain a search pointer for only one of
  the two searches remain 1,504 bytes but have 102–202 differences.

Do not repeat these controls under new names. An initial source generator for
the last family assumed both searches read `.pal`; inspection showed the second
reads `.mode`. The corrected generator targets that real field. The resulting
sources all compiled and passed independent source/object authentication.

## Verification and next action

`analyze.py` and `verify.py` both ran successfully with checked return codes;
`VALIDATION.json` and their logs record that execution. The verification covers:

- 67 source hashes, object relocations, function roots, complete body scores,
  excluded predecessor equality, and absent retained objects;
- 36 output-preserving replays and 4,113 events: 21 allocator graphs, four
  origin graphs, and eleven coalescing traces;
- 21 native assembly captures independently resolved against strict contexts;
- unchanged traced/untraced/native REG and ILB outputs, the compiler hash,
  exact register allocation reconstruction, and all 27 native homes;
- the exact nine differing bytes and their instruction words.

No compiler, intermediate, physical register assignment, object, or ROM byte
was edited. No fake uses or padding were introduced. Offline models predict
source experiments only. Production's 54 pins and both ROM hashes are checked
separately on both Nuada checkouts. Existing checkpoint-192 clean-build and
253-test evidence remains the production validation; no fresh full-suite run
is claimed for these excluded diagnostics.

The next investigation is branch layout: trace where a positive versus reversed
source conditional becomes the final block order while keeping the now-known
allocation inputs visible. Compare `predicted_536`, `reuse_o_7_direct_search`,
and `neutral_o_5`; isolate one of the three sites before changing more source.
The open question is how to preserve the desired f/t candidate order while
emitting the ROM's positive-branch layout. If a source reaches zero differences,
expand the real retained prefix to include this initializer and run both full
production build/test gates. Never integrate the mismatching predecessor or
count this near-match as C progress. Literal 100% remains active.
