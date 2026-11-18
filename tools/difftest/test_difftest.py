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

    def test_step_limit_in_a_call_counts_like_one_in_the_code(self):
        # nonmatching_check.py reads "hit the limit" as a count above the
        # limit, wherever the limit was hit
        class Counting(TinyCPU):
            def on_call(self, target, ret, at):
                self.steps += 1
                if self.steps > self.limit:
                    raise emu.StepLimit("step limit in calls")
                return ret

        loop = {0x1000: 0xB002, 0x1002: 0x0009, 0x1004: 0xAFFC, 0x1006: 0x0009}   # bsr; nop; bra 0x1000; nop
        for limit in (2, 3, 4, 5, 6, 7):
            c = Counting(loop)
            c.limit = limit
            with self.assertRaises(emu.StepLimit):
                c.run(0x1000, limit)
            self.assertEqual(c.steps, limit + 1, limit)

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


@unittest.skipUnless((dt.ROOT / "build" / "prog.bin").exists(), "needs build/prog.bin")
class StaticArityTests(unittest.TestCase):
    """The static arity guess reads only code control flow reaches."""

    def test_pool_word_is_not_an_argument_read(self):
        # f_2e5aa(char n): its pool at 0x2e5c4 holds 0x1878, which decodes as
        # mov.l r7,@(32,r8); fn.py's pool detection misses that word
        code = dt.reachable_code(0x2E5AA, 0x2E6FC)
        self.assertNotIn(0x2E5CA, {pc for pc, _ in code})
        self.assertEqual(emu.static_arg_regs(code), 1)

    def test_known_arities_are_kept(self):
        # f_2af5e(int, int, long), f_29cc6(4 arguments) and f_e412 (4)
        import fn
        for rom, n in ((0x2AF5E, 3), (0x29CC6, 4), (0xE412, 4)):
            code = dt.reachable_code(rom, fn.extent(rom)[0])
            self.assertIsNotNone(code, hex(rom))
            self.assertEqual(emu.static_arg_regs(code), n, hex(rom))

    def test_unresolved_jump_falls_back(self):
        # an indirect jump other than a switch table leaves the set unknown
        import asm_scan
        with patch.object(asm_scan.Func, "walk", lambda self: self.unknown_jumps.append(0)):
            self.assertIsNone(dt.reachable_code(0x2E5AA, 0x2E6FC))


class BootTargetArityTests(unittest.TestCase):
    def test_boot_block_target_is_not_measured(self):
        # the warm-reset entry 0x686 runs from ROM; measuring it used to crash
        self.assertEqual(dt.Ctx()._measure(0x686), (1, 0))


class ResultRangeTests(unittest.TestCase):
    def test_values_inside_stay_and_the_rest_are_mapped_in(self):
        for h in range(0, 1 << 20, 4099):
            self.assertEqual(dt.in_range(3, h, (0, 4)), 3)
            self.assertEqual(dt.in_range(0xFFFFFFFF, h, (-1, 4)), 0xFFFFFFFF)
            for r0 in (5, 0xFFFFFFF5, 0x0612EB58):
                self.assertIn(dt.in_range(r0, h, (0, 4)), range(5))
                self.assertIn(emu.s32(dt.in_range(r0, h, (-3, -1))), (-3, -2, -1))
        self.assertEqual({dt.in_range(99, h << 8, (0, 4)) for h in range(50)}, set(range(5)))

    def test_spec(self):
        resolve = {"f_2beca": 0x0602B74A}.get
        self.assertEqual(dt.parse_returns(["f_2beca=0..4"], resolve), {0x0602B74A: (0, 4)})
        self.assertEqual(dt.parse_returns(["f_2beca=-1..0x3f"], resolve), {0x0602B74A: (-1, 63)})
        for bad in ("f_2beca=4..0", "f_2beca=0-4", "nothing=0..4", "f_2beca=0..0x100000000"):
            with self.assertRaises(ValueError, msg=bad):
                dt.parse_returns([bad], resolve)


class StatusRunTests(unittest.TestCase):
    def test_discarding_log_keeps_nothing(self):
        log = dt._Discard()
        log.append(("W", 0, 4, 1, 0))
        self.assertEqual(len(log), 0)
