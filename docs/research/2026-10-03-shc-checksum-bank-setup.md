# SHC bank-count allocation and emission

Continuation108 investigates the remaining setup discrepancy in the genuine
30880–30D40 checksum context, preserving107's correct accumulator expression.
The original ROM, compiler executables, production source, split map and default
prefix verifier are unchanged. Work remains solo, without brainstorming.

## Finding

The original ROM shifts the flags left in R1, copies R1 into R4 at30928, shifts
right in R4, and stores the resulting count on the stack. The current source
performs all seven shifts in R2 and then stores R2. Hardware tracing proves the
copy is already absent at SHCGEN's emitter41B3D0; it remains absent at SHCPEP's
writer414330 and in the final native object. Scheduling changes alone therefore
cannot repair this specific discrepancy.

Splitting the calculation into assignments changes SHCMDL allocation before
code generation. The variable-priority routine40FF00 accumulates weighted uses
and definitions. Observed contributions for the count are:

| Source form | Contributions | Priority / profit | Count location |
|---|---|---|---|
| Single expression |1+1+2|4 / -3|Stack|
| Split only the last division |2+0+0+1+2|5 / -2|R9|
| Split after the left shift |2+0+1+1+2|6 / -1|R10|
| Both divisions as assignments |2+0+1+0+1+2|6 / -1|R10|

The extra source assignment is not a neutral way to separate code-generator
recipes: it changes the scored use chain. Even the priority5 form keeps the
count in a register. These changes also displace constants and other variables,
remove the stack slot, and move the following function roots.

The source cast to unsigned int, multiplication spelling, and an inline
consumed assignment produce binaries identical to the wide-counter control.
Ordinary assignments, compound assignments and assigning the flags before all
three operations produce identical binaries within their own group. These are
verified complete-context identities, not comparisons of mismatch counts.

## Native controls

All controls retain the real three-function context and original compiler flags.
The suffix is accepted only when its actual root and all274 bytes remain exact.

| Control | First-function differing bytes | Context bytes | Original suffix exact |
|---|---:|---:|---|
|107 correct-register baseline|122|1216|Yes|
|32-bit counter|149|1216|Yes|
|Compound / ordinary / all-compound divisions|407|1208|No|
|Multiply / inline value / unsigned-int expression|149|1216|Yes|
|Split after left shift|397|1204|No|
|Split last division|394|1208|No|
|Named consumed audio byte|162|1216|Yes|
|Low-then-high initialization, wide counter|148|1216|Yes|
|Named audio plus ordered initialization|285|1212|No|

All13 controls fail the full30880 and30A44 proofs. No relocated similarity is
credited. The122-byte baseline remains the correct-register starting point.
Named audio and initialization ordering are genuine source controls; neither is
a complete fix and neither is promoted.

## Verification

`tools/shc_probe/cases/session_checksum_setup108/analyze.py` verifies:

-13 source hashes, native/captured object equivalence excluding only `.comment`,
 actual ELF function roots, byte differences and unchanged verifier results;
-26 original-output middle-stage replays and977 allocation score records;
-8 hardware priority replays,192 constant contributions and227 variable
 contributions, with64 constant and52 variable totals matching debug records;
-9 hardware GEN/PEP replays,592 scratch-selector predictions,553 target GEN
 emitter events and532 target PEP writer events;
-the exact original-ROM R1→R4 copy and the uninterrupted candidate shift chains.

Traced GEN output matches its untraced control; `.sua` also matches the native
capture. Recorded `.asa` cleanup metadata differences remain explicit. PEP
`.sub` and `.asb` both match the native capture. Tracers write CPU debug registers
only, never compiler instructions, configuration or generated operands.

PEP's current-section index is not source definition order. The broad trace
contains all three code sections: section1 is30880, section2 is30A44 and section0
is the prototyped30C2E. The analyzer binds section1 to the unique flag-load and
seven-shift sequence; it does not mistake the87-event suffix for the target.

Both remote checkouts still verify all35 checkpoint104 production pins, the
original rebuilt ROM hashes, and127326/200698 matched bytes (63.441585865%).
The236-test full-suite pass belongs to checkpoint104; it was not rerun for these
rejected research controls. Remaining bytes:73372, including all1638 runtime
bytes. The literal100% goal remains active and incomplete.

## Next investigation

Keep107's correct-register context and stop varying assignment spellings for
this calculation. Examine the equally scored constant candidates: the bank
base0x24060000, masks0x100 and0x300, and hardware addresses all compete at
priority4/profit-3. The current allocation keeps the base inR9 and0x100 inR8;
the ROM instead keeps0x100 inR9 and0x300 inR8. Trace their creation/sort order and
run a fixed-graph allocation prediction before choosing another genuine source
change. This is a distinct remaining allocation discrepancy, independent of the
now-explained count priority threshold.
