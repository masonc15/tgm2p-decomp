# Animation decrement and traversal dependency — continuation147

Production remains checkpoint122:128,316 /200,698 bytes (63.93487%). No C was
integrated. There are72,382 bytes remaining, including the runtime exceptions.

## Recovered constraints

Frozen146 `count_zero_not` has the desired header/setup pair, three R9-to-home0
stores and a zero test from R10, but exchanges mask15 and constant2. Its
explicit `n = n - 1;` adds assignment weight2. Standalone `--n;`, `n--;` and
`n -= 1;` each reduce this to weight1, giving header priority5 while preserving
the zero-condition header read. All allocated sections of these three forms
are identical, but differ in eight text bytes from the frozen146 control.
They still exchange the constants. This rules out count priority alone as the
remaining explanation. All have body1744, frame108 and zero test
`EXTS.W R10,R2; TST R2,R2` at candidate A6B2/A6B4.

The faithfully reproduced allocator has88 candidates for this form. Both
constants tie at priority16/profit8; constant2 sorts sixth and mask15 seventh.
Fixed-graph sensitivity predicts increasing lastpos priority7 to9 would give
all six desired roles. This prediction was tested with a consumed calculation:

```c
/* Before */
lastpos += tbl[lastpos].w2 >> 10;
/* After */
lastpos = lastpos + (tbl[lastpos].w2 >> 10);
```

The hardware trace confirms priority9: initial assignment1, two reads inside
the traversal4, explicit assignment2 (formerly0), later use2. The stock native
result gives holdR10, mask15R9, constant2R8, total on stack, headerR10 and
setup spriteR9 together. It retains frame108, homes sprite_id0/sel4/tbl8/total12
and three actual R9 word stores to home0. Every candidate retains six loops,
no inline-return gotos,36 real exported functions and no unnamed local calls
in the audited body.

## Remaining mismatches

The new candidate still fails the strict verifier. It starts atA604 instead
ofA608 because of the preceding unresolved function and has1740 body bytes
instead of1748. Its zero test reads R10, but uses scratchR3 rather than the
originalR2. Several homes remain wrong: spr1=48 versus16, lastpos=40 versus44,
cnt=56 versus72, last=64 versus60. Its raw relative mismatch count is1488;
that comparison is diagnostic only and is not a byte-credit metric. Recovering
individual constraints must not be confused with recovering the full function.

## Validation

Four native controls give two distinct text hashes. Four watched captures and
eight normal/debug middle replays match stock output. Four CFG audits resolve
stack accesses and preserve the three R9 stores. Eight read-only hardware
replays (sort, eligibility, variable weights and homes for each of two sources)
preserve captured REG/ILB outputs. Models reproduce176 allocation candidates,
54 homes and270 home-frequency terms;100 variable priorities reproduce from
440 weighted terms. No merge occurs in either of these variable traces.

The first capture invocation omitted `--watch`; the subsequent replay correctly
failed on missing watched intermediates. Its error log and three successful
unwatched captures are retained. The watched captures were then produced and
all replay checks completed. Those initial three captures are additional
compiler executions, not three additional source controls.

`verify.py` and `recheck_production.py` pass. The latter verifies44 production
pins and32 prior frozen cases in both Nuada checkouts, unchanged target ROM
hashes and progress, and all23 reference ROM files. Full production builds and
tests were not rerun for private probes; checkpoint122 remains the latest full
gate evidence.

Case: `tools/shc_probe/cases/session_animation_decrement147/`.
Retain `lastpos_explicit` as the next baseline. Inspect initial traversal
scheduling and scratch history, then the real uses determining spr1/lastpos/cnt
homes. Avoid another blind decrement-spelling sweep: its three forms are now
proven identical.
