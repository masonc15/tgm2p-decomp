"""Native checksum display proof, including rejection of the near-match."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeChecksumDisplayTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x30a44,0x30c2e,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x30880)

    def test_complete_display_exact_proof_with_genuine_neighbors(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/checksum_display_30a44.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x30a44:0x30c2e])
            manifest=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['functions'],['_f_30a44'])
            self.assertEqual(manifest['kept_bytes'],490)
            self.assertEqual(manifest['context_bytes'],1216)
            self.assertEqual(len(manifest['literal_dependencies']),13)
            self.assertFalse(manifest.get('tail_proofs'))
            self.assertFalse(manifest.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                elf=ELFFile(f)
                roots={s.name:s['st_value']+0x30880 for s in elf.get_section_by_name('.symtab').iter_symbols()
                       if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            self.assertEqual(roots,{'_f_30880':0x30880,'_f_30a44':0x30a44,'_f_30c2e':0x30c2e})
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                     if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_30a44'})

    def test_postincrement_rejects_wrong_setup_registers(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/checksum_display_30a44.c').read_text()
        self.assertEqual(source.count('\tn = n + 1;'),1)
        source=source.replace('\tn = n + 1;','\tn++;')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x30a58'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
