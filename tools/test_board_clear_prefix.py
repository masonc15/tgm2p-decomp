"""Protect the complete board clear and its genuine compiler-context dependency."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeBoardClearTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x278c0,0x27b50,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x276e8)

    def test_complete_clear_with_excluded_preceding_context(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/board_clear_278c0.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x278c0:0x27b50])
            raw=out.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(raw),1128)
            self.assertNotEqual(raw[:472],build.fn.ROM[0x276e8:0x278c0])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_278c0'])
            self.assertEqual(proof['kept_bytes'],656)
            self.assertFalse(proof['literal_dependencies'])
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            with out.with_suffix('.context.elf').open('rb') as f:
                elf=ELFFile(f)
                roots={s.name:s['st_value']-0x5fff880 for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(roots['_f_278c0'],0x278c0)
                self.assertEqual(roots['_f_27862'],0x27860) # excluded, not credited
                self.assertEqual(len(roots),11)
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_278c0'})

    def test_repeated_index_calculation_cannot_shift_retained_body(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/board_clear_278c0.c').read_text()
        self.assertEqual(source.count('idx *= 4;'),1)
        source=source.replace('\tidx *= 4;\n','').replace('128 + idx)', '128 + idx * 4)')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'contextual function boundaries'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
