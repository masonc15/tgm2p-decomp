"""Exact field scan and independent negative controls for both source changes."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeFieldRecipeTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x81cc,0x8314,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x81cc)

    def test_field_scan_and_context_match(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/field_hole_81cc.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x81cc:0x8314])
            self.assertEqual(out.with_suffix('.context.bin').read_bytes(),build.fn.ROM[0x81cc:0x8518])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_81cc'])
            self.assertEqual(proof['kept_bytes'],328)
            self.assertEqual(proof['context_bytes'],844)
            self.assertEqual(len(proof['literal_dependencies']),2)
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_81cc'})

    def test_equality_zero_control_rejects_register_rotation(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/field_hole_81cc.c').read_text()
        for col in ['y','k']:
            old='!f->cells[y * f->width + '+col+'].flags'
            self.assertEqual(source.count(old),1)
            source=source.replace(old,'f->cells[y * f->width + '+col+'].flags == 0')
        self.reject(source, '0x8260')

    def test_base_first_control_rejects_address_destination(self):
        import build
        source=(build.ROOT/'src/field_hole_81cc.c').read_text()
        for col in ['y','k']:
            old='((((y+1)*f->width)+'+col+')*6) + (long)&f->cells->flags'
            self.assertEqual(source.count(old),1)
            source=source.replace(old,'(long)&f->cells->flags + ((((y+1)*f->width)+'+col+')*6)')
        self.reject(source, '0x8278')

    def reject(self, source, address):
        from c_prefix import PrefixError
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at '+address):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
