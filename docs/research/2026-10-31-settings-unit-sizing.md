# Settings unit 0x9c0c–0xe74f: sizing status

Solo work on nephele, 2026-10-31. Coverage is unchanged at **139,096 / 200,698
linked C bytes (69.306122%)**.

The settings unit holds about 3 KB of near-exact functions: `f_ad80` (524),
`f_af8c` (872), `f_b3c0` (1234) and `f_bc02` (392). Their remaining
differences are almost all literal identities. Those are addresses of the
unit's own `.rodata` strings and initialisers, plus `f_e3e0`/`f_e412`/`f_e520`,
which depend on every later body having its ROM size. Truncating the unit
changes earlier code (research/2026-10-31-bc02-literal-owner), so the whole
unit has to be sized genuinely. `f_e074` (808 bytes) is in the same position:
it compiles closer to ROM inside the full unit (263/404 address-normalised
instructions equal) than in its own context (222/404).

## Best draft and how to measure it

- Source: `research/2026-10-31-settings-e074-literals/unit.c`, based on
  `research/2026-10-18-nephele-bg-resource-count/resource_count.c`.
- Measure with `research/2026-10-31-settings-rebase/relink.py`. It links
  `.rodata` at its ROM address (0x6032a3c), as the 2026-10-18 packet did.

With that link:

- the unit's 2,098 bytes of initialised data match ROM exactly;
- the retained `f_b2f4`–`f_b39a` range is exact;
- `f_bc02` differs in 1 byte, `f_af8c` in 12, `f_ad80` in 23 and `f_b3c0`
  in 26.

| Function | Draft size | ROM size | Error |
|---|---:|---:|---:|
| f_be28 | 1018 | 1010 | +8 |
| f_c21a | 2112 | 2222 | -110 |
| f_cac8 | 1214 | 1184 | +30 |
| f_cf68 | 792 | 804 | -12 |
| f_d394 | 1324 | 1356 | -32 |
| f_d8e0 | 130 | 130 | 0 |
| f_d962 | 646 | 646 | 0 |
| f_dbe8 | 1156 | 1164 | -8 |
| f_e074 | 810 | 808 | +2 |
| f_e3b8 | 42 | 40 | +2 (pool alignment from the upstream shift) |
| f_e3e0, f_e412, f_e490, f_e520 | ROM-sized | | 0 |

The cumulative error at `f_e3e0` is -120 bytes. At the start of the session
the unit was off by roughly +390.

## Mechanisms confirmed this session

All of these are ordinary-compiler controls with frozen predictions.

- **Goto-built loops.** `f_e074`, `f_d962` and `f_dbe8` use goto loops: no
  loop weighting, no preheader motion, and invariants in the loop test block.
- **Returning centring helper.** The helper is `#pragma inline static short
  center(char *s, short y) { return f_2bf58(...); }`. Returning the result
  keeps the centred-label arms separate as in ROM. Every `void` helper,
  expression form, block macro, goto else, string-literal label and flag
  variant is factored (research/2026-10-31-tailmerge-*).
- **Short modulo cases.** `c = (g & m) + 1; c %= n;` gives ROM's 16-bit sum
  before the modulo call in `f_e074`.
- **Constant-index clears.** Five constant-index stores in `f_d394` give
  ROM's hoisted `&tab[k]` pointers.
- **Position locals.** Short position locals in the `f_c21a` Y/N loop move
  the position arithmetic before the row test.
- **Pool deferral.** Literal-pool deferral across function boundaries depends
  on the next dump point. `f_d8e0`'s pool lives in `f_d962` (0xd9f0), and
  `f_e074`'s modulo-helper literal is dumped with `f_e39c`/`f_e3b8` at 0xe3d0.
- **Initialiser copy width.** SHC copies a local initialiser in long or word
  moves depending on its offset in the unit `.rodata`, so exact `.rodata`
  layout matters for code size (research/2026-10-31-init-align-probe).

## Open gaps (evidence, no recovered source yet)

- **f_c21a (-110).** ROM loads the Y/N label leaves (r10/r11), constant 8 (r9)
  and `f_2ee0c` (r12) at entry. In the draft the Y/N leaves rank at priority 6
  (counted before unrolling), far below constant 200 (24). ROM's frame is 88
  with a 44-byte scalar area; the draft's is 40.
