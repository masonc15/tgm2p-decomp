# SHCMDL's row-offset cache: creator identified and suppressed

Checkpoint84 remains **127,276 / 200,698 bytes (63.41668%)**. The five source
controls in this round fail full matching and add no production bytes. The
compiler investigation establishes which pass creates the unwanted row cache,
and one genuine source rewrite suppresses it at the cost of other allocations.

## Direct observation of the creating pass

The frozen split_both source from88 has no synthetic row-offset assignment at
SHCMDL optimizer entry40F8B0. The assignment is absent immediately before the
call40FA16 and present immediately after40FA1B. That call enters41C100 for the
second time, with the extraction mode enabled by global4590B4=1 in the original
pipeline. Earlier observations isolate it from adjacent analysis/cleanup calls.

The41C100 walker calls41C160/41C180. In41C180, eligible invariant children of
non-invariant expressions are copied to synthetic temporaries. The observed row
expression is unsigned-long cast(left shift(bitwise-and(promoted px,15),7)).
Its value is the byte row offset, not the quotient or the clipping bound.

Hardware observations at41C471/41C476 show40AC50 creating symbol−3. At41C4D0,
its assignment exists but is not yet reachable from the function root; after
41C580 returns at41C4D5, the assignment is reachable outside the column loop.
The same trace observes the separate record-index multiplication temporary−2.
Both constructors and both insertions are checked structurally, not inferred
from final assembly alone. Negative symbols remain scoped to their function
and stage; a later symbol−3 in a different source is not necessarily this cache.

Five original-binary hardware replays reproduce captured.reg/.ilb exactly.
The24 snapshots contain7,689 node observations. No compiler input/configuration,
instruction or memory was patched. Hardware debug registers provide the stops.
The compiler itself writes its common file during normal execution; input and
output copies and its changed offsets are preserved. An initial tracer build
needed a brace correction, and an initial post-run common-file equality check
was too strict; neither failure is counted among the five successful replays.

## Source experiments from the recovered rule

Four controls replace row shifts with multiplication by the actual byte or
longword stride, on both existing decode forms. They are **byte-identical to
their corresponding prior complete native contexts**. My initial hypothesis
that multiplication might bypass this pass was false: the recovered operation
names identify multiplication as opcode44, which the invariant walker handles.
These exact forms are closed; do not repeat them.

A fifth control stages the real address calculation inside the clipping block:

```c
long offset = px;
offset &= 15;
offset <<= 7;
*(long *)((char *)dest + offset + ((py & 31) << 2)) =
    ((unsigned long)it->palette << 24) | tile;
```

This preserves the same nonnegative mask/scale and final typed store. It adds
no dummy consumer, read, padding or forced physical register. The actual staged
updates keep this value local to the column iteration for invariant analysis.
The second41C100 call creates only the record-index cache; the row cache is
absent, as verified by a separate unchanged-output hardware replay.

The frame shrinks36→28 bytes, but the source still mismatches442 positioned
target bytes and the following dispatch begins27694 rather than2769E. The
new offset has priority16/profit6; px falls to priority9 and shares physicalR7
with offset in the serialized allocation. yy moves toR13, record toR10, and
other roles change. This is evidence that the cache can be suppressed through
real source, not evidence of a matching implementation. The original requires
a32-byte frame; smaller is not inherently better.

| Source | Frame | Dispatch | Target differences |
|---|---:|---|---:|
| Split decode, multiply byte stride |36|2769E|330|
| Split decode, multiply word stride |36|2769C|395|
| Compound decode, multiply byte stride |36|2769C|416|
| Compound decode, multiply word stride |36|2769A|421|
| Split decode, staged row offset |28|27694|442|

## Evidence and next action

All five generated sources reproduce exactly and preserve432 preceding bytes.
Five independent native captures equal their original compile sections outside
compiler comments. Ten normal/debug middle-stage replays preserve.reg/.ilb, and
146 WEB CHAIN score checks pass. Source and trace analyses are separate:
analyze_named.py validates source controls; analyze_trace.py validates the
original stage boundaries, constructors, insertion reachability, unchanged
outputs, four context identities, and the staged control's missing row cache.
The capture/replay summary JSON describes the last staged-source batch; all five
per-source captures and replay outputs are checked by analyze_named.py.

Evidence: tools/shc_probe/cases/session_tile_hoist89. The relevant inferred C,
original x86 calls, complete tree snapshots, native objects and reproducible
runners are retained. The original split_both candidate still provides the
better byte alignment and expected core register roles. Next inspect whether
a consumed staged address can preserve the original row lifetime and priority
without reintroducing the cache. Use the source/IR evidence to bound that test;
do not repeat multiplication/shift spellings or continue tracing an already
identified creation pass. No full-game semantic proof is claimed here.

Both production checkouts freshly pass33 checkpoint84 pins, original ROM hashes
and reports. The234-test suite and two clean builds remain historical integration
evidence from84. No production build was needed for these rejected private
controls. The full100% goal remains active;73,422 bytes are incomplete.
