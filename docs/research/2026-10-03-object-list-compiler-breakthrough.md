# Object-list initializer breakthrough — checkpoint161

The complete82-byte function251CC–2521E is now verified matching C in both
Nuada checkouts. Progress rises from128316 to128398 of200698 bytes:63.97573%.
The gain is82 bytes;72300 remain, including all1638 runtime assembly bytes.
The full100% goal remains active.

## Source and mechanism

The accepted source retains the genuine traversal discovered in101/103 and uses:

```c
extern short g_24003800[];
/* ... */
*(hw = g_24003800) = 0;
*((short *)0x24003802) = 1;
```

Both stores access the original hardware addresses. The consumed assignment
initializes the pointer used by the real traversal and final marker store.
There are no additional reads/writes, dummy consumers, register numbers, padding,
compiler patches, new proof modes or denominator changes.

The numeric initializer from103 gave hw priority5 and the desired traversal
allocation, but a separate numeric-address record introduced a redundant move.
A symbolic address removes that numeric record, but ordinary `hw = symbol;
hw[0] = 0; hw[1] = 1;` raises hw's priority to7. The integer intermediate is
native-identical to that symbolic form. Four direct merge traces observe zero
copy-merge calls, ruling out the initially suspected merge explanation.

Variable-weight traces show the actual distinction, including source lines:

| Initializer form | Observed weighted contributions | hw priority |
| --- | --- | ---: |
| numeric alias | 1 + 3 + 1 | 5 |
| symbol, both initial stores through hw | 3 + 0 + 3 + 1 | 7 |
| symbol, separate second hardware address | 2 + 0 + 3 + 1 | 6 |
| consumed symbol assignment, separate second address | 1 + 3 + 1 | 5 |

The extra initialization weight is attached to the definition, not two ordinary
positive-weight use records. The consumed assignment removes it without
reintroducing the separate numeric-address value. This restores k/R4, s/R5,
i/R6, hw/R7, next-table/R14, bound127/R13 and bucket/R12 in the allocator.
The final native code uses next-table/R1, matching the original. That final
mapping is established by raw code equality; this turn did not add another
late-remapping hardware trace.

## Bounded controls

| Control | Function bytes | Nominal target differences | Both retained ranges pass |
| --- | ---: | ---: | --- |
| hardware_alias | 84 | 71 | no |
| hardware_symbol | 80 | 71 | no |
| hardware_integer | 80 | 71 | no |
| symbol_direct_init | 82 | 20 | no |
| symbol_separate_second | 82 | 19 | yes |
| next_before | 82 | 20 | yes |
| next_per_bucket | 88 | 71 | no |
| consumed_symbol | 82 | 0 | yes |

Eight native/capture comparisons and16 exact middle replays pass. Five graph
traces reproduce42 candidates/122 eligibility calls and every allocation.
Four variable traces authenticate74 contributions/20 priorities;68 debug
scores are recomputed. Four merge traces reproduce stock output and observe
zero merges. Symbol identities connect records across logs; heap pointers are
joined only inside the same compiler process. Research evidence is frozen in
`tools/shc_probe/cases/session_obj_address161/`.

## Integration and validation

Production changes are confined to `src/objfile_248a8.c`, `splits.txt` and
`tools/test_objlist_prefix.py`. The new C range exports only `_f_251cc` and
contains no relocations. Default verification authenticates five outside literal
loads; no optional tail/switch proofs apply. All2532 bytes of native context
248A8–2528C match ROM, including all nine original function roots and the two
previously retained ranges. Only the82 newly retained bytes receive credit.

The isolated checkout passed a clean `make split check report` build and all245
tests with PREFIX_COMPILE_TESTS=1 and PERM_SETUP_TESTS=1, without skips. Tests
include the complete native context and a negative control restoring the
unconsumed assignment, which the ordinary verifier rejects at251CD.

The canonical checkout independently passed a clean build after the transfer
was confirmed complete. An initial run started too early, read the old split
map and failed; INITIAL_TRANSFER_RACE.log is preserved and excluded. Both
checkouts now agree on46 production pins, reports, native linked context,
retained objects and original ROM/EPROM hashes. Original compiler object
metadata differs in three comment bytes only. ROM SHA1 remains
c84cf538962ddbb377290301d76d75d8b542e5a2.

The authoritative checkpoint is
`tools/shc_probe/cases/session_checkpoint161_objlist/RESULTS.json`. Before/after
source/map snapshots, native objects, proofs and build/test logs are preserved.
The research case's PRODUCTION_RECHECK.json is explicitly a pre-integration
checkpoint122 receipt; it is superseded for production by checkpoint161.

## Next target

Revisit the766-byte score prefix7658–7956 with the variable/constant weight and
allocation tools. First recompile the genuine canonical-ABI reference in
session_score7658_next/best_negative.c and authenticate its roots/protected
literal interval. Its historical19-byte mismatch includes swapped constant2
and0000ffff registers plus the bonus-test scratch sequence. Trace actual
weight contributions and creation order before choosing source changes;
avoid the earlier coefficient, prototype and bonus-guard sweeps. Preserve
checkpoint161 production pins while testing privately.
