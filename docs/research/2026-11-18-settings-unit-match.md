# The settings unit matches (2026-11-18)

The test-mode and settings translation unit runs from 0x9c0c to 0xe74f: the
test-mode menu (f_a226), the object test (f_a608), the screen test, the game
settings and sound test, the BG test, the play-status and maintenance pages,
and the text helpers they share. SHC compiled it as one file, and its
functions reach each other through same-unit literals, so no piece of it
could be admitted until the whole file compiled to the right length. Earlier
rounds had parts of it (f_9c0c..f_9ede, f_b2f4, f_b352, f_b36e, f_bd8a,
f_be04, f_d28c, f_e3b8, f_e520) retained from drafts whose later bodies were
only context.

`src/settings_reset_9c0c.c` now compiles every function at its ROM address
and byte-identical, except f_a608, which differs in 31 bytes. Its `.rodata`
(the menu tables and strings) is identical to the ROM copy at
0x603290c..0x603326e, so the rows link it there with `genuine-rodata:` and
the check compares it. The splits retain 0xa226-0xa608, 0xacdc-0xb36e,
0xb36e-0xb39a (code-only, see below), 0xb3c0-0xbd8a, 0xbd8a-0xbe04,
0xbe28-0xd28c, 0xd394-0xe3b8 and 0xe3e0-0xe520 from it: 14,772 more bytes,
coverage 139,096 → 153,868 / 200,698 (69.31% → 76.67%).

## The forms that mattered

Every function was scored by bytes against the ROM, in the full unit, at its
original address and parity. The fixes were ordinary source forms, mostly
found by factorial sweeps over a few sites at once rather than one edit at a
time:

- Testing the input globals directly instead of copying them into locals
  first (f_dbe8: `(g_6079304 & 0x80)` and so on).
- Choosing per site between `x++`, `x = x + 1` and `x--; if (x < 0)`
  (f_dbe8, f_a226).
- `extern void soft_reset(void);`, the boot ROM restart at 0x686 that the
  menu's exit jumps to, and the tail `f_2b4e8(); f_2b4e8(); soft_reset();`
  (f_a226).
- A dead block that really is in the ROM (f_cac8's unused "Bg is Zero"
  branch), a separate pointer copy, and block-local key-repeat bytes.
- `extern unsigned long g_6065640;` and the h/m/s divisions (f_cf68).
- `long run = 1;` declared before the table, and a plain counted loop
  (f_d394).

f_b892 and f_c21a each keep one `if (0)` dead store (`tmp[0] = 0`,
`g_6079306 = sel`), the README's "dead code still ranks registers" idiom.
Without them they differ in 248 and 95 bytes. Sweeps of 16,384 and 3,072
natural variants found nothing better than 73 for f_c21a.

## Tail proofs

f_be28, f_c21a and f_cf68 end in a call to f_2b4e8 as a literal tail through
r3 after the full r8-r14/PR epilogue, sometimes after restoring MACL and
releasing the local frame. `tools/c_prefix.py` gains three exact forms for
that epilogue (D: MACL; E: frame release; F: both). The frame release must be
a positive multiple of 4 below 128, every restore must be present in order,
and the jump must be through r3; `tools/test_long_tail.py` checks every
single-bit mutation, reorderings, dropped restores and other registers.

f_a226 leaves the menu by jumping to the boot ROM routine at 0x686, which
clears VBR, masks interrupts, reloads the stack pointer from the reset vector
and branches to the reset entry at 0x400. The boot ROM runs in place, so the
literal is 0x00000686 itself and not a RAM-copy address, and 0x686 is below
the reviewed tail-target domain. `tail_targets.json` gains a separate
`boot_entries` list with that routine (`_soft_reset`, 0x686-0x6ac,
SHA-pinned like the other entries; it is the only 0x00000686 word in the
ROM), and the header flag `boot-rom-tails: 1` lets a row prove a tail to a
reviewed boot entry. Such a tail must be one of the exact full-frame
epilogues (D-F, or G: the same restores with no MACL or frame, through r1),
and the literal must equal the entry's ROM address with no delta.
`tools/test_tail_targets.py` and `tools/test_long_tail.py` cover both.

f_b36e's tail sets up arguments between the load and the jump, which only
the function-code-range mode can prove, so its row is code only and its
38-byte pool at 0xb39a stays assembly.

## What is left

f_a608 (0xa608-0xacdc, 1,748 bytes) differs in 31 bytes: the home of `cnt` (72 in the ROM, 80 ours), the
slots of two key bytes, the slots of the prev/rep counters and a register
rotation in the inlined brightness helper. The rank model finds no pair of
edits that gives both the key-byte swap and the counter rotation. The jobs
ruled out are listed in `config/frontier_routes.json`.

The non-matching track loses the units these ranges replace
(`settings_b3c0.c`, `settings_be28.c`, `settings_d394.c`,
`text_width_e39c.c`, `text_e3e0.c`), and `settings_a226.c` becomes
`settings_a608.c` with f_a608 alone.
