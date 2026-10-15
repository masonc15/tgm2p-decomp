# SHC effect-coordinate instruction recipes — continuation116

The code generator explains why several plausible rewrites miss the original
136-byte f_20a9c. Operand order can recover its R0 accumulator, but the tested
forms change addressing, intermediate allocation, or call setup. Eight native
controls all fail the unchanged verifier. The production baseline remains best
at26 differing target bytes. No new C bytes are claimed.

Production remains **127,988 / 200,698 (63.77144%)**, with72,710 bytes remaining.
Checkpoint115's172-byte task-allocator gain and39 production pins remain intact.

## Observed mechanism

The baseline forms `(height - 21) * 8` in R3, reads signed `dy` into R0,
then copies R3 into the allocated `sy` register R4 and adds R0 to R4.
The original instead adds R3 to R0. It also retains height in R5 while
forming the second product in R3, then subtracts that product from R0.

Grouping the subtraction into the expression removes the initial named-result
copy, but keeps the sum in R3. It copies height from R5 to R2 for the second
product, then eventually moves the result into `sy`/R4. Including the final
minus6 in that expression moves the R4 copy later, without fixing allocation.
The two grouped controls have equal difference counts but different native
bytes; they are not equivalent compiler outputs.

Commuting the initial addition inside the grouped expression produces
`add R3,R0`, as desired. The whole-expression form also subtracts from R0.
However, evaluating `dy` first occupies R0 while loading height. SHC generates
an offset load into R5, adds the field pointer, and performs an indirect byte
load. That adds an instruction compared with the original indexed byte load.
Naming height before the expression restores indexed access but places height
in R4; it still copies the finished R0 result into `sy`/R4. Its73 target-byte
differences remain worse than the26-byte baseline.

Inlining the entire calculation as the call argument changes liveness during
argument setup. The middle stage assigns the field pointer to R14, the shake
pointer to R13, and spills the reused height. GEN pushes115 and prepares the
other arguments before calculating the coordinate. Removing the named result
therefore does not simply free R4. Changing only `sy` from int to long produces
an exactly identical384-byte native context to baseline.

## Direct recipe evidence

The read-only hardware tracer observes the emitter at41B3D0 and its wrapper
at41B9D0. It identifies the generic recipe interpreter call at40CB57, whose
return address is40CB5C. At wrapper entry, preserved EBX identifies the current
six-word recipe; these words are recorded beside every applicable emission.
The original x86 listing is saved as `recipe_vm.x86`.

The inferred VM listing at40B690 includes an optional operand swap when recipe
flag2 is set and the first operand names R0. Original x86 instructions at
40CAEB–40CB02 corroborate that condition. **The flag is zero in every observed
coordinate addition.** The recovered R0 destination comes from the source's
operand order and selected two-address recipe, not from taking that swap branch.
The branch was a hypothesis investigated here, not a successful explanation by
itself. `ADDITION_RECIPES.json` asserts this distinction for all eight controls.

| Control | Sum source → destination | Context bytes | Target differences | Protected-prefix differences |
|---|---|---:|---:|---:|
| production baseline | R0 → R4 |384|26|0|
| grouped subtraction | R0 → R3 |388|79|12|
| whole expression | R0 → R3 |388|79|12|
| calculation inside call | R0 → R5 |392|127|12|
| long sy | R0 → R4 |384|26|0|
| commuted grouped subtraction | R3 → R0 |388|112|12|
| commuted whole expression | R3 → R0 |388|107|12|
| height loaded first | R3 → R0 |388|73|12|

All three function roots remain nominal. Larger contexts fail the required end
boundary and change shared literal positions in the already-retained prefix.
Difference counts cover the fixed original ranges; excess bytes are recorded
separately through context size and boundary rejection, not hidden by the count.
No rejected object is retained as production code.

## Validation and limits

`tools/shc_probe/cases/session_effect_recipe116/analyze.py` verifies eight normal
compiler/native-object comparisons,16 exact normal/debug middle-stage replays,
135 priority/profit calculations,16 hardware replays,470 scratch predictions and
335 target emitter events. Every traced GEN output equals its untraced control;
SUA also equals the native capture. Cleanup-snapshot ASA metadata differs and
is explicitly recorded, as in the earlier stage experiments. Compiler code and
data are unmodified; tracing writes CPU hardware debug registers only.

The core seven-control receipts and final height-first receipts are saved
separately. Repeated captures for unchanged controls retain multiple timestamped
intermediate families; verification uses the selected replay family and compares
the actual native object. Frozen prior cases are unchanged. New negative source
experiments did not warrant another full project build. Fresh read-only checks
verify all39 checkpoint115 production hashes, original ROM hashes, and reports
in both nuada checkouts. The240-test passing suite belongs to checkpoint115,
not a new full-suite run in this continuation.

## Next experiment boundary

Park these eight exact source forms and the earlier effect32/effect69 accessor,
assignment and permutation families. The remaining useful question is how SHC
binds a genuinely consumed height value to the reused argument register R5 while
leaving the coordinate result in a scratch register until call setup. This
requires a new, measured lifetime/coalescing hypothesis, not another spelling
of the grouped arithmetic. Parameter reuse can be investigated as a distinct
source mechanism: height is an unsigned byte, so reusing the exhausted short x
parameter for it does not narrow its value. Verify the optimized WEBs before
attempting combinations; abandon the hypothesis if it merely recreates R4.
Do not force registers, add inert uses, patch output, or weaken matching rules.
The literal100% goal remains active and incomplete. Work remains solo.
