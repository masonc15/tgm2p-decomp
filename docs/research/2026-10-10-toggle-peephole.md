# Toggle: locate the residual in SHCPEP

Fresh `make split check report` passes both original EPROM hashes and program
SHA-256 `d194be16dc778cdc21fe3507b3e6253f69696a08e7d4d9bf031bbe69b45896c7`.
Coverage remains **137,090 / 200,698 = 68.30660993133962%**. New accepted bytes:
**0**; change **0 percentage points**. Canonical source and splits are unchanged.

## The middle-pass hypothesis was too early

The genuine full-context candidate from the preceding investigation was captured
with the ordinary, unchanged SHC driver. Independent assembly and relocation
resolution reproduces its 476-byte strict context exactly. A normal SHCMDL replay
and a diagnostic-flag replay produce identical REG and ILB files. The register
file decoder round-trips every byte.

The final middle tree still contains the occupied-slot load (`qualify short`,
offset 8, pp42). The two allocator result webs are separate: logical registers
6 and 7 both use physical R4, at disjoint expression intervals [5,6] and [9,10].
They are not a single merged result web. Thus middle-pass CSE/result merging is
not the direct cause proposed in the earlier note.

Read-only hardware breakpoints then observed SHCGEN output. In the toggle's
47 recorded emissions, task+8 loads occur at indices **13,21,33,37**. Index21
is the occupied-slot reload. Both allocator calls are followed by an R0-to-R4
copy and an R4-to-R0 copy before storing. GEN retains the reload too.

SHCPEP output has three such loads (indices **11,27,30** of its 41 recorded
operations). A separate deletion trace observes the occupied reload passed to
`FUN_0041ae40` from **0x409611**, inside **FUN_00409450**. That routine removes
a branch prefix after dependency checks. The first call's inverse result moves
are deleted from **0x40c2cf / 0x40c2e4**, inside **FUN_0040c0c0**. Later store-tail
sharing and copy simplification leave the second R0-to-R4 copy in place.

These are observations of this exact candidate. They do not establish that
all equivalent sources must behave this way. The inferred next mechanism is
instruction order at entry to the occupied block: GEN emits load/copy before
stride104, while the ROM places stride104 before the load/copy. A natural source
that changes that order may stop PEP's prefix elimination. No side-effect,
volatile, instruction patch, or compiler modification was introduced to do so.

## Bounded source controls

All counts compare the fixed original [0x277d0,0x27862) interval, including its
pool, and protect [0x2774a,0x277d0). Every control failed the unchanged strict
proof. Actual object roots and independently resolved context bytes were checked.

| Packet / control | Target differing bytes | Protected differing bytes |
| --- | ---: | ---: |
| Branch baseline | 20 | 0 |
| Explicit inner else | 20 | 0 |
| Explicit outer else | 20 | 0 |
| Explicit inner and outer else | 95 | 14 |
| Explicit occupied-slot goto | **18** | **0** |
| Separate branch-local results | 20 | 0 |
| Shared consumed slot local | 20 | 0 |
| Goto: direct first result store | 18 | 0 |
| Goto: direct second result store | 38 | 7 |
| Goto: direct both result stores | 25 | 7 |
| Both direct stores: assignment inside index | 89 | 14 |
| Both direct stores: byte-offset local, either product order | 102 | 14 |
| Both direct stores: multiply slot in place | 36 | 7 |
| Both direct stores: record pointer local | 79 | 14 |
| Both direct stores: inline scaled-offset assignment | 89 | 14 |

There are **18 ordinary source controls** across the three packets, including
repeated baselines, plus one ordinary diagnostic-capture compilation. The stage
replays are separate from that count. The source budget for each hypothesis was
written before execution (7, 4, and 7 compiles).

The explicit goto restores the first allocator call's R3 scratch choice and
preserves all ten genuine roots, including successor 0x27862. Its 18 differences
are the remaining reload/result-copy sequence; it is the best protected candidate.
Direct stores remove the second result copy while keeping both calls, but still
lose the reload. Their pool moves two bytes early, breaking seven setter literal
displacements. Equal successor roots do not excuse these failures. The six
consumed-index/address forms do not recover the missing sequence. Stop them.

## Evidence and replay

Canonical packets under `research/`:

- `2026-10-10-toggle-middle-pass/`: complete capture, MDL replay, GEN emitter,
  PEP output and deletion traces, independent assembly authentication, verification.
- `2026-10-10-toggle-branch-pep/`: seven source controls; best `inner_goto.c`.
- `2026-10-10-toggle-goto-results/`: four result-store controls.
- `2026-10-10-toggle-prefix-order/`: seven address/order controls.

Each traced GEN/PEP output agrees with a fresh untraced control. Native SUA and
PEP SUB/ASB match the original capture. GEN ASA cleanup-position differences
are recorded and are not mistaken for instruction differences. Compiler hashes,
source hashes, commands, raw residuals and strict rejection messages are retained.
The final verifier independently resolves all 18 ordinary objects and checks all
reported roots, protected bytes, raw residuals, source hashes and stage evidence.

```sh
/drive2/tgm2p/venv/bin/python research/2026-10-10-toggle-middle-pass/verify.py
```

It writes `VERIFICATION.json`; `passed: true` is required. Research scripts that
regenerate packets should be copied to a new directory before rerun. No full test
suite or cold integration replay was claimed for rejected sources. No dashboard
increase was published because no linked-byte increase occurred.

## Next route

Park the tested source forms. Reopen toggle tuning only with a predicted change
to SHCPEP prefix eligibility or genuine GEN ordering, not another spelling sweep.
The exact observed deletion routine and source-backed 18-byte candidate provide
replayable controls. Any future widened allocator declaration still needs its
actual definition/callers reconciled before integration.

A frontier review also recovered prior 20a9c evidence missing from the route
index. Its best known control has 13 differences and preserves the 248-byte
prefix. Checkpoint242 already explained the y-load scratch reset and rejected
unsigned-sum boundaries. Its next unexplained mechanism is recipe scratch
planning at 0x420900 / caller 0x421159. Read that evidence before choosing it;
this turn did not rerun those old probes or claim their historical metric current.
