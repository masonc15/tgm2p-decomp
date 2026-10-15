# Joint allocation experiments: correct loop registers, rejected setup

Continuation103 applies both recovered allocation stages to the object-list
routine251CC. It produces a genuine candidate with the required loop registers
and R14→R1 remapping, but an extra setup instruction makes the function84 bytes
instead of82. All15 native candidates fail the unchanged target proof. Verified
production remains127,276/200,698 bytes (63.41668%); no new bytes are credited.

## Model predictions and observed source effects

The fixed graph from condition_outer101 has priorities i7, l3, k11, s11,
zero2, hardwarebase7, next-address4 and bound1274. Searching single and paired
priority changes in1..21 finds seven single-value targets for the desired
internal allocation: hardware5, s9, i10, k14, next7, k15 or k21. These are
counterfactual targets under an unchanged graph, not claims that corresponding
C exists or that its instructions will match. `model_targets.py` reproduces the
original allocation before searching.

Consuming the genuine k increment in the hardware-store index drops k11→8,
rather than raising it. Consuming the genuine next-list assignment in the
loop condition drops s11→8. Signed-short casts preserve the original narrow
index transition when incrementing before computing the previous index; these
forms still introduce unwanted instructions. Replacing final predecrement with
a consumed compound decrement leaves k11, and the complete context is identical
to its predecrement control. A separate outer load raises s8→10 but still misses
the joint target.

Naming the hardware pointer reaches priority5, with measured contributions
1+3+1. It obtains the desired main allocation:

| Value | Physical register |
|---|---|
| k | R4 |
| s | R5 |
| i | R6 |
| hardware pointer | R7 |
| next-table address | R14 |
| bound127 | R13 |
| bucket pointer | R12 |

The compiler also creates a distinct address constant with priority2 that
shares R4 before k becomes live. It loads the constant into R4 and copies it to
R7. The resulting extra move, initial instruction scheduling and pool movement
invalidate the candidate. The native traversal loop is byte-identical for34
bytes only after accounting for its two-byte displacement: ROM251E6–25208 versus
candidate251E8–2520A. This is diagnostic evidence, explicitly not matching-byte
credit or permission to relocate the comparison.

A hardware trace of unchanged SHCGEN confirms the desired late mapping
R14→R1 over compiler range7–13. The continuation102 model reproduces all nine
normalized table positions, the mapping and final scratch free intervals.
Traced/control outputs agree, and the reconstructed front/middle stages allow
`.sua` to match the original native capture exactly.

## Source controls

| Source | Nominal target byte differences | Function bytes | Both retained ranges pass |
|---|---:|---:|---|
| increment_in_store |54|86|No|
| consumed_next_condition |57|88|No|
| consumed_both_updates |55|88|No|
| terminal_pointer |23|82|Yes|
| terminal_value |28|84|No|
| hardware_alias |71|84|No|
| separate_load_compound_final |55|88|No|
| consumed_load_compound_final |57|88|No|
| inner_goto |55|80|No|
| both_goto |82|88|No|
| hardware_consumed_init |67|84|No|
| hardware_after_stores |67|84|No|
| hardware_after_bucket |76|84|No|
| hardware_address_helper |26|82|No|
| hardware_init_helper |75|86|No|

The explicit inner backedge lowers k/s to8 and hardwarebase to6; spelling both
loops with backedges lowers k5/s4/i4, spills the bucket pointer and removes the
hoisted next/bound candidates. Thus loop spelling changes the compiler's
recognized loop weights, but does not selectively adjust one modeled priority.
The branches express the actual original loops; no neutral branch or fake use
was added.

The two new inline helpers perform real work: returning the hardware address,
or writing the initial0/1 entries and returning their base. They are fully
inlined, with the nine original context functions verified. The address helper
avoids the extra two-byte function length but changes allocation, scheduling,
addressing and the following retained range. Neither is a new best. Ordinary
hardware initialization placement and consumed initialization also fail.

## Validation and next target

All15 original compiler captures agree with their independently compiled native
ELF sections. Thirty normal/debug middle replays reproduce original `.reg` and
`.ilb`;128 score checks pass. Twelve hardware priority replays authenticate65
constant contributions/22 totals and92 variable contributions/26 totals. The
late remapping replay has four snapshots. Each proof uses the original ROM,
fixed context addresses, production flags and unchanged verifier.

Direct middle replay of the first inline helper initially failed output equality
using post-compilation cleanup inputs. Reconstructing its original frontend
input resolves this: `replay_helper_middle.py` runs unchanged SHCFRT first,
then checks both normal and diagnostic SHCMDL outputs against the original
captures for both helpers. Failed equality was never treated as a valid trace.
The raw comparison analyzer also uses absolute-address subtraction for the
shifted loop window, avoiding confusion between function and context offsets.

The best eligible nominal object candidate remains named_next101 at9 differences;
none of these rejected sources replaces it. These specific store/index,
terminal-pointer, hardware-alias/initialization/helper and explicit-backedge
forms are now closed. The model does not establish that every possible source
form is exhausted.

Next, apply SHCGEN scratch/emission tracing to the genuine50-byte checksum21170
in canonical `src/savestatus_213da.c`, preserving the surrounding accepted code.
Continuation68 already rejected12 final-result/word-load/accumulator helpers;
do not repeat those. Its seven-byte baseline mismatch concerns temporary
registers and adjacent load/add scheduling, so inspect the compiler state that
selects those temporaries before proposing new source forms. The older four-byte
checksum draft contains an inert branch and is not the baseline to promote.

All33 production pins and original ROM/report metrics remain verified in both
remote checkouts. No broad test rerun is claimed for these rejected research
candidates. Evidence: frozen `tools/shc_probe/cases/session_obj_joint103`.
The full100% goal remains active; work continues solo without brainstorming.
