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

    def test_text_size_pads_the_code_with_nop(self):
        import shiftable
        with tempfile.TemporaryDirectory() as td:
            data, layout = shiftable.build_shiftable([], Path(td), text_size=0x30C7C + 1024)
        # the code is followed by nop, and the RAM data after it moved
        self.assertEqual(bytes(data[0x313FC:0x313FC + 1024]), b"\x00\x09" * 512)
        ct = 0x313FC + 1024
        self.assertEqual(int.from_bytes(data[ct + 16:ct + 20], "big"), 0x30C7C + 1024)
        self.assertEqual(int.from_bytes(data[ct + 24:ct + 28], "big"), 0x06030C7C + 1024)

    @unittest.skipUnless((ROOT / "splits_nonmatching.txt").exists(), "no non-matching units")
    def test_nonmatching_units_keep_the_alignment_after_them(self):
        import shiftable
        with tempfile.TemporaryDirectory() as td:
            data, layout = shiftable.build_shiftable([], Path(td), nonmatching=True)
            islands = list(Path(td).glob("island_*.s"))
        self.assertEqual(len(data), 0x100000)
        kinds = {u["kind"] for u in layout["units"]}
        self.assertIn("c-nonmatching", kinds)
        for u in layout["units"]:
            if u["kind"] in ("vectors", "filler", "c-nonmatching") or not 0x780 <= u["start"] < 0x313FC:
                continue
            orig = u["start"] - 0x780 + 0x06000000
            self.assertEqual((u["vma"] - orig) % 4, 0, hex(u["start"]))
        # kept code loads shared-pool words from inside replaced ranges: those
        # words were copied to literal islands, with their original values
        self.assertTrue(islands)

    def test_padding_inside_a_text_unit_is_rejected(self):
        import shiftable
        with tempfile.TemporaryDirectory() as td, self.assertRaises(SystemExit):
            shiftable.build_shiftable([(0x1913E, 256)], Path(td))


@unittest.skipUnless(HAVE_ROM and os.environ.get("SHIFTABLE_TESTS") == "1"
                     and (ROOT / "build" / "tgm2p.elf").exists(), "set SHIFTABLE_TESTS=1 after make build")
