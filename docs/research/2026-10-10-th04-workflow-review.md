# TH04 workflow review and application to TGM2+

Reviewed 2026-10-10. Reference repository:
[N0zoM1z0/th04](https://github.com/N0zoM1z0/th04), pinned locally at
`e450889ba6dc35d1c3e58eb018f907567d7241fe`. The reference checkout is at
`research/th04-reference` in the local workspace, outside the TGM2+ mirror.
This review inspected source, matching/replay scripts, policy, generated ledgers,
and commit history. It did not independently build TH04: its private DOS targets
and Borland installation were not supplied for this review.

## First establish what TH04 actually completed

The pinned [progress ledger](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/docs/PROGRESS.md)
reports 661/663 accepted exact functions across four artifacts. MAIN has 493/495;
OP 93/93; MAINE 72/72; ZUN 3/3. MAIN's reviewed authored byte extent is
83,444/83,469, reported as 99.97%. That is a selected authored-owner denominator,
not the full executable. Packed OP/MAINE/ZUN function acceptance is a separate
decoded plane, with no claimed raw packed-file byte denominator.

The current [roadmap](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/docs/RE_ROADMAP.md)
explicitly defers carpet and checkerboard exactness and pursues independently
built playable products. The maintained source builds all four executables, but
the README/handoff still list gameplay/rendering validation gaps. Thus this is
an advanced reconstruction with useful methods, not proof of 100% exact C or
complete whole-game behavioral equivalence.

TGM2+'s 68.306610% cannot be directly compared with that 99.97%. Our denominator
remains 200,698 declared code bytes including unresolved regions and approved
runtime assembly. We retain our original ROM gates and exception agreement.

## What the history shows

The repository began on September 6 with target ingestion, toolchain attestation,
binary comparators, Ghidra checks, and ReC98 calibration. The following day's
[“reach 98%” commit](https://github.com/N0zoM1z0/th04/commit/62a56f41)
covered 99/101 reviewed functions and 12,448/12,494 authored bytes at that time.
It did not mean 98% of the whole game was reconstructed in one day.

Later commits repeatedly expanded the reviewed ownership universe, recovered
previously missed functions, and promoted bounded source owners after replay.
The project had a substantial starting advantage: pinned ReC98 source and build
scaffolding. TGM2+ has no comparably complete upstream source corpus. The rapid
TH04 gains are not a realistic throughput guarantee for our SHC reconstruction.

Historical prose also drifts: the accepted-batch note retains earlier snd_load
and dialog blockers that later ledgers supersede. Its current routing docs tell
readers to use ledgers first. This directly supports replacing our long diary as
the work selector with a fresh split-derived queue.

## Producer recovery produced concrete wins

The [accepted reconstruction note](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/docs/reconstruction/main/TH04_MAIN_EXACT_BATCH.md)
describes fixes involving original translation-unit boundaries, declarations,
segment ownership, expression grouping, control-flow shape, and inline helpers.
For example, restoring dialog_init's TU boundary fixed relocation ordering even
though its instruction bytes had already matched. Reviewing raw ownership also
expanded sparse or absent Ghidra function bodies to include real code and tables.

The transferable lesson is to reproduce the complete producer context. Borland
OMF fixup mechanics themselves do not transfer to SHC/SYSROF. In TGM2+, the
analogous observed problems are predecessor state, literal pooling, function
parity, register history, and actual C declarations.

Our own evidence already supports this: `src/time_18708.c` needed the literal
frame address for the expected schedule, and the 191A4 playfield function became
exact after measured SHC priority changes from ordinary loop increments. Those
are stronger guides than copying another architecture's flags or syntax.

## Layered evidence and cold replay

TH04's [oracle policy](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/docs/ORACLES.md)
separates target identity, file/container integrity, layout/relocations, bounded
semantics, compiler behavior, and runtime observations. Supporting evidence
cannot override a failed exact comparison. Negative controls deliberately mutate
different dimensions to check that the comparators reject them.

The [replay driver](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/scripts/replay_th04_main_exact_units.py)
freezes declared inputs and constructs two independent source trees. It checks
owner placement, raw slices, ordered relocations, object integrity, and input
drift. Source and tool identities make a result replayable rather than dependent
on an old object sitting in a build folder.

TGM2+ already compares full ROM output and has strict retained-range proofs.
The added `tools/cold_replay.py` closes a practical reproducibility gap: normal
full-C objects can otherwise survive through mtime caching. The new runner
freezes actual sources/tools, cold-builds twice, and independently checks the
full image and both EPROM identities. It does not change existing acceptance.

## Some final TH04 matches use hybrid source

The [unit ledger](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/config/units.csv)
records the v836 snd_load closure as a 234-byte hybrid owner. Its last register
direction primitive relies on independently justified symbolic assembly.
The [source](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/src/shared/sound/load.cpp)
and separate assembly owners make this distinction inspectable. Likewise,
enemy_bullet_template_push was accepted as symbolic TASM with cross-game
producer evidence after natural compiler probes failed.

These are not demonstrations that endless source permutation solved every C
codegen obstruction. TGM2+ has its own assembly policy and only its approved
runtime exceptions. We cannot copy the hybrid policy to turn our unresolved
register choices into accepted C bytes.

## Preserve negative results and change targets deliberately

TH04's [knowledge guide](https://github.com/N0zoM1z0/th04/blob/e450889ba6dc35d1c3e58eb018f907567d7241fe/docs/KNOWLEDGE_BASE.md)
indexes facts, recipes, hazards, negative results, and open questions with scope
and evidence. Failed experiments narrow the next search. Its actual history
also includes many prolonged final-blocker probes, so merely adopting a ledger
does not prevent a plateau.

For TGM2+ we add an explicit bounded-experiment rule: one falsifiable hypothesis,
a predicted observable, and normally at most 24 compiles. Stop when the relevant
mechanism does not change; a larger batch needs a new recorded justification.
This is our adaptation, not a claim that TH04 used a 24-compile limit.

The `f10788` investigation remains useful compiler research: a forced scratch
exclusion diagnoses the missing R0 pair. It has not produced legitimate exact
source. Further work must explain genuine IR/liveness or predecessor context,
not rerun thousands of equivalent dead-code forms. The previous recommendation
to investigate SHC directly is preserved with this constraint.

## Changes implemented in TGM2+

- Root `AGENTS.md`: durable project instructions, solo/no-brainstorming preference,
  exact coverage contract, producer-first work, and verification/publication.
- `docs/DECOMP_WORKFLOW.md`: operational commands, mismatch routing, experiment
  receipt template, cold replay, and honest handoff requirements.
- `tools/frontier.py`: generated inventory of every assembly interval, exact
  split-based accounting, heuristic source candidates/dependency groups, and
  attached hypotheses. `--check` rejects a stale generated inventory.
- `config/frontier_routes.json`: selected target hypotheses, prior negatives,
  reopening conditions, and bounded experiment policy. It is an initial routing
  ledger, not an assertion that every remaining owner has been analyzed.
- `tools/cold_replay.py`: two independent builds from frozen inputs, ROM checks,
  tool/source receipts, and input-drift rejection.
- `tools/test_cold_replay.py`: negative controls for program/EPROM mutations,
  content-hash changes, and denominator drift.

The fresh queue identifies 42 assembly ranges, including two fully exempt
runtime ranges. At this review's baseline, 61,970 non-exempt bytes remain.
Archive review also established that 2090C's obvious arithmetic/prototype and
combined-context families were already tested. Its route is parked pending a
new source-allocation mechanism rather than spending another batch on them.

This work establishes a more reproducible and less repetitive process. It
does not add matched game bytes by itself. Validation results are in the dated
cold-replay and test receipts, and subsequent actual gains must be admitted
through the original ROM checks and published dashboard workflow.
