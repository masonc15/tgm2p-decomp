# VM result reuse — continuation237

Keeping the row offset live through the loop condition does not change the
addition destination. SHC saves the offset in a stack byte and still treats its
working copy as disposable. The new context grows to752 bytes with a52-byte
frame and640 differing/missing bytes. The required original frame is44 bytes.
This is a rejected experiment; production remains135594/200698 (67.56121%).

The equivalent loop tests the already computed offsets0,30,60 instead of the
incremented row against3. Both execute exactly three rows. The compiler emits
MOV.B R2,@(R0,R15) before EXTS.B and ADD R1,R2, preserving the later offset use
in memory. It does not preserve the working register as the hypothesis required.

Three frozen controls are reused byte-for-byte: baseline235 and both equal-rank
operand orders236. Across all four, native hardware observations show no assigned
or requested result home for the row-address addition. Its child states remain
0 and3 through the zero/used-mask branch. Original PE recipe tables and operand
descriptors predict all four additions and confirm reuse of the state0 child.

The branch model covers92 observed additions:68 take the zero/used-mask branch;
24 already have a home. An initial expected-count assertion incorrectly classified
16 fixed-inline-register cases as constrained-mask cases. Inspection of original
instructions at41D610-41D61B confirms fixed kind1 is handled before requested masks.
The model already included that condition; its expected summary is corrected.
No constrained-mask-path internals are claimed as modeled.

Validation covers four output-preserving reuse replays (252 events), four GEN
replays (1203 emissions),92 branch choices, four recipe/operand predictions, and
one independent assembly of the new native capture. Both validation commands pass.
All four complete contexts remain strict negatives. No source/split or credited
production bytes changed. The last full gates remain checkpoint225's two clean
builds and266-test suites; they were not rerun for these rejected experiments.

Simple source-order and this specific lifetime-extension route are now closed.
A future VM attempt needs a meaningful computed base accumulator or actual result
home without extra memory traffic. Next diversify to the136-byte effect function
f_20a9c and test a distinct callee-lifetime hypothesis against its retained context.
