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
        rows = ir.summarise([("a", 0x200, 0x280)], [(ref, cand, marks)])
        # frame 1: the original did not wait (boot), so it is not counted
        self.assertEqual(rows, [("a", 2, 600, 800, 0)])

    def test_a_missed_frame_is_reported(self):
        ref = {5: 700}
        cand = {5: 0}
        rows = ir.summarise([("a", 0x200, 0x280)], [(ref, cand, {0x200: {5}})])
        self.assertEqual(rows[0][4], 1)

    def test_read_idle_lines(self):
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "idle.txt"
            p.write_text("0 0\n1 1200\nmark 1 06000200\n")
            counts, marks = ir.read_idle(p)
            self.assertEqual(counts, {0: 0, 1: 1200})
            self.assertEqual(marks, {0x06000200: {1}})


if __name__ == "__main__":
    unittest.main()
