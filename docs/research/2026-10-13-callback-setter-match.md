# Callback setter and getter contract

The complete 98-byte callback setter at 276e8-2774a now compiles exactly with
the unmodified SHC 5.0r32 toolchain. Its seven following setters remain exact.
The ordinary range proof retains their combined 232 bytes and authenticates
all 20 external literal loads. No verifier extension or runtime exception is
needed. Integration and publication receipts are in
`research/2026-10-13-callback-setter-acceptance/`.

## Source and compiler evidence

The old source spilled all three short arguments. A full-width incoming first
callback identifier with a real signed-short conversion recovered the eight-byte
frame, but its anonymous converted value was assigned to outgoing R4. Native
middle-pass evidence distinguished argument preassignment from ordinary register
allocation; merely increasing a guessed priority was not a justified fix.

The matched source makes the first identifier a named `register int` formal and
narrows it with `a = (short)a` before both its guard and lookup. Both resulting
webs retain the storage-class priority. The narrowed value uses R14 and its
record pointer uses R13, matching the original early EXTS.W and first callback
path. This preserves the original low-word semantics for all incoming values.

The second short identifier stays on the stack across the first call. A single
`int second = b` immediately before the second guard supplies its signed value
to both consumers. Authenticated memory-use frequencies become i=2 and b=1.
The compiler's frequency sorter then puts i at SP0 and b at SP4, rather than
reversing equal-frequency entries. The complete first path and all boundaries
matched, leaving eleven register bytes on the second path.

The remaining narrowing cast came from the getter declaration. Original f928
accepts the incoming word in R4, explicitly sign-extends its low 16 bits for the
table index, and returns a raw 32-bit table entry in R0. The coherent C contract
is `long f_f928(int i)` with `g_6033440[(short)i]`. Callers explicitly convert
the returned address to the existing callback pointer type. Removing the
caller-side int-to-short conversion lets the second value occupy R4 and its
record pointer occupy R14. All 98 bytes then match.

The getter's complete 608-byte accepted unit also remains byte-identical.
All nine board declarations use the same contract, including the two genuine
calling contexts. The duplicate context in the board-clear source uses the
same matched setter body. Seven other declarations are unused in their units.
SHC documents four-byte int, long and pointer representations and preservation
of the corresponding bits for integer-to-pointer conversion.

## Scope and evidence

No callback, branch, write, or table lookup was invented. The original null
guards, lookup count, store order, signed low-word indices, and callback address
bits are preserved. The following toggle and cache helper remain excluded.

The earlier unprototyped-getter experiment was diagnostic only and was not
integrated. The accepted form reconciles caller and callee declarations instead.
Frozen experimental packets are `callback-setter-binding`,
`callback-default-promotion`, `callback-named-conversion`, and
`callback-getter-contract`, all under `research/2026-10-13-*`.
Independent review is in `research/2026-10-13-callback-contract-review/`.

Canonical and staged whole-ROM builds, all 304 enabled tests (zero skips),
independent review, and two cold whole-ROM replays pass. The final acceptance
receipt verifies all 236 current input pins, original program and both chip
identities, and live dashboard publication 157. Coverage increases from
137,546 to 137,644 / 200,698 bytes: 68.53381697874418% to 68.58264656349341%,
a gain of 0.048829584749225205 percentage points. The remaining agreed work
is 61,416 non-exempt bytes; the approved runtime ASM total remains 1,638.
See `research/2026-10-13-callback-setter-acceptance/FINAL_REVIEW.json`.
