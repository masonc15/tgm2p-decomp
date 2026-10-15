"""Native matching and strict rejection for the complete palette context."""
import json
import os
from pathlib import Path
import tempfile
import unittest


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativePaletteGradientTests(unittest.TestCase):
    def compile(self, source, output):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(source, output, 0x29bf8, 0x2a0d4,
            context_start=0x29bf8, rom=build.fn.ROM, shcc=build.SHCC,
            version=build.SHC_VERSION, flags=build.c_flags(source),
            symbols=build.read_symbols(), assembler=build.AS,
            literal_tail_calls=True, canonical_functions=build.reviewed_tail_functions())

    def test_complete_palette_context_is_exact(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            output=Path(td)/'kept.o'
            self.compile(build.ROOT/'src/palette_29bf8.c', output)
            want=build.fn.ROM[0x29bf8:0x2a0d4]
            self.assertEqual(len(want), 1244)
            self.assertEqual(output.with_suffix('.context.bin').read_bytes(), want)
            self.assertEqual(output.with_suffix('.prefix.bin').read_bytes(), want)
            proof=json.loads(output.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['kept_bytes'], 1244)
            self.assertEqual(len(proof['functions']), 13)
            self.assertIn('_f_29f94', proof['functions'])
            self.assertFalse(proof['literal_dependencies'])
            self.assertEqual(len(proof['tail_proofs']), 2)
            with output.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, set(proof['functions']))

    def assert_rejected(self, substitutions):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/palette_29bf8.c').read_text()
        for old,new in substitutions:
            self.assertIn(old, source)
            source=source.replace(old,new)
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);output=Path(td)/'kept.o'
            with self.assertRaises(PrefixError):self.compile(src,output)
            self.assertFalse(output.exists())
            self.assertFalse(output.with_suffix('.prefix.json').exists())

    def test_removing_consumed_blue_copy_is_not_a_match(self):
        self.assert_rejected([('\tunsigned char b_base;\n',''),
            ('\tb_base = b0;\n',''),('b_base','b0')])

    def test_removing_consumed_divisor_copy_is_not_a_match(self):
        self.assert_rejected([('\tint divisor;\n',''),
            ('\tdivisor = n;\n',''),('/ divisor','/ n')])
