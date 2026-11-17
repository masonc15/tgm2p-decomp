# Example mod

`make mod MOD=mods/example` builds the game with a banner and a frame counter
in the top left corner of every screen. It is here to show the three things a
mod can do, and `tools/test_shiftable.py` builds it.

`src/frame_2b4e8.c` replaces the tree's file of the same path. It is that
file with a call to `mod_frame()` added at the top of each of the four
per-frame drivers, so the functions grow and everything after them moves. It
is a copy, so when the tree's file changes, copy it again and re-add the four
calls.

`src/mod_banner.c` has no counterpart in the tree, so it is new code. It
defines `mod_frame()`, a counter that lives after the game's own variables,
and a string that the boot code copies to RAM with the game's initialized
data. It calls the game's `draw_text()` by name.

See `docs/MODDING.md` for the rules.
