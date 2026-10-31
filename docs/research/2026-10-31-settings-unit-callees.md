# Settings unit: f_bc02 literals and same-unit callees

Solo work on nephele, 2026-10-31. Baseline and result: **138,752 / 200,698
linked C bytes (69.134720%)**, a gain of **0 bytes**. One ordinary SHC v5.0r32
driver invocation (8 of the 24 campaign stages). No source, split, tool or
coverage change.

## Zero-compile harvest

Each c-range and c-prefix build already links its whole context at the original
origin. Comparing those linked contexts with the ROM, for every contextual
function that starts in an unresolved interval, finds no exact function. After
separating code bytes from PC-relative literal bytes, only one function has no
code difference: `f_bc02` [0xbc02,0xbd8a), 392 bytes, in the accepted
`settings_mirror_bd8a.c` context. The `short` coordinate correction made for the
`f_bd8a` acceptance is what made its instructions exact.

Its four differing literals are link-time identities:

| pool | ROM | context | meaning |
|---|---|---|---|
| bc7c, bde8 | 0600dc92 | 0600de18 | `f_e412`, defined later in the unit (+390) |
| bdf8 | 0600dda0 | 0600df34 | `f_e520`, defined later in the unit (+404) |
| bc90 | 06032cf8 | 0600e408 | `"Yes"` in discarded `.rodata` |
| bde4 | 06032cfc | 0600e40c | `"No"` in discarded `.rodata` |

ROM 0x33498 contains `"Yes\0No\0"` (address 0x06032cf8).

## The unit really spans 0x9c0c to about 0xe74f

The same string-literal addresses are loaded on both sides of 0xbe04 and of
0xd28c/0xd394, for example "Palette No." from 0xa9c4 and 0xcdb8, and
"Exit = Button-1" from 0xd20c and 0xd918. The menu string table is monotonic in
code order from 0xa27c to 0xe0de. Separate units would not share literal storage,
so this is one translation unit. No direct call crosses 0xbe04 within BSR range.
In the menu block, a JSR at +0xfa6 and a BSR at +0xf9e bound SHC's same-file
BSR choice, so call instructions do not decide the extent.

## Measured control: ending the unit after f_be04

Prediction, recorded before output: keep the genuine 0x9c0c origin, end the unit
after `f_be04`, and use `extern char g_6032cf8[], g_6032cfc[]` for the labels.
This follows the existing convention for menu text and the
`text_palette_e3b8.c` context, which leaves later helpers undefined. The
prediction was that every instruction in [0x9c0c,0xbe04) would stay identical and
only relocations would change.

The output falsified it. Without the later bodies, SHC changed `f_a226`
register homes (r14/r13/r11), its first code difference at 0xa229. It also moved
later entries by -28 to -126 bytes and changed 165 instruction halfwords of
`f_bc02` itself, even after correcting for the shift. SHC code generation for
callers depends on same-unit callee definitions, so ending or truncating this
unit is not a valid route. The `text_palette_e3b8.c` precedent works only
because its retained bytes do not observe that dependency.

## Route

`f_bc02` and the shared pool [0xbddc,0xbe04) remain blocked by unit layout.
Admitting them needs the full unit with genuine, size-correct bodies for
`f_be28`, `f_c21a`, `f_cac8`, `f_cf68`, `f_d394`, `f_d8e0`, `f_d962` and
`f_dbe8`. Their current size errors are +8, -108, -80, -12, -20, +38, +150 and
+412 bytes. Only `f_e074` and `f_e3b8` already have exact-size bodies, in the
`text_palette_e3b8.c` context. Admission also needs an owner for the unit's
`.rodata` labels. The same blockers explain why `f_ad80` and `f_af8c` cannot be
admitted from code fixes alone.

Private packets, with replay scripts and hashes:
`research/2026-10-31-context-harvest/` and
`research/2026-10-31-bc02-literal-owner/`, which contains `INTENT.json`,
`RESULT.json`, the candidate source and its context outputs.
