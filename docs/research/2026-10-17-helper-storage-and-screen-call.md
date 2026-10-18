# Animation lookup storage and screen-test call evidence

Canonical coverage remains **138,670 / 200,698 bytes (69.09386242015367%)**.
The two bounded controls below add **0 verified bytes, 0 percentage points**.
The 199,060-byte C threshold and existing 1,638 ASM exceptions are unchanged.
No canonical source, split, proof, or compiler was modified.

## Animation-entry lookup at 0x2722c: register control closed

The genuine canonical context starts at 0x26f64 and contains both accepted redraw
wrappers, the unresolved redraw loop, and its animation-entry lookup helper.
An ignored-file-inclusive source inventory found 5,997 paths mentioning the
helper, 227 definitions, and five distinct bodies. Of these, 148 definitions
are old synthetic g_dummy stubs and are excluded. No inspected genuine body
qualified the initial field-pointer local as register.

The original lookup initially holds the field pointer in R14 and subsequently
uses R14 for a record104 pointer. The canonical draft instead uses R4 for the
initial field pointer and an indexed word load for the ID. One preregistered
control adds the standard register storage class to that consumed field-pointer
local, without changing any expression, value, lifetime, or effect. Its eight
ordinary native compiler stages produce **exactly the same 892-byte context**.
The predicted allocation change does not occur. Stop this register-hint route.

Both baseline and control place the lookup at **0x271fe**, 46 bytes early.
Its emitted extent is 226 bytes, versus the original 228. Fixed-address comparison
of `[0x2722c,0x27310)` has **224 differing bytes, including 48 missing bytes**.
The 140 accepted prefix bytes and their pool remain exact. The unchanged strict
validator rejects the target because the emitted context is too short.

The original helper interval contains a shared literal at **0x272cc** holding
`__modls = 0x0603076c`. Original predecessor loads at **0x271a0, 0x271f4, and
0x2720c** consume it. Thus the helper is not established as an independent
compilation unit merely because its entry and returns are known. Recover real
predecessor layout and scalar pointer storage; do not manufacture a literal,
insert padding, rebase the helper, or repeat the register-hint experiment.

Packet: `research/2026-10-17-animation-entry-storage/`. Its verifier checks source
identity, complete binary identity, actual roots, fixed-address counts, the
accepted prefix, and all three original PC-relative literal consumers.
VERIFICATION.json reports passed. There were eight compiler stages.

## Screen test at 0xad80: actual zero argument restored

Previous menu-owner research established the three screen-test string addresses
and described a missing R4=0 call setup. The original JSR at **0xadc8** calls
f29bf8 and its delay slot at **0xadca** is `MOV #0,R4`. Current full menu context
declares the callee void(void), supplies no argument, and emits NOP there.
A whitespace-tolerant archive search found no prior screen-test zero-argument
control; its only zero-argument hit was an unrelated startup draft.

One preregistered native control declares an int input and supplies the actual
zero argument in the complete canonical 0x9c0c context. The only emitted text
change across **19,684 bytes** is **0xadca: 0009 -> e400**. This recovers exactly
the observed call setup. It leaves the swapped input-global R9/R10 allocation
and late R2/R3 scheduling unchanged, closing that secondary mechanism.

| Measurement | Baseline | Actual zero argument |
| --- | ---: | ---: |
| Target entry | 0xad80 | 0xad80 |
| Successor entry | 0xaf8c | 0xaf8c |
| Raw differences in full 524-byte target | 32 | 30 |
| Protected b2f4-b39a differences | 0 | 0 |
| Following af8c-b2f4 differences | 79 | 79 |

All 166 retained neighboring bytes and every literal dependency recorded by
their three existing proofs remain exact. Strict target validation still rejects
at 0xae3d. Two fewer raw differing bytes are **not** two accepted bytes.

The accepted palette callee consumes no input. The ROM establishes the actual
supplied value but does not uniquely establish the original formal width/count.
Any later admission must reconcile the callee definition and affected callers
and reprove ordinary output; no production ABI declaration changed here.

The full native rodata has 1,182 bytes. Rechecking the original boot data
copy descriptor and all three screen-test pool loads confirms that its first
**302 bytes** equal original ROM331DC-3330A at RAM6032A3C, reproducing the earlier
owner result in the full context. The remainder differs, starting at offset302;
800 data bytes differ over the full comparison. No data relocation, patch,
partial data match, or adjusted call target is credited. Actual text-callee
placement and source data ownership remain separate admission questions.

Packet: `research/2026-10-17-screen-test-call-abi/`. VERIFICATION.json passes and
records eight native stages, source/object evidence, the sole text change,
actual roots, raw counts, protected dependencies, and original data ownership.
An initial verifier incorrectly treated the three retained ranges as one generic
c-range; it rejected the special function-code boundary. The corrected verifier
checks their actual byte intervals and existing dependency manifests separately,
without changing production validation. The target remains strictly rejected.

## Archived field-draw route recovered

The earlier field-array packet `tools/shc_probe/cases/session_field_array243/`
was missing from the specific frontier route index. Its historical verifier
records five genuine controls, preservation of 800 prefix bytes, and no strict
positive. Named coordinate storage had 380 raw target differences; adding a
pointer cast or cached state did not solve it. It explicitly parks further cached
state and broad register sweeps. Those are archived results, not fresh compiles
or current coverage gains. They now inform routing before another f7324 trial.

Both new packets are frozen with replay scripts and hashes. Rejected research
sources do not warrant an integration test suite, cold replay, or dashboard
coverage publication. The goal remains incomplete.
