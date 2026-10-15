# Score cache lookup and suppression — continuation166

Checkpoint161 remains **128,398 / 200,698 bytes (63.9757247%)**, with72,300 bytes remaining. Continuation165 made evidence progress by rejecting the tested bit-field load width. This continuation resolves the proposed cache-suppression mechanism and produces a small native reproduction. No C bytes are newly retained.

## Dynamic cache lookup

A new tracer watches the four-byte flag at460A78, counts low-level emissions at41B3D0, identifies functions at4269A0, and observes42B2B0 entry/return by moving a hardware execution breakpoint. It writes only hardware debug registers. The first run used a misplaced DR7 field and recorded zero writes; the run was rejected. `INITIAL_WATCH_FAILURE.txt` records that error. Corrected DR7 is0D000055, and all accepted runs use the corrected tracer.

Baseline and split-after-flags sources each produce75 observed lookups. A model of the original integer-cache search predicts every returned register orFFFF miss. It checks occupied40hex, operand value and symbol identity while scanningR0–R3. This evidence includes no successful nonnumeric lookup; the verifier refuses to model a nonzero equal-ID symbol candidate without the full recursive symbol chain. `SYMBOL_COMPARISON_DISASSEMBLY.txt` shows why checking only the first ID would be insufficient generally.

Both full contexts have exactly one hit: lookup17 reuses constant3 inR3, at emit-before93, source line80. The hit writes1 at42B3FB; the enclosing expression restores1, and later expression initialization resets0. The verifier checks270 writes across all three cases against pinned original instructions:

-413B15 resets the flag from zeroEBX.
-432076 and40FDBE restore it fromEAX.
-42B3FB writes1 on a successful lookup.

The five bonus-guard lookups are all misses: mask4096, g6064880 address, constant1, d38f60 address, and cap12300. The flag is zero at every lookup and return in both full contexts. It therefore does not explain suppressed installation in these observed guards. The original two-entry occupancy and late eviction remain the relevant obstruction.

## Native reduced reproduction

`guard_probe.c` is derived mechanically from the baseline structure/declarations and the real f1199c call, conditional bonus guard, stores and calls through f11cc6. It removes the preceding score arithmetic and following return calculation, renames the diagnostic function, and returns void. No dummy uses, synthetic effects, padding additions or forced registers are introduced. This is a compiler experiment, not a proposed replacement for the complete score function.

The standalone object is144 bytes. At the threshold lookup its R2 cache holds4096 and its R3 cache holds the global-address symbol, both occupied with age5. Its emitted threshold-address load at offset2A usesR0, followed by the threshold word load intoR1 and player-level load intoR3. The relocation at84 authenticates d38f60. Thus the same unavailable-R2/R3 obstruction and threshold-R0 choice occur in a small function without the earlier constant2/FFFF allocation tie. This does not claim that every instruction/register in the reduced guard equals the full function: its mask and global-address homes are reversed, and its prologue and epilogue differ.

The reduction's eight lookups all miss and its flag remains zero. A fresh original driver capture matches its native object in every section except comment metadata. Three traced code-generator runs equal untraced controls in both `.sua` and `.asa`, and their `.sua` files equal original driver cleanup snapshots. Cleanup `.asa` metadata differences are recorded explicitly. Every frontend/middle replay uses unchanged compiler input flags and checks the original success status.

## Verification and next action

`verify.py` passes158 lookup predictions,270 flag-write checks,15 guard misses and the144-byte reduced native capture. It verifies source derivation, original compiler instruction bytes, original captured input hashes, output equality and the reduction's threshold instruction/relocation. `LOOKUP_ANALYSIS.json` retains snapshots and the exact two numeric hits. Both production checkouts retain46 pins,52 preceding frozen cases, original ROM hashes and unchanged coverage. All23 reference-ROM files still match their recorded source manifest. Latest full production gates remain checkpoint161's245 enabled tests and independent clean builds.

Next use the reduced probe to inspect the integer-test recipe's operand lifetime marking: identify how the compiler separates a temporary mask operand from a reusable cached constant afterTST. A useful source hypothesis must preserve the original word read and TST while explaining that distinction. Do not repeat cache-suppression, bit-field, condition-partition, predicate-helper or flag-local sweeps already rejected. The goal remains active.
