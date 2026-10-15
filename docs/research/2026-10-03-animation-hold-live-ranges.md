# Animation hold live ranges — continuation133

Splitting the real hold-counter update into increment and mask operations merges its two compiler live ranges. That produces original `hold` in R10 and mask15 in R9. It still gives total R8, where the original keeps constant2, and shrinks the frame to104 instead of the original108 bytes. No candidate matches.

## Source and observed allocation

The starting source is continuation132's `staged_sprites.c`, SHA-256 `7dad6adceaead00d1d3944f2d2fd8ae42ddcb87be52084356109213657840e9c`. Its two sprite-pointer caches are already removed through real staged address calculations.

For either or both input-repeat paths, replace:

```c
hold = (hold + 1) & 0xfff;
```

with:

```c
hold++;
hold &= 0xfff;
```

A control uses `hold += 1` instead of post-increment. All calculations are consumed by the real counter. The preparation script exhaustively checks the lower twelve result bits for all65536 signed-short inputs, including SHC's two's-complement narrowing. No extra memory access, volatile local, forced register, fake consumer, compiler patch or production flag is introduced.

Before this change, the compiler has two hold candidates with priorities15 and6. After changing either update, it has one hold candidate with priority21, profit12 and internal physical9 (SH2 R10). The native original increments R10 at A860 and A8EC and masks it at A8B8 and A8F6. The result observes the joined candidate and native output; this turn does not claim to have traced the exact compiler instruction that joins the webs.

| Source family | Hold | Mask15 | Total | Constant2 | Local frame | Routine bytes |
| --- | --- | --- | --- | --- | ---: | ---: |
| Original | R10 | R9 | stack | R8 | 108 | 1748 |
| Staged sprite baseline | stack; short-lived R5 | R10 | R8 | R9 | 108 | 1694 |
| First hold update split | R10 | R9 | R8 | no register | 104 | 1682 |
| Second/both hold updates split | R10 | R9 | R8 | no register | 104 | 1678 |
| Both split, while loop | R10 | R8 | R9 | no register | 104 | 1674 |

All candidates start at A604 rather than A608. Even candidates of equal length have to pass exact byte comparison; the unchanged strict verifier rejects all13 cases. No relocated or normalized comparison earns credit.

## Closed controls

Three controls move total's initialization into its actual declaration. They preserve its priority and the selected register roles. Replacing the guarded do/while with an equivalent while loop changes traversal/order but does not recover the original allocation. Three controls use `(flag & 2) == 2` for either/both real flag tests; constant2 remains priority16 and unallocated in the joined-hold source. These forms produce different native bytes but do not solve the allocation problem.

The post-increment and `+= 1` controls with both updates split produce identical full emitted text. Other equality classes are recorded explicitly in `NATIVE_TEXT_GROUPS.json`; equal body lengths are not treated as equal text.

## Model and next source question

Fresh hardware breakpoints observe the actual candidate order, profits, caller-register eligibility and final allocation. The model predicts all1121 preallocation candidates across13 cases exactly; additional records appended during allocation are excluded from this claim.

In `hold_both`, total has priority15,84 interfering candidates with summed priority727, and profit15-floor(727/84)=7. Constant2 has priority16,77 interfering candidates with summed priority700, and profit16-floor(700/77)=7. The observed sort places total before constant2, allowing total to take the last available callee-saved register.

The new fixed-graph diagnostic predicts the four desired roles if total priority becomes14 while hold remains21. This is not an implemented source change or evidence about the original source. Initializing total elsewhere in its declaration did not lower its priority. The next investigation should explain the real interference and tie ordering of total, mask15 and constant2, and identify a consumed source form that changes those relations while retaining the verified original operations. Do not patch priorities or carry over continuation130's different graph.

## Verification

Twelve new native compiles and captures pass. Their24 normal/debug middle replays reproduce captured `.reg` and `.ilb` bytes. The unchanged baseline uses its frozen132 capture and two replays. All13 cases have new hardware sort and eligibility traces, exact36-function register serialization round trips, and resolved control-flow stack audits. Stack checks find no unresolved accesses or stored stack addresses. `verify.py`, `VERIFICATION.json`, `ALLOCATION_ANALYSIS.json`, `CANDIDATE_STACKS.json` and the individual trace files make this reproducible.

Both remote checkouts retain all44 checkpoint122 production pins, eighteen earlier freezes, original ROM hashes and128316/200698 matched bytes. All23 copied ROM-reference files still match their recorded source hashes. No new production test claim is made; checkpoint122's clean builds and243 enabled tests remain the last production build/test gate.

**New verified C bytes:0. Progress remains63.93487%. The goal is active.**
