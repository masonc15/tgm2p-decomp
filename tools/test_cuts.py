"""Regression checks for filtering already-linked C spans (run on nuada)."""
import contextlib
import io
import sys
from pathlib import Path
import tempfile
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

    def test_middle_range_counts_only_retained_span(self):
        import build
        segs = [(self.lo - 4, self.lo, 'asm', None),
                (self.lo, self.mid, 'c-range', 'context.c'),
                (self.mid, self.hi, 'asm', None)]
        out = io.StringIO()
        with (patch.object(build, 'read_splits', return_value=segs),
              patch.object(build.fn, 'call_targets', return_value=[self.lo - 4, self.lo, self.mid]),
              contextlib.redirect_stdout(out)):
            self.assertEqual(cuts.c_ranges(), [(self.lo, self.mid)])
            build.progress()
        self.assertIn(f'{self.mid-self.lo:#x}/{self.hi-self.lo+4:#x} code bytes', out.getvalue())
        self.assertIn('1/3 called functions', out.getvalue())

    def test_middle_ranges_of_same_source_have_distinct_objects(self):
        import build
        first = build.c_object(self.lo, 'c-range', 'src/context.c')
        second = build.c_object(self.mid, 'c-range', 'src/context.c')
        self.assertNotEqual(first, second)
        self.assertEqual(build.c_object(self.lo, 'c', 'src/context.c').name, 'context.o')

    def test_literal_tail_proof_is_explicit_opt_in(self):
        import build
        with tempfile.TemporaryDirectory() as td:
            source = Path(td) / 'case.c'
            for header, expected in [
                    ('/* context-rom: 0x1278 */', False),
                    ('/* literal-tail-calls: 1 flags: -speed */', True)]:
                source.write_text(header + '\n')
                self.assertEqual(build.c_literal_tail_calls(source), expected)
            for header in ['/* literal-tail-calls: 0 */',
                           '/* literal-tail-calls: yes */',
                           '/* literal-tail-calls: 1 literal-tail-calls: 1 */']:
                source.write_text(header + '\n')
                with self.subTest(header=header), self.assertRaises(ValueError):
                    build.c_literal_tail_calls(source)

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
