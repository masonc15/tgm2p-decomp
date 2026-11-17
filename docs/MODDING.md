# Modding

`make mod` links the game from its C source as that source compiles right now,
instead of from the original bytes. Functions can grow, shrink or gain callers,
new files can add code and variables, and everything after an edit moves to
make room. The result is a 1 MB program image and a MAME ROM set.

```
make mod                       # the tree as it is, edits included
make mod MOD=mods/example      # the tree plus a mod directory
mame tgm2p -rompath build/mod/roms
```

The matching build doesn't know about any of this. `make check`, `splits.txt`
and the coverage figures read the same files they always did, so an edit made
in place under `src/` will fail `make check` for that file, as it should. To
keep the tree matching, put the changed copy in a mod directory instead.

## A mod directory

A mod directory mirrors the tree. A C file in it replaces the file with the
same path, and a file with no counterpart is new code. `mods/example` holds
both kinds: `src/frame_2b4e8.c` is the per-frame driver with one extra call in
each of its four variants, and `src/mod_banner.c` is a new file with a new
function, a new variable and a new string. It draws a frame counter in the
top left corner of every screen.

## Which file to edit

`build/mod/functions.txt` lists every function with the source that defines
it. Use it, because many files under `src/` hold copies of functions that
another file owns. The matching proof needs those copies to reproduce SHC's
shared literal pools, and the mod build ignores them, so editing a copy
changes nothing. The header of such a file names the range it owns
(`rom: 0x1896 len: 0x7e`), and `splits.txt` has the same information.

Functions are found by name. A function keeps its place in the game as long as
it keeps its name, either `f_<rom offset>` or one of the names in
`config/function_names.txt`. Everything that referred to the original address
(calls, function pointer tables, the interrupt vectors) follows the name.
Removing or renaming such a function fails the build with the address that
still needs it.

## What the source may do

Call anything, add functions, change sizes freely. A few rules follow from how
the image is laid out.

In a file that holds copies of other units' functions, put a new function
directly before or after the function that uses it. The build cuts the unit's
own functions out of the compiled file, and a new function goes with the one
next to it. This matters most for `static` helpers, which SHC compiles without
a symbol: the build finds them by the calls that reach them. In a file that
one unit owns, and in a new file, a function can go anywhere.

Name addresses by symbol in new code. Code and initialized data are relinked,
so `(struct x *)0x06031234` would point at whatever lands there. Write
`g_6031234`, `d_<rom offset>` or `f_<rom offset>` and declare it `extern`; the
linker resolves those names by address. The matching sources do write such
addresses as integers, and the build moves them, but it only recognises the
values the original game already loads. If an integer in new code is one of
those, the build stops and asks for the symbol. Any other constant that merely
falls in an address range (300000 is `0x493e0`, which is inside the ROM data)
stays a number, and the build prints a note saying so. The game's own
variables from `0x06060000` up, and hardware registers, never move, and
integers are fine for them.

Define new variables in a new file, or in a file that one unit owns. A file
cut for a `c-prefix` or `c-range` unit can't define them, because other units
compile the same source, and the build says so. New variables go after the
game's own at `0x060b19b4` and are cleared at boot with them; initialized ones
join the data the boot code copies to RAM.

A cut unit reaches the functions of other units with the `bsr` SHC emitted
when they sat in one file, which reaches about 4 KB. If an edit pushes a
caller further than that from its target the link stops with a "cannot reach"
message naming both. Calls through `jsr` (anything declared `extern` and
defined in another file) have no such limit.

Code and initialized data run from RAM below `0x06060000`. `make mod` prints
how much of that is left, about 170 KB for the unmodified game.

## What stays assembly

The boot code at `0x400`, the SHC runtime helpers at the end of the code,
three literal pool tails and the code-entry and option menus of the service
mode (`0xd394`-`0xe39c`) are still assembly. They are relinked symbolically
and move with everything else, but can't be edited as C. The other ranges the
matching decompilation has not resolved are linked from the functionally
equivalent C in `src/nonmatching/`, which is ordinary C and can be edited like
the rest. That C is not byte-matching and never counts as matching coverage.
`splits_nonmatching.txt` lists the units that are accepted, and
`splits_nonmatching_staged.txt` those that pass their checks but wait for a
decision on how one of them is measured.

## Checking that nothing changed

`make mod-check` builds the tree with no mod directory and plays the two
recorded replays (12,000 frames of attract mode and 24,000 of play) against
the original ROM, comparing the screen every 100 frames and the game's frame
counter every frame. With unedited sources this is the proof that the
source-linked game is the same game. After a refactor that isn't supposed to
change behaviour, it's a test of that.

It can't check a real mod. The game seeds its random numbers from the number
of idle loops it spins while waiting for vertical blank, so any change in how
long a frame takes changes the piece sequence. The check pins that count in
both images; a mod that changes what happens on screen will differ anyway.

Two other things to expect from a changed program ROM: MAME reports wrong
checksums for `2b.u21` and `1b.u22` and runs it regardless, and the ROM check
in the game's own service menu no longer shows the original sum.
