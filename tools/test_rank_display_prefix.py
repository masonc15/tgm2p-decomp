"""Authenticate the full rank renderer and reject a semantically equivalent near match."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeRankDisplayTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x12ad4,0x12fa8,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x120c0)

    def test_complete_function_and_original_context_boundaries(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/rank_display_12ad4.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x12ad4:0x12fa8])
            raw=out.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(raw),4404)
            self.assertNotEqual(raw[:0x12ad4-0x120c0],build.fn.ROM[0x120c0:0x12ad4])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_12ad4'])
            self.assertEqual(proof['kept_bytes'],1236)
            self.assertEqual(proof['context_start'],0x120c0)
            self.assertFalse(proof['literal_dependencies'])
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            expected=[0x120c0,0x121fe,0x12252,0x122cc,0x12380,0x124a8,0x12530,0x12570,0x125c8,0x12608,0x12a58,0x12ad4,0x12fa8]
            with out.with_suffix('.context.o').open('rb') as f:
                elf=ELFFile(f)
                roots={s.name:0x120c0+s['st_value'] for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(roots,{'_f_%x'%a:a for a in expected})
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_12ad4'})

    def test_uniform_pointer_updates_reject(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/rank_display_12ad4.c').read_text()
        self.assertEqual(source.count('p = p + 1;'),2)
        self.assertEqual(source.count('q = q + 1;'),1)
        source=source.replace('p = p + 1;','p++;').replace('q = q + 1;','q++;')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x12be0'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
