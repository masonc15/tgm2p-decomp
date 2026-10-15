# Score guard cache eviction — continuation164

Continuation163 made evidence progress by identifying scratch-cache occupancy at the first bonus-guard mismatch. This continuation observes the writes that create and remove that occupancy. Production coverage remains checkpoint161:128398/200698 bytes (63.9757247%),72300 bytes remaining.

## The actual cache policy

Read-only hardware watchpoints observe the R2/R3 flag bytes at460579/460591. Separate hardware execution breakpoints observe the cache-capacity routine42AD80 before and after its search. No compiler instruction or data memory is written by either tracer.

The routine scansR3,R2,R1,R0, counts records whose value is nonzero or whose flags contain40hex, and remembers the smallest age. It replaces the remembered record only for a strictly smaller age, so ties retain the first encountered register. When exactly two records are occupied it clears the oldest before installing the incoming value. This maintains at most two cached values on this path. The model reproduces all80 observed decisions, including three actual capacity evictions across the two sources.

In the baseline guard, the4096 mask inR3 and global-address record inR2 both have age36. Installing the threshold address triggers eviction ofR3. That eviction occurs inside cache installation, after scratch-register selection has already selectedR0. It therefore cannot retrospectively give the threshold addressR3. The split-after-flags control flushes the pair before the next expression, but its separate history chooses another wrong register sequence. Its raw mismatch count is24 rather than19.

Watchpoint observations also distinguish flag operations:

-42AF8A writes40hex, marking an installed cache value.
-42B059 and42B141 clear the cached-value flags.
-413AF5 clears only80hex; an existing40hex remains occupied.
-432047 sets80hex while preserving40hex.
-41158A copies a record word;4115AF zeroes record storage.

The verifier pins these exact instruction bytes from the original PE image and checks each of1161 watched writes against the appropriate operation. It does not misclassify every write as an invalidation. `SHCGEN_CACHE_DISASSEMBLY.txt` and `SHCGEN_CACHE_FLAGS_DISASSEMBLY.txt` preserve the relevant original disassembly.

## Consumed-source controls

The two fresh baselines and eight new controls all fail the normal prefix verifier:

| Source | Nominal-position differing bytes | Context bytes |
|---|---:|---:|
| Baseline |19|1456|
| Split after flags |24|1456|
| Inline-requested flag predicate returningint |482|1468|
| Inline-requested flag predicate returningshort |472|1472|
| Inline-requested threshold predicate returningint |716|1472|
| Inline-requested threshold predicate returningshort |714|1472|
| Inline-requested complete guard predicate |524|1468|
| Numeric address for global flags |263|1460|
| Numeric address for threshold |19|1456|
| Both numeric addresses |263|1460|

Predicates preserve the same reads, ordering and short-circuit conditions. Their results are Boolean0/1, so int/short return types preserve values. The numeric reads use the original types and linked addresses:6064880 for the unsigned-long flags,38F60 for the signed-short threshold. Numeric threshold access produces the exact baseline context. These sources introduce no dummy consumers, volatile qualifiers, register constraints or padding.

The threshold helper is not expanded despite the pragma. It leaves a20-byte body before the named score function, moving that function's start. The compiler's middle-pass dump identifies three functions, while the converted ELF exposes only the two external f_ names. This is explicitly recorded; the helper prefix is not accepted or counted as a new target function. All shifted outputs fail the original boundary requirement. f7956 text is unchanged and still has the inherited unresolved f17834/RNG call assumption; the whole context is diagnostic.

## Replay warning and verification

The helper's original successful driver trace records SHCMDL exit5. The first replay adapter incorrectly required exit1. An attempted fresh-frontend replay also returned the non-expansion warning and is retained as an excluded failed attempt. The final adapter replays the captured middle-stage input, requires its exact original driver status (5 only for this helper,1 for the other cases), and still requires byte-identical `.reg` and `.ilb` outputs. Both helper replays pass those checks. Warning status is not used to excuse an output difference.

Verification covers ten native controls, four native capture comparisons, eight exact normal/debug middle replays, two flag-write traces and two eviction traces. Each traced code-generation replay preserves original `.sua`; its `.sua` and `.asa` also equal an untraced control. The captured cleanup `.asa` metadata differences are explicitly recorded. Fresh baseline context objects differ from the prior case only in their comment sections. Sources regenerate exactly from the pinned baseline.

Both Nuada checkouts retain46 production pins,50 prior frozen cases, original ROM hashes and128398/200698 report. The most recent full gates remain checkpoint161's245 enabled tests and independent clean builds. No production source, split, compiler, proof rule or denominator changes; zero new C bytes.

## Next experiment

The eviction mechanism is now explained; do not repeat predicate helpers, numeric-address spelling or condition grouping. A distinct hypothesis is that the original single-bit mode check used a bit-field access rather than a general integer AND expression. First use a small compiler probe to establish SHC's bit-field layout and generated load/test recipe for bit12 of a16-bit word. Only consider a game-source view if its storage size, exact bit position and reads are proven identical; do not add padding or artificial live values. If that recipe still installs the same cached mask, close the hypothesis without another broad sweep. Goal active.
