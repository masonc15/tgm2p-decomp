# Board queue producer: 390 bytes recovered through SHC allocation evidence

The complete queue producer at 2733A–274C0 now matches the original ROM under
SHC 5.0r32 and the unchanged default verifier. This adds 390 bytes and takes
verified coverage to **127,276 / 200,698 (63.41668%)**. The literal 100% goal
remains incomplete, with 73,422 bytes remaining, including 1,638 runtime bytes.

## What changed

The original reconstruction was 34 bytes away. Giving the existing selected-record
value `k` register storage raises its observed priority from 3 to 65,538. This
moves `k` from R13 to the original R14, moves the field pointer from R14 to R13,
and corrects the parameter stack homes. Nine bytes remain different, all in the
address calculations between 27376 and 2738F.

Separating the real record pointer from its field load fixes those nine bytes:

```c
register short k;
struct rec104 *record;
/* Existing selector read and guard are preserved. */
record = (struct rec104 *)((char *)g_60ad228 +
                         (short)(f->a24[(short)cur] * 104));
k = record->w84;
```

SHC now allocates the named pointer as its own logical value, with priority 2
and physical register R4. Its recorded lifetime is [8,9], ending as the selected
value's [9,11] lifetime begins. The earlier nested expression has no corresponding
named pointer in the allocation output and uses R14 for the temporary address.
No fake use, extra memory event, dead branch, physical-register directive,
compiler patch, object edit or verifier exception is involved.

## Evidence and rejected controls

Eight native source controls were compiled in the same genuine four-function
context. Target differences were 34 for baseline, 9 for register k, 46 for
register field, 23 for both, zero for named record, zero for register record,
and 31 for either named or register record-index forms. The simpler ordinary
record pointer was selected. Preparing both sets is reproducible through
prepare.py and prepare_address.py.

Three normal driver captures reproduce the respective original native ELF
sections outside compiler comments. Six private SHCMDL normal/debug replays
reproduce .reg and .ilb exactly. All three .reg files round-trip losslessly;
34 producer diagnostic priority/profit records pass the recovered score check.
The target is function symbol 83 in all three inputs. The named pointer is a
new logical value, not an assumed unchanged identity from a preceding stage.

The profit checks deliberately cover the producer. An initial attempt to apply
the same join across every context function found discarded diagnostic records
and a constant's different score convention. Neither is silently folded into
the claimed 34 checks or treated as a complete allocator model.

Three original compiler objects were also independently relocated to reproduce
the linked context, validating all 33 DIR32 relocations. No coverage is inferred
from normalized comparison scores.

## Retention and validation

New src/board_queue_2733a.c supplies only the complete 390-byte producer. Its
980-byte context contains the genuine preceding wrapper and following tile
writer/dispatch. The 590 context-only bytes remain excluded. The unretained
dispatch still starts two bytes before its nominal original address; this does
not shift the target or its following helper boundary.

The retained object exports only _f_2733a, has no relocations, and needs no
outside literal dependencies, tail proof or switch proof. The full 390 bytes,
including its 30-byte literal pool, equal ROM. The existing callback source is
unchanged. Splits add the producer and resume assembly at 274C0.

Two native tests verify the complete match, original context roots, retention,
exports and relocation absence. The negative test restores the nested address
expression and requires rejection at 27376 with no retained object or proof.
The isolated full suite passes 234 enabled tests with zero skips. Independent
isolated and canonical clean builds and 33 production pins are recorded in
session_checkpoint84_board_queue/RESULTS.json; the report and original EPROM/ROM
hash checks are part of that checkpoint's final integration audit.

Research artifacts: tools/shc_probe/cases/session_board_producer84.
Integration artifacts: tools/shc_probe/cases/session_checkpoint84_board_queue.
The next candidate is the genuine following tile writer at 274C0, using its
existing semantic reconstruction and the measured allocation behavior. Keep
ordinary read/dead-consumer and historical compiler controls from 82–83 closed.
