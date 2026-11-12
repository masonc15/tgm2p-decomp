# First-principles review (2026-11-12)

Solo work on phrixus. Coverage is unchanged at **139,096 / 200,698 linked C
bytes (69.306122%)**; this round added 0 bytes. The owner asked for a fresh
attack on the whole problem with current outside research. This note records
what was tested, what was ruled out, and a different way of working that moved
a stalled function a long way in one sitting.

The private packet is `research/2026-11-12-first-principles/`.

## What the outside work says (November 2026)

Read in full: the atolm findings ledger and Chris Lewis's long-tail post.
Read in part: the toolchain and compiler-model sections of Baumgarten's note,
and excerpts of the Mizuchi write-up and the Echo paper.

- **atolm** (Panzer Dragoon Saga, also Hitachi SHC) found that its game was
  built with an SHC older than the surviving Release31, proved it with a
  controlled recompile of a library source, and measured that only about a
  third of its distinct code is reachable with Release31. It also found the
  same translation-unit effects this project documents: `bsr` distances, shared
  pools, and code generation that depends on the functions before it.
  <https://github.com/blythos/atolm/blob/main/docs/ATTRIBUTION_AND_FINDINGS.md>
- **Baumgarten, "Matching Decompilation as a Puzzle"** (Zenodo, v1.4,
  2026-10-31) describes the method closest to this project's: run the original
  compiler under instrumentation, find the tie that decides a difference, then
  compute the source change instead of searching for it. Two of its rules apply
  directly. Prove the compiler gives the same output across environments before
  trusting any near miss, and when a function stalls, rewrite it from the
  assembly instead of patching the old draft.
  <https://doi.org/10.5281/zenodo.23806061>
- **Echo** (arXiv 2609.18706) and **Mizuchi** both treat the compiler as a
  cheap exact judge and generate many candidates against it. Mizuchi reports
  74% of 60 functions matched on two projects.
  <https://arxiv.org/abs/2609.18706>, <https://github.com/macabeus/mizuchi>
- **Chris Lewis** reports a plateau near 75% on an N64 game, with large
  functions and macro-heavy code left over, and found that showing the model
  similar matched functions helped more than ranking by difficulty.
  <https://blog.chrislewis.au/the-long-tail-of-llm-assisted-decompilation/>

The academic decompilers (LLM4Decompile, SK²Decompile, Idioms) report
re-executability, not byte identity, so their numbers say nothing about this
project's remaining ranges.

## Global causes, tested and ruled out

Fifty-two zero-gain campaigns could mean one shared cause. Three were testable.

**The compiler does not depend on its host.** SHC's passes are 1998 Win32
programs linked with the Visual C++ 4.2 runtime, and a compiler that reads
uninitialised heap memory would behave differently under wibo than on the
original Windows. `tools/shc_probe/hostcmp.py` compiled 204 units (163
`src/` files and 41 `wip/` drafts; the 8 files that need asmsh are left out) with
wibo 1.2.0 on phrixus and with the same executables run natively on Windows 11.
All 204 give identical `.text`, relocations and symbols. Native Windows is also
a fast oracle: 204 compiles in 41 seconds on one thread.

**No available release fits better than v5.0r32.** Compiling 212 units (169 `src/` files and 43 `wip/` drafts) with all eleven
releases:

| Release | Built | Units with code identical to r32 |
|---|---|---|
| v5.0r10 | 1998-04-16 | 89 |
| v5.0r26 | 1998-06-24 | 162 |
| v5.0r28 | 1998-07-13 | 162 |
| v5.0r31 | 1998-09-25 | 212 |
| v5.0r32 | 1998-10-19 | 212 |
| v5.1r01 | 1998-11-09 | 193 |
| v5.1r03 | 1999-02-07 | 87 |
| v5.1r04 | 1999-04-14 | 90 |
| v5.1r08 | 1999-07-15 | 90 |
| v5.1r11 | 2000-02-07 | 92 |
| v5.1r13 | 2000-09-04 | 92 |

r31 and r32 are indistinguishable on this corpus, so the game's compiler sits
in the September to November 1998 window of Hitachi's code. A build from that
window that nobody has preserved cannot be excluded, but nothing here points
to one.

**The later releases' apparent wins are pool effects.** `versweep.py` scored
every unresolved function in every draft under six releases. Under v5.1r03 and
v5.1r13, `f_e412` and `f_e490` become exact and `f_e3e0` drops to 4 halfwords;
`f_20be` improves by about 390 halfwords under v5.1r01. In each case the later
release only flushes the literal pool at a different point, which happens to
give the function the ROM's start parity or keeps a pending literal out of
reach. These are layout facts about the draft's predecessors, not evidence for
another compiler. They are useful as a diagnostic: a body that goes exact
under v5.1r03 is right and is waiting on its predecessors.

