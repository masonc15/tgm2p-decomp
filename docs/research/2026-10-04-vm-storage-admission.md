# VM sprite storage and native candidate admission

Continuation226 adds no C credit. Production remains135594/200698 bytes
(67.561211%), with65104 bytes incomplete, including1638 runtime assembly bytes.
The unchanged full-context gate rejects all22 source controls. This investigation
identifies how the sprite ID becomes ineligible for a register and recovers the
original four stack stores and eight loads in semantically equivalent C.

## Source experiments

Two parents are authenticated: checkpoint225's exact-tail source and224's
meaningful y-product source with y priority20. A local one-element short array,
a one-member struct, and a one-member union each hold the same single short ID.
They remove scalar promotion but create address-register pressure and produce
844–852-byte contexts instead of760. Struct and union objects agree byte-for-byte
for each parent. No dummy storage or effects are used.

Addressing only the assignment, only the word-store read, or only the first color
read produces byte-identical output within each parent. The three forms therefore
do not offer separate matching paths. The explicitly addressed scalar uses the
original direct stack slot at R15, but performs three loads per branch instead
of two: the intervening byte store prevents load reuse.

A genuine sprite pointer shared by the two color fields reduces these twelve ID
loads to the original eight. It reduces the addressed tail parent's context from
840 to788 bytes and the y-product parent's from848 to792. Frames remain52 and48
bytes respectively, versus44 in ROM. These are partial structural recoveries,
not complete function matches, and receive no credit.

Four paired-color variants share the actual color writes between branches1/4 and
2/3. They reduce contexts to740–756 bytes but disrupt hoisted letter addresses,
stack layout and function boundaries. The y-product paired form has a32-byte
frame. Their smaller code is not evidence of a better match. The original
656-byte EC0C function remains assembly; its already-matched104-byte tail remains
protected by checkpoint225's source and production pins.

## Native admission mechanism

A read-only observer uses hardware execution breakpoints at SHCMDL4155C0,
415899,4158BA and41578E. It records symbol62's node type, reference head,
symbol class and alias flags, then whether the original compiler queues the
candidate. No compiler instructions, IR, configuration, or emitted code change.
Fresh untraced REG/ILB outputs equal the observed outputs and native captures.

An independent model reproduces all35 observed decisions:

- Ordinary scalar:14 visits, including four admitted self-headed assignments;
  signed-short type8, local class5, alias flags0.
- Array:one admitted address, type0x88; the ID value is no longer four scalar
  register candidates.
- Struct:sixteen admitted address expressions, type0x68; the member remains
  memory-backed.
- Addressed scalar:four self-headed assignments, type8 and alias flags0x04.
  All are rejected by the native `flags & 0x0e` test at415781–415788.

The selected-address controls demonstrate that this alias property affects the
whole symbol. The allocation traces independently show the four scalar ID groups
only in the ordinary parent. Seven complete allocation tables verify with47,48,
40,48,39,43 and39 candidates respectively.

## Independent checks and limitations

A small SH2 control-flow decoder visits executable instructions from EC0C,
including delay slots and branch targets while excluding unreachable literal
pools. It identifies seven draw calls and follows each of the first four calls
through shared color blocks to its loop backedge. ROM has four MOV.W R0,@R15
stores and two direct slot loads after each. The addressed scalar has three;
the two color-pointer controls recover two. All22 candidate control-flow graphs
and the original are inspected.

Two observer implementation failures are preserved. The first C observer did
not compile because a hex literal ending in e needed whitespace before `+id`.
The initial storage decoder assumed a store immediately followed the call delay
slot; one candidate schedules MOV #21,R1 first. The corrected decoder skips only
bounded immediate loads that cannot alter R0,R15 or memory. Both original
observer sources and diagnostic notes are retained; neither failure is credited
as validation.

Final validation runs the allocation, admission and reachable-storage models,
then independently re-resolves all22 compiler objects against source/context
hashes and strict rejections. Eleven output-preserving replays contain1013 events
(943 allocation and70 admission); seven native assembly captures reproduce their
compiler objects. The frozen evidence lives in
`tools/shc_probe/cases/session_vm_storage226/`.

No production source, split, toolchain, acceptance gate or denominator changes.
Checkpoint225's two independent clean builds,266 tests without skips, ROM hashes,
and64 production pins remain authoritative; the full suite was not rerun for
research-only changes.

## Next action

Keep `native_y_base_address_write_color_pointer.c` as the storage-correct starting
point, while retaining the original short completion counter. Investigate why
sharing color blocks changes promotion/hoisting of the four letter pointers and
constant63. Compare native block structure before allocation and the original
four stack-held letter addresses. Avoid repeating aggregate forms or selective
address positions: their mechanisms and equivalent outcomes are now verified.
A useful next candidate must recover those lifetimes and the44-byte frame before
byte-level scheduling work. Every newly retained function still needs the
unchanged original-boundary and exact-byte gates.
