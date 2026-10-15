# Ranking context without a dead assignment (255)

Production remains checkpoint249: 135,910 / 200,698 verified bytes.

The archived best215 rank draft contains `if (0) done = 10`, an excluded
allocator hint. This experiment removes it entirely and retains all nine real
function bodies. No padding, dummy effects, physical register selection,
compiler patches or changed admission rules are used.

The clean 14D50 routine is464 bytes against the original460. Its nominal
460-byte comparison has71 differing bytes; subsequent functions move four
bytes late. Earlier149B4–14B10 and14BCE–14D50 retain734 exact raw bytes, but
four dispatch-table bytes in14B10 change because those function roots move.
This is not a drop-in replacement for the retained dispatch context.

Ten source controls include the clean baseline and nine real-operation changes:
ordinary/compound timer decrement; ordinary loop increments; a bound consumed
by both the final call and loop; a reused phase counter; and an early real
use of the duration in the20-frame argument. Eight complete resolved outputs,
including the baseline, are identical. The other two worsen the target:
ordinary increments give75 nominal differences and the int bound403, with
the latter growing the routine to468 bytes. All fail strict admission.

Original compiler diagnostics show ordinary increments raise both counter
priorities from5 to7 but do not recover the original allocation/size. The int
bound spills. Three normal/debug middle-pass pairs reproduce driver REG/ILB
exactly. Ten Mac native outputs independently reproduce on Nuada, including
resolved bytes, roots and relocations. Original compiler/source provenance
and unchanged ROM hashes are verified. No full production suite was rerun.

This establishes a reproducible clean baseline and closes these decrement,
shared-bound, early-constant and counter-reuse forms. Do not restore the dead
assignment or report the archived96-percent score as a genuine candidate.
The earlier callee screen found no second confirmed error. Further ranking
work needs evidence explaining the original constant/register choice; another
blind spelling or identifier sweep is not supported by these results.
