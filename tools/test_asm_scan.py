"""Checks for the SH-2 decoder and the hand-written assembly scan."""
import unittest
from pathlib import Path

import sh2isa

ROOT = Path(__file__).resolve().parents[1]


class DecodeTests(unittest.TestCase):
    def test_prologue_and_epilogue_forms(self):
        i = sh2isa.decode(0x2FE6, 0x1000)  # mov.l r14,@-r15
        self.assertEqual(i.text(), "mov.l r14,@-r15")
        self.assertEqual(i.mem, ("r15", "predec", 4, True))
        i = sh2isa.decode(0x4F22, 0x1000)  # sts.l pr,@-r15
        self.assertEqual(i.text(), "sts.l pr,@-r15")
        i = sh2isa.decode(0x000B, 0x1000)
        self.assertEqual((i.mnemonic, i.flow, i.delay), ("rts", "ret", True))

    def test_pc_relative_and_branches(self):
        i = sh2isa.decode(0xD103, 0x30DDE)  # mov.l @(0x30dec),r1
        self.assertEqual(i.pool, (0x30DEC, 4))
        self.assertEqual(i.defs, frozenset({"r1"}))
        i = sh2isa.decode(0x894B, 0x30D44)  # bt 0x30dde
        self.assertEqual((i.flow, i.target, i.delay), ("cond", 0x30DDE, False))
        i = sh2isa.decode(0xAFFC, 0x65C)  # bra 0x658
        self.assertEqual((i.flow, i.target), ("jump", 0x658))
        i = sh2isa.decode(0xC70A, 0x19C0)  # mova @(0x19ec),r0
        self.assertEqual(i.mova, 0x19EC)

    def test_def_use(self):
        i = sh2isa.decode(0x3304, 0)  # div1 r0,r3
        self.assertTrue({"r0", "r3", "t"} <= i.uses)
        self.assertIn("r3", i.defs)
        i = sh2isa.decode(0x222A, 0)  # xor r2,r2
        self.assertEqual(i.text(), "xor r2,r2")
        i = sh2isa.decode(0xFFFF, 0)
        self.assertFalse(i.valid)


@unittest.skipUnless((ROOT / "build" / "tgm2p.elf").exists() and (ROOT / "build" / "prog.bin").exists(),
                     "needs make split check")
class CalibrationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import asm_scan
        cls.report = asm_scan.analyse()

    def test_controls_separate(self):
        c = self.report["controls"]
        self.assertGreater(c["negatives"], 400)
        self.assertGreaterEqual(c["positives"], 9)
        # New matched C can add negatives; a false-positive rate above 1% means
        # a rule needs revisiting, not that a source is wrong.
        self.assertLessEqual(c["false_positive"], c["negatives"] // 100, c["false_positive_functions"])
        self.assertEqual(c["false_negative"], 0, c["false_negative_functions"])

    def test_boot_block_is_flagged(self):
        boot = next(r for r in self.report["ranges"] if r["start"] == "0x000400")
        main = next(f for f in boot["functions_detail"] if f["entry"] == "0x00400")
        self.assertEqual(main["verdict"], "hand-written")
        self.assertIn("system-op", main["strong"])

    def test_every_unresolved_range_is_judged(self):
        starts = {r["start"] for r in self.report["ranges"]}
        import asm_scan
        for lo, hi, exempt in asm_scan.unresolved_ranges():
            if exempt != hi - lo:
                self.assertIn(f"0x{lo:06x}", starts)


if __name__ == "__main__":
    unittest.main()
