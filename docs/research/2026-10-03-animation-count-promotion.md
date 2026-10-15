# Animation count promotion — continuation129

The copied animation count's type changes SHC's reuse of its promoted value.
This is concrete compiler evidence, but all nine native candidates still fail
strict matching. Production remains checkpoint122, 128,316 / 200,698 bytes.

## Source controls and behavior

The frozen genuine 36-function unit from continuation125 is the baseline.
Within `f_a608`, `short n` reads the header count, and `long cnt` copies it.
There are two subsequent uses of `cnt`: the initial traversal bound and the
later `total >= cnt` wrap check. Neither local is subsequently assigned or
address-taken. Both uses are preserved in the experiments.

Changing `cnt` to signed short preserves its entire input range. Changing the
loop index `kk` to signed short is also range-preserving: for every signed-short
count, the loop exits at an index between 0 and 32766. `RANGE_CHECK.json` records
the exhaustive 65,536-value bound check. No signedness changes, dummy uses,
forced registers, padding, compiler patches, or production flags were used.

An initial source-generation guard caught an overlooked fourth `cnt` reference
before writing candidates. The direct-count variant was corrected to replace
both uses after inspecting the entire routine. A separate follow-up invocation
initially lacked its input files because of a local working-directory error;
the failed log is retained. Neither error affected production.

## Native outcomes

Every candidate starts at A604 rather than original A608. The original function
is 1748 bytes with a 108-byte local frame. These sizes exclude saved registers.

| Candidate | Body bytes | Local frame | Zero-test value |
| --- | ---: | ---: | --- |
| Baseline, long count | 1680 | 112 | Stack long |
| Int count | 1680 | 112 | Identical native output to baseline |
| Short count | 1736 | 116 | Short in R9 |
| Remove count copy, use n twice | 1676 | 112 | Stack int |
| Short loop index | 1680 | 112 | Stack long |
| Short count and loop index | 1736 | 116 | Short in R9 |
| Short count and chained brightness | 1676 | 112 | Short in R10 |
| Long count, loop bound uses n | 1740 | 116 | Stack long |
| Short count, long cast in wrap comparison | 1736 | 116 | Identical native output to short count |

The chained brightness stores preserve their original assignment order, as in
continuation128. They lower constant63's priority from 18 to 9. Combined with
the short count, the original `exts.w r10,r2` zero-test instruction appears at
A6AE, compared with original A6B6. This is a useful local correspondence, not
a byte match at the required address or proof of the original source.

## What the compiler records prove

Six independent normal driver captures equal their regular compile outputs in
every native ELF section except the build comment. Twelve middle-pass replays
(six ordinary, six with diagnostic logging) reproduce the captured `.reg` and
`.ilb` files exactly. All six complete register tables round-trip through the
decoder/encoder. No diagnostic output is consumed by production.

In the baseline, SHC rewrites `n == 0` to test anonymous **long** leaf74, created
by promotion at `cnt = n`. Removing the count copy leaves anonymous **int**
leaf73 and the stack reload; spelling the loop bound with `n` while retaining
the long copy produces another anonymous long and still reloads the stack.

With a short count, the zero-test tree instead contains an explicit int cast
of named short `n`. The emitted register table merges symbols178 (`cnt`) and
185 (`n`) into the same register entry. Both short-count controls and the
brightness combination exhibit this behavior. Casting the later comparison
back to long does not change native output. Thus these experiments isolate the
effect to the copied count's type and its resulting promotion/copy handling,
not merely the comparison's width or the loop index's width.

The observations do not locate the exact middle-pass instruction that performs
the rewrite or prove that the original source declared `cnt` as short.

## Next evidence target

The original loads constant2 into R8 at A7EE and consumes it in the two step
selections and input masks. The combined draft allocates R8 to `total`, while
constants5 and15 occupy R10 and R9 over their later live ranges; constant2 is
unallocated. All three constants have priority16/profit7 in the diagnostic
allocation list. `total` has priority15. Register reuse across disjoint live
ranges means these priorities alone do not explain the outcome.

Next inspect those live ranges and the real `step` arithmetic types. `step` only
takes values1,2,5, so signed int/short type controls can be evaluated without
changing its range. Verify all uses, including unsigned loop-index comparisons
and conversion back into short `total`, before trying them. Keep the original
division calls and control flow; do not delete the repeated step5 branches just
to lower a constant's allocation priority.

Evidence is in `tools/shc_probe/cases/session_animation_count129/`: nine native
compiles, six captures, twelve exact middle replays, linked disassemblies,
source reproduction guards, and strict rejections for all nine candidates.
The production receipt rechecks all44 pins, original ROM/report hashes and
fourteen prior frozen cases in both Nuada checkouts. No production C, split,
proof, target ROM, or denominator was changed. The full goal remains active.
