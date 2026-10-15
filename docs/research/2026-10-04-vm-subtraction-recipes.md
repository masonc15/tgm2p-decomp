# VM subtraction recipes — checkpoint 231

Production remains checkpoint 225: **135,594 / 200,698 bytes (67.56121%)**.
All thirteen compiled contexts fail the original-address EC0C–EF04 gate.
No new C credit; 65,104 bytes remain, including 1,638 runtime bytes.

## Direct compiler result

Read-only hardware breakpoints observe SHC GEN at 0x4269A0, 0x41B3D0 and
0x41B9D0 using the existing authenticated emitter observer. In twelve captures,
3,566 emitter events reproduce fresh untraced SUA/ASA output exactly. SUA also
matches the normal-driver capture. Cleanup ASA metadata differences are recorded
and independently checked; full equality with that saved metadata is not claimed.
The compiler, intermediate inputs, object bytes, flags and ROM are unmodified.

The baseline from checkpoint 230 uses R9 for row*29, then GEN emits 62 into R3,
SUB R9,R3, MOV R3,R9. This happens before peephole optimization/scheduling.
The subtraction recipe is [99,0,170,94,250,0], observed through the generic
recipe interpreter at return address 0x40CB5C. Its flag word is zero. Changing
the source to separate the product changes its destination operands but does
not select a different subtraction recipe. The original ROM instead has 62
in R9 and the product in R3.

## Allocation explains why the obvious rewrite fails

| Source form | y priority | Native result |
| --- | ---: | --- |
| baseline y=row; y*=29; y=62-y | 20 | y in R9; extra result copy |
| expanded product assignment | 22 | y in R9; same arithmetic sequence |
| one expression with short product | 14 | y spilled at SP+8 |
| y=62; y-=short product | 16 | y spilled at SP+8 |
| separate named height | 16 | y spilled; product allocated R4 |
| expanded seeded subtraction or addition of negative product | 18 | y still spilled |

The width table has priority20 and the drawing-function address has priority19.
When y drops below them, both addresses take saved registers and y spills.
In seed_subtract, native assembly loads f_2bf58 into R8 and the width table into
R9. Thus merely repairing the subtraction operand order changes allocation and
call setup elsewhere. The expanded seeded forms raise y from16 to18 but produce
exactly the same native bytes as compound seeded subtraction. This is a measured
priority boundary, not a proposed register override.

Changing y to int/long does not prevent the spill and adds signed-word extension
of the product. Unsigned short also spills and produces the same machine bytes
as signed short seeded subtraction. int and long seeded controls are identical;
the two short-product expression casts are identical. Named-height and
seed-before-height controls have equal difference counts but different bytes.

The baseline and expanded-product variants retain a44-byte frame, but the latter
changes the letter-slot order. The separate-product variants have48-byte frames.
Contexts range740–756 bytes against the required760; every original-address
whole-context rejection is independently recomputed. No normalized score or
shifted comparison admits code.

## Validation

Six validation commands pass. Ten positive/negative allocation models, complete
layout models and frequency/home models cover108 home records. Thirty unchanged
middle-stage replays cover6,047 events (graph1,201; layout4,708; homes138).
Twelve native assembly captures independently resolve to their strict compiler
objects. Twelve GEN replays add3,566 events. Source hashes, compiler hashes,
control equality, native SUA equality, cleanup metadata differences, source
provenance and complete-context strict rejections are checked.

No production source changed, so full suites were not rerun. Checkpoint225's
64 pins, clean ROM builds and two266-test runs remain the production evidence.

## Next useful experiment

The constraint is now joint: y must have a separate product lifetime while
outranking the priority19 drawing-function address, and keep the original stack
and saved-register roles. Examine genuine call/lifetime structure and native
priority contributions around the four in-loop calls versus three tail calls.
Do not repeat these13 arithmetic/type spellings, force registers, add dummy
uses, patch the compiler or output, or weaken acceptance. The original literal
base/order and shared color-path differences also remain unresolved.

Case: tools/shc_probe/cases/session_vm_subtract231. Goal active; solo.
