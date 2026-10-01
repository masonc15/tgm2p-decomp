"""Regression checks for filtering already-linked C spans (run on nuada)."""
import contextlib
import io
import sys
import unittest
from unittest.mock import patch

import cuts


class CoverageFilterTests(unittest.TestCase):
    lo, mid, hi = 0x18FEC, 0x19014, 0x1913C

    def output(self, ranges, *args):
        out = io.StringIO()
        with (patch.object(cuts, "spans", return_value=[(self.lo, self.hi, [self.lo])]),
              patch.object(cuts, "c_ranges", return_value=ranges),
              patch.object(sys, "argv", ["cuts.py", *args]),
              contextlib.redirect_stdout(out)):
            cuts.main()
        return out.getvalue()

    def test_prefix_counts_only_its_kept_range(self):
        import build
        with patch.object(build, 'read_splits', return_value=[
                (self.lo, self.mid, 'c-prefix', 'context.c'),
                (self.mid, self.hi, 'asm', None)]):
            self.assertEqual(cuts.c_ranges(), [(self.lo, self.mid)])

    def test_single_c_segment_is_hidden(self):
        self.assertEqual(self.output([(self.lo, self.hi)]), "")

    def test_adjacent_c_segments_are_hidden(self):
        self.assertEqual(self.output([(self.lo, self.mid), (self.mid, self.hi)]), "")

    def test_clipped_adjacent_segments_are_hidden(self):
        self.assertEqual(self.output([(self.lo - 4, self.mid), (self.mid, self.hi + 4)]), "")

    def test_gap_remains_visible(self):
        self.assertIn("018fec-01913c", self.output([(self.lo, self.mid), (self.mid + 2, self.hi)]))

    def test_empty_coverage_remains_visible(self):
        self.assertIn("018fec-01913c", self.output([]))

    def test_include_c_marks_adjacent_segments_complete(self):
        self.assertIn("(C)", self.output([(self.lo, self.mid), (self.mid, self.hi)], "--c"))


if __name__ == "__main__":
    unittest.main()
