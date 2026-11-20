# Board toggle f_277d0 match

f_277d0 (0x277d0-0x27862, 146 bytes) now matches, so the verified span of
`src/board_setters_2774a.c` grows from 0x276e8-0x277d0 to 0x276e8-0x27862.
The non-matching packet shrinks to f_27862 alone and is renamed
`src/nonmatching/board_setters_27862.c`.

## What was wrong

The old body read `t->w8` straight into the array index and stored the
`f_26098()` result through an `int id`. That compiled two bytes short. In the
occupied branch the ROM reloads `t->w8` into r0, copies it to r6 and scales
r6 by 104 (`mov r0,r6; muls.w r3,r6; sts macl,r6; exts.w r6,r6; add r5,r6`),
while ours scaled r0 in place and added it to the base, then needed an extra
`exts.w r0,r4; mov r4,r0` to store the call result. The earlier frontier note
blamed a peephole branch prefix.

## The source form

Two ordinary changes give the ROM:

```c
void f_277d0(struct tsk *t)
{
	short id;

	t->w16 = (t->w16 + 1) & 1;
	if (t->w16) {
		if (!t->w8) {
			t->w8 = f_26098();
			return;
		}
		id = t->w8;
		g_60ad228[id].w0 = 0;
		t->w8 = f_26098();
		return;
	}
	if (t->w8) {
		id = t->w8;
		g_60ad228[id].w0 = 0;
		t->w8 = 0;
	}
}
```

A `short id` holds the reloaded `t->w8` before indexing, which is what puts
the copy in r6 and scales it there, and the call results go straight into
`t->w8`. The empty test is written `!t->w8`: with `t->w8 == 0` the same body
is 20 bytes off (r2 instead of r3 for the first `f_26098` address and the
copy scheduled ahead of `mov #104,r3`). Of 72 combinations of test form,
call-result form and index form (job j388), only the four with `!t->w8` and a
plain `id = t->w8;` copy in both branches match; `(id = t->w8) != 0` in the
else branch is 11 bytes off. Before that, 24 index and type combinations
(j387) found the copy local, which brought the function from 42 to 20.

## Verification

`make split check report` passes both chip hashes and reports 378 verified
C bytes for 0x276e8-0x27862 with 98 context bytes excluded. All 377 tool
tests pass with `PREFIX_COMPILE_TESTS=1` (23 skipped), `make shiftable` is
identical to the ROM, `make mod-check` plays the replays like the original,
and `make shiftable NONMATCHING=1` with `tools/replay_check.sh --pin-idle`
is identical over attract 12,000 and gameplay 24,000 frames.
