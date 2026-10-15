# Wave stack-store ordering and setup allocation — continuation197

Production remains checkpoint192, **131,780 / 200,698 matching bytes
(65.6608436556%)**, with **68,918 bytes remaining**, including all 1,638 runtime
bytes. This investigation adds no matching C. The preceding turn made progress
by proving the split-store expression invalidation rule; this turn identifies
the independent cause of the four setup differences and the register regression
when their source order is corrected.

## The setup mismatch is constrained by SHCASM

The final scheduler is in SHCASM, downstream of SHCGEN. Read-only hardware
breakpoints observe its input buffers, dependency graphs, and selected output.
Stage inputs are captured at the assembler's process-entry boundary, avoiding
stale cleanup snapshots. No compiler instructions, input IR, flags, or output
instructions are modified.

The baseline enters setup buffer32 with the final attribute OR at node23,
phase load at24–25, j spill at26, phase conversion at27 and phase spill at28.
Although the OR precedes the spill in this input stream, scheduling moves it.
The final four relevant instructions are:

| ROM address | Baseline | Original ROM |
|---|---|---|
|28100|MOV.L R12,@(12,SP)|OR R0,R11|
|28102|EXTU.B R4,R4|EXTU.B R4,R4|
|28104|MOV.L R4,@(16,SP)|MOV.L R4,@(16,SP)|
|28106|OR R0,R11|MOV.L R12,@(12,SP)|

The baseline dependency graph has edge **26 -> 28**, preserving j-store before
phase-store. Swapping only the OR and j spill to reproduce the ROM violates
exactly that edge. These are different stack offsets, but the observed graph
still constrains their order. This is evidence about the observed stream, not a
claim that every SHC memory operation has the same dependency rule.

The ordinary C reorder `sh = phase; ang = (unsigned char)sh; j = half;` makes
phase-store node27 precede j-store node28. Its selected sequence now has the
ROM's operation order. The 16-byte mismatch in that source is instead ten setup
bytes involving R4/R5 plus the unchanged six-byte tail R5/R6 mismatch.
Moving j earlier, before the phase load, retains the wrong store dependency and
has17 differences. Neither the input stream alone nor a scheduler guess explains
these outcomes as well as the observed graphs.

## The corrective reorder exposes a middle-pass allocation change

The setup phase value is symbol92, priority2; the palette index k is symbol88,
priority3. These are already assigned R4/R5 in SHCMDL's `.reg` output, before
SHCGEN scratch selection. Fresh complete37-candidate graph models reproduce
native priorities, profit sorting, eligibility, and assigned homes in both cases.

The phase value loses exactly one conflict: j, symbol93, priority9. Its other
neighbors have identical normalized identities and priorities. SHC computes
profit as priority minus the floored average priority of conflicting candidates:

| Value | Baseline | j after phase conversion |
|---|---:|---:|
|Phase conflict count|9|8|
|Sum of conflicting priorities|262207|262198|
|Phase profit|2 - floor(262207/9) = -29132|2 - floor(262198/8) = -32772|
|k profit|-30251|-30251|
|Phase home|R4|R5|
|k home|R5|R4|

Losing a low-priority neighbor raises the average because several other neighbors
carry register-hint bonuses. Thus the phase sorts below k and receives R5. The
setup regression is not a generator scratch-cache effect. The previously modeled
tail allocation tie remains separate.

## Controls and authentication

Case: `tools/shc_probe/cases/session_wave_schedule197/`.
Five fresh native compiles retain all four nominal roots, the full538-byte
preceding context, and the1220-byte total context. All are strict negatives:

| Source | Differing target bytes | Status |
|---|---:|---|
|baseline|10|Existing source, fresh control|
|j_before_sh|17|Existing source, fresh control|
|j_after_phase|16|Existing source, fresh control|
|phase_nested|61|New nested phase assignment, j afterward|
|phase_narrow_first|61|New conversion into sh before assigning ang, j afterward|

The two new forms are byte-identical and are excluded. They compute the same
initial phase value, and sh is overwritten in the loop before its later use.
They do not supply the needed matching allocation. No retained `.kept.o` exists.

Three scheduler replays reproduce assembly text byte for byte against fresh
untraced controls and complete native driver captures. All three captures were
independently assembled and relocated, reproducing strict compile bytes.
There are864 scheduled nodes and1984 dependency edges; every node is selected
exactly once and every edge is respected. FLABEL source-line records independently
identify f2808A as function4. Two middle-pass replays reproduce both `.reg` and
`.ilb` against fresh controls and the native captures. Across all five replays,
2812 events are verified. Capture manifests, graph models, exact ROM words, raw
object relocation, source hashes, roots, and mismatch counts are authenticated
by `analyze_schedule.py` and `verify.py`.

The first independent-assembly script invocation failed because a copied helper's
`decode_reg` import was missing; the unchanged helper dependency was added and
all three assembly comparisons then passed. This was a harness packaging issue.
No full production suite was rerun for excluded diagnostics; checkpoint192's
independent clean builds and253 enabled tests remain the last production gates.

## Next action

Keep phase-store before j-store. Do not try to fix this by changing scheduling
flags, injecting barriers, or patching instructions. The next source experiment
must preserve genuine semantics while addressing SHCMDL's phase/k conflict-cost
ordering; validate its complete graph before pursuing generator scratch traces.
A successful setup must also preserve the existing loop and solve the independent
tail allocation tie. Avoid repeating the two new phase forms or treating a larger
raw difference count as production progress. The literal100% goal remains active,
with solo work and the fixed denominator unchanged.
