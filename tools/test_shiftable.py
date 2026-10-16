"""Checks for the shiftable build (tools/shiftable.py).

The build tests relink the whole ROM, so they need `make build` output and
run only with SHIFTABLE_TESTS=1.
"""
import os
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HAVE_ROM = (ROOT / "build" / "prog.bin").exists()


@unittest.skipUnless(HAVE_ROM, "needs build/prog.bin")
class ClassifierTests(unittest.TestCase):
    def setUp(self):
        import shiftable
        self.sh = shiftable

    def test_memory_map_from_copy_table(self):
        m = self.sh.MAP
        self.assertEqual((m.text_lma, m.text_vma), (0x780, 0x06000000))
        self.assertEqual(m.text_lma + m.text_size, 0x313FC)
        self.assertEqual(m.data_lma, 0x3141C)
        self.assertEqual(m.data_vma, 0x06000000 + m.text_size)
        self.assertEqual(m.filler, 0xB00FF)
        self.assertEqual(m.target(0x06000000)[0], 0x780)
        self.assertEqual(m.target(0x2004002C)[:2], (0x4002C, 1))
        self.assertIsNone(m.target(0x0607926C))  # BSS does not move

    def test_integer_shapes(self):
        rn = self.sh.round_number
        for v in (0x40000, 0x7FFFF, 0x80008, 0x8FFF8, 0x81400):
            self.assertTrue(rn(v), hex(v))
        for v in (0xA7130, 0x67110, 0x3633C, 0xACD88):
            self.assertFalse(rn(v), hex(v))

    def test_pool_rule(self):
        pp = self.sh.pool_pointer
        self.assertTrue(pp(0x0602AD12))   # RAM code
        self.assertTrue(pp(0x2004002C))   # cache-through header pointer
        self.assertTrue(pp(0x000A7130))   # a frame record
        self.assertFalse(pp(0x00050000))  # a flag
        self.assertFalse(pp(0x0007FFFF))  # a mask
        self.assertFalse(pp(0x0607926C))  # BSS
        self.assertFalse(pp(0x00034BC0))  # 216000, inside the ROM copy of RAM data

    def test_nonmatching_splices_only_asm(self):
        segs = [(0x100, 0x200, "asm", None), (0x200, 0x300, "c", "src/x.c")]
        out = self.sh.apply_nonmatching(segs, [(0x140, 0x180, "src/nonmatching/y.c")])
        self.assertEqual(out, [(0x100, 0x140, "asm", None), (0x140, 0x180, "c-nonmatching", "src/nonmatching/y.c"),
                               (0x180, 0x200, "asm", None), (0x200, 0x300, "c", "src/x.c")])
        with self.assertRaises(SystemExit):
            self.sh.apply_nonmatching(segs, [(0x1c0, 0x240, "src/nonmatching/y.c")])


@unittest.skipUnless(HAVE_ROM and os.environ.get("SHIFTABLE_TESTS") == "1"
                     and (ROOT / "build" / "tgm2p.elf").exists(), "set SHIFTABLE_TESTS=1 after make build")
class BuildTests(unittest.TestCase):
    def test_zero_shift_is_the_original_rom(self):
        import shiftable
        with tempfile.TemporaryDirectory() as td:
            data, layout = shiftable.build_shiftable([], Path(td))
        self.assertEqual(bytes(data), shiftable.ROM)
        self.assertEqual(layout["fixups_changed"], 0)

    def test_padding_moves_everything_after_it(self):
        import shiftable
        with tempfile.TemporaryDirectory() as td:
            data, layout = shiftable.build_shiftable([(0x1913C, 256), (0x72194, 256)], Path(td))
        self.assertEqual(len(data), 0x100000)
        units = {u["start"]: u for u in layout["units"]}
        self.assertEqual(units[0x1913C]["vma"], 0x060189BC + 256)
        self.assertEqual(units[0x3141C]["vma"], 0x06030C7C + 256)   # RAM data follows the code
        self.assertEqual(units[0x72194]["lma"], 0x72194 + 512)
        rom = shiftable.ROM
        # the vector table's RAM interrupt handlers (irq_vblank, irq_nop) moved;
        # its ROM boot entries did not
        for a in range(0, 0x400, 4):
            v = int.from_bytes(rom[a:a + 4], "big")
            w = int.from_bytes(data[a:a + 4], "big")
            if 0x060189BC <= v < 0x06030C7C:
                self.assertEqual(w, v + 256, hex(a))
            else:
                self.assertEqual(w, v, hex(a))
        # the copy table describes the larger image
        ct = 0x313FC + 256
        self.assertEqual(int.from_bytes(data[ct + 16:ct + 20], "big"), 0x30C7C + 256)
        # pointers found through another pointer's target survive the split
        # of the data unit at the padding (0xa5aec is a frame record)
        for a in (0xACDFC, 0xACE34):
            self.assertEqual(int.from_bytes(data[a + 512:a + 516], "big"), 0xA5AEC + 512, hex(a))
        # 216000 (frames per hour, a pool word before the padding) lies in the
        # ROM copy of RAM data, which moved, and must stay as it is
        self.assertEqual(int.from_bytes(data[0xD0EC:0xD0F0], "big"), 216000)

    def test_padding_inside_a_text_unit_is_rejected(self):
        import shiftable
        with tempfile.TemporaryDirectory() as td, self.assertRaises(SystemExit):
            shiftable.build_shiftable([(0x1913E, 256)], Path(td))


if __name__ == "__main__":
    unittest.main()
