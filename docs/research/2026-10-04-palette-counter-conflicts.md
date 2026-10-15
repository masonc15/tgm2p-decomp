# Palette counter conflicts and reduction tools

Checkpoint 204 is excluded diagnostic evidence. Verified production remains
131,780 / 200,698 C bytes (65.66084%); all 68,918 remaining bytes, including
1,638 runtime bytes, remain incomplete.

## Native evidence

The case `tools/shc_probe/cases/session_gfx_counter204` authenticates 13 native
strict negatives: 11 new controls and two carried baselines. There are 21
output-preserving SHCMDL replays, 5,329 events, 13 complete allocator graph
checks, 13 independently assembled native captures and 72 independently
recomputed counter/color priorities. None fixes the actual initializer root.
No production source, compiler, object, matching rule or denominator changed.

The early unsigned-short loop counter has priority 65,558 and 20 neighbors
whose priority sum is 262,667. SHC subtracts the integer neighbor average,
13,133, giving profit 52,425. The color accumulator has lower priority 65,547
but only 16 neighbors, sum 197,098, average 12,318, and profit 53,229. Thus the
color wins R4 and the counter gets R5. Comparing priorities alone gave the
wrong explanation: the conflict costs determine the order.

Pairing each red and green update with its own packed expression lowers the
color weight from 12 to 9 and removes a generated-pointer conflict of weight
6. With 15 neighbors its average cost rises to 13,139, dropping profit to
52,405. The counter now wins R4 and the color gets R5. The object and outer
counter also occupy the desired R6/R7. The unsigned buffer base still spills,
and instruction ordering and higher registers still disagree with the ROM.
Each comma expression sequences its own update; C does not guarantee an
order between the operands of the combining OR expression.

The blue-in-store control is a useful negative: it also lowers the color
weight to 9 but retains the sixteenth neighbor. Its profit remains 53,226,
so it still wins R4. The missing conflict, rather than just the reduced
counted uses, explains the paired control. Updating all channels before
packing removes three low-weight conflicts and also recovers the four main
homes, but its generated instruction order differs. Changing color signedness
produces identical bytes. Do not repeat these exact forms.

The best initializer body difference in this case remains 119, with a wrong
entry point. The green-blue-in-store candidate has an apparently attractive
220 nominal-offset differences but actually starts four bytes late and has
1,218 differences when aligned at its real root. It remains rejected.

## Tooling audit and installation

The user explicitly authorized downloading and installing tools that speed
this work. Ghidra 12.1.4, ReVa 7.3.1, an SH-2-adapted decomp-permuter,
asm-differ, objdiff, pypcode and local decomp.me already exist on nuada.
The released Ghidra installation requires Java 21, already installed; the
master branch's newer Java requirement is not a reason to replace it.

Installed C-Vise 2.10.0 and GDB 15.1 from nuada's configured Ubuntu repositories.
GDB successfully disassembled SHCMDL's actual candidate sort call and priority
routine. The compiler SHA-256 remains unchanged. Installation log, version
receipt and disassembly are under the case's `tooling/` directory.

The C-Vise test compiles both an interleaved source and a mechanically paired
red/green source with the actual SHC flags. Its predicate preserves the first
four counter increments in R5 versus R4 and the first eight color OR results
in R4 versus R5. It requires both original update/pack/store sequences and the
counter/color register declarations. It does not accept a crash or compiler
failure as interesting. The native compiler runs independently in temporary
directories, with four reducer workers and a bounded run.

This is a compile-time diagnostic predicate, not a program-equivalence proof.
The reducer may remove initialization, alter control flow or retain undefined
runtime behavior. Its output must never enter production. The native controls
and unchanged-output traces above remain the evidence for the real routine.
See `tooling/reduction/REDUCTION_RESULTS.json` for the measured reduction and
independent final predicate check. The four-minute run reduced 2,828 bytes to
1,094 bytes (61.3154%); a fresh pair of native compiles passed in 2.27 seconds.
No minimum-size claim is made.

C-Vise documentation: https://github.com/marxin/cvise/blob/master/README.md
Installation documentation: https://github.com/marxin/cvise/blob/master/INSTALL.md
Ghidra release: https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_12.1.4_build
Permuter upstream: https://github.com/simonlindholm/decomp-permuter
The upstream permuter's architecture list does not include SH-2; this project
uses its existing local adaptation.

## Next useful experiment

Use the reduced contrast to isolate the pointer lifetime/conflict mechanism,
then validate any resulting source hypothesis against the full native control.
Compare admission and allocation of the hardware address and actual buffer
base in the interleaved and paired controls. Correct main variable homes alone
are insufficient; preserve original instruction order, high homes, stack size,
actual function boundaries, literals and the existing matched suffix.
