"""Checks for the flag sweep's option grid and flag handling."""
import unittest

import flag_sweep


class GridTests(unittest.TestCase):
    def test_full_grid_covers_documented_options(self):
        g = flag_sweep.grid("full")
        self.assertEqual(len(g), 312)
        self.assertEqual(len(set(g)), len(g))
        joined = " ".join(g)
        for opt in ("-optimize=0", "-optimize=1", "-speed", "-nospeed", "-size", "-noinline",
                    "-inline=80", "-noloop", "-loop", "-nestinline=3"):
            self.assertIn(opt, joined)
        self.assertIn("-optimize=1 -speed", g)
        self.assertFalse(any("-noinline" in c and "-nestinline" in c for c in g))

    def test_keep_flags_drops_only_optimization_options(self):
        self.assertEqual(flag_sweep.keep_flags("-macsave=1 -optimize=1 -speed -inline=40 -noloop"),
                         ["-macsave=1"])
        self.assertEqual(flag_sweep.keep_flags("-macsave=1 -division=cpu -size"),
                         ["-macsave=1", "-division=cpu"])

    def test_build_flag_sets(self):
        self.assertTrue(flag_sweep.in_build("-optimize=1 -speed", "-macsave=1 -optimize=1 -speed"))
        self.assertFalse(flag_sweep.in_build("-optimize=1 -speed -inline=80", "-macsave=1 -optimize=1 -speed"))
        self.assertTrue(flag_sweep.in_build("baseline", "-macsave=1 -optimize=1 -speed -inline=80"))


if __name__ == "__main__":
    unittest.main()
