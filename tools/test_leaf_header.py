import tempfile
import unittest
from pathlib import Path
import build


class LeafHeaderTests(unittest.TestCase):
    def read(self, header):
        with tempfile.TemporaryDirectory() as td:
            source = Path(td) / 'case.c'
            source.write_text(header + '\nvoid f(void) {}\n')
            return build.c_leaf_argument_tails(source)

    def test_defaults_and_separate_opt_in(self):
        self.assertFalse(self.read('/* literal-tail-calls: 1 */'))
        self.assertFalse(self.read('/* no proof flags */'))
        self.assertTrue(self.read('/* literal-tail-calls: 1 leaf-argument-tails: 1 */'))

    def test_missing_literal_mode_fails(self):
        with self.assertRaises(ValueError):
            self.read('/* leaf-argument-tails: 1 */')

    def test_invalid_and_duplicate_markers_fail(self):
        for marker in ['0', '2', 'true', '1 leaf-argument-tails: 1']:
            with self.subTest(marker=marker), self.assertRaises(ValueError):
                self.read('/* literal-tail-calls: 1 leaf-argument-tails: ' + marker + ' */')
