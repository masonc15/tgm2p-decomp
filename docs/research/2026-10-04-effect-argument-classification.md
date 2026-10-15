# Effect argument classification — continuation239

The best f_20a9c candidate improves from21 to13 differing bytes. Its384-byte
context retains all three original function addresses and the248-byte protected
prefix exactly. It remains a strict negative; production is135594/200698
(67.56121%). No matching-byte credit is earned by a closer candidate.

## Binding mechanism and successful source prediction

Read-only SHCMDL traces separate call binding405220, incoming-parameter
binding405EE0 and general allocation406450. The baseline sy already receives
internal home1/R4 during call binding. The direct inline-helper return receives
the same home; the named helper result remains unbound and later spills.
This is not a decision first made by the general allocator.

The original argument-count helper4054D0 explains the wrong preferred position.
At its4055BB observation, the first argument d_a8bdc has type0x80 (array).
For the observed integer request it does not increment the count. The next
argument sy therefore gets count1 and prefers R4. This is an optimization-home
choice; later call setup still passes the arguments correctly.

The pre-recorded prediction changes only the equivalent first argument address:
`f_1159c((struct frame *)d_a8bdc, sy, sx, 0xba, 115)`.
The explicit cast survives as type0x40, is counted, and moves sy's call-bound
home to2/R5. It recovers the original ADD R3,R0, SUB into R0, and final result
transfer to R5 before minus6. The remaining differences are early literal-load
order and scratch-register choices for the second product and call setup.

An address-of-element-zero control has the same C pointer value but retains
internal type0x80 at this stage. It remains byte-identical to baseline at21
mismatches. This distinction is observed in the compiler, not inferred from C
source syntax. The ordinal model predicts every increment in38 observations
across13 native calls for the three representations.

## Remaining scratch issue

For the13-difference candidate's second product, the scratch cache is empty.
The last selected scratch isR3. The original selector assigns rank3 toR3 and
rank2 toR2, so it choosesR2. This differs from the recent VM problem, whereR3
was still occupied. The independently checked selector model reproduces all
177 observed choices across baseline, pointer-cast and element-address replays.

A signed-long cast around the initial arithmetic sum was tested as a potential
planning boundary. SHC int and long are both32 bits, preserving this calculation.
The complete native context is byte-identical to the13-difference control, so
that specific source form does not change the scratch planning. Do not repeat it.

## Controls and validation

| Control | Context bytes | Target differences | Protected differences |
|---|---:|---:|---:|
| reused baseline238/117 |384|21|0|
| reused helper result238 |396|126|12|
| reused helper argument238 |384|29|0|
| explicit frame pointer cast |384|13|0|
| address of frame element zero |384|21|0|
| long boundary around initial sum |384|13|0|

Both validation commands pass. The three new sources have independently assembled
native captures; all six full contexts are checked against the original ROM and
default boundary/byte gate. Four fresh ordinary/debug middle replays, three sort
replays(12 events), five binding replays(20), three ordinal replays(38), two emitter
replays(316) and three scratch replays(177 predictions) preserve required outputs.
Native SUA equals traced SUA; cleanup ASA position differences are recorded and
fresh traced/control ASA agrees. No compiler instructions or IR were patched.

The first sort observer selected the third .reg entry, which is the added inline
helper, not f_20a9c. Those observations are preserved with initial_helper labels.
The corrected observer selects the final function symbol and validates its full
WEB-address set against the named f_20a9c diagnostic records. Those corrected
traces support the binding conclusions; initial helper observations do not.

The exact13-difference source is frame_pointer_cast.c in this frozen case.
Next investigate GEN's last-scratch update between the two product-planning
requests, using this source and the original ROM sequence. The array-versus-pointer
call-binding discovery can also be tested against other incomplete drawing-call
contexts when their traces show the same skipped array argument. Avoid repeating
callee-position, element-zero or sum-cast variants without new evidence.

No source/split integration occurred. Checkpoint225's two clean builds and266-test
suites remain the last complete production gates; this continuation does not
claim a fresh full-suite run. Fixed denominator, runtime remainder and ROM identity
are unchanged; the literal100% goal remains active.
