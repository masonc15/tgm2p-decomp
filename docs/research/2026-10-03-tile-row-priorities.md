# Tile writer: row and record priorities measured directly

Checkpoint84 remains **127,276 / 200,698 bytes (63.41668%)**. Eleven private
source controls preserve all 432 preceding wrapper/producer bytes but fail the
unchanged full-function matching gate. No production change or coverage gain.

The useful candidate is `session_tile_row86/record_byte_compound.c`. It now uses
the ROM's descriptor iterator R5, column counter R6, row counter R7, row value
R14, record pointer R11 and tile value R10. It still has 416 positioned target
byte differences, a 36-byte frame rather than 32, and a following dispatch two
bytes early. The zero/divisor registers are swapped: native MOV #16,R12 at274FA
and MOV #0,R13 at27562, versus original divisor R13 and zero R12. Correcting
six variable roles does not establish a match or supersede the earlier
32-byte-frame candidate's separate advantages.

## Source changes and measured effects

The starting candidate from85 computes the row in one expression. Read-only
hardware observations reconstruct its priority10 from contributions [6,0,4].
Two compound steps give16 from [6,0,3,3,4]; a single final compound addition
produces13 from [6,0,3,4]. Ordinary two-step assignments produce priority22 in
the diagnostics. The higher priorities initially take R6/R7 from the counters.

Changing the genuine counter increments to ordinary assignments corrects that
ordering. `xx = xx + 1` raises its priority15→19; `yy = yy + 1` raises11→14.
Together with the final compound row addition, priority13 now places px after
the two counters, yielding the original R6/R7/R14. The prior prediction that
compound counter increments would provide that increase was false: compound
increments and equivalent end-of-body compound updates normalize to unchanged
priorities. These controls are retained as negative evidence.

Splitting record-address construction into an actual base assignment followed
by the byte offset raises the record pointer's priority8→10 and gives it R13;
tile moves to the correct R10. Keeping the byte address in a char pointer and
using a compound offset update instead gives priority9 and the correct R11:

```c
char *rec;
/* Same guarded record selection, same signed16 byte offset. */
rec = (char *)g_60ad228;
rec += (short)(f->a24[cur] * 104);
/* Existing member accesses use ((struct rec104 *)rec)->member. */
```

This uses the existing character-view record arithmetic and the same typed
member lvalues. It adds no memory access, object, artificial consumer or dead
branch. Row staging introduces intermediate signed-short conversions; on the
project's SHC target their modulo16 result agrees with the existing final
conversion. The intermediate promoted sums are bounded by signed-word values
and loop counters, so no new signed32 overflow is required. No new unrestricted
portable-C or full-game semantic proof is claimed.

## Controls and validation

| Source | px priority/register | Frame | Following dispatch | Target differences |
|---|---|---:|---|---:|
| Baseline |10/R11|32|276A2|443|
| Two compound row steps |16/R6|32|276A2|445|
| Two ordinary row steps |22/R6|36|276A6|451|
| Final ordinary row step |16/R6|32|276A4|450|
| Final compound row step |13/R7|32|276A2|443|
| Compound counters only |10/R11|32|276A2|443|
| Final compound row plus compound counters |13/R7|32|276A2|443|
| Final compound row plus ordinary counters |13/R14|40|27696|449|
| Final compound row plus body compound counters |13/R7|32|276A2|443|
| Record base then ordinary offset assignment |13/R14|36|2769C|416|
| Record byte base then compound offset update |13/R14|36|2769C|416|

The last two contexts differ despite equal counts. The analyzer separately
checks actual byte identity for the equivalent counter controls. All eleven
sources regenerate exactly from the saved input.

Eleven driver captures equal the original native compile sections outside
compiler comments;22 normal/debug SHCMDL replays reproduce .reg/.ilb exactly.
All register streams round-trip losslessly.308 WEB CHAIN priority/profit checks
pass, with symbol aliases used instead of unstable serialized logical numbers.

Three hardware replays preserve .reg/.ilb and reconstruct132 completed priority
calculations,87 for the tile function. Each trace also has one incomplete
constructor observation in the preceding wrapper (function72), retained and
excluded from completed totals. Function and address scope prevent its reused
web address from being conflated with later functions. The three row-value
constructors have complete traces and unchanged totals at the later diagnostic
snapshot. No compiler instructions, memory or production flags were patched.

Both production checkouts freshly pass all33 checkpoint84 pins, original ROM
hashes and reports. The234 enabled tests and two clean builds remain the latest
full integration gates from84; no full rebuild was warranted for these private
rejected controls.

Evidence: tools/shc_probe/cases/session_tile_row86. Next inspect SHCMDL's
ordering of the equal-profit zero/divisor constants and the remaining frame
slots from record_byte_compound, while retaining85's 32-byte-frame control.
Do not repeat the closed row/counter/base-address forms or claim matching
from register-role agreement.
