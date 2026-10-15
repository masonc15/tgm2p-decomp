"""Protect exact palette update, initialization, tails and strict negative controls."""
import json
import os
from pathlib import Path
import re
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativePaletteInitializerTests(unittest.TestCase):
    def compile(self, source, output):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(source, output, 0x2a0d4, 0x2ac4c,
            context_start=0x2a0d4, rom=build.fn.ROM, shcc=build.SHCC,
            version=build.SHC_VERSION, flags=build.c_flags(source),
            symbols=build.read_symbols(), assembler=build.AS)

    def test_complete_palette_range_is_exact(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            output=Path(td)/'kept.o'
            self.compile(build.ROOT/'src/gfx_2ab56.c',output)
            want=build.fn.ROM[0x2a0d4:0x2ac4c]
            self.assertEqual(len(want),2936)
            self.assertEqual(output.with_suffix('.prefix.bin').read_bytes(),want)
            context=output.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(context),2936)
            self.assertEqual(context,want)
            proof=json.loads(output.with_suffix('.prefix.json').read_text())
            functions=['_f_2a0d4','_f_2a576','_f_2ab56','_f_2abce','_f_2abd6','_f_2abde']
            self.assertEqual(proof['functions'],functions)
            self.assertEqual(proof['kept_bytes'],2936)
            self.assertFalse(proof['literal_dependencies'])
            with output.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'},set(functions))

    def test_equivalent_unaliased_source_is_not_accepted_as_a_match(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/gfx_2ab56.c').read_text()
        source,count=re.subn(r'\t\t\tstruct pfade \*current;\n\t\t\tcurrent = o;\n\t\t\tcurrent->cur', '\t\t\to->cur',source)
        self.assertEqual(count,12)
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);output=Path(td)/'kept.o'
            with self.assertRaises(PrefixError):self.compile(src,output)
            self.assertFalse(output.exists())
            self.assertFalse(output.with_suffix('.prefix.json').exists())

    def test_reversed_shared_labels_fail_exact_pool_layout(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/gfx_2ab56.c').read_text()
        old='case 0x44:\n\t\t\tcase 0x40:'
        self.assertEqual(source.count(old),1)
        source=source.replace(old,'case 0x40:\n\t\t\tcase 0x44:')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);output=Path(td)/'kept.o'
            with self.assertRaises(PrefixError):self.compile(src,output)
            self.assertFalse(output.exists())
            self.assertFalse(output.with_suffix('.prefix.json').exists())