- **f_c21a, findings without a compile.**
  - *Pool attribution.* ROM dumps `f_c21a`'s tail pool at its own end. The
    pool holds 200, the short table 0xcc/0xbe/0xd0/0xc2/0xb4, `f_e63a`, the
    strings and `f_2f2cc`/`f_2f3dc`/`f_2b4e8`. The best draft defers that pool
    into `f_cac8`'s first dump point, so about 50 bytes of `f_cac8`'s "+26"
    belong to `f_c21a`. Correctly attributed, `f_c21a` is about -60 and
    `f_cac8` about -24.
  - *The outer loop is syntactic.* ROM loads Y (0x6032b64, r10), 8 (r9), N
    (0x6032b68, r11) and `f_2ee0c` (r12) at c2a0..c2a6, just before the
    back-edge target c2a8. That is preheader motion, which goto-built loops
    do not get. The draft's preheader holds 200 (r8), 8 (r11) and `f_2ee0c`
    (r12). The only leaf/constant difference is Y/N versus 200.
  - *The extra frame slot.* ROM's extra 4-byte scalar is the sign-extended
    `sel` temporary. It is spilled to @4 and reloaded with `mov.l` before
    nine `cmp/eq #k` tests, because r8..r14 are all taken. The draft keeps
    it in r9.
  - *The goto test.* A goto outer loop (research/2026-10-31-settings-c21a-goto)
    moves the tail pool to `f_c21a`'s end as in ROM. It still holds 200 in
    r14 and leaves Y/N unregistered, so it was rejected.
- **f_d394 (-32).** ROM keeps `run` (@36), `w4` (@4) and `mode` (@32) on the
  stack; the main loop is syntactic (entered directly).
- **f_cac8 (+30).** The unreachable "Bg is Zero" countdown fragment
  (cb44..cb66) is placed between the initialisation and the setup reached by
  the entry `bra`. The constant-false resource-count form adds code on the
  reachable path.
- **f_cf68 (-12).** Pool placement relative to an `f_e63a` call, and a reload
  of `m` before `m*3600`.
- **f_be28 (+8).** ROM pushes `f_e63a`'s zero arguments from the int-0 web
  (r14). `long`/`char` parameter types are original: unprototyped and `int`
  variants both shrink the `f_a608` region by 36 bytes.
- **f_e074 (+2).** In the unit, ROM's register order implies `sel` and
  `g_606006c` one priority point lower and 64 one higher than the draft.
- **f_dbe8 (-8).** The `b & 0x20` modulo cases and the r13/r10 roles.

## What the near-exact bodies still lack (zero compiles)

Halfword diff of the best draft against ROM, with `.rodata` at 0x6032a3c:

- **f_bc02 (392):** one halfword, the low half of the `f_e412` literal (0xdc16
  vs 0xdc92), caused by the -124 shift. It is exact once the unit is
  size-exact through `f_e412`.
- **f_af8c (872):** four code halfwords at af8e..afa8 (the constant 1 and
  block-move register swap), plus the `f_e3e0`/`f_e412`/`f_e520` literals.
- **f_ad80 (524):** the `f_29bf8(0)` argument at adca, r9/r10 and r2/r3 swaps,
  the `g_6079304`/`g_6079305` pool order and the `f_e3e0` literal.
- **f_b3c0 (1234):** register swaps, the save-slot load at b420, the shifted
  `f_e3e0`/`f_e520` literals, and the `f_2f36e`/`f_20206` callee names.
  The research unit predates commit 76d26c9, which corrected those names in
  `src/`.

## Admission gap

Even with every size exact, the literal-gated bodies need their string and
initialiser literals to equal ROM addresses in `.rodata`. The current strict
proof links `.rodata` after `.text`. A proof mode that validates `.rodata` at
its genuine address would be new proof tooling. This needs an explicit
decision before it is built; it is not assumed here.

Packets: `research/2026-10-31-settings-*`, `research/2026-10-31-tailmerge-*`,
`research/2026-10-31-textmenu-*`, `research/2026-10-31-*-sort`,
`research/2026-10-31-init-align-probe`.
