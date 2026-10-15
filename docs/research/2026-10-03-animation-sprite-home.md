# Sprite home and counted brightness references — continuation149

Production remains checkpoint122:128,316 /200,698 bytes (63.93487%), with72,382
remaining. None of these probes qualifies for integration or byte credit.

## Observed home-frequency mechanism

Frozen148 c2_mask_explicit places spr1 at stack48. Its ordinary-variable
frequency is4: two setup uses attributed to the defining call and one counted
brightness use weighted by loop depth. Changing the existing brightness read
to consume a short pointer to spr1 makes it an addressed memory entry. This
counts seven weighted uses: initial assignment1, first call1, initial flag1,
brightness reference2, final call2. It moves the home to32 but loses other
allocation constraints and reduces the frame to104.

Two distinct references consumed by the two real brightness writes add two
weighted uses, giving frequency9 and spr1home16. With explicitly staged signed
long offsets this also retains frame108 and preceding homes sprite_id0,sel4,
tbl8,total12. Both addresses and every temporary are consumed by real field
writes; no dummy references, padding or forced allocation are used.

## Nine native controls

| Control | spr1 frequency/home | Frame | Body bytes |
|---|---|---:|---:|
| brightness_ref |7 /32|104|1748|
| brightness_split_refs |9 /16|108|1762|
| brightness_array_refs |9 /16|104|1756|
| brightness_preload_refs |9 /16|108|1762|
| brightness_shared_ref |7 /36|108|1762|
| brightness_shared_array |7 /36|108|1756|
| brightness_direct |5 /40|108|1728|
| brightness_split_values |4 /48|108|1758|
| split_refs_decrement |9 /16|108|1762|

The preloaded form computes both referenced offsets before either write; it
still emits two spr1 loads and two address calculations. This rules out write
ordering alone as the cause. Sharing one pointer drops the counted address
uses and spr1's frequency back to7. Ordinary direct accesses and two staged
value offsets do not supply sufficient frequency either. All nine native text
hashes are distinct; no identity claim is based on equal lengths or distances.

The split-reference form exchanges constant2/mask15. Its verified89-candidate
allocator model predicts that header priority+1 restores all six desired roles.
Replacing its existing `--n;` with consumed `n = n - 1;` raises observed header
priority5->6 and produces `split_refs_decrement`. The second89-candidate model
reproduces that result. This candidate simultaneously has:

- holdR10, mask15R9, constant2R8, total on stack, headerR10, setup spriteR9;
- first five homes0/4/8/12/16 and frame108;
- native `EXTS.W R10,R2; TST R2,R2` at candidate A6B2/A6B4;
- three real R9 word stores to home0.

## Exact remaining difference identified

A CFG stack audit of the original ROM resolves one spr1home16 write and four
static read sites: A748,A7BE,AC7C,ACA0. The new candidate has one write and five
static read sites: A740,A7B4,AC70,AC84,ACAA. AC70/AC84 are the two brightness
loads, whereas the original AC7C supplies the shared brightness calculation.
These are static instruction sites, not counts of runtime loop executions.
The retained comparison is `SPR1_STACK_COMPARISON.json`.

The candidate is14 bytes too long (1762 versus1748) and still startsA604 rather
thanA608. It also swaps x/y homes (x24/y20 versus original x20/y24) and retains
wrong lastpos40,cnt56,last64 homes (original44,72,60). The extra brightness load
is concrete evidence for the next compiler investigation, not proof that it
alone explains every remaining byte. Candidate148 c2_mask_explicit retains the
original1748 length and xhome20 but has spr1home48; preserve both controls.

## Validation

Nine native compiles/captures,18 normal/debug middle replays and nine candidate
CFG audits pass. All have36 exported real functions, six detected loops, no
inline-return gotos and no unnamed local calls in the audited body. An
additional original-ROM CFG audit supplies the exact spr1 comparison.

Thirteen read-only hardware replays preserve captured REG/ILB: nine home traces
and two each for sort/allocation and register eligibility. Home models reproduce
242 records and1210 weighted contributions, including the array-reference
case's distinct record count versus shared physical homes. Allocation models
reproduce178 candidates across the split-reference and explicit-decrement forms.
`verify.py` regenerates source, checks captures and REG round trips, reruns
these models and native constraints, and requires every strict context rejection.

`recheck_production.py` confirms44 production pins and34 prior frozen cases in
both Nuada checkouts, unchanged target ROM/report hashes and all23 reference
ROM files. Full build/test gates were not rerun for private probes;
checkpoint122 remains the latest full gate evidence.

Case: `tools/shc_probe/cases/session_animation_spritehome149/`.
Next inspect the compiler's treatment of the addressed brightness loads and
why their two consumed references are not shared into the original calculation.
Preserve the recovered stack and register constraints when testing a supported
source transformation. Avoid repeating the nine already measured variants.
