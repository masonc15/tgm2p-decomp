# Palette constant collection and copy-loop induction

Continuation206 authenticates excluded experiments only. Production remains
131,780 / 200,698 verified C bytes. The previous turn made progress by recovering
five main register homes and the68-byte local frame; this turn explains the
constant ordering and obtains a genuine source form retaining the buffer inR10.
No candidate is an exact match.

## Collection order is a hash-table traversal

Existing Ghidra output located SHCMDL4159E0 and415C20. Fresh GDB disassembly and
hardware execution breakpoints corroborate the relevant instructions without
modifying the compiler. At415D42 the collector has a kind, value, and source
node; the hash bucket is value&127. Kind1 values come from the IR node's signed
16-bit tag at+88. The buffer has symbol59/tag12. Tag12 is a compiler identifier,
not a frame displacement. It does not change when moving the buffer declaration.

Within a bucket, first encounter determines insertion order.4159E0 walks buckets
in increasing order and appends candidates whose calculated weight exceeds1.
The native baseline candidate sequence is:

- bucket0: zero,0000FC00,FFFFFC00,00FC0000,FC000000;
- bucket12: buffer address;
- bucket16:16;
- bucket32:32;
- bucket127:-1.

13 fresh traces authenticate collection order against the independent allocator
table and preserve native REG/ILB output. Moving the buffer declaration first or
last, reversing independent for-loop initializers, reversing independent loop
increments, or reversing both produces byte-identical compiled contexts. These
forms are closed. Six offline permutations of the three tied mask entries in
bucket0 still leave the buffer last among the five tied constants on the fixed
baseline graph. Merely changing those masks' insertion order cannot fix that
graph; these diagnostic permutations were never passed to the compiler.

## Stack-address usage weights

The collector trace records source nodes, common-use chains and enclosing-loop
depths. The stack address's baseline weight is20: its two unrolled color loops
contribute8 each, and the copy contributes4. For these nonliteral address nodes,
each contribution is (loop_depth+2)*chain_count. The native candidate weight and
allocator priority equal the independently recomputed total in all13 traces.

Unsigned-address and char-address paired copy expressions move the copy's base
use into an outer loop scope, changing its contribution4->3 and total20->19.
They make retention harder. The signed-int address pair keeps20 and reduces the
raw predecessor difference709->663, but remains1184 bytes instead of1186, with
1267 initializer body differences. Matching a function boundary is not inferred
from a close size.

An explicit `offset=i*2` variable retains the stack address inR9 and the red mask
inR8, but the signed mask isR10. Its stack-address weight is still20: the changed
candidate graph and unstable partition sorting cause the different result.
The initializer has119 body differences but starts four bytes late at2A57A.
This is not an improvement over the best earlier119-byte-difference body.

Updating a signed long offset directly (`offset+=2`) retains the stack address
in the desiredR10 and preserves the68-byte local stack frame and five main
homes. Green-mask and signed-mask instead occupyR9/R8, and red-mask remains
unallocated. Its predecessor is1170 bytes and differs in1061 positions; the
initializer starts16 bytes early and differs in121 body bytes. Register-home
agreement alone does not make it an integration candidate.

The unsigned offset-counter form is byte-identical to the signed-int address
pair. Standard register hints on the actual destination-row pointer also
retain the buffer in some forms but fail the remaining register/boundary/order
checks. These ordinary source alternatives and full native output are saved;
none uses forced registers, patched priorities, padding or fabricated uses.

## Verification and next action

Case: `tools/shc_probe/cases/session_gfx_ties206`.
16 strict negatives:15 new sources and1 carried source.29 output-preserving
replays contain2923 events, comprising16 allocator graphs and13 constant
collections.16 captured assemblies independently assemble to the strict
contexts. The verifier recomputes roots, raw bytes, actual-body comparisons,
source provenance and exclusion from production. Original compiler SHA-256
remains181b111ac20a19871a04a3a29e24a679f4b4376648dc79d6a4fc17bf39f1bb60.

No production change occurred, so checkpoint192's independent clean builds and
253 enabled tests remain the applicable full gates. Fresh checks authenticate
both Nuada checkouts' manifest,54 production pins, report and ROM hashes.

Next trace the copy-loop induction/common-address extraction that distinguishes
baseline,copy_shared_offset,and offset_counter_signed. The original loop retains
a destination base inR5 and a shared byte offset inR13; some new forms introduce
extra saved induction or destination temporaries. Removing those through real
source structure could change both instructions and allocation ties. Inspect
the generated candidates' origins before trying more declaration or mask-order
permutations. Preserve the full literal100% goal and unchanged matching rules.
