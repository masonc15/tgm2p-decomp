# Score guard conditional eligibility — continuation172

Production remains checkpoint161: **128,398 / 200,698 bytes (63.9757247%)**.
The best complete score candidate remains13 differing bytes. A new15-difference
control matches the global test and isolates a threshold/level register swap.
Neither passes the strict verifier; this continuation adds no C credit.

## Why the literal becomes eligible

Five original SHCMDL replays observe335 literal/AND candidate inputs at4155C8,
242 immediate-fit calls, and their admission decisions. All observed types are
integer types. A literal reaches the fit check exactly when its node+3C link
points to itself. An AND node or a literal without that self-link exits earlier.
Every observed integer literal with a zero fit result is admitted; this does
not claim to model unobserved type families or the full immediate-fit routine.

| Source | Inputs | Fit calls | Admitted | First global literal |
|---|---:|---:|---:|---|
| baseline | 67 | 48 | 21 | no candidate link |
| positive labels | 67 | 49 | 22 | self-linked, admitted |
| combined-entry label | 67 | 48 | 21 | no candidate link |
| joint early exit | 67 | 48 | 21 | no candidate link |
| global+level positive group | 67 | 49 | 22 | self-linked, admitted |

The 32-bit AND's immediate-fit result is zero on this observed path. The earlier
self-link decision therefore determines whether its literal joins register
allocation. This accounts for continuation171's extra constant1 priority term
and original rewrite to R12.

## The original conditional-evaluation counter

Five additional replays arm a data watchpoint at414CC4, before final allocation
preparation. They check literal opcode71, type1C, value1 and exact node identity
through the4155C8 gate. The entire gate ancestry agrees with the separate
eligibility replay, rather than merely reusing a numeric address.

Original routine419770 first looks for a reusable expression. On its observed
no-hit path,41978E checks byte459634. A nonzero value calls41A180, whose
instruction41A18A clears node+3C. Zero calls419DA0, whose instruction419DB1
sets node+3C to the node itself. The watchpoint records both the write and the
actual caller return address41979D or4197A7.

The counter is1 at the baseline, combined-entry and joint-exit literal writes;
it is0 for positive labels and the global+level positive group. Original
routine419720 visits its first child before incrementing this byte, visits the
remaining children with the increment in effect, then decrements it. This
explains why grouping the global test with later checks does not suppress its
promotion: it remains the first operand of that group. Keeping it after the
mode check does suppress promotion.

The watches observe the final pass reasserting an existing link state; they do
not establish the first chronological construction of that state. An earlier
diagnostic armed before optimization followed storage later freed and reused.
Those three output-preserving runs are retained with
`EARLY_ADDRESS_LIMITATION.md` and excluded from the valid replay count.

## Genuine source controls

Seventeen native controls use the normal compiler and flags. Every object keeps
roots7658/7956, context size1456 and the entire following function unchanged.
All are rejected by the strict prefix verifier. Five independently captured
native objects equal normal builds in every ELF section except compiler comments.

| Control | Target byte differences |
|---|---:|
| baseline / positive labels / combined-entry label | 19 / 13 / 24 |
| positive group: global+level / plus byte flag | 24 / 30 |
| conditional-operator group: global+level / plus byte flag | 24 / 22 |
| negative group of2 /3 /4 real checks | 24 / 22 / 24 |
| joint mode/global early exit, NOT or De Morgan spelling | 15 / 15 |
| combined-entry label with negative remaining checks | 24 |
| joint early exit plus negative remaining checks | 15 |
| joint early exit with reversed level comparison | 24 |
| joint early exit with separate negative level exit | 29 |
| joint early exit with positive level label | 22 |

The three15-difference contexts are identical. Their new guard is:

```c
if (n) {
    if (!((p->w30c & 0x1000) && (g_6064880 & 1)))
        goto bonus_done;
    if (d_38f60 <= p->level && !p->b3a1 && p->l350 <= 0x300c) {
        p->b3a1 = 1;
        p->w38a = 18;
        f_2e6fc(15);
    }
bonus_done:
    f_11cc6(p);
}
```

It preserves the original real checks, order, short-circuit behavior and effects.
No dummy uses, added memory reads, dead stores, padding, volatile access or
physical-register forcing is introduced.

Its six guard differences are exactly778A,778E,7790,7791,7792,7793: threshold
valueR1 versus targetR2, levelR2 versus targetR1. The threshold address already
uses targetR3; the global test, byte flag and cap sequence match. The other nine
differences remain the independent earlier constant2/FFFF allocation problem.

## Why the two candidates choose opposite result registers

Two fresh SHCGEN replays compare the15-byte joint exit with the13-byte positive
labels. The recovered original selector predicts all115 result selections,
preference masks, inherited exclusions and stored slots. Traced SUA/ASA outputs
equal untraced controls; SUA also equals the native capture.

At the threshold, both candidates have zero preferred registers, last-register3,
the same exclusions/used/other/mode fields, and four empty scratch cache slots.
Empty-slot ages differ but do not enter the ranking rule.

| Candidate | Recent mask | Threshold ranks R0/R1/R2/R3 | Threshold result | Level result |
|---|---:|---|---|---|
| joint early exit | 5 (R0,R2) | 3 /2 /3 /3 | R1 | R2 |
| positive labels | 3 (R0,R1) | 3 /3 /2 /3 | R2 | R1 |

Thus the ranking-relevant difference at this selection is the recent-use mask,
not occupied cached values or a hidden register preference. A model-only change
of that mask predictsR2 for the joint exit. No compiler state is changed, and
that calculation earns no matching credit.

## Validation and next action

`tools/shc_probe/cases/session_score_eligibility172/` contains the original
disassembly, pinned compilers, source generation,17 native controls,335 input
observations,5 stable-node watch traces,115 allocator observations,12 valid
output-preserving replays, and `verify.py`/`RESULTS.json`.

Both Nuada production checkouts retain46 pins,58 earlier frozen cases, original
ROM hashes and the fixed report. All23 reference ROM files match their recorded
MacBook source. Last full build/test gates remain checkpoint161's independent
clean builds and245 enabled tests; this research-only continuation claims no
new full-suite run.

The eligibility question is now closed for these controls. Retain both the
13-byte positive-label and15-byte joint-exit candidates. Next trace updates to
SHCGEN's recent-use mask460534 through the matching immediate global test and
the threshold selection. Compare expression-boundary handling with the
register-test control to find a genuine source structure that keeps the
immediate test while allowing the targetR2/R1 result pair. Do not manipulate
the mask or repeat literal suffixes, simple casts, condition groups or the
three rejected level rewrites. The full100% goal remains active;72,300 bytes
remain incomplete, including1,638 runtime assembly bytes.
