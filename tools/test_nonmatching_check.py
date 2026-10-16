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

    def test_step_limit_annotation(self):
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "x.c"
            p.write_text("/* rom: 0x163fc len: 0xd60 func: f_163fc flags: -optimize=1 */\n"
                         "/* difftest-steps: f_16938=30000000 f_163fc=30000000 */\n")
            self.assertEqual(nc.step_limits(p), {"f_16938": 30000000, "f_163fc": 30000000})


HANG = """  input 1: ok (120/118 steps)
  input 2: ok (2000001/2000001 steps, truncated)
  input 3/m6064750.4=0: ok (300/290 steps)
INCONCLUSIVE: only 2 of 3 trials ran to completion (step limit 2000000)
  trials: 2 complete, 1 hit the step limit (compared up to it), 0 unsupported; 3 run, 1 of them mutated inputs
"""


class NonReturningTests(unittest.TestCase):
    def test_hanging_on_both_sides_passes(self):
        ok, why, hang = nc.nonreturning(HANG, 2000000, 3)
        self.assertTrue(ok, why)
        self.assertEqual(hang, ["2"])

    def test_candidate_returning_where_the_original_hangs_fails(self):
        log = HANG.replace("2000001/2000001 steps", "2000001/4000 steps")
        ok, why, _ = nc.nonreturning(log, 2000000, 3)
        self.assertFalse(ok)
        self.assertIn("only the original", why)

    def test_a_difference_fails(self):
        log = HANG.replace("INCONCLUSIVE: only 2 of 3 trials ran to completion (step limit 2000000)",
                           "DIFFERENT: input 2: memory writes after call #1 differ")
        self.assertFalse(nc.nonreturning(log, 2000000, 3)[0])

    def test_too_few_trials_fails(self):
        self.assertFalse(nc.nonreturning(HANG, 2000000, 500)[0])

    def test_annotations(self):
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "x.c"
            p.write_text("/* difftest-nonreturning: f_a226 f_a608 (menus wait for a button) */\n"
                         "/* difftest-inputs f_a226: 3/m6064750.4=0,m6064754.1=28 7/c1=0 */\n")
            self.assertEqual(nc.nonreturning_functions(p), {"f_a226", "f_a608"})
            self.assertEqual(nc.returning_inputs(p), {"f_a226": ["3/m6064750.4=0,m6064754.1=28", "7/c1=0"]})


if __name__ == "__main__":
    unittest.main()
