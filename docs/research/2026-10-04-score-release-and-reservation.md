# Score scratch release and operand reservation — continuation167

Checkpoint161 remains **128,398 / 200,698 bytes (63.9757247%)**. The preceding turn made evidence progress by isolating the guard and ruling out cache suppression. This continuation checks register release directly, tests genuine local-mask controls, and identifies an alternative selector input that warrants tracing. No new C bytes are retained.

## Cleanup preserves cached values

`trace_release.c` observes entry/return of SHCGEN42B0F0 with hardware execution breakpoints. It records the four general-register cache slots, source line, emission count, caller and release mask. The three cases are the full baseline, the144-byte guard reduction from166, and a genuine local-int-mask control.

All150 observed calls come from4065BA with mask000F000F. The verifier predicts all600 general-register snapshots from the original routine: occupied flag40hex preserves a cached value; another branch preserves entries whose kind has high bits80hex; remaining selected entries have their value and flags cleared. There are115 cached-slot preservations and zero observed slot changes. The kind80 branch and clearing of a nonempty uncached entry are not dynamically demonstrated by these cases; their behavior is only read from the pinned original instructions. The trace does not claim to model the floating-register half of the mask.

In the reduced guard,4096 survives three cleanup calls because it remains a cached value. In the full baseline it has already been evicted by the time the corresponding later cleanup snapshots occur. These are compiler planning events, not runtime observations after a SH2 instruction. This distinction prevents treating ordinary scratch cleanup as an explanation for the original threshold register.

## Consumed mask controls

Each local is initialized to4096 inside the existing `if(n)` and read by the existing mode test. Reads, short-circuit order, calls and stores are otherwise unchanged. f7956 remains the original diagnostic following context, including its inherited unresolved f17834/RNG ABI assumption.

| Native control | Differing target bytes | Context bytes |
|---|---:|---:|
| Baseline, SHC5.0r32 |19|1456|
| Unsigned-short local mask |19|1456|
| Int local mask |43|1456|
| Const-int local mask |43|1456|
| Unchanged baseline, SHC5.0r28 |19|1456|
| Unchanged baseline, SHC5.0r31 |19|1456|

All six have nominal roots7658/7956 and fail the normal prefix verifier. The unsigned-short control and both older compiler revisions reproduce the complete baseline context byte for byte. The int and const-int contexts equal each other. Compiler executables for all three versions are hash-pinned; no compiler or production version was changed.

The int mask loads4096 intoR4, outside the scratch-cache bank. A fresh traced lookup confirms that the threshold request has no cached4096 and R3 is empty. Its threshold-address/load pair at7788–778C now matches the ROM's R3/R2 pair. However, the mask load, global-address choice, player-level register and earlier allocations differ, so this is a rejected diagnostic control, not partial coverage or a fix.

Four read-only codegen replays preserve `.sua` and `.asa` against untraced controls, with `.sua` equal to the original driver cleanup snapshots. Cleanup `.asa` metadata differences remain explicitly recorded. The new int-mask native capture equals the ordinary compiled object outside comment metadata.

## A better next discriminator

The frozen163 selector snapshot at full-baseline call63 picksR0. `counterfactual.py` changes only the exclusion input by reservingR0, leaving all cache records and other selector inputs untouched; the model then selectsR3. **This is model-only evidence, not an executed compiler variant.** It shows that clearing the cached mask is not the only theoretical route to the target register.

Direct original disassembly supplies a concrete place to investigate: caller4227D3 passes an operand to42C190, ORs the returned register-dependency mask intoEDI at4227DC, then passes that exclusion mask to418900 at4227E9. The42C190 dispatch table selects an implicit-R0 dependency for operand kinds9 and12. Original instruction/table bytes and the prior selector-input hash are pinned by `verify.py`.

Next trace that operand and its returned dependency mask at the threshold-address request, together with the exclusion state before the OR. Determine which source addressing form supplies the operand and whether the original word-load sequence can requireR0 reservation. Do not force a physical register or alter compiler state. Avoid repeating cache-release, local-mask, tested version, bit-field or condition-partition sweeps.

## Production verification

The native/replay verifier passes. Both Nuada checkouts preserve46 production pins,53 prior frozen cases, the original ROM hashes and unchanged coverage; all23 reference ROM files still match their recorded source manifest. Latest full production gates remain checkpoint161's245 enabled tests and independent clean builds. Evidence is frozen and synchronized;72,300 bytes remain, including1,638 runtime assembly bytes. The goal remains active.
