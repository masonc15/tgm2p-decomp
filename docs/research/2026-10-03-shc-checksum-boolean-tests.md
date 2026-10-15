# SHC checksum-display Boolean tests

Continuation110 targets the490-byte30A44–30C2E function independently, retaining
the genuine30880 preceding context and the already verified274-byte30C2E suffix.
The prior turn made progress by explaining the bank-constant ordering constraint;
these experiments do not repeat those source families.

## Source-directed improvement

The native baseline has54 differing bytes in30A44. Grouping the two pointer
initializations before the two zero initializations reduces this to51. Replacing
the three `q->w6 == 0` tests with the exactly equivalent `!q->w6` reduces it to23.
All changes are ordinary consumed C operations. The best source is saved as
`tools/shc_probe/cases/session_checksum_display110/best_context.c`.

The complete400-byte interior30A76–30C06 now matches the original ROM, at its
original address. The actual function roots remain30880,30A44 and30C2E, and the
complete1216-byte context length is unchanged. The full function still fails the
normal verifier. This interior match earns no coverage credit.

## Why the Boolean spelling matters

The grouped control and logical-NOT candidate have byte-identical902-byte `.reg`
files. SHCMDL allocation is unchanged. Their `.ilb` files differ by24 bytes.
Hardware traces from unchanged SHCGEN establish the precise remaining effect:

| Site |Internal test emission index|Zero argument index|`== 0`|`!value`|
|---|---:|---:|---|---|
|First checksum result|65|67|R2|R3|
|Second checksum result|130|132|R2|R3|
|Third checksum result|165|167|R2|R3|

At each test, SHC's internal opcode133 has size2 for equality and size1 for NOT
(long versus word recipe metadata). This is not a different SH-2 TST width;
the resulting machine test is the same instruction. Each equality form makes
four scratch-selection calls during the preceding load/test construction; NOT
makes three. Immediately before emitting the zero argument:

-Equality: last registerR3, recent mask1, selector choosesR2.
-NOT: last registerR0, recent mask0, selector choosesR3, matching the ROM.

The recovered selector predicts every observed choice. A counterfactual that
changes only those two history fields also predictsR3 at all three sites. The
following branch argument choices and loop-tail temporary choices change too,
accounting for the28-byte improvement from51 to23.

Both versions emit208 target-function events. The only opcode/size differences
are those three internal test-size fields; operands differ as recorded. A new
emitter counter in the scratch tracer binds each selection to its exact position
in the separately verified emitter trace. The grouped control makes84 target
scratch selections; NOT makes81. Traced `.sua` output equals the native capture,
and traced/control `.sua` and `.asa` agree. Cleanup metadata differences in the
native `.asa` snapshot remain explicitly recorded.

## Controls and boundaries

| Source control |30A44 differing bytes|
|---|---:|
|Baseline|54|
|Grouped initialization|51|
|`y = n = 0`|163|
|`n = y = 0`|83|
|Signed y / y initialized from n|54|
|n initialized from y|86|
|Unsigned checksum fields / unsigned w6|51|
|Logical NOT for the three checksum tests|23|
|Also negate initial pointer test|23|
|Also negate final flags test / negate both|27|
|Explicit counter `!= 0`|23|
|Move actual array declaration first|23|
|Compound counter decrement|23|
|Ordinary counter decrement|27|

The analyzer checks whole-context byte identity for the named equivalent pairs,
not merely equal mismatch counts. Every one of the17 native controls preserves
the protected274-byte suffix. Every complete30A44 proof rejects. Earlier30880
remains unverified as well; no source is promoted into production.

## Validation

`session_checksum_display110/analyze.py` binds source hashes, actual ELF roots,
native/captured object sections (excluding only `.comment`), exact byte
comparisons and unchanged verifier results. It verifies:

-17 native source controls and34 exact normal/debug middle-stage replays;
-1276 allocation-score records;
-16 hardware-traced stage runs;
-100 sort positions and allocation predictions, including four deleted nodes;
-96 applicable caller-register eligibility entries;
-793 scratch predictions,333 in30A44, and832 target emitter events;
-all three history-state changes and the exact400-byte interior span.

The compiler's deleted/coalesced node category is skipped by the allocator
model, rather than mistakenly treating its zero physical value as an available
candidate. All four25-entry allocation tables are reproduced exactly.

Both remote checkouts reverify35 checkpoint104 production pins, original rebuilt
ROM hashes and127326/200698 matching bytes (63.441585865%). No new credited bytes.
The236 enabled tests were last run at104; they are not claimed as newly rerun for
these rejected research controls. The literal100% goal remains active with73372
bytes incomplete, including1638 compiler-runtime bytes.

## Next action

Retain the23-difference source and investigate its initialization and final
counter block. Original initialization usesR6 for zero andR4/R5 for the two table
addresses; the candidate usesR3 for zero and swaps those table registers. The
remaining tail usesR0 where the ROM usesR3 for the countdown load/decrement/store.
Trace zero materialization and address-constant allocation separately from that
tail. Avoid repeating the closed initialization chains, sign changes, pointer
NOT, ordinary decrement, and declaration-order controls. `rom_30a44.dis` and the
candidate's disassembly preserve the exact current comparison.
