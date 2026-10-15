"""Authenticate the full speed-screen function and reject its near-match control."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeSpeedScreenTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x7956,0x7c08,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x7658)

    def test_complete_function_only_exact_original_roots_and_pool(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/speed_7956.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x7956:0x7c08])
            raw=out.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(raw),1456)
            self.assertEqual(raw[0x797a-0x7658:0x799c-0x7658],build.fn.ROM[0x797a:0x799c])
            self.assertEqual(raw[0x7bda-0x7658:0x7bde-0x7658],bytes.fromhex('000b6ef6'))
            self.assertNotEqual(raw[:766],build.fn.ROM[0x7658:0x7956])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_7956'])
            self.assertEqual(proof['kept_bytes'],690)
            self.assertEqual(proof['context_bytes'],1456)
            self.assertFalse(proof['literal_dependencies'])
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                elf=ELFFile(f)
                roots={s.name:0x7658+s['st_value'] for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(roots,{'_f_7658':0x7658,'_f_7956':0x7956})
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_7956'})

    def test_separate_quotient_assignment_rejects(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/speed_7956.c').read_text()
        needle='ox = -(k * (t = ((v - 63) * 1024) / 63));'
        self.assertEqual(source.count(needle),1)
        source=source.replace(needle,'t = ((v - 63) * 1024) / 63;\n\tox = -(k * t);')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x79ae'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
