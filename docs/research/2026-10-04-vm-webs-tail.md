# VM sprite webs and complete tail recovery

The unchanged strict range gate accepts EE9C–EF04 (104 bytes) from ordinary C
compiled with SHC 5.0r32, `-macsave=1 -optimize=1 -speed`. This contains the
20-byte VM text-limit setter, three existing four-byte empty functions, and the
72-byte VM signed-division function. The net new match is 92 bytes. EC0C–EE9C
(656 bytes) is unmatched compiler context and remains assembly.

## Native evidence

`tools/shc_probe/cases/session_vm_webs225/` preserves 31 source controls: two
strict positives and 29 rejected controls. Five read-only compiler replays
produce 735 events with REG and ILB outputs identical to fresh untraced runs
and native captures. Five independent assembly captures reproduce their
respective compiler objects. The allocation model verifies all 47 candidates
for each of the five traced sources.

Sprite ID symbol62 forms four separate definition/use groups in blocks12,14,16,
and18. Each contains an assignment (opcode95) and later leaf (opcode112), both
signed-short, with the same reference head. Each block's draw call occurs before
the assignment; none occurs between assignment and use. The ordinary member
access gives one definition reference and priority6. A byte-addressed signed
word access gives two references and priority9. Every observed group still
receives a register; these experiments do not recover the original sprite spill.

Using the equivalent signed-word view for only the first and fourth branches
restores the six-long shared pool and 760-byte context, but the retained roots
remain two bytes late. Chaining the genuine zero initialization of i,row,col
moves those roots two bytes early. Making the completion counter unsigned char
then restores every original function root exactly. A declaration-initializer
variant independently reaches the same strict result.

## Source semantics and scope

The selected source is `pair_chain_counters_byte_done.c`. All three chained
counters are long and are initialized to zero before their first use. The
completion counter starts at zero; its only increment immediately breaks the
inner loop, so it increments at most once in each of exactly three outer rows.
Its value is therefore 0..3, fully representable by unsigned char. Row-derived
y is 62,33,4 and fits signed short. Sprite word accesses retain signed-short
width, structure stride64 and field offset26. No extra writes or dummy effects
are introduced.

The production source `src/vm_tail_ee9c.c` changes only the metadata and comments
from that selected source. The range gate authenticates five complete function
exports at original boundaries, all104 retained bytes, and zero outside-literal
dependencies. It needs no literal-tail opt-in or proof extension. The preceding
656 bytes are excluded. The division runtime helper remains assembly and is
still counted as incomplete in the fixed200698-byte denominator.

## Production validation

Integration evidence is in `session_checkpoint225_vm_tail`. Both checkouts must
pass clean split/build/check/report and all266 tests with no skips, retain the
original full-ROM hashes, and agree on the104-byte retained object before
progress is published. Three new tests check exact retention and exclusion,
then reject the two boundary-shifting counter variants.

The first isolated test run exposed a test-only KeyError: `tail_proofs` is absent
when optional tail analysis is disabled. The assertion now accepts an absent or
empty field. That initial failure and its successful build are preserved in
`first-isolated-run`; they are not counted as passing validation.

Next work should target the remaining656-byte EC0C function using the native
web evidence, or reassess other incomplete units by available complete-function
matches. The current context still promotes four sprite IDs where ROM stores
and reloads a signed word; matching the tail does not establish a prefix match.
