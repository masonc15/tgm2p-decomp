# Controller initialization and tied allocation — continuation181

Production remains checkpoint179: 130,324/200,698 bytes (64.9353755394%).
All 50 production pins, both ROM hashes and both checkout reports remain exact.
No new C bytes. The full 100% goal remains active. Work is solo.

## Native evidence

Sixteen new source controls all reject the unchanged strict verifier. Each has
4392 context bytes, all 13 nominal roots, and all 1236 protected rank-display
bytes exact. Five initial type/prototype/long-seed controls reproduce the prior
35-difference best. Signed/unsigned char and unsigned-short seeds also yield35.
Separately copying flag to timer yields510; timer to flag170. A shared signed
short seed yields153, and assigning timer before flag reduces this to146.
The short-prototype variant of that ordering also yields146.

The genuine consumed source block is:

```c
{ short initial = 0; done = t = initial; f_2164e(initial); }
```

Its complete opening36 bytes at12FA8–12FCC match ROM, including zero inR4,
copies toR0/R8 and the flag store in the call delay slot. Both input-pointer
stack homes match: input0 atSP+4, input1 atSP+0. Their two initial stores have
moved from12FDA/12FDC to12FDC/12FE0, so this is a home match, not positioned-byte
credit. All four later pointer loads match at their original addresses.

Remaining code differs: the frame-address constant and134 swap saved registers;
134 is synthesized as155 minus21. Other threshold temporaries still differ.
This candidate is worse overall than the prior35-difference candidate and is
not integrated. No artificial consumers, register directives or padding used.
The loader ignores incoming argument registers; original C arity remains unknown.

## Compiler explanation

Four SHCGEN replays capture554 events, each with a fresh untraced control.
SUA/ASA equal their controls. SUA also equals the native cleanup snapshot;
ASA differs from that older snapshot because symbol positions have advanced,
which is recorded explicitly and is not treated as native snapshot equality.

The chained constant initializer plans timer inR8. Its literal's preference is
R0, but emission directly loadsR8 and stores the flag. A separate call-argument
zero plansR4; later native output shares the zero through a copy. Preference
and emitted destination are distinct; the complete later optimizer is not yet
reconstructed. Moving initialization after the call creates two zero loads.

Two SHCMDL replays preserve REG/ILB against both native outputs and fresh
controls. The read-only model reproduces all profits, sort order and positive
allocations for32 and33 candidates. The additional real short initializer has
priority2 and lives briefly inR4. Its presence changes tied partition ordering:
frame0xA767C and134 (both priority12/profit5) exchangeR10/R11, while input-pointer
order becomes the desired order. This is evidence for the next experiment,
not permission to alter backend state or inject fake uses.

## Next action

Preserve the35-difference candidate and the initializer-exact146-difference
candidate. Investigate genuine drawing-expression relationships that restore
constant134/frame ordering with the consumed short initializer. Inspect the
native partition inputs before changing source; do not repeat the recorded
seed widths, prototypes or assignment-order controls. A valid result must match
all576 bytes, roots, literals and the protected renderer under the same verifier.

Evidence: tools/shc_probe/cases/session_controller_zero181/ including verify.py,
RESULTS.json, INITIAL_REPLAY_RESULTS.json, GRAPH_MODEL_RESULTS.json and FREEZE.json.

The trilogy copy was separately rechecked at10:42UTC:23 files/61,822,311 bytes
match the original source manifest locally and onNuada, all52 payload hashes
and448 shape bytes verify. MacBookSSH timed out on this recheck; last successful
source comparison was09:57UTC. These references add no verified C bytes.
