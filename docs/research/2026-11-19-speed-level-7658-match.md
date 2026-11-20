# Speed/level f_7658 match

f_7658 (0x7658-0x7956, 766 bytes) now matches, so `src/speed_7956.c` becomes
a whole `c` unit for 0x7658-0x7c08 (f_7658 and the already-matching f_7956)
and `src/nonmatching/speed_7658.c` is retired. Two labelled dead lines do it.

## The constant web (19 → 10 bytes)

A dead `if (0) p->w328 = 65535;` at the start of the `if (p->w30c & 0x40)`
block gives the 0xffff constant one more use. That ranks its web ahead of
the #2 web, so they take the ROM's r6 and r7 homes in the gain code. Removed
on its own, the function goes back to 9 bytes off, all in that r6/r7 swap.

## The level test (10 → 0 bytes)

The remaining bytes were scratch temporaries in the five-term condition
`(p->w30c & 0x1000) && (g_6064880 & 1) && d_38f60 <= p->level && !p->b3a1
&& p->l350 <= 0x300c`. The ROM puts d_38f60's address and value, the level
and 0x300c in r3/r2/r1; the flat `&&` chain gave r0/r1/r3. No comparison
form, operand order, type, or plain dead line fixed it.

Nesting after the second term,

```c
if ((p->w30c & 0x1000) && (g_6064880 & 1)) {
	if (0) f_2e6fc(15);
	if (d_38f60 <= p->level && !p->b3a1 && p->l350 <= 0x300c) {
		...
	}
}
```

changes the scratch selector's state at the inner test. Nesting alone, at
any of the four split points, gives 15-29 bytes. The dead call between the
two levels is what lands on the ROM's r3/r2/r1. It came from a sweep of 56
dead statements at both ends of the outer block for every split point (448
variants). Only `f_2e6fc(15)` after the second term matched.

## Verification

`make split check report` passes both chip hashes (sha1 c84cf538), all 377
tool tests pass with `PREFIX_COMPILE_TESTS=1` (`test_speed_prefix` now
asserts the whole 0x7658-0x7c08 context equals the ROM), `make shiftable` is
identical to the ROM, `make mod-check` plays the replays like the original,
and `make shiftable NONMATCHING=1` (sha1 5e512e53) with
`tools/replay_check.sh --pin-idle` is identical over attract 12,000 and
gameplay 24,000 frames, frame counter identical.
