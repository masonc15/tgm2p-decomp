import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from c_prefix import PrefixError, compile_prefix, validate_prefix


def words(*xs):
    return b''.join(x.to_bytes(2, 'big') for x in xs)


class PrefixValidationTests(unittest.TestCase):
    def check(self, code, *, compiled=None, end=6, entries=None):
        return validate_prefix(code if compiled is None else compiled, code, 0, end,
                               entries if entries is not None else {0: '_f_0', end: f'_f_{end:x}'})

    def test_cross_boundary_word(self):
        code = words(0x9003, 0x000b, 0x0009, 0x000b, 0x0009, 0x0364)
        self.assertEqual(self.check(code), [dict(pc=0, target=10, size=2, value='0364')])

    def test_cross_boundary_long(self):
        code = words(0xd102, 0x000b, 0x0009, 0x000b, 0x0009, 0xffff, 0x0606, 0x475a)
        self.assertEqual(self.check(code)[0]['value'], '0606475a')

    def test_changed_prefix_rejected(self):
        code = words(0xe000, 0x000b, 0x0009, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.check(code, compiled=words(0xe001) + code[2:])

    def test_changed_discarded_pool_rejected(self):
        code = words(0x9003, 0x000b, 0x0009, 0x000b, 0x0009, 0x0364)
        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.check(code, compiled=code[:-2] + words(0x0365))

    def test_missing_pool_rejected(self):
        code = words(0x9003, 0x000b, 0x0009, 0xffff, 0xffff, 0x0364)
        with self.assertRaisesRegex(PrefixError, 'outside compiled context'):
            self.check(code, compiled=code[:6])

    def test_cut_inside_function_rejected(self):
        code = words(0xe000, 0x000b, 0x0009, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'function boundaries'):
            self.check(code, entries={0: '_f_0'})

    def test_delay_slot_literal_rejected(self):
        code = words(0x000b, 0x9000, 0xffff, 0x1234)
        with self.assertRaisesRegex(PrefixError, 'delay-slot'):
            self.check(code, end=4)

    def test_indirect_jump_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'indirect jump'):
            self.check(words(0x412b, 0x0009, 0xffff, 0xffff))

    def test_mova_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'MOVA'):
            self.check(words(0xc701, 0x000b, 0x0009, 0xffff))

    def test_control_transfer_in_slot_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'control transfer'):
            self.check(words(0x000b, 0x000b, 0x0009, 0xffff))

    def test_conditional_reaches_both_literal_loads(self):
        code = words(0x8902, 0x9005, 0xa001, 0x0009, 0x9104, 0x000b, 0x0009,
                     0x000b, 0x1234, 0xffff, 0x5678)
        self.assertEqual([v['target'] for v in self.check(code, end=14)], [16,20])

    def test_unknown_exit_rejected(self):
        code = words(0xa004, 0x0009, 0x0009, 0xffff, 0xffff, 0xffff, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'unverified control-flow exit'):
            self.check(code)

    def test_misplaced_context_function_rejected(self):
        code = words(0xa001, 0x0009, 0x0009, 0x000b, 0x0009)
        with self.assertRaisesRegex(PrefixError, 'unverified control-flow exit'):
            self.check(code, entries={0: '_f_0', 6: '_f_8'})

    def test_verified_context_tailcall_allowed(self):
        code = words(0xa001, 0x0009, 0x0009, 0x000b, 0x0009)
        self.assertEqual(self.check(code), [])

    def test_unreachable_pool_not_decoded_as_code(self):
        code = words(0x000b, 0x0009, 0xc700, 0x412b, 0xffff)
        self.assertEqual(self.check(code, end=8), [])


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'requires nuada SHC/binutils')
class PrefixCompileTests(unittest.TestCase):
    def run_case(self, filename, start, end, mutation=None):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            source = (build.ROOT / 'src' / filename).read_text()
            if mutation:
                old, new = mutation
                self.assertIn(old, source)
                source = source.replace(old, new, 1)
            src = td / 'case.c'
            src.write_text(source)
            obj = td / 'kept.o'
            compile_prefix(src, obj, start, end, rom=build.fn.ROM, shcc=build.SHCC,
                           version=build.SHC_VERSION, flags=build.c_flags(src),
                           symbols=build.read_symbols(), assembler=build.AS)
            with obj.open('rb') as f:
                elf = ELFFile(f)
                self.assertEqual(elf.get_section_by_name('.text').data(), build.fn.ROM[start:end])
                names = {s.name for s in elf.get_section_by_name('.symtab').iter_symbols()}
            return names, json.loads(obj.with_suffix('.prefix.json').read_text())

    def test_rotation_prefix_has_no_das_export(self):
        names, manifest = self.run_case('f_2f68.c', 0x2f68, 0x3c88)
        self.assertIn('_f_3a02', names)
        self.assertNotIn('_f_3c88', names)
        self.assertEqual(manifest['kept_bytes'], 3360)
        self.assertEqual(manifest['literal_dependencies'],
                         [dict(pc=0x3c4c, target=0x3d14, size=2, value='0364')])

    def test_draw_prefix_has_no_context_export(self):
        names, manifest = self.run_case('draw_15f10.c', 0x15f10, 0x15fae)
        self.assertIn('_f_15f10', names)
        self.assertNotIn('_f_15fae', names)
        self.assertEqual(manifest['kept_bytes'], 158)
        self.assertEqual(len(manifest['literal_dependencies']), 13)

    def test_changed_field_layout_fails(self):
        # This changes an earlier copy of the offset as well as the external
        # pool word, so the retained-byte check rejects it first.
        with self.assertRaisesRegex(PrefixError, 'compiled prefix differs'):
            self.run_case('f_2f68.c', 0x2f68, 0x3c88, ('char pad6;', 'char pad6[5];'))

    def test_corrupt_compiler_literal_fails(self):
        import subprocess
        real_run = subprocess.run
        def corrupt_pool(command, **kwargs):
            result = real_run(command, **kwargs)
            if command[0] == 'sh-elf-objcopy':
                output = Path(command[-1])
                data = bytearray(output.read_bytes())
                data[0x3d14 - 0x2f68] ^= 1
                output.write_bytes(data)
            return result
        with patch('c_prefix.subprocess.run', side_effect=corrupt_pool):
            with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
                self.run_case('f_2f68.c', 0x2f68, 0x3c88)

    def test_changed_retained_c_fails(self):
        with self.assertRaisesRegex(PrefixError, 'compiled prefix differs'):
            self.run_case('f_2f68.c', 0x2f68, 0x3c88, ('k &= 15;', 'k &= 7;'))


if __name__ == '__main__':
    unittest.main()
