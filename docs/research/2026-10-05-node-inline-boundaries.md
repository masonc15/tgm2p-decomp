# Ranking-list inline boundaries — checkpoint247

No new verified C bytes. Local and fresh Nuada checks preserve64 production pins
and135594/200698 matched bytes. Text-width research remains parked after246.

Target238A8–238FC is an independently reconstructed84-byte leaf with74 instruction
bytes and10 pool bytes, followed by238FC. It clears the list's state and connects
two actual sentinels using the ROM's distinct unsigned-byte and signed-word offsets.
Prior bounded pointer-expression sweeps are documented in session_code238a8_next.

This experiment copies inline_goal/nodes238_v14.c as baseline. The used-list and
free-list link operations become real inline helpers, retaining the initial clear,
decrement, store order and explicit narrow scaling. One version accepts unsigned
char/short index parameters; another accepts int. A final version passes the real
list object into both helpers. No dummy effects, padding or register assignments.

| Source | Bytes | Raw differences over84 bytes |
|---|---:|---:|
| baseline |84|41|
| typed index helpers |84|41|
| int index helpers |84|41|
| list-pointer helpers |80|81|

Both scalar-index helpers emit byte-identical baseline code. Their first10 bytes,
last10 pool bytes and original function root match ROM, but register allocation
and instruction ordering differ starting238B2. The pointer-parameter form also
creates a separate g6077608+0xA4 literal and shortens the context. Missing bytes
count as differences. No helpers remain as exported functions: every original
object defines only f238a8. Thus no artificial context or extra call was admitted.

All four were freshly compiled on Mac and independently on Nuada. Independent
relocation confirms identical code, actual exports and all relocation tuples.
The unchanged prefix validator rejects all four. The verifier authenticates
source/compiler provenance,64 production pins, objects and exact differences.
Its initial single-relocation assumption was corrected after inspection showed
the pointer-helper object has two genuine relocations; neither was suppressed.

Stop these helper-boundary probes. They do not address the remaining allocation
and ordering, and no new matching source lead is claimed. Full production suite
was not rerun for rejected research-only changes; checkpoint225 retains its two
266-test gates. Literal100% remains unachieved.
