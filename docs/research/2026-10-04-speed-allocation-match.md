# Speed screen allocation and complete match — continuation177

The complete f_7956 candidate matches all690 bytes at7956–7C08 using the
unchanged strict range verifier. The preceding f_7658 remains unlinked compiler
context with19 differing bytes; none of those bytes receives C credit. The
native source has nominal roots7658/7956, context size1456, exact shared pool
797A–799C, and the original RTS7BDA/delay7BDC. The packaged object exports only
f_7956, has no relocations and no external literal dependencies.

## Compiler evidence and source change

Three SHCMDL hardware-breakpoint replays preserve both REG and ILB output,
equal to fresh untraced controls and captured native output. The recovered
profit computation, partition sort, eligibility results and allocation model
reproduce every observed assignment in the baseline, staged and shift-step
candidates. There are23/25/25 candidate entries and78/83/83 eligibility events.

A staged offset has priority2, which excludes it from saved registers under the
compiler's priority3 cutoff. Both offsets also fail scratch-register eligibility.
Splitting each calculation into a negated product assignment and a consumed
arithmetic right shift gives both variables priority3. The model and native
output confirm that both now remain in registers. This reduces raw target
mismatches from125 in the earlier baseline to37 and restores the terminal and
literal layout.

The final source assigns the quotient inside its first consumed multiply:

```c
v = p->a300[1];
ox = -(k * (t = ((v - 63) * 1024) / 63));
ox >>= 10;
oy = -(t * 8);
oy >>= 10;
f_11680(d_3a8c0[mode], oy + 120, p->x + ox,
        dy, 125, v, v, 0);
```

Every assignment is consumed. The quotient assignment is sequenced before its
later use by the intervening full expressions. There are no dummy reads, dead
stores, padding, volatile tricks, compiler changes, physical-register hints,
or verifier exceptions. Multiplication replaces negative signed left shift;
previous continuation176 exhaustively checked signed16 v, both k values, and
signed32 arithmetic bounds. All RNG calls, arguments and order stay unchanged.
Three whole callees and the original call ABI were authenticated in that case.

This final expression produces exact native output, including the otherwise
incidental R0-to-R4 copy; no dead source copy was added to request it. The graph
replay covers the three controls, not the final exact source, so its detailed
allocation trajectory is not inferred from those traces.

## Bounded native controls

| Source | Raw target differences | Context bytes |
| --- | ---: | ---: |
| shift_steps | 37 | 1456 |
| negate_steps | 112 | 1456 |
| x_shift_step | 88 | 1456 |
| y_shift_step | 170 | 1460 |
| compound_products | 189 | 1464 |
| reverse_declarations | 180 | 1460 |
| product_step | 39 | 1456 |
| quotient_in_product | 36 | 1456 |
| quotient_in_offset | 0 | 1456 |
| commute_product | 37 | 1456 |
| quotient_in_commute | 31 | 1456 |
| shift_reverse_declarations | 37 | 1456 |

All12 original ELF objects are independently resolved and compared with their
linked binaries. The successful default proof includes no tail/switch exception
and keeps exactly one function. Eleven negatives remain excluded.

Production source: src/speed_7956.c. The split map adds the range7956–7C08
between two existing assembly portions. New native tests check its exact bytes,
original exports, protected pool, terminal, relocation-free packaged object,
and rejection when the quotient assignment is separated again.

## Validation record

The first isolated build used the old layout because rsync placed the three
changed files inside an extra tgm2p-decomp directory. Its logs are retained as
old-layout-* and do not validate this integration. The paths were corrected,
the misplaced copies removed after byte comparison, and the corrected isolated
clean build plus247 enabled tests passed with zero skips. The canonical checkout
then passed an independent clean build. Both rebuilt ROM hashes equal the
original.48 production pins agree across the checkouts. Final results are in
session_checkpoint177_speed/RESULTS.json.

Checkpoint177 is129,088/200,698 bytes (64.3195248582%), a690-byte gain.
There are601 matched functions and207 units,163 complete. Remaining71,610 bytes
comprise69,972 non-runtime bytes and all1,638 runtime bytes.

Evidence is in session_speed_interference177; production integration evidence
is in session_checkpoint177_speed. The fixed denominator remains200,698 and
all1,638 runtime assembly bytes remain incomplete. The literal100% goal remains
active, with solo execution as requested by the user.
