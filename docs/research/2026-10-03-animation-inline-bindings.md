# Animation inline argument bindings — continuation152

Production remains checkpoint122: **128,316 /200,698 bytes (63.93487%)**.
Continuation151 was progress: its cache traces excluded an immediate cause and
its one-ID helpers supplied a shared-calculation control. This continuation
separates parameter width, evaluation order and final register assignment.
No new matching credit is earned.

Case: `tools/shc_probe/cases/session_animation_binding152/`.
All sources derive reproducibly from frozen151 value/pointer sprite helpers.
All seven stock candidates retain frame108, spr1 frequency7/home36 and four
static spr1 read sites. All beginA604 rather thanA608 and fail the unchanged
contextual prefix verifier. Original spr1 is home16; original body is1748 bytes.

|Control|Body bytes|bri1 /bri0 /ID argument registers|
|---|---:|---|
|id_int|1750|R6 /R5 /R4|
|reverse_short|1752|R6 /R5 /R4|
|reverse_int|1750|R6 /R5 /R4|
|reverse_pointer|1752|R6 /R5 /R4|
|array_short|1752|R7 /R5 /R6|
|array_int|1752|R7 /R5 /R6|
|wide_brightness|1754|R6 /R5 /R4|

The corresponding original value/address use is R4 /R5 /R6. Register numbers in
this table are hardware registers. Raw allocator IDs1–4 denote R4–R7.

## Observed distinctions

- Changing the sprite ID parameter from short to int removes the extra EXTS.W
  after the brightness ID load. This saves two body bytes in the explicit
  offset/local-pointer form. It does not change allocation or fix the boundary.
- Reversing the formal parameters and corresponding actual arguments reverses
  the inline assignment order, but retains the same final argument registers.
  `id_int` and `reverse_int` have equal lengths and relative mismatch counts;
  exact allocated-section comparison proves their text is nevertheless different.
- Each argument temporary has observed priority4/profit-5 in all seven controls.
  The explicit offset/address temporaries have priority6/profit-3. These are
  observations, not proof that modifying one priority alone would give a match.
- Direct `g_606006c[id].b14/b15` access removes the explicit offset/local-pointer
  declarations. It moves the ID argument toR6, but its common address usesR4
  and bri1 moves toR7. This does not recover the original joint binding. Both
  short and int forms remain1752 bytes despite differing sign-extension behavior.
- Unsigned-int brightness parameters keep the same argument registers and add
  two native EXTU.B instructions. They do not solve the allocation problem.

`analyze_bindings.py` parses the pre-allocation tree, identifies each anonymous
parameter assignment by its actual source value, and joins its symbol to the
final logical-register table in the same compiler run. It records assignment
order, symbol, logical register, physical register, priority and profit. No
cross-run heap pointer identity is used. `analyze_controls.py` independently
checks the native brightness tail, sign extensions, CFG sprite loads, observed
homes, and exact allocated-section comparison.

## Verification and next action

Seven native compiles and captured builds, fourteen normal/debug middle replays,
seven CFG audits and seven read-only home hardware traces pass. All replayed
REG/ILB bytes equal captured stock output. All controls inline with36 game exports,
no anonymous local calls, six loops and zero inline-return gotos. Home models
reproduce **189 records and959 weighted contributions**.

Both Nuada checkouts retain44 production pins,37 prior frozen cases, original
ROM hashes and all23 reference-ROM files. Full production build/test gates remain
checkpoint122's independent clean builds and243 enabled tests. Production files,
compiler inputs, matching rules and the fixed denominator are unchanged.

Preserve `id_int` for the removed conversion and `array_int` for the contrasting
argument/address allocation. Stop treating argument order as an ABI register
selector after inlining: the observed final assignments contradict that model.
Next inspect the anonymous address/argument interference and binding relations,
using these two graphs, before proposing another declaration or type change.
The spr1 home ordering and preceding function boundary remain separate required
constraints. No forced registers, dummy uses or fabricated storage are permitted.
