# Sprite expression sharing and R9 cache — continuation137

Nine consumed-source controls identify why the ordinary sprite-ID source misses SHC's block cache. Mixed signed-offset and array-index expressions raise its setup use count from2 to3 and cause a hardware-observed R9 allocation. A signed-offset candidate also retains the original main-loop register roles and108-byte frame. It remains unmatched: the original repeated R9-to-stack stores and stack-slot order are still absent.

## Cause and counterexample

Continuation134's four setup fields use `g_606006c[spr2]`. The native middle-stage dump shows one anonymous pointer assignment reused by b2a, w18, b1c and w2c. The sprite ID consequently has only two setup reads: one call argument and one array-address conversion. Hardware tracing at407DCE confirms count2; minimum3 is required.

Three signed byte-offset field expressions, while leaving b1c as an array index, produce distinct signed and unsigned address-conversion paths. Each index still derives from the same initialized signed-short ID, and multiplication by64 fits signed32. All computations feed real existing stores; no extra read, volatile qualifier, artificial assignment, padding or register hint is added.

The setup chain now contains the call argument plus two conversion reads. Three forms demonstrate this independently: per-field staged offsets, a reused named offset, and direct `(long)spr2 *64` expressions. Each reaches count3 and actual block-cache allocation407F3A with internal register10 (hardware R9). Applying the same staged expression to all four fields collapses the ID chain back to count2 and disables that cache. This negative control ties the result to distinct expression types rather than the number of source statements.

`CHAIN_EVIDENCE.json` saves the complete observed chains, parent opcodes and program points. Every reported count is checked against the number of actual target-ID members, excluding the assignment root. Baseline points171/176 correspond to the call argument and unsigned array conversion; staged-three adds a separate conversion use. The loop's single read remains below threshold.

## Source controls

| Variant | Bytes | Frame | Setup R9 cache |
| --- | ---: | ---: | --- |
| staged_three | 1718 | 104 | Yes |
| staged_four | 1748 | 108 | No |
| signed_three | 1724 | 108 | Yes |
| reused_offset_three | 1718 | 104 | Yes |
| staged_three_legacy_call | 1750 | 108 | No |
| signed_three_legacy_call | 1722 | 108 | Yes |
| staged_three_int_id | 1750 | 108 | No |
| signed_helper_three | 1756 | 108 | No |
| legacy_signed_helper_three | 1756 | 108 | No |

The original has1748 bytes and a108-byte frame. All nine candidates still begin atA604 instead ofA608 and fail the unchanged strict contextual verifier. `staged_four`'s equal length does not establish a match. Three pairs have equal native sections outside comment metadata: staged/reused offsets, staged legacy/int-ID calls, and prototyped/old-style signed-offset helpers.

The two signed-three direct-expression variants retain hold inR10, mask15 inR9, constant2 inR8 and total on the stack. Their setup cache uses R9 before the loop reuses it for mask15. The typed-call variant has spr2 at local44; the legacy-call variant has it at40. Original spr2 is at0. Neither emits any R9 writeback to its sprite-ID home.

## Call conversion evidence

With the R9 cache active, the typed short first parameter emits `mov r9,r4` in the second f253bc call's delay slot. An unspecified declaration restores `exts.w r9,r4`, matching the original delay-slot instruction atA75E. CFG-based call analysis confirms the candidate call atA75C targets actual f253bc; this is not a literal-pool disassembly artifact.

Changing only the first parameter to int is a diagnostic isolation control, not a recovered production signature. It is native-identical to the unspecified declaration for the staged-three source and also drops that source's cache count to2. The direct signed-three source retains count3 under default promotion. Call declarations can therefore alter common-subexpression sharing and allocation, not merely final argument instructions. No production interface changed.

Two inline helpers take the ID by value and compute the signed offset, one with a prototype and one with an identifier-list short parameter. They emit no standalone helper and are native-identical. Neither produces the desired R9-to-home stores. Their repeated threshold visits have counts2/1/2/1; no target-ID cache allocation occurs.

## Verification

Nine stock compiles, nine independently captured native objects,18 exact normal/debug middle replays,10 hardware-breakpoint replays (including the earlier baseline) and nine CFG stack audits pass. The final call-target assertion initially compared the in-memory tuple to a JSON-style list; that analysis-code error was corrected and its failure log retained. All intermediate records round-trip, all objects emit exactly36 real functions, all stack accesses resolve, and return-stack balance holds. Hardware tracing writes only debug registers and preserves `.reg`/`.ilb` byte for byte.

`REGISTER_ROLES.json`, `SLOT_MAPS.json`, `SPRITE_HOME_WRITES.json` and `CALL_CONVERSION.json` distinguish the recovered properties from unresolved differences. All nine sprite homes lack the original R9 writebacks. The strict rejection is retained even when individual instructions or routine length agree.

Both Nuada checkouts retain all44 checkpoint122 production pins,22 prior freezes, original ROM hashes and all23 reference-ROM hashes. Production source, split map, compiler and acceptance rules remain unchanged. The latest full production build/test gate remains checkpoint122's two clean builds and243 enabled tests.

## Next action

Use `signed_three.c` and `signed_three_legacy_call.c` as controlled inputs for the SHC code-generator trace, with continuation134 retained as the no-cache control. Follow the cached short ID through register-to-memory materialization and signed/unsigned conversion lowering. Explain why these sources use direct EXTS.W but the original repeatedly stores and reloads the short home. The recovered cache and call conversion are evidence for that next experiment, not proof of the original source or an exact match. Do not repeat these CSE/type/helper controls or pad the remaining byte/slot differences.

**New verified C bytes:0. Production remains128,316 /200,698 (63.93487%), with72,382 bytes remaining including all1,638 runtime bytes. The full goal stays active.**
