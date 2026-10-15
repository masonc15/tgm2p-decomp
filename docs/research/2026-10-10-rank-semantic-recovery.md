# Ranking routine: semantic recovery before register tuning

Target: `[0x15794,0x15f10)`, 1,916 bytes. Genuine context begins at
`0x149b4`; `[0x149b4,0x14d50)` contains 924 protected bytes.
All work remains isolated research. Canonical sources and splits are unchanged.

## The old draft was structurally incomplete

The saved canonical context has all nine nominal function roots, but its last
body is 60 bytes short. Raw comparison finds 1,693 different present bytes plus
60 missing bytes. Thus argument-register tuning was premature.

Three bounded array-binding controls were recorded in
`research/2026-10-10-rank-array-binding/`. Casting the four coordinate calls
produces 1,745 total target differences; casting all five produces 1,698.
Neither is an accepted improvement, and both still have missing bytes.

The older `agent_c/STUCK.md` near-match score is not a raw exactness claim.
Its corresponding draft contains unsequenced pointer assignment/read in call
arguments, four volatile temporary pointers and dead statements. None of those
artifacts was imported into the recovered target.

## Target-backed semantic repairs

The archived ROM transcription `agent_a/r/rom_15794.txt` was checked against
the current authenticated program: all 958 listed halfwords match.

- At 15adc–15af0 and 15baa–15bbe, both replay pointers derive from table + frame;
  the second adds 1260. The old canonical draft passed the unadjusted table as
  the first argument. The retained callee `src/frame_2b4e8.c` confirms that it
  consumes both pointed-to input bytes.
- The animation frame counter is stored and incremented as a long at stack+24.
  The recovered draft uses an unsigned long rather than a short frame counter.
- The initial polling loop precedes the animation loop; it is not an outer
  loop that repeats the full initialization.
- The ROM narrows the modulo input with EXTU.W at 15bf2. This is an operand
  observation, not a claim that the compiler calls an unsigned remainder helper.
- Both sprite coordinates and centered text subtract the scrolling x position.
  The old draft lost the text subtraction.
- The text sequence supplies a single text pointer to f_e39c and five arguments
  to f_e490, including the zero stack argument. The recovered declaration agrees
  with the already-retained HUD caller. Complete cross-TU ABI verification
  remains an integration gate.
- The mode updates use separate mask and OR stores.

These repairs are saved in `research/2026-10-10-rank-semantic-recovery/`.
They preserve all nine function roots and all 924 protected bytes:

| Legal producer form | Context bytes | Target differing bytes |
| --- | ---: | ---: |
| inline helper, local computed pointer | 5480 | 1287 |
| inline helper, adjusted parameter | 5492 | 1374 |
| directly sequenced pointer calculation | 5472 | 1322 |

All fail strict extraction. Their frames still differ from the target; these
figures are diagnostic residuals, not accepted byte gains.

## Stack-layout mechanism and bounded negatives

The target saves the base table into four separate homes at stack+56, +52, +48,
and +44; the computed pointer uses stack+4. Its frame is 60 bytes. The ordinary
single-use helper and directly sequenced form use 40-byte frames.

Four consumed nested/helper address-taking controls are recorded in
`research/2026-10-10-rank-inline-homes/`. Each introduces an out-of-line body,
moves f_15794 and breaks two protected dispatcher bytes. Nominal roots become
157c0, 157b6, 157a4 and 157b8 for address_parameter, address_local, pair_aggregate
and address_base respectively. They are rejected without rebasing or padding.

SHC manual section 3.6 (local `refs/shc_manual_1997.txt`, around line 3060)
explicitly excludes inlining when the function references a parameter address.
This explains the address-parameter failure and is a reason to stop that family.
It does not prove every nested helper is impossible.

The earlier `agent_a/STUCK.md` also proposed inline parameter homes, but its
chk helper has an unsequenced q assignment/read. Its apparent stack shape is
not evidence that the legitimate helper tested here must produce that shape.

Two further legal controls in `research/2026-10-10-rank-repeated-input/` use
the base parameter in both actual argument expressions. Repeated addition and
indexed addressing compile to identical output: all nine nominal roots, all
924 protected bytes, 1,454 differing target bytes, and a 44-byte frame. The
code local now uses offset40, as in the ROM, but the four original parameter
homes are still unexplained. Repeating these spellings is not the next task.

## Reproducibility and next action

```sh
/drive2/tgm2p/venv/bin/python research/2026-10-10-effect-recipe-planning/verify.py
```

The shared research verifier checks source hashes, original compiler identities,
independent object relocation resolution, roots, all reported residuals and ROM
transcription bytes. Each packet contains its pre-recorded hypothesis, compile
budget, source hashes, ordinary outputs, rejection and replay command.

Next: reduce and inspect SHC's treatment of inline parameter lifetimes and
coalescing. Explain the four independent base-table homes and shared computed
pointer using legitimate operations before another register search. A useful
control must predict that mechanism and preserve predecessor layout. Do not add
volatile effects, dead stores, new ABI declarations or extra calls to create
the desired frame.

Current outcome: zero accepted bytes; 137,090 / 200,698 linked C bytes
(68.306610%). The canonical full-ROM validation passed both original EPROM
SHA-1 checks and the interleaved image check. No integration or dashboard
percentage change occurred, and no cold replay or full test suite is claimed
for these research-only changes.

Follow-up: [authenticated inline lifetimes and bounded parameter controls](2026-10-10-inline-lifetimes-piece-inputs.md). The scalar clones survive optimization; register/storage allocation is the measured distinction.
