"""Native checksum match and accumulator-width rejection."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeSaveChecksumTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x21170,0x211a2,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x21170)

    def test_complete_checksum_exact_default_proof_and_original_roots(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/savechecksum_21170.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x21170:0x211a2])
            manifest=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(set(manifest['functions']),{'_f_21170'})
            self.assertEqual(manifest['kept_bytes'],50)
            self.assertEqual(manifest['context_bytes'],740)
            self.assertEqual(len(manifest['literal_dependencies']),2)
            self.assertFalse(manifest.get('tail_proofs'))
            self.assertFalse(manifest.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                elf=ELFFile(f)
                roots={s.name:s['st_value']+0x21170 for s in elf.get_section_by_name('.symtab').iter_symbols()
                       if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            self.assertEqual(roots,{'_f_21170':0x21170,'_f_211a2':0x211a2,'_f_211e8':0x211e8,
                '_f_21206':0x21206,'_f_212f8':0x212f8,'_f_213da':0x213da,'_f_2140e':0x2140e})
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                     if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_21170'})

    def test_wide_accumulator_rejects_temporary_register_mismatch(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/savechecksum_21170.c').read_text()
        needle='unsigned short f_21170(void) {\n    unsigned short sum;'
        self.assertEqual(source.count(needle),1)
        source=source.replace(needle,'unsigned short f_21170(void) {\n    long sum;')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x2118c'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
