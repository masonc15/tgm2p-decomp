"""Checks for tools/idle_report.py."""
import json
import tempfile
import unittest
from pathlib import Path

import idle_report as ir


class IdleReportTests(unittest.TestCase):
    def test_ranges_end_at_the_next_unit(self):
        with tempfile.TemporaryDirectory() as td:
            units = [{"kind": "asm", "vma": 0x100, "arg": None},
                     {"kind": "c-nonmatching", "vma": 0x200, "arg": "src/nonmatching/a_200.c"},
                     {"kind": "c", "vma": 0x280, "arg": "src/b.c"}]
            (Path(td) / "layout.json").write_text(json.dumps({"units": units}))
            self.assertEqual(ir.nonmatching_ranges(Path(td)), [("a_200", 0x200, 0x280)])

    def test_minimum_over_frames_the_unit_ran(self):
        ref = {1: 0, 2: 900, 3: 800, 4: 700}
        cand = {1: 0, 2: 850, 3: 600, 4: 0}
        marks = {0x200: {1, 2, 3}}
        rows = ir.summarise([("a", 0x200, 0x280)], [(ref, cand, marks, [])])
        # frame 1: the original did not wait (boot), so it is not counted
        self.assertEqual(rows, [("a", 2, 600, 800, 0)])

    def test_no_idle_loops_is_not_a_missed_frame(self):
        # the one loop of a frame that starts blocking work can land on
        # either side of the frame boundary
        rows = ir.summarise([("a", 0x200, 0x280)], [({5: 1}, {5: 0}, {0x200: {5}}, [])])
        self.assertEqual(rows, [("a", 1, 0, 1, 0)])

    def test_a_missed_frame_is_a_frame_counter_slip(self):
        ref_idle = {1: 0, 2: 700, 3: 700, 4: 700, 5: 700}
        ref_game = {1: 0, 2: 10, 3: 11, 4: 12, 5: 13}
        cand_game = {1: 99, 2: 10, 3: 11, 4: 11, 5: 12}   # frame 1 is boot, not compared
        n, behind, ahead, lag = ir.frame_slips(ref_idle, ref_game, cand_game)
        self.assertEqual((n, behind, ahead, lag), (4, [4], [], 1))
        rows = ir.summarise([("a", 0x200, 0x280), ("b", 0x280, 0x300)],
                            [(ref_idle, {4: 300, 5: 300}, {0x200: {4}, 0x280: {5}}, behind)])
        self.assertEqual([r[4] for r in rows], [1, 0])

    def test_getting_ahead_is_reported_too(self):
        n, behind, ahead, lag = ir.frame_slips({1: 5, 2: 5}, {1: 10, 2: 11}, {1: 10, 2: 12})
        self.assertEqual((n, behind, ahead, lag), (2, [], [2], -1))

    def test_read_idle_lines(self):
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "idle.txt"
            p.write_text("0 0 0\n1 1200 17\nmark 1 06000200\n")
            counts, marks = ir.read_idle(p)
            self.assertEqual(counts, {0: 0, 1: 1200})
            self.assertEqual(marks, {0x06000200: {1}})
            self.assertEqual(ir.read_game_frames(p), {0: 0, 1: 17})

    def test_frames_line(self):
        with tempfile.TemporaryDirectory() as td:
            ref, cand, old = (Path(td) / n for n in ("ref.txt", "cand.txt", "old.txt"))
            ref.write_text("1 0 0\n2 700 10\n3 700 11\n")
            cand.write_text("1 0 0\n2 650 10\n3 0 11\n")
            self.assertEqual(ir.frames_line(ref, cand),
                             (True, "game frame counter over 2 frames: identical; frames missed: 0"))
            cand.write_text("1 0 0\n2 650 10\n3 0 10\n")
            ok, line = ir.frames_line(ref, cand)
            self.assertFalse(ok)
            self.assertIn("falls behind in 1 (first at frame 3); frames missed: 1, final lag 1", line)
            # a log from before the counter was recorded cannot pass
            old.write_text("1 0\n2 700\n")
            self.assertFalse(ir.frames_line(old, cand)[0])


if __name__ == "__main__":
    unittest.main()
