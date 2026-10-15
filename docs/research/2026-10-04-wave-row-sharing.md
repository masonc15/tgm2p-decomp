# Wave row sharing and priority — continuation195

Production remains checkpoint192: 131,780 /200,698 bytes, 65.6608436556%.
All17 native controls fail the unchanged strict verifier; no candidate is retained.
Compiler, flags, source map, runtime exceptions and denominator are unchanged.

## Direct compiler evidence

Eighteen read-only, output-preserving SHCMDL replays contain7972 events: five
allocator graphs, five constructor traversals, three variable-priority traces,
three detailed weight traces and two temporary-extraction traces. Every traced
.reg/.ilb equals a fresh untraced control and the captured native output. Five
captured assembly outputs independently assemble through the existing project
inline_asm path and resolve exactly to their strict compiler contexts.

The five constructor/sort models reproduce candidate append order and the five
full allocation graphs. Four have37 candidates; row_value has38. No compiler
binary, IR, generated object or register assignment was modified.

The79 initial variable priorities and230 contributions reproduce exactly. For
row_value's symbol101, SHCMDL40FF50–40FF5C reads the representative's shared-use
count1;40FF75 adds a definition point because this is assignment0x5F in block
kind4;40FFBA doubles that2 inside the loop. The definition contributes4. The leaf
use points to that representative, is not itself a representative, starts0 and
contributes0. Thus the cost is (1+1)*2+0=4, not a multiplier of4. The existing
model then assigns the row to physicalR4, displacing the array base toR5.
Each priority trace also observes one generated candidate at2 that reaches the
sort table at8 (symbol-6). That later adjustment is recorded explicitly; the
initial-priority summation is not misrepresented as explaining that later stage.

The nested assignment preserves its row definition/use at allocator construction,
with priority4. It also preserves exactly one packed OR expression. Its eight
extra bytes are NOT a repeated packed calculation or removal of the row local.
The disassembly has one SHLL16/OR pair, two stores, but reloads the board pointer
and rebuilds the array address for the second store. It is byte-identical to the
previous genuine store_split control.

Extraction40E580 creates seven temporaries for baseline and six for row_nested.
Baseline's common array expression(op0x34,type0x90,line116) has count2 and a shared
chain and reaches creation40E783. The split-statement version has no corresponding
array-expression creation. Both packed OR expressions have count1 and never
reach temporary creation. The single packed OR also remains in each final tree.
This narrows the loss to sharing of the array address, rather than value packing.
The precise earlier invalidation rule is still unproven.

Naming the shared row base explicitly restores the original1220-byte context
length, but produces30 differences for chained stores or48 for a memory copy.
It does not solve the function. The copy form fixes the tail; the chained form
still has the original six tail differences. A separately shifted height fixes
the tail but has29 total differences. These are excluded diagnostic evidence,
not additional matching coverage.

## Native outcomes

All original function roots remain nominal. Except where stated, the preceding
538 matched context bytes remain exact. Difference counts compare the682-byte
target window; excess or missing bytes are also reported separately.

| Control | Window differences | Size difference | Context |
| --- | ---: | ---: | --- |
| baseline |10|0|exact|
| row_value |31|0|exact|
| mirror_load |442|+12|exact|
| mirror_reverse |441|+12|exact|
| chain_reverse |19|0|exact|
| n_const |14|0|exact|
| half_const |95|0|506-byte predecessor changes|
| dimensions_const |103|0|exact|
| inline_pair |455|-4|exact|
| inline_pack |611|+16|506-byte predecessor changes|
| height_copy |10|0|exact; identical to baseline|
| height_shift |29|0|exact|
| height_ulong |10|0|exact; identical to baseline|
| height_ushort |11|0|exact|
| row_nested |437|+8|exact|
| row_base |30|0|exact|
| row_base_copy |48|0|exact|

The ordinary inline C helpers did not preserve the target length. The inherited
fixed-multiply assembly primitive and historical inert context remain unchanged;
no new instruction primitive, fake use or fixed-register binding was added.
No .kept.o exists for any negative control.

## Review and next action

A preliminary narrative incorrectly attributed nested-store growth to repeated
packing. Native disassembly, the one-OR final tree and retained row candidate
correct that interpretation; all final analysis and receipts use the direct
observations. Similarly, the detailed trace refines the priority calculation
from a total weight4 to the actual (1+1)*2 construction.

Next inspect where the array-expression representative chain loses sharing across
stores, and compare that with the explicit row-base forms. Preserve the original
common array-base candidate and scratch packed value rather than adding another
retained local. The four setup scheduling bytes remain an independent problem.
Do not repeat the tested const, inline-helper, mirrored-load or height-copy forms.
Production remains protected by54 pins and checkpoint192's two clean builds and
253 enabled tests. Full production suites were not rerun for excluded diagnostics.
