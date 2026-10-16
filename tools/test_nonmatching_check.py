"""Checks for tools/nonmatching_check.py's verdict parsing."""
import tempfile
import unittest
from pathlib import Path

import nonmatching_check as nc

MATCHED = """f_7fa0: ROM 0x7fa0-0x8156 (438 bytes), compiled 464 bytes, r0 compared: no (void) bits
MATCHED_TRIALS
  trials: 500 complete, 0 hit the step limit (compared up to it), 0 unsupported; 500 run, 250 of them mutated inputs
"""


class VerdictTests(unittest.TestCase):
    def test_matched_over_all_trials(self):
        ok, v, t = nc.verdict(MATCHED, 500)
        self.assertTrue(ok)
        self.assertEqual(v, "MATCHED_TRIALS")

    def test_too_few_trials_is_not_enough(self):
        self.assertFalse(nc.verdict(MATCHED, 600)[0])

    def test_step_limited_trials_fail(self):
        log = MATCHED.replace("500 complete, 0 hit", "499 complete, 1 hit")
        self.assertFalse(nc.verdict(log, 500)[0])

    def test_different_fails(self):
        log = ("DIFFERENT: input 3: call #11 to f_e3e0: arguments differ\n"
               "  trials: 0 complete, 2 hit the step limit (compared up to it), 0 unsupported; 3 run\n")
        ok, v, _ = nc.verdict(log, 500)
        self.assertFalse(ok)
        self.assertTrue(v.startswith("DIFFERENT"))

    def test_rom_end_annotation(self):
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "x.c"
            p.write_text("/* rom: 0xcac8 len: 0x4a0 func: f_cac8 flags: -optimize=1 */\n"
                         "/* difftest-rom-end: f_cac8=0xcf68 (the loop at 0xcb44 is part of it) */\n")
            self.assertEqual(nc.rom_ends(p), {"f_cac8": 0xCF68})


if __name__ == "__main__":
    unittest.main()
