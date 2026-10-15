"""Retained redraw wrapper: real argument spills, local calls, and shared pool."""
import json
import os
from pathlib import Path
import tempfile
import unittest


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeRedrawLockTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src, out, 0x26faa, 0x26ff0, rom=build.fn.ROM,
            shcc=build.SHCC, version=build.SHC_VERSION, flags=build.c_flags(src),
            symbols=build.read_symbols(), assembler=build.AS, context_start=0x26f64)

    def test_exact_wrapper_pool_and_callee_identity(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / 'kept.o'
            self.compile(build.ROOT / 'src/board_redraw_lock_26faa.c', out)
            raw = out.with_suffix('.prefix.bin').read_bytes()
            self.assertEqual(raw, build.fn.ROM[0x26faa:0x26ff0])
            self.assertEqual(len(raw), 70)
            self.assertEqual(raw[56:].hex(), '0e50060ad6a0060ad27c060b13ae')
            manifest = json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['functions'], ['_f_26faa'])
            self.assertEqual(manifest['literal_dependencies'], [])
            with out.with_suffix('.context.elf').open('rb') as stream:
                elf = ELFFile(stream)
                roots = {s.name:s['st_value'] - 0x5fff880
                         for s in elf.get_section_by_name('.symtab').iter_symbols()
                         if s['st_info']['type'] == 'STT_FUNC' and s['st_shndx'] != 'SHN_UNDEF'}
            for name, address in [('_f_26f64',0x26f64),('_f_26faa',0x26faa),('_f_26ff0',0x26ff0)]:
                self.assertEqual(roots[name], address)
            for pc in (0x26fc2, 0x26fd6):
                word = int.from_bytes(raw[pc-0x26faa:pc-0x26faa+2], 'big')
                displacement = word & 0xfff
                if displacement & 0x800:
                    displacement -= 0x1000
                self.assertEqual(pc + 4 + 2 * displacement, roots['_f_26ff0'])
            with out.open('rb') as stream:
                elf = ELFFile(stream)
                exports = {s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                           if s['st_info']['bind'] == 'STB_GLOBAL' and s['st_shndx'] != 'SHN_UNDEF'}
                self.assertEqual(exports, {'_f_26faa'})
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size']
                                     for s in elf.iter_sections()))

    def test_narrow_incoming_arguments_remove_required_spills_and_reject(self):
        import build
        from c_prefix import PrefixError
        source = (build.ROOT / 'src/board_redraw_lock_26faa.c').read_text()
        source = source.replace('void f_26faa(long, long, long, long);',
                                'void f_26faa(short, short, short, long);')
        needle = 'void f_26faa(long i, long cur, long k, long flag)'
        self.assertEqual(source.count(needle), 1)
        source = source.replace(needle, 'void f_26faa(short i, short cur, short k, long flag)')
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / 'negative.c'
            src.write_text(source)
            out = Path(td) / 'kept.o'
            with self.assertRaises(PrefixError):
                self.compile(src, out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeCurrentRedrawTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src, out, 0x26f64, 0x26ff0, rom=build.fn.ROM,
            shcc=build.SHCC, version=build.SHC_VERSION, flags=build.c_flags(src),
            symbols=build.read_symbols(), assembler=build.AS, context_start=0x26f64)

    def test_both_complete_wrappers_and_owned_pool(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / 'kept.o'
            self.compile(build.ROOT / 'src/board_redraw_lock_26faa.c', out)
            raw = out.with_suffix('.prefix.bin').read_bytes()
            self.assertEqual(raw, build.fn.ROM[0x26f64:0x26ff0])
            self.assertEqual(len(raw), 140)
            proof = json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(set(proof['functions']), {'_f_26f64', '_f_26faa'})
            self.assertEqual(proof['literal_dependencies'], [])
            self.assertEqual(raw[0x26fe8-0x26f64:0x26fec-0x26f64].hex(), '060ad27c')
            # The signed product and actual indexed word load are essential.
            self.assertEqual(raw[0x26f94-0x26f64:0x26f9c-0x26f64].hex(), '0d1a6ddf4f1606dd')
            with out.open('rb') as stream:
                elf = ELFFile(stream)
                exports = {s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                           if s['st_info']['bind'] == 'STB_GLOBAL' and s['st_shndx'] != 'SHN_UNDEF'}
                self.assertEqual(exports, {'_f_26f64', '_f_26faa'})

    def test_pointer_add_recipe_cannot_displace_the_following_wrapper(self):
        import build
        from c_prefix import PrefixError
        source = (build.ROOT / 'src/board_redraw_lock_26faa.c').read_text()
        needle = '*(short *)&g_60ad27c[offset]'
        self.assertEqual(source.count(needle), 1)
        source = source.replace(needle, '*(short *)((char *)g_60ad27c + offset)')
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / 'negative.c'; src.write_text(source)
            out = Path(td) / 'kept.o'
            with self.assertRaises(PrefixError):
                self.compile(src, out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
