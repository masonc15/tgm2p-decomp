# SHC function-address candidate gate — continuation124

**No new matching bytes.** Production checkpoint122 remains128,316/200,698
(63.934867%), with72,382 bytes incomplete. This investigation replaces the old
claim of unexplained global symbol/hash effects with a specific, measured
compiler rule. The next useful source investigation concerns genuine callee
definitions and compilation-unit ownership.

## Clean settings baseline

The archived `agent_b/stuck/f_af8c.c` contains artificial trailing text-renderer
definitions and an unused `pad_n` data object. Its historic near-match scores
cannot establish acceptable source. The new case removes both, corrects the
text-call declarations, and uses actual ROM tables through external aggregate
copies. No unused stack space, fake body, patched instruction or register
binding is introduced.

The original boot descriptor atROM31410 maps initialized data fromROM3141C to
RAM6030C7C. The coordinate, input-mask, defaults and limit arrays are verified
atRAM6032A6E/6032A96/6032ABE/6032B6A/6032B80. The copy helper is independently
identified by original poolB0D4 asRAM60308EC. Its diagnostic symbol binding is
local to this case; `symbols.txt` is unchanged. Prior continuations44–48 already
established genuine automatic-array ownership and alignment. This work does not
claim to rediscover or supersede that data-layout evidence.

Compiling the three settings routines alone preserves their nominal roots.
The checksum/validation and defaults-copy spans differ at onlyB2F9/B31D/B355/B359:
four displacement bytes in four native PC-relative literal loads. Each load
has the original register and literal value, but its pool is20 bytes early.
The original preceding input-test function owns five additional shared pool
entries. Neither this two-function122-byte span nor its individual functions
passes the unchanged default verifier.

With genuine preceding input-test source, later roots move74 bytes early.
Making the input-port read volatile produces identical linked bytes. Both
controls remain rejected. Native struct initializers were initially rejected
by SHCerror2130; separate genuine struct assignments compile. The failed source
and log are preserved, as is the initial missing-helper-binding diagnostic.

## Definition visibility changes candidate eligibility

Baseline `f_af8c` has twelve calls to `f_e412`. Its address is one shared
candidate: leaf21, priority36, profit25, internal physical7 (R12). SHC then
keeps that renderer address in a register. The ROM instead reloads it.

The diagnostic controls add a reconstruction of the actual renderer's loop:
newline and space handling, glyph drawing, and advancing by the real width
table. One places it in the ordinary section; another in a separate named
section. Both remove the renderer-address candidate. A further control adds
the actual centered-text behavior and the retained `f_e520` glyph renderer;
all three renderer addresses then disappear from the input-test allocation
candidate list.

These are compiler experiments, **not recovered original translation units**.
The callees are nonadjacent to the settings routines in the ROM, and the separate
section layouts have no authenticated original provenance. Their native objects
remain intact. The existing verifier rejects the additional allocated sections;
it has not been extended or bypassed. The reconstructed renderer is not claimed
byte-exact, and no callee or caller code from these controls is integrated.

## The measured compiler gate

Read-only hardware breakpoints atSHCMDL414BF0 and415C20 observe the address
candidate before selection. All four normal/traced REG and ILB outputs match
the fresh native captures exactly. The relevant observations are:

| Source | Function-address type | Symbol kind | Flags byte7 | Address candidate |
| --- | ---: | ---: | ---: | --- |
| External declaration | 0x48 | 2 | 0 | Present, R12 |
| Ordinary-section renderer definition | 0x48 | 1 | 0 | Absent |
| Separate-section renderer definition | 0x48 | 1 | 0 | Absent |
| All three renderer definitions | 0x48 | 1 | 0 | Absent |

Native x86 instruction415CD4 compares the symbol kind with2. For function-address
type0x48, kind2 reaches candidate collection at415D2B; kind1 instead reaches the
early return at415CF4. The earlier flags-byte7 bit0x40 test is **not** the cause
in these controls: that byte is zero in every observation. This is eligibility
before priority scoring, not a priority reduction or an accidental hash-order
change. Builtin-name exclusion is a separate preceding check.

The tracer initially assumed one gate event per source call. It observes one
shared candidate representing the twelve calls, consistent with leaf21 and the
allocation log. That failed audit expectation is preserved separately; the
corrected audit checks the single event and its exact identity and metadata.

## Remaining mismatch and next step

Native main-section root offsets, measured with its intended originAF8C:

| Source | B2F4/B352/B36E displacement | Main .text bytes |
| --- | ---: | ---: |
| External callees | −74 each | 1,000 |
| One renderer in a separate section | −28 each | 1,044 |
| All three renderers in separate sections | −22 each | 1,052 |

The separate-section measurements are original ELF offsets, not successfully
linked/verified original addresses. Nominal AF8C–B3C0 is1,076 bytes. The main
section remains too short, and the controls do not establish the original
compiler's section organization. Ordinary-section placement also changes call
reachability; that control has later roots60 bytes early and includes a relocated
renderer, so it is likewise ineligible.

This identifies a plausible cause for the ROM's repeated renderer loads and
explains a concrete effect of the old fake definitions. It does not uniquely
prove the game's original file boundaries. Investigate the genuine source unit
through the text routines, using the existing real menu bodies and actual
renderer definitions; do not restore fake stubs, padding, or force the symbol
kind through inert declarations. The prior A226 frame discrepancy remains
unresolved and must not be repaired with an unused local buffer.

## Evidence and validation

`tools/shc_probe/cases/session_settings_context124/` contains six native source
controls, four fresh captures, eight normal/debug middle-stage replays, four
hardware gate replays, the native gate disassembly and374 verified register-profit
calculations. `analyze.py` rechecks source/object bindings, section contents,
literal values, table bytes, exact replay outputs, lossless REG decoding, and
the candidate-presence predictions. `ANALYSIS.json`, `SOURCE_ANALYSIS.json`,
`REGISTER_ANALYSIS.json` and `CALLEE_REPLAY_RESULTS.json` record those checks.

Production sources, compiler flags, split map, proof rules, ROM and denominator
remain unchanged. The fresh production check covers44 pins, the exact ROM/report
and nine preceding evidence freezes in both Nuada checkouts. Checkpoint122's
independent clean builds and243 enabled tests remain the latest complete gates;
no broad build/test run was repeated for rejected diagnostic sources. The full
100% goal remains active and incomplete; work continues solo.
