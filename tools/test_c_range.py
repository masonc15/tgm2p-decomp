"""Verified middle-range extraction; compile tests run on nuada."""
import json
import os
from pathlib import Path
import tempfile
import unittest

from c_prefix import PrefixError, compile_prefix, validate_prefix


def words(*xs):
    return b''.join(x.to_bytes(2, 'big') for x in xs)


class RangeValidationTests(unittest.TestCase):
    def check(self, code, compiled=None, *, end=0x110, entries=None, start=0x108, origin=0x100):
        return validate_prefix(code if compiled is None else compiled,
                              b'\0' * 0x100 + code, start, end,
                              entries if entries is not None else
                              {0x100: '_f_100', 0x108: '_f_108', end: f'_f_{end:x}'},
                              context_start=origin)

    def test_backward_call_into_discarded_context(self):
        code = words(0x000b, 0x0009, 0xffff, 0xffff,
                     0xbffa, 0x0009, 0x000b, 0x0009, 0x000b, 0x0009)
        # Leading C need not match: the linked program keeps original assembly
        # there, and the retained branch still targets its verified entry.
        compiled = words(0xc700, 0x412b, 0xffff, 0xffff) + code[8:]
        self.assertEqual(self.check(code, compiled), [])

    def test_misplaced_backward_callee_rejected(self):
        code = words(0x000b, 0x0009, 0xffff, 0xffff,
                     0xbffa, 0x0009, 0x000b, 0x0009, 0x000b, 0x0009)
        with self.assertRaisesRegex(PrefixError, 'unverified control-flow exit'):
            self.check(code, entries={0x100: '_f_102', 0x108: '_f_108', 0x110: '_f_110'})

    def test_suffix_literal_uses_context_origin(self):
        code = words(0x000b, 0x0009, 0xffff, 0xffff,
                     0x9004, 0x000b, 0x0009, 0x000b, 0x0009, 0xffff, 0x0364)
        self.assertEqual(self.check(code, end=0x10e),
                         [dict(pc=0x108, target=0x114, size=2, value='0364')])
        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.check(code, code[:-2] + words(0x0365), end=0x10e)

    def test_changed_kept_bytes_rejected(self):
        code = words(0xffff, 0xffff, 0xffff, 0xffff, 0xe000, 0x000b, 0x0009)
        with self.assertRaisesRegex(PrefixError, 'compiled range differs'):
            self.check(code, code[:8] + words(0xe001) + code[10:], end=0x10e)

    def test_cut_inside_delay_slot_rejected(self):
        code = words(0xffff, 0xffff, 0xffff, 0xffff, 0x000b, 0x0009)
        with self.assertRaisesRegex(PrefixError, 'instruction crosses range boundary'):
            self.check(code, end=0x10a)

    def test_invalid_origin_rejected(self):
        code = words(*([0x0009] * 16))
        for origin in (0x109, 0x10a, -2):
            with self.subTest(origin=origin), self.assertRaisesRegex(PrefixError, 'invalid range bounds'):
                self.check(code, origin=origin)

    def test_short_context_rejected(self):
        code = words(*([0x0009] * 16))
        with self.assertRaisesRegex(PrefixError, 'compiled context shorter than range'):
            self.check(code, code[:10])

    def test_start_must_be_function_boundary(self):
        code = words(*([0x0009] * 16))
        with self.assertRaisesRegex(PrefixError, 'function boundaries'):
            self.check(code, start=0x106)


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'requires nuada SHC/binutils')
class RangeCompileTests(unittest.TestCase):
    def run_case(self, *, wrong_sound=False, origin=0x2f68):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory(prefix='c-range-test-') as td:
            td = Path(td)
            source = (build.ROOT / 'src/gravity_3e2e.c').read_text()
            if wrong_sound:
                self.assertEqual(source.count('f_2e6fc'), 2)
                source = source.replace('f_2e6fc', 'f_2df7c')
            src = td / 'case.c'
            src.write_text(source)
            self.assertEqual(build.c_context_start(src), 0x2f68)
            obj = td / 'gravity.o'
            compile_prefix(src, obj, 0x3e2e, 0x4044, context_start=origin,
                          rom=build.fn.ROM, shcc=build.SHCC, version=build.SHC_VERSION,
                          flags=build.c_flags(src), symbols=build.read_symbols(), assembler=build.AS)
            with obj.open('rb') as f:
                elf = ELFFile(f)
                self.assertEqual(elf.get_section_by_name('.text').data(), build.fn.ROM[0x3e2e:0x4044])
                functions = {s.name: s['st_value'] for s in elf.get_section_by_name('.symtab').iter_symbols()
                             if s['st_info']['type'] == 'STT_FUNC'}
            context = obj.with_suffix('.context.bin').read_bytes()
            self.assertNotEqual(context[0x3c88-origin:0x3e2e-origin], build.fn.ROM[0x3c88:0x3e2e])
            return functions, json.loads(obj.with_suffix('.range.json').read_text())

    def test_only_gravity_functions_are_exported_at_range_offsets(self):
        functions, manifest = self.run_case()
        self.assertEqual(functions, {'_f_3e2e': 0, '_f_3f92': 0x164})
        self.assertEqual(manifest['context_start'], 0x2f68)
        self.assertEqual(manifest['start'], 0x3e2e)
        self.assertEqual(manifest['end'], 0x4044)
        self.assertEqual(manifest['kept_bytes'], 534)
        self.assertEqual(manifest['literal_dependencies'], [])

    def test_wrong_sound_address_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'compiled range differs'):
            self.run_case(wrong_sound=True)

    def test_wrong_context_origin_rejected(self):
        with self.assertRaises(PrefixError):
            self.run_case(origin=0x2f6a)

    def test_missing_or_duplicate_origin_marker_rejected(self):
        import build
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / 'case.c'
            for source in ('void f(void) {}', '/* context-rom: 0x2f68 */\n' * 2):
                p.write_text(source)
                with self.assertRaisesRegex(ValueError, 'exactly one'):
                    build.c_context_start(p)


if __name__ == '__main__':
    unittest.main()
