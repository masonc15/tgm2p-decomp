# Score bonus-guard scratch cache — continuation163

The preceding turn made evidence progress by explaining the early2/65535 allocation tie. This continuation traces the separate bonus guard at7770–77A6 using stock SHCGEN and ordinary consumed-source controls. Production remains checkpoint161:128398/200698 bytes,63.9757247%,72300 remaining.

## Observed mechanism

The baseline again has19 differing bytes and two exact original function roots in1456 context bytes. Immediately after the existing calls, scratch selection55 (before emitter121) sees all fourR0–R3 cache slots empty: each value0 and flags0. Its last register isR2, recent mask0, and it choosesR3. The complete24-byte machine sequence7770–7788 already matches the ROM. This is a useful boundary: the guard reconstructs scratch cache state locally after the calls, rather than beginning with cached2 or65535 in those slots.

During planning of the compound condition, cacheR3 acquires the4096 mode mask; cacheR2 acquires a symbolic address. At selection63, before the guard's buffered emission begins, R1 is excluded, R2 andR3 are occupied, and R0 is chosen. Emitter134 then loads the threshold address intoR0;135 loads the word intoR1;137 loads the player level intoR3. These are bound to native instructions at the original ROM positions, not to aligned similarity:

| ROM interval7788–7794 | Words |
|---|---|
| Candidate |`d030 6101 9054 03ed 633d 3137`|
| Original |`d330 6231 9054 01ed 611d 3217`|

The recovered selector predicts all2080 observations across the eight traced sources, including1272 within7658. In a diagnostic snapshot counterfactual, clearing onlyR3's cached4096 value and occupied bit makes selection63 chooseR3. This is a model experiment only: no compiler input or memory is changed, and it neither reconstructs all dependent choices nor proves a matching source. The threshold value, level and later300C cap choices still need to be explained together.

## Source controls

Twenty-four full native compilations all fail the ordinary prefix verifier. No source bypasses a proof or gains credit from an interior match.

| Change | Differing target bytes | Context bytes |
|---|---:|---:|
| Baseline |19|1456|
| Global bit test as!=0, doubleNOT, or int cast |19|1456|
| Global bit test narrowed to unsigned short |265|1460|
| Mode mask narrowed to short or unsigned short |204|1456|
| Mode mask tested with!=0 |19|1456|
| Split after the two flag tests |24|1456|
| Thirteen other partial partitions of the five conditions |24–35|1456|
| Threshold expressed as the actual array elementd_38f58[4] |24|1456|
| Only reverse threshold operands top->level>=d_38f60 |24|1456|

The global!=0/doubleNOT/int and mode!=0 controls have byte-identical complete contexts to baseline. Both narrow mode casts have identical contexts to each other. The first six traced controls, through mode_word, have byte-identical `.reg` files despite the two narrowing controls' native regressions. Nested conditions change the scratch choices but do not produce the target. The previously tested fully nested five-if form is not repeated; all partial boundary combinations are now covered. The array alias is checked against actual linked symbols:38F58+4*2=38F60.

All guard terms retain their order and short-circuit behavior. Mask expressions have values0/1 or0/4096, making the tested casts value-preserving; threshold reversal compares the same promoted operands. No artificial consumers, extra game-state accesses, volatile annotations or physical register constraints are introduced. The inherited f7956 source remains unchanged, including its unresolved f17834/RNG calling-convention assumption; this context remains diagnostic.

## Verification and next action

`session_score_scratch163/verify.py` regenerates the24 sources from a SHA-256-pinned baseline, checks raw differences and actual ELF roots, compares eight native captures outside comment metadata, verifies sixteen normal/debug middle replays, and validates sixteen read-only code-generator replays. Traced `.sua` equals the original capture; traced/control `.sua` and `.asa` agree, with cleanup-snapshot `.asa` metadata differences explicitly recorded. It verifies2080 scratch predictions and2664 target emitter events. See `GUARD_CACHE_ANALYSIS.json`, `TRACE_ANALYSIS.json`, `NATIVE_ANALYSIS.json` and `VERIFICATION.json`.

Both checkouts retain46 production pins,49 prior frozen cases, original full-ROM hashes and the128398/200698 report. The latest full build/test gates remain checkpoint161's245 enabled tests and independent clean builds; no production source changed here.

Stop Boolean-width, comparison-orientation and condition-partition variants. The next direct experiment is to watch writes toR2/R3 cache occupied flags in SHCGEN while processing this guard, using hardware watchpoints and original-output equality. Identify the exact cache installation/invalidation callers and the source nodes they represent. This should explain why the global address and mode mask remain occupied at the threshold planning step, and provide a source-directed hypothesis instead of another grouping sweep. Goal active; zero new matching C bytes.
