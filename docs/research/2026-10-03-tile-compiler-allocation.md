# Tile writer allocation: frame restored, full match still negative

Checkpoint84 remains **127,276 / 200,698 bytes (63.41668%)**. This investigation
adds no production bytes. Ten source controls preserve all 432 preceding bytes
(the wrapper and now-matched queue producer), but fail the unchanged verifier
for the complete tile writer at 274C0–2769E.

The useful result is a source change that restores the ROM's 32-byte frame and
row-counter register. Starting from the independently reviewed signed-long
single-read descriptor reconstruction and genuine descriptor iterator:

```c
if (px >= (clip = rec->l24[k] >> 4) && px < clip + 16)
    /* unchanged clipped store */;
```

The original standalone clip assignment gives its logical value priority 12
and R7. Moving that same consumed assignment into the first comparison lowers
its priority to 8 and spills it. The row counter `yy` then gets R7, and the
frame shrinks from 36 to the original 32 bytes. The memory read still occurs
once before its first comparison; the second guard and palette store retain
their short-circuit ordering. This is a real value use, not an artificial
consumer or dead branch.

This candidate is still four bytes too long, with 443 positioned target byte
differences. Its row value is R11 rather than original R14; record/tile/constants
also remain allocated differently. An aligned frame alone is not a match.

## Bounded controls

| Source control | Frame bytes | Actual following dispatch | Target differing bytes |
|---|---:|---|---:|
| Current genuine baseline |36|2769C|352|
| Prior signed-long descriptor snapshots |36|276A0|407|
| Snapshots, register row value |48|2769E|424|
| Snapshots, register record pointer |48|276A6|455|
| Iterator, register row value |48|276A6|454|
| Iterator, postincrement |36|276A6|454|
| Iterator, ordinary assignment increment |36|276A6|454|
| Iterator, compound increment |36|276A6|454|
| Clip assignment in first guard |32|276A2|443|
| Clip assignment in guard plus compound upper-bound update |36|276AE|450|

The row register hint raises priority from 10 to 65,545 but selects R4, not the
ROM's R14. It displaces other values and expands the frame to 48 bytes. The
register strategy that solved the prior across-call producer does not transfer
unchanged to this short-lived row value.

The ordinary iterator increment changes allocation despite retaining the same
number of target differences. Postincrement and compound increment produce
identical full contexts; ordinary assignment differs. Do not equate equal raw
difference counts with identical output or repeat these controls.

## Validation and diagnostic identity

All ten sources regenerate exactly from saved input sources. Eight original
native captures equal their normal compile sections outside compiler comments.
Sixteen SHCMDL normal/debug replays reproduce .reg and .ilb byte-for-byte. Every
.reg stream round-trips losslessly. The recovered priority-minus-average-conflict
formula passes 221 WEB CHAIN diagnostic records across these eight tile cases.
Constant and deleted-record score conventions are outside that claim.

Diagnostic logical numbers change during reflection/serialization: joining
solely by that number gives incorrect role names in this function. The analyzer
uses explicit symbol aliases and preserves multiple candidate webs for reused
`raw`. It also stops parsing before later frequency-sort/reflection blocks that
reuse record numbers. One serialized symbol (109 in clip_guard_update) has no
remaining named diagnostic record; it is reported as unjoined, not assigned an
invented identity. The successfully explained clip_in_guard case has a direct
symbol join for the relevant clip/row roles.

The record evidence comes from unchanged native output and compiler logs; no
compiler code or object bytes were modified. Existing semantic reviews support
the snapshot, iterator and destination-expression inputs. No new complete-game
or exhaustive helper semantic execution is claimed. Both current production
checkouts revalidate all 33 checkpoint84 pins, original ROM hashes and reports.
The 234-test pass and two clean builds remain checkpoint84's latest full gates;
they were not rerun for these rejected private source controls.

Evidence: tools/shc_probe/cases/session_tile_priority85/{ANALYSIS,REGISTERS,
ALL_RESULTS,PRODUCTION_RECHECK,FREEZE}.json, source generators and replay scripts.
Next start from clip_in_guard and inspect the row value's priority relative to
constant 16, constant zero, tile and record pointer. Its restored frame, original
R5 iterator/R6 column/R7 row counters, single descriptor reads, and protected
432 bytes are concrete constraints. Avoid another blanket register-hint sweep.
