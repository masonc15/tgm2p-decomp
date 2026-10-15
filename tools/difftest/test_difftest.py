"""Small deterministic tests; run on nuada with unittest discover."""
import contextlib
import io
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

import sh2emu as emu
import difftest as dt


class TinyCPU(emu.CPU):
    def __init__(self, code, values=()):
        mem = emu._FlatMem(b"")
        for addr, size, value in values:
            getattr(mem, "w" + str(size * 8))(addr, value, 0)
        super().__init__(mem, code.__getitem__)
        self.code = code

    def in_region(self, pc):
        return pc in self.code

    def on_leave(self, pc):
        return None

    def on_call(self, target, ret, at):
        return target


class DelaySlotTests(unittest.TestCase):
    # Hitachi SH-1/SH-2 Programming Manual h12p0, MOV immediate and MOVA:
    # a PC-relative load in a delayed branch slot uses destination + 2.
    def test_movw_uses_destination_pc(self):
        c = TinyCPU({0x1000: 0xA00E, 0x1002: 0x9100}, [(0x1022, 2, 0x8123)])
        c.run(0x1000, 10)
        self.assertEqual(c.R[1], 0xFFFF8123)

    def test_movl_uses_aligned_destination_pc(self):
        c = TinyCPU({0x1000: 0xA00E, 0x1002: 0xD100}, [(0x1020, 4, 0x12345678)])
        c.run(0x1000, 10)
        self.assertEqual(c.R[1], 0x12345678)

    def test_mova_uses_destination_pc(self):
        c = TinyCPU({0x1000: 0xA00E, 0x1002: 0xC700})
        c.run(0x1000, 10)
        self.assertEqual(c.R[0], 0x1020)

    def test_untaken_conditional_still_has_delay_slot(self):
        for opcode, t in [(0x8D00, 0), (0x8F00, 1)]:
            with self.subTest(opcode=opcode):
                c = TinyCPU({0x1000: opcode, 0x1002: 0xA00D})
                c.T = t
                with self.assertRaises(emu.Unsupported):
                    c.run(0x1000, 10)

    def test_untaken_pc_relative_load(self):
        c = TinyCPU({0x1000: 0x8D00, 0x1002: 0x9100}, [(0x1006, 2, 0x1234)])
        c.T = 0
        c.run(0x1000, 10)
        self.assertEqual(c.R[1], 0x1234)

    def test_return_target_captured_before_slot(self):
        c = TinyCPU({0x1000: 0x000B, 0x1002: 0x412A})  # rts; lds r1,pr
        c.pr = 0x2000
        c.R[1] = 0x3000
        left = []
        c.on_leave = lambda pc: left.append(pc)
        c.run(0x1000, 10)
        self.assertEqual(left, [0x2000])
        self.assertEqual(c.pr, 0x3000)

    def test_slot_respects_step_budget(self):
        c = TinyCPU({0x1000: 0xA00E, 0x1002: 0xE101})
        with self.assertRaises(emu.StepLimit):
            c.run(0x1000, 1)
        self.assertEqual(c.R[1], 0)

    def test_unsupported_instruction(self):
        c = TinyCPU({0x1000: 0x001B})  # sleep is not modeled
        with self.assertRaises(emu.Unsupported):
            c.run(0x1000, 10)


class VerdictTests(unittest.TestCase):
    def verdict(self, statuses):
        args = types.SimpleNamespace(seed=1, trials=len(statuses), steps=10,
            time=60, verbose=False, cov=False)
        runs = []
        for status in statuses:
            cpu = types.SimpleNamespace(cmps=[])
            runs.append(([], 1, status, cpu))
            if not status.startswith("unsupported"):
                runs.append(([], 1, status, cpu))
        with patch.object(dt, "execute", side_effect=runs), \
             patch.object(dt, "mutations", return_value=[]), \
             patch.object(dt, "compare", return_value=(None, 0)), \
             patch.object(dt, "code_words", return_value=set()), \
             contextlib.redirect_stdout(io.StringIO()):
            return dt.check(args, None, "test", object(), object(), 0x1000, 0x1002, 32)

    def test_all_complete_is_only_matched_trials(self):
        self.assertEqual(self.verdict(["ok", "ok"]), "MATCHED_TRIALS")

    def test_one_unsupported_is_inconclusive(self):
        self.assertEqual(self.verdict(["ok", "unsupported: sleep"]), "INCONCLUSIVE")

    def test_one_truncated_is_inconclusive(self):
        self.assertEqual(self.verdict(["ok", "limit"]), "INCONCLUSIVE")


@unittest.skipUnless(os.environ.get("DIFFTEST_COMPILE_TESTS") == "1", "requires SHC on nuada")
class CompileTests(unittest.TestCase):
    def run_case(self, source, expected, *args):
        with tempfile.TemporaryDirectory(prefix="difftest-regression-") as directory:
            case = Path(directory) / "case.c"
            case.write_text(source)
            result = subprocess.run([sys.executable, str(dt.HERE / "difftest.py"),
                str(case), "f_1260", "--trials", "8", *args], cwd=dt.ROOT,
                capture_output=True, text=True, timeout=60)
        self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
        return result.stdout

    def test_matched_header(self):
        output = self.run_case((dt.ROOT / "src/rom_header.c").read_text(), 0)
        self.assertIn("MATCHED_TRIALS", output)

    def test_wrong_global_symbol(self):
        source = (dt.ROOT / "src/rom_header.c").read_text()
        output = self.run_case(source.replace("g_607926c", "g_6079270"), 1)
        self.assertIn("DIFFERENT", output)

    def test_unresolved_symbol_cannot_borrow_rom(self):
        source = (dt.ROOT / "src/rom_header.c").read_text()
        output = self.run_case(source.replace("g_607926c", "unrecognized_global"), 2)
        self.assertIn("INCONCLUSIVE", output)

    def test_wrong_stored_value(self):
        source = (dt.ROOT / "src/rom_header.c").read_text()
        output = self.run_case(source.replace("= rom_hdr_ptr->l28;", "= rom_hdr_ptr->l28 + 1;"), 1)
        self.assertIn("DIFFERENT", output)

    def test_step_limit_cannot_pass(self):
        output = self.run_case((dt.ROOT / "src/rom_header.c").read_text(), 2, "--steps", "1")
        self.assertIn("INCONCLUSIVE", output)

    def test_known_indirect_jump_extent(self):
        result = subprocess.run([sys.executable, str(dt.HERE / "difftest.py"),
            "src/board_28334.c", "f_296b6", "--trials", "1", "--rom-end", "0x29bf8"],
            cwd=dt.ROOT, capture_output=True, text=True, timeout=60)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("MATCHED_TRIALS", result.stdout)


if __name__ == "__main__":
    unittest.main()
