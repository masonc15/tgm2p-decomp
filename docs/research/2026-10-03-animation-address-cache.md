# Animation address-taken cache exclusion — continuation136

Eight source variants and direct hardware-breakpoint traces close the address-taken spr2 hypothesis for the observed SHC block-cache pass. None is an exact match. Continuation134's register-correct source remains the primary candidate; original R9-to-slot0 stores remain unexplained.

## Native source experiments

The first six variants start from continuation134 `initial_separate.c`. Address identity and byte-view forms are diagnostic controls, not proposed production hints. The inline helpers supply real consumed sprite indices or pointers. Restricting reference helpers to the visibility loop leaves all setup accesses direct, but still marks spr2 as address-taken for the whole function.

| Variant | Routine bytes | Frame bytes |
| --- | ---: | ---: |
| address_identity | 1710 | 104 |
| byte_view | 1710 | 104 |
| inline_index | 1752 | 108 |
| loop_index_only | 1756 | 112 |
| loop_ref_only | 1740 | 108 |
| loop_address_only | 1710 | 104 |
| legacy_lookup_value | 1690 | 104 |
| legacy_index_value | 1720 | 108 |

The original routine is1748 bytes with108 local-frame bytes. All eight candidates begin atA604 rather than originalA608 and fail the unchanged contextual verifier. Matching an end address or body length does not count. The three address-identity variants emit identical native text. `loop_ref_only` places spr2 at slot0 but does not emit the original R9 writebacks.

The final two controls use identifier-list inline helpers with a `short id;` value parameter. They preserve value flow without taking spr2's address. The pointer-returning helper is native-identical to continuation135's prototyped `inline_value_all`. The short-returning helper remains mismatched; no favorable inference about old-style argument conversion is supported.

## Compiler evidence

All addresses below are in the pinned stock `shcmdl.exe`, not the game. `compiler_evidence/COMPILER_PIN.json` pins the executable; `.dis` files preserve exact disassembly. Copied inferred C is explicitly decompiler output, not original compiler source.

1. At414ED4, the ordinary leaf route tests flags with mask0x0e. Baseline spr2 has flags0 and enters ordinary WEB construction. Both `inline_ref_all` and `loop_ref_only` have flags4 on all eight observed spr2 nodes and take the memory route at414EEC.
2. The storage-class hypothesis was false. All three variants report storage class5. The machine code at41600F accepts this class without the class7/8 flag restriction. Each reference variant reaches memory-group acceptance at416063 eight times, with matching returns.
3. Candidate-list construction is a separate operation. FUN4155C0 can inspect an assignment root even when its ID child took the memory route. At415781 it tests that ID's flags with mask0x0e. The baseline reaches this gate twice with flags0 and reaches insertion415899 twice. Both reference variants reach it once with flags4 and never insert spr2.
4. A snapshot of every block's candidate list at entry to407BB0 independently confirms two spr2 entries for the baseline and zero for both reference variants. This is not caused merely by a shortage of free registers.
5. The baseline's two entries reach the later cache-use threshold407DCE with counts2 and1, below the required3. The setup candidate is considered for internal register10 (hardware R9). Reference variants never reach this threshold for spr2, so adding direct uses cannot fix their earlier address-flag exclusion.

These findings distinguish ordinary allocation, memory grouping, candidate-list insertion and final block-cache allocation. The existence of a memory group does not imply that its value is eligible for this later register cache. The original stores must be explained through a different source/dataflow shape or another compiler path; do not patch the compiler or clear the flag.

## Verification

Eight stock compiles and eight independently captured native objects agree outside comment metadata. Sixteen normal/debug middle-stage replays reproduce `.reg` and `.ilb`; all intermediate records round-trip, and all objects emit exactly36 functions. Eight CFG stack audits resolve every access, preserve return-stack balance, and agree with serialized spill frames.

Twenty-three hardware-breakpoint replays preserve both native stage outputs byte for byte. They write only debug registers and read compiler memory. `GATE_EVIDENCE.json` contains the positive/negative gate assertions; `ALL_HARDWARE_RESULTS.json` binds each replay to source and compiler hashes. `SPRITE_HOME_WRITES.json` records actual stack-home stores.

The diagnostic work encountered a working-directory mistake before file creation, two C format/spacing compile errors, and verifier count/indentation errors. These were orchestration/analysis-code failures; no production gate was relaxed. Available failed logs are retained alongside the corrected passing verification. Helper counts distinguish the pre-existing inline pragma from newly added helper records.

Both Nuada checkouts retain all44 checkpoint122 production pins,21 previous frozen cases, original ROM hashes and all23 reference-ROM hashes. No production source or split changed. The latest full production build/test gate remains checkpoint122's two clean builds and243 enabled tests.

## Next action

Retain continuation134 as the primary source. Follow its setup candidate through expression/common-subexpression construction to explain why several genuine sprite uses become only two block-cache uses, then test a consumed source expression that changes that chain without taking spr2's address. Original setup needs R9 as a temporary before the main loop reuses R9 for mask15. Do not repeat address-only forms, reference helpers or the old-style pointer-returning helper already ruled out here. Short-returning helper results are a negative control, not an improved baseline.

**New verified C bytes:0. Production remains128,316 /200,698 (63.93487%);72,382 bytes remain, including all1,638 runtime bytes. The goal remains active.**
