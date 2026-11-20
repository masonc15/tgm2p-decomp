# Field setup f_12e2 matches (2026-11-19)

f_12e2 (0x12e2-0x1340, 94 bytes) sets a field's spawn coordinates
`w314`/`w316` from the `d_36304` table: x = 160 in mode 4, otherwise the
entry for the field's id, or the entry two further on when mode bit 2 and
the caller's flag are set. It sits after the retained f_1278 in
`src/startup_setup_1278.c`, whose c-prefix now ends at 0x1340.

## What fixed it

The context draft was 9 bytes off in two places.

1. The default branch reads x before it sets `p`:
   `f->w314 = (p = &d_36304[id])->x;`. Two statements in either order
   reschedule or re-rank registers; the embedded assignment gives the ROM's
   `mov.w @r6,r2; mov r6,r5`. This took it to 5 (4 lines).
2. The remaining lines were the shared `&d_36304[id + 2]` temporary in r7
   instead of p's r5. The ROM's unfilled `bra` delay slot is the copy
   `mov r5,r5` deleted after slot filling, so the temporary and `p` shared a
   register. SHC gives each web the lowest free index (r7 before r6, r5),
   so r7 had to be busy: the `g_6064880` value web (r7) must live through
   the branch. A dead `if (0) f->w314 = g_6064880;` at the end of the branch
   does exactly that, and is labelled in the source. Placed anywhere else
   it changes nothing; a `fl` local holding `g_6064880` doesn't extend the
   web either.

Negative controls: `p = &d_36304[id + 2]; f->w314 = p->x;` or an embedded
assignment in the mode-bit branch shrink the function by 2-8 bytes,
`p += 2` forms, `d_36304 + id + 2` spellings, `(short)` index casts and
`id` types (`int`, `char`, `unsigned char`) are all worse, and dead lines
using `p`, `id`, `a` or table reads don't move the temporary.

## Verification

`make split check report` passes both chip hashes (`startup_setup_1278.c`
0x1278-0x1340: 200 verified C bytes). All tool tests pass with
`PREFIX_COMPILE_TESTS=1`, `make shiftable` is identical to the ROM and
`make mod-check` plays the recorded replays like the original. The
non-matching unit becomes `field_setup_1340.c` (0x1340-0x1896); `make
shiftable NONMATCHING=1` (sha1 ac5172da) with `tools/replay_check.sh
--pin-idle` is identical over attract 12,000 and gameplay 24,000 frames.
