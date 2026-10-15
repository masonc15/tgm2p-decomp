# Palette array views and copy-loop origins

Production remains 131,780 / 200,698 verified C bytes (65.66084%). This continuation makes a genuine full-context source place the initializer at 0x2a576 while recovering the predecessor's major register homes. Neither function is exact, and nothing was integrated.

## Checkpoint206 correction

The final added assertion in frozen206/analyze.py failed. It expected hardware symbol55 in R14 and outer index57 in R7, but offset_counter_signed has R7/R14 respectively. The preceding research note's claim that all five main homes were preserved is wrong. The outer orchestration continued with hashing/synchronization, which did not prove execution of that analyzer. The strict negative results and unchanged production pins were valid.

Frozen files remain intact. Fresh session_gfx_copy207/ERRATUM.md records the correction, and audit206.py rechecks the observed allocation plus the remaining13 constant-collection and six offline permutation assertions, writing only to207. The corrected audit passed. All207 verification is run with checked process return codes before freezing.

## Source experiments

Three equivalent buffer layouts (a long/short union, an SPAL structure, and a long-array structure) produce identical compiled contexts. A hinted row pointer and reusing the hinted color accumulator as that pointer also produce identical contexts. These establish that those source names alone do not solve the graph.

Combining the union buffer with a hinted row pointer changes the graph enough to retain bufferR10, signed maskR9, red maskR8, unsigned maskR12, zeroR11, hardwareR14, objectR6, outer indexR7, colorR5, and inner indexR4. The frame has32 saved-register bytes plus68 locals.

The strongest source, union_struct_nopal.c, uses `register SPAL *dst`, assigns `dst = &pals[o->pal]`, and copies `dst->c[i] = buf.s[i]`. Removing the saved short palette variable eliminates a redundant sign-extension and using the structure view selects the target destination addressing sequence. Its54-byte setup/copy loop is byte-identical to ROM2A352..2A388 at candidate2A356..2A38C, four bytes later. This shifted internal sequence is evidence about code generation only. The complete predecessor is1186 bytes with637 differing positions, and the initializer has119 body differences at the correct actual root2A576. No bytes are claimed as newly decompiled.

Five follow-up controls distinguish pointer-to-short, pointer-to-SPAL, saved palette index, direct field use, and outer do/goto loops. The do/goto forms stop inner-loop unrolling and shrink the predecessor to770 bytes; they are not closer matches.

## Native evidence

Hardware execution breakpoints now record candidate assignment IR trees (opcode, type, source symbol and line, children, common-use and shared-use links) alongside allocation tables. Compiler output is compared against both fresh untraced replay and original native captures; REG/ILB remain byte-identical. The traces only read process memory and set hardware breakpoints.

Baseline introduces a priority6 candidate for converting array symbol59 to a short pointer (symbol-14). Hinted destination alone leaves this conversion as symbol-13 and spills it. Direct union member access removes that conversion candidate. Constant traces independently recompute stack-address weight24 from six contributions of4; the prior cast form had weight20. The destination pointer hint puts its actual web in R5. The combined graph gives the target high-register allocation rather than merely shifting the buffer to a different wrong register.

In the explicit shared-offset206 source, IR origins show the user offset being assigned from a generated induction variable; the generated variable spills while the user offset takes R13. Updating a signed offset directly removes that extra conversion but increases hardware use and swaps hardware/outer-index homes. The union form avoids this workaround.

## Verification and next step

verify.py passes for15 strict negatives (12 new,3 carried),22 unchanged-output replays/2837 events,12 allocator graphs,7 graphs with IR origins,3 constant collections, and15 independently assembled native captures. It checks actual roots, source and binary hashes, all register-home assertions, the54-byte sequence, all119 aligned initializer differences, output controls, and absent retained matching objects. audit206.py separately passes.

Next work from union_struct_nopal.c and INITIALIZER_DIFF.json on the aligned initializer119 differences, first at2A709. The preceding real C context now naturally establishes its correct address, without padding or object shifting. Preserve its boundary and major homes while inspecting initializer masks and later register lifetimes. Full clean builds and the253-test gate remain checkpoint192's production evidence; this continuation changes only excluded research artifacts.
