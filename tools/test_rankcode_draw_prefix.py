"""Retain the complete rank-code drawing owner; reject its near-match."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeRankCodeDrawTests(unittest.TestCase):
    def compile(self, source, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(source, out, 0x23e3e, 0x23f08,
            rom=build.fn.ROM, shcc=build.SHCC, version=build.SHC_VERSION,
            flags=build.c_flags(source), symbols=build.read_symbols(),
            assembler=build.AS, context_start=0x238fc)

    def test_complete_owner_and_excluded_predecessor(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as temporary:
            out=Path(temporary)/'kept.o'
            self.compile(build.ROOT/'src/rankcode_draw_23e3e.c',out)
            raw=out.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(raw),1548)
            self.assertNotEqual(raw[:1346],build.fn.ROM[0x238fc:0x23e3e])
            self.assertEqual(raw[1346:],build.fn.ROM[0x23e3e:0x23f08])
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x23e3e:0x23f08])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_23e3e'])
            self.assertEqual(proof['kept_bytes'],202)
            self.assertEqual(proof['context_start'],0x238fc)
            self.assertEqual(proof['literal_dependencies'],[])
            with out.open('rb') as stream:
                elf=ELFFile(stream)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                names={s.name for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(names,{'_f_23e3e'})

    def test_semantically_equivalent_update_is_rejected(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/rankcode_draw_23e3e.c').read_text()
        self.assertEqual(source.count('x = x + 10;'),1)
        source=source.replace('x = x + 10;','x += 10;')
        with tempfile.TemporaryDirectory() as temporary:
            p=Path(temporary)/'negative.c';p.write_text(source)
            out=Path(temporary)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM'):
                self.compile(p,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
