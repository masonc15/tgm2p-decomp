"""Authenticate the complete object-list gap and reject its near-match control."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeObjectListTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x251cc,0x2521e,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x248a8)

    def test_object_list_and_entire_context_match(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/objfile_248a8.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x251cc:0x2521e])
            self.assertEqual(out.with_suffix('.context.bin').read_bytes(),build.fn.ROM[0x248a8:0x2528c])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_251cc'])
            self.assertEqual(proof['kept_bytes'],82)
            self.assertEqual(proof['context_bytes'],2532)
            self.assertEqual(len(proof['literal_dependencies']),5)
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_251cc'})

    def test_unconsumed_pointer_assignment_rejects(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/objfile_248a8.c').read_text()
        old='*(hw = g_24003800) = 0;'
        self.assertEqual(source.count(old),1)
        source=source.replace(old,'hw = g_24003800;\n\thw[0] = 0;')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x251cd'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