C++ is excluded by evidence already in the tree: accepted units depend on
unprototyped calls.

## The shape of what is left

The halfword residual in `funcscore.py` and `flag_sweep.py` compares by
position, so one extra instruction makes the rest of a function count as
wrong. `tools/shc_probe/triage.py` aligns instructions instead (pool words
skipped, branch targets and pool displacements masked) and reports each
unresolved function's best tracked draft twice, with and without register
numbers. For the 61 unresolved functions that have a draft in `src/` or
`wip/` (46,550 code bytes):

| Class | Functions | Bytes |
|---|---|---|
| far: under 80% of instructions align even with registers masked | 22 | 26,428 |
| mid: 80 to 95% | 24 | 12,230 |
| near: 95% or more | 14 | 7,450 |
| registers only | 1 | 442 |

So more than half of the drafted bytes are in functions whose source shape is
still wrong, and the functions the recent campaigns studied at compiler level
(`f_10788`, `f_1050e`, `f_12e2`, `f_3c88`, `f_7658`) are in the smallest
classes. Drafts in `research/` are not counted, so some functions have better
private drafts than this table shows.

## Enumerating source forms: f_d962

`f_d962` (620 bytes, a four-item menu in the settings unit) was "far": 118 of
269 instructions aligned. Reading the ROM listing from the top with
`tools/shc_probe/fdiff.py` showed three things the draft did not have.

1. The main loop is entered by a jump to its test, keeps loop weighting, and
   has no preheader: the values it holds in `r8`, `r11`, `r13` and `r14` are
   loaded inside each pass.
2. `f_2bf58(s, y, (320 - f_2c0d0(s)) >> 1)` evaluates `s` and `y` before the
   inner call and spills them, as parameters of an inlined helper do. Written
   out by hand, SHC reloads the constants after the call.
3. `run` lives on the stack for the whole function.

A script then built the function from interchangeable parts (loop forms,
helper or not, which locals are memory-resident, declaration order, switch
shapes), compiled every combination and ranked them by alignment. Three rounds,
about 2,400 compiles, two minutes each:

| Form | Instructions equal (of 269) |
|---|---|
| tracked draft | 118 |
| main loop as `goto test; do { ... test:; } while (run);` | 208 |
| plus a `static` centre-print helper, `(void)&run;`, and the three-way choice as an `if` chain | 233 |

The best variant is 261 instructions against the ROM's 269. What remains is
specific: the ROM dispatches the three-way choice as a switch but keeps three
unmerged copies of the call (every switch form tried gets its tails merged),
it loads `d_3a8f4` and `g_606006c` at their first use instead of at entry, and
its five stack slots are in a different order. The variants and rankings are in
`research/2026-11-12-first-principles/d962/`.

This earns nothing until the settings unit lines up, and the do-while entered
by `goto` is unlikely to be what the programmer wrote. It does show that the
unit's "far" functions respond to source forms, and that a few thousand
mechanical compiles find in minutes what a 24-compile campaign cannot reach.
Replacing all 13 hand-expanded centre prints in the unit with the helper, with
no other change, already adds 22 aligned instructions to `f_e074`.

## Working method from here

Forms before registers. For a far function, read the ROM listing, write down
each structural fact the draft lacks, and enumerate the forms that could
produce it, scoring by alignment. Only once a function is register-only does
allocator evidence pay.

Enumeration is not a campaign. The compile caps exist to stop unprincipled
sweeps by an agent; a scripted product of named forms is cheap, reproducible
and leaves a ranked table of what was tried. The acceptance rules are untouched.

Work by unit, largest first. The settings unit (0x9c0c to 0xe74f) holds 21 of
the 61 functions and about 16 KB. Its menu functions share the two forms found
here, so the next step is to apply them to `f_dbe8`, `f_e074`, `f_a608`,
`f_c21a`, `f_cac8` and `f_d394`, then come back to sizes and parity.

Open questions worth a decision or a search: what ordinary C gives SHC a loop
entered at its test without a preheader (a `continue` gives the entry jump but
not the rest), and what keeps identical switch tails apart.

## Tools added

- `tools/shc_probe/fdiff.py DRAFT.c FUNC`: aligned side-by-side listing of one
  function against the ROM, with pool values resolved; `--summary` for one line.
- `tools/shc_probe/triage.py OUT.json`: the table above.
- `tools/shc_probe/versweep.py OUT.json`: per-function residuals under several
  releases.
- `tools/shc_probe/hostcmp.py make|compare`: the cross-host check.

None of them grants credit or changes a proof.
