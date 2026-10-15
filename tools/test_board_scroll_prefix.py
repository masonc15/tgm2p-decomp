"""Protect the full board-scroll range and its loop-scheduling requirement."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeBoardScrollTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x27b50,0x27e70,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS)

    def test_complete_scroll_without_excluded_context(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/board_scroll_27b50.c',out)
            want=build.fn.ROM[0x27b50:0x27e70]
            self.assertEqual(len(want),800)
            self.assertEqual(out.with_suffix('.context.bin').read_bytes(),want)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),want)
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_27b50'])
            self.assertEqual(proof['kept_bytes'],800)
            self.assertFalse(proof['literal_dependencies'])
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            with out.with_suffix('.context.elf').open('rb') as f:
                elf=ELFFile(f)
                roots={s.name:s['st_value']-0x5fff880 for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(roots,{'_f_27b50':0x27b50})
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_27b50'})

    def test_earlier_counter_update_is_not_accepted(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/board_scroll_27b50.c').read_text()
        old='pa++, pb++, px++, pc++, tbl++, i--'
        self.assertEqual(source.count(old),1)
        source=source.replace(old,'i--, pa++, pb++, px++, pc++, tbl++')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaises(PrefixError):self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
