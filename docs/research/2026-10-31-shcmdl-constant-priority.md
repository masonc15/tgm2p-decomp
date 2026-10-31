# SHCMDL constant and leaf web priority

Solo work on nephele, 2026-10-31. Coverage is unchanged at **139,096 / 200,698
linked C bytes (69.306122%)**. This note records how SHC v5.0r32's register
allocator (SHCMDL) ranks constant and symbol-address ("leaf") webs. The model
comes from native traces on nuada (REG/ILB native-equal) and from
disassembling `shcmdl.exe`. Several parked routes depend on it: f_e074, f_780,
0x57d4, 0x7658, 0x14d50, 0x14f1c, 0x152d4 and 0x20be.

## Two priority routines

Variable webs (kind 1) are ranked by routine `0x40ff00`, which stores the
result at `0x41019f`:

- For each reference node, the weight `w` is the block field `+0x5c`, taken
  only when the block is its own representative. A move node (op `0x5f`) adds 1.
- The weight is multiplied by `m`. Inside a loop, `m` is 2 plus the loop
  nesting chain length (`+0x30`/`+0x14`). Outside a loop, `m` comes from
  `0x4159b0`.
- `register` storage classes 6 and 8 add `0xffff`.

Constant and leaf webs (kind 0) are ranked by routine `0x415a45..0x415b9b`,
which stores the result at `0x415b9b`. Their priority is a sum over reference
groups (roughly one group per block) of `m × count`, where:

- **m** depends on loop membership:
  - in a CFG loop (block `+0x30` set), `m` is 2 plus the parent-loop chain
    length, plus 1, plus 1 more for a constant node (op `0x71`). So constants
    in a depth-1 loop get m = 4 and leaves get 3.
  - otherwise, `0x4159b0` counts enclosing syntactic loop statements in the
    AST (ancestors with op `0x14`, `0x16` or `0x17`, up to op 4). With none,
    m = 1.
- **count** is the number of nodes in the group's `+0x40` chain. A constant
  used twice in one block counts 2; in f_e074, 180 appears in two calls in the
  same block. A constant node does not count when `0x404cd0(parent,
  operand index, value)` accepts it as an immediate operand; `0x40a4d0`
  returns the operand position. Some op `0x5f` → `0x35` children are also
  skipped.
- A candidate becomes a web only if its priority is at least 2 (`0x415b4f`).
  Single-use constants are therefore never webs; they are materialised at
  their use.
- When `[0x459140]+0x18 == 4` and the type tag is `0x30`, the priority is
  halved.

Profit is the priority minus a cost. Every constant and leaf in the f_e074
captures shows profit = priority − 3; variables show −3 to −5. Physical
registers are then assigned in profit order, each web taking the lowest free
index that does not clash (index p is register r(19−p), so r14 is taken first).

## Immediate-operand exclusion (`0x404cd0`)

`0x404cd0(parent, operand index, value)` decides whether a constant node does
not count. It first requires an arithmetic, compare or assignment parent, then
dispatches on `parent op - 0x40` through the table at `0x4051e0`/`0x405184`:

| Parent op | Excluded when |
|---|---|
| 0x40, 0x50, 0x60, 0x61 | value in -128..127 (`add #imm`, `cmp/eq #imm`) |
| 0x41, 0x51 | value in -127..127 and operand 2 |
| 0x44, 0x54 | 16-bit value, operand 2, with variable-type checks on operand 1 |
| 0x46-0x49, 0x56-0x59, 0x4c-0x4e, 0x5c-0x5e | separate handlers |
| 0x5f (assignment) | operand 1 is op `0x35` |
| 0x64-0x67 (ordered compares) | value 0 and not unsigned |
| 0x78 (call argument) | value in -128..127 and passed in an argument register (`0x4054d0` < 5) |
| others | never |

So `timer < 500`, `i < 7`, `j < 2` and similar ordered loop bounds always
count, and the ROM's unregistered loop bounds in f_780 and f_e074 cannot come
from immediate exclusion. Small constants passed directly as call arguments do
not count.

## Consequences confirmed by compiles

All on f_e074 in the `text_palette_e3b8.c` context:

| Source form | Effect | Result |
|---|---|---|
| goto-built loops | no loop weighting, no preheader motion; common values sit in the loop test (ROM e13c..e14e) | 790 bytes, confirmed |
| `c = (g & m) + 1; c %= n;` | ROM and/add/exts order; 1 and 3 lose their function-wide registers | 794 bytes, raw 510, 220/404 opcodes exact (best) |
| `while` inner loops | 64 weighted into a register, but invariants hoisted to a preheader as `for` loops do | 782, rejected |
| do-while entered at the condition | weighted, no preheader, but `d_3aa9c` and `g_60792dc` are also registered | 786, rejected |
| do-while j loop only | 64 registered and f_2c0d0 not, as ROM, but every def moves to the function entry | 790, 156/404, rejected |
| do-while outer loop too | all defs hoisted ahead of the wait loop | 784, rejected |
| `(b & K) == K` button tests | masks counted twice, but compiled as and/cmp/eq, not ROM `tst` | 804, rejected |
| `char *` helper parameter | byte-identical | rejected |

Any syntactic loop inside f_e074's pass pulls leaf and constant definitions to
the function entry. The ROM keeps them per pass, so its loops are not
syntactic. The ROM's remaining choices are still unexplained:

- 64 is registered while `f_2c0d0` is not;
- 16 and 32 are given r8;
- `b` is copied before testing 0x80 and 0x10.

Together these imply reference counts that the current button-test spelling
does not produce.

f_780 (fn 117 in the startup unit, native trace) shows the same tension. Its
inner loop has strength-reduced cursors, which only syntactic loops get. Yet
the ROM leaves the loop bounds 500 and 7 unregistered. In the `for` draft,
500 scores 20 (4 × 5 tests through `<`, op `0x64`) and 7 scores 10, while the
`"- Warning! -"` label and f_e3e0 score about 9. Goto-built warning loops lose
the strength reduction and move the root to 0x10fc (rejected).

## Packets

The private packets on nephele and nuada contain replay scripts and hashes:

- `research/2026-10-31-priority-watch/`
- `research/2026-10-31-const-priority-watch/`
- `research/2026-10-31-const-count-trace/`
- `research/2026-10-31-modshort-sort/`
- `research/2026-10-31-startup-const-trace/`
- `research/2026-10-31-textmenu-{inner-goto,center-charp,modulo-short,inner-while,inner-do,j-do,all-do,mask-eq}/`
- `research/2026-10-31-startup-goto-loops/`

## Use

A source change can now be predicted from its effect on reference groups:
which block each use lands in, whether the block is in a syntactic loop, and
whether the constant can be an immediate operand. A prediction should state
the expected priority change before compiling. These are evidence controls;
no acceptance or proof rule changes.
