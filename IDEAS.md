# What 100% unlocks

Ideas for what becomes possible once the decomp is complete, kept here for later. As of Sep 27 2026 the project sits at 5.85% of code bytes matched, up from 1.00% on Sep 24.

## Where it stands

At 100% the whole game is C that compiles back to the exact TGM2+ ROM, so every idea below becomes a code change instead of an assembly patch. The headline number counts matched executable code only. Graphics, tables and other data are separate work (see the last section).

## Native ports

A native port is the biggest prize. Mario 64, Ocarina of Time and Perfect Dark all got ports after their decomps finished, by swapping the console hardware layer (video, sound, input) for something like SDL.

- **PC build without MAME:** lower input lag, clean integer scaling, any controller, no emulator setup.
- **Frame-exact timing by construction:** TGM players care about exact frame behavior (ARE, DAS, lock delay). A port built from the real game logic keeps all of it; a fan clone can only approximate it.
- **Other targets:** Switch or Steam Deck homebrew, and a WebAssembly build that runs in a browser tab.
- **Quality-of-life layer:** key remapping, pause, save states and a replay recorder, all outside the untouched game logic.

## The rules, read from the source

The community worked out most of TGM2+'s rules by testing; with the source, each one is a function you can read. That settles edge cases and makes exact tools possible.

- **Authoritative mechanics:** the piece randomizer and its history rolls, rotation and wall kicks, the speed and timing curves per level, Master mode's hidden grade points and the M-roll requirements, T.A. Death's time checks.
- **Exact simulator:** a headless library of the real game logic that other tools link against, instead of each one reimplementing the rules.
- **Replay verification:** feed recorded inputs to the real engine and confirm a claimed grade or time, useful for leaderboards and tool-assisted runs.
- **Seeded races:** force the same piece sequence for two players, for fair head-to-head races.
- **Training tools:** drill a single section, a speed level or the invisible roll on demand, with the real timings.

## Mods and new modes

Once the code can move (see the last section), a mod is an edit to C, not hand-patched SH-2 instructions.

- **Practice mode:** start at any level or section, pick the gravity, toggle the invisible roll.
- **Debug and training overlays:** live timers, the next several pieces, current grade points, input display.
- **Rebalanced or remixed modes:** custom speed curves, new garbage rules, a Master mode variant with different grade requirements.
- **Fixes and accessibility:** optional fixes for known bugs, colorblind-friendly piece palettes, a larger HUD.
- **Translation and text:** clean English menus and messages without byte-counting a patch into existing space.

## Bigger swings

These are longer projects, but a full decomp is what makes them practical.

- **Online versus with rollback netcode:** the game is deterministic from its inputs, and once its state is known exactly, it can be saved and rewound each frame. That is what rollback netcode needs to make remote versus feel local.
- **Headless AI environment:** run the real engine with no rendering at thousands of frames per second, as a training environment for bots that play by the real rules.
- **Unused and hidden content:** with every function named, dead code, leftover debug menus, unused modes or test screens show up as code nothing calls.
- **Glitch hunting:** fuzz the engine with random or edge-case inputs and look for crashes, score overflows or grade exploits.
- **Sound and music:** once the sound driver is understood, music can be swapped, remixed or played back outside the game.

## Help for other SHC decomps

The matching work already pays off beyond this game. Every rule learned about how Hitachi SHC v5.0 generates code applies to other games built with it, including many Sega Saturn titles.

- **Documented codegen rules:** the README's list of SHC behaviors, like the file-state effect found in the medal code, saves the next project hours.
- **Shared tooling:** the decomp.me Saturn SHC patch, the SH-2 permuter support, the position-independent function scorer and the arcade platform for decomp.dev.
- **A reference project:** a finished SH-2 matching decomp shows the next Saturn or arcade project that the whole pipeline works.

## What comes after 100%

Matching proves the C is right; the ideas above need a few more steps after it.

1. **Matching code (100%):** every function compiles back to the original bytes.
2. **Labelled data:** graphics, tables and strings are named symbols instead of raw bytes, so code refers to them by name.
3. **Shiftable build:** code and data can move without breaking hardcoded addresses, so adding a line doesn't break the ROM. This is where easy modding starts.
4. **Hardware layer:** replace the arcade video, sound and input code with a portable layer. This is where native ports start.

The repo can never ship the game's assets or ROM. Like every decomp, a port or mod build would need the player's own ROM dump.