class SourceBuildTests(unittest.TestCase):
    """The mod build: every C unit linked from its source by symbol
    (shiftable.py build --source, tools/srcunit.py)."""

    @classmethod
    def setUpClass(cls):
        import shiftable
        cls.td = tempfile.TemporaryDirectory()
        cls.out = Path(cls.td.name) / "out"   # shared: the compiled objects are cached in it
        cls.data, cls.layout = shiftable.build_shiftable([], cls.out, source=True)
        cls.symbols = shiftable.elf_symbols(cls.out / "tgm2p.elf")

    @classmethod
    def tearDownClass(cls):
        cls.td.cleanup()

    def mod(self, files):
        """A mod directory with these {path: text} files."""
        d = Path(tempfile.mkdtemp(dir=self.td.name))
        for name, text in files.items():
            (d / name).parent.mkdir(parents=True, exist_ok=True)
            (d / name).write_text(text)
        return d

    def test_function_names_file_is_current(self):
        import shiftable
        self.assertEqual(shiftable.read_names(), shiftable.named_functions())
        self.assertEqual(shiftable.NAMES.read_text(), shiftable.names_text(shiftable.named_functions()))

    def test_every_matching_function_is_linked_by_name(self):
        import asm_scan
        matching = asm_scan.c_entries()
        for a, name in matching.items():
            self.assertIn(name, self.symbols, hex(a))
            self.assertIn(f"_f_{a:x}", self.symbols, hex(a))   # others call it by address
        # (the matching ELF has no symbols for the retained c-prefix and
        # c-range bytes; the sources name those functions too)
        self.assertLessEqual(set(matching), {f[1] for f in self.layout["functions"]})
        self.assertEqual(len(self.data), 0x100000)

    def test_units_keep_their_order_and_alignment(self):
        last = 0
        for u in self.layout["units"]:
            if not 0x780 <= u["start"] < 0x313FC:
                continue
            self.assertGreater(u["vma"], last, hex(u["start"]))
            last = u["vma"]
            if u["kind"] == "asm":
                self.assertEqual((u["vma"] - (u["start"] - 0x780 + 0x06000000)) % 4, 0, hex(u["start"]))

    def test_unedited_whole_files_are_the_original_code(self):
        # a file one unit owns is linked whole: its bytes are the ROM's, apart
        # from the words that hold an address this build moved
        import shiftable
        rom, units = shiftable.ROM, self.layout["units"]
        used = [u["arg"] for u in units]
        checked = 0
        for u, nxt in zip(units, units[1:]):
            if u["kind"] != "c" or used.count(u["arg"]) != 1:
                continue
            new = self.data[u["lma"]:u["lma"] + (u["end"] - u["start"])]
            for off in range(0, len(new), 2):
                if new[off:off + 2] != rom[u["start"] + off:u["start"] + off + 2]:
                    a = (u["start"] + off) & ~3
                    self.assertIsNotNone(shiftable.MAP.target(int.from_bytes(rom[a:a + 4], "big")),
                                         f"{u['arg']} +{off:#x}")
            checked += 1
        self.assertGreater(checked, 50)

    def test_example_mod_adds_code_and_variables(self):
        import shiftable
        data, layout = shiftable.build_shiftable([], self.out, source=True, mod=ROOT / "mods" / "example")
        sym = shiftable.elf_symbols(self.out / "tgm2p.elf")
        sec = layout["sections"]
        text = sec[".text"]
        self.assertTrue(text[0] <= sym["_mod_frame"] < text[0] + text[2])
        # the new variable follows the game's own and is cleared at boot with them
        self.assertEqual(sym["_mod_frames"], shiftable.MAP.bss_hi)
        self.assertEqual(sym["__bss_end"], shiftable.MAP.bss_hi + 4)
        ct = sec[".copytab"][1]
        self.assertEqual(int.from_bytes(data[ct + 4:ct + 8], "big"), sym["__bss_end"])
        # the edited file grew, and what follows it moved along
        self.assertGreater(text[2], self.layout["sections"][".text"][2])
        self.assertGreater(sym["_f_2b64a"] - sym["_f_2b4e8"], 0x2B64A - 0x2B4E8)
        self.assertGreater(sym["_f_2beca"] - self.symbols["_f_2beca"], 0)
        # each of the four frame drivers loads the new function's address
        lo = lma = shiftable.lma_of(sym["_f_2b4e8"], [(n, *v) for n, v in sec.items()])
        self.assertEqual(layout["numbers"], [])
        hi = lma + (sym["_f_2beca"] - sym["_f_2b4e8"])
        words = [int.from_bytes(data[a:a + 4], "big") for a in range(lo & ~3, hi, 4)]
        self.assertGreaterEqual(words.count(sym["_mod_frame"]), 1)
        self.assertEqual(len(data), 0x100000)

    def test_a_removed_function_fails_the_build(self):
        import shiftable
        mod = self.mod({"src/main_loop.c": "/* flags: -macsave=1 -optimize=1 -speed */\nvoid other(void)\n{\n}\n"})
        with self.assertRaises(SystemExit) as e:
            shiftable.build_shiftable([], self.out, source=True, mod=mod)
        self.assertIn("defines no function", str(e.exception))

    def test_new_code_names_addresses_by_symbol(self):
        # 0x06033014 is the glyph width table draw_text loads: an address the
        # game uses, which new code has to name
        import shiftable
        mod = self.mod({"src/peek.c": "long peek(void)\n{\n\treturn *(long *)0x06033014;\n}\n"})
        with self.assertRaises(SystemExit) as e:
            shiftable.build_shiftable([], self.out, source=True, mod=mod)
        self.assertIn("looks like an address this build moves", str(e.exception))

    def test_a_number_that_could_be_an_address_stays_a_number(self):
        # 300000 is 0x493e0, inside the ROM data, and nothing in the game loads it
        import shiftable
        src = (ROOT / "src" / "main_loop.c").read_text()
        line = "\t\tg_60657a0 += f_2beca(0x4a8) + 1;\n"
        self.assertIn(line, src)
        mod = self.mod({"src/main_loop.c": src.replace(line, line + "\t\tg_60657a0 += 300000;\n"),
                        "src/bonus.c": "long bonus(long v)\n{\n\treturn v + 300001;\n}\n"})
        data, layout = shiftable.build_shiftable([], self.out, source=True, mod=mod)
        self.assertEqual(layout["numbers"], [["src/bonus.c", "0x000493e1"], ["src/main_loop.c", "0x000493e0"]])
        sym = shiftable.elf_symbols(self.out / "tgm2p.elf")
        sections = [(n, *v) for n, v in layout["sections"].items()]
        for name, value in (("_main_loop", 300000), ("_bonus", 300001)):
            lma = shiftable.lma_of(sym[name], sections)
            words = [int.from_bytes(data[a:a + 4], "big") for a in range(lma & ~3, lma + 0x100, 4)]
            self.assertIn(value, words, name)

    def test_a_context_source_cannot_own_variables(self):
        # src/startup_reset_1896.c is compiled for the unit that owns f_1896
        # and f_18ca, and holds copies of three functions other units own
        import shiftable
        src = (ROOT / "src" / "startup_reset_1896.c").read_text()
        head = "void f_1896(struct field *f)\n{\n\tshort i;\n\n"
        self.assertIn(head, src)
        mod = self.mod({"src/startup_reset_1896.c":
                        src.replace(head, head.replace("\tshort", "\tstatic long calls;\n\tshort") + "\tcalls++;\n")})
        with self.assertRaises(SystemExit) as e:
            shiftable.build_shiftable([], self.out, source=True, mod=mod)
        self.assertIn("writable data of its own", str(e.exception))

    def test_a_cut_unit_takes_its_new_functions(self):
        # a static helper ahead of the unit's first function (SHC gives it no
        # symbol; it is found by the calls) and a new function after its last
        import shiftable
        src = (ROOT / "src" / "startup_reset_1896.c").read_text()
        first = "void f_1896(struct field *f)\n{\n"
        last = "void f_18ca(struct field *f)\n{\n\tf_1896(f);\n"
        self.assertIn(first, src)
        self.assertIn(last, src)
        helper = ("static int bump(struct field *f, int n)\n{\n\tint i, s = 0;\n\n"
                  "\tfor (i = 0; i < n; i++) {\n\t\ts += f->w4[i & 7][i % 12] * 3;\n"
                  "\t\tif (s > 300000)\n\t\t\ts -= f_2beca(s);\n"
                  "\t\tf_1175c(f);\n\t\tf_2d06(f);\n\t\tf_15f10(f, s);\n\t}\n\treturn s;\n}\n\n")
        src = src.replace(first, helper + first)
        src = src.replace(last, last + "\tbump(f, 3);\n\tbump(f, f->w4[0][0]);\n")
        src += "\nvoid extra_reset(struct field *f)\n{\n\tf_18ca(f);\n\tf_1175c(f);\n}\n"
        data, layout = shiftable.build_shiftable([], self.out, source=True,
                                                 mod=self.mod({"src/startup_reset_1896.c": src}))
        sym = shiftable.elf_symbols(self.out / "tgm2p.elf")
        self.assertLess(sym["U_001896"], sym["_f_1896"])          # the helper leads the unit
        self.assertLess(sym["_f_18ca"], sym["_extra_reset"])
        self.assertLess(sym["_extra_reset"], sym["U_001914"])
        self.assertEqual(layout["numbers"], [["src/startup_reset_1896.c", "0x000493e0"]])
        # both calls reach the helper
        lma = shiftable.lma_of(sym["_f_18ca"], [(n, *v) for n, v in layout["sections"].items()])
        calls = set()
        for a in range(lma, lma + 0x30, 2):
            w = int.from_bytes(data[a:a + 2], "big")
            if w >> 12 == 0xB:
                d = w & 0xFFF
                calls.add(sym["_f_18ca"] + (a - lma) + 4 + 2 * (d - 0x1000 if d & 0x800 else d))
        self.assertLessEqual({sym["_f_1896"], sym["U_001896"]}, calls)

    def test_rom_set_holds_the_image(self):
        import interleave
        import mod
        roms = mod.romset(self.out / "tgm2p.bin", self.out)
        image = interleave.join((roms / "2b.u21").read_bytes(), (roms / "1b.u22").read_bytes())
        self.assertEqual(image, (self.out / "tgm2p.bin").read_bytes())
        self.assertGreater(len(list(roms.iterdir())), 2)


if __name__ == "__main__":
    unittest.main()
