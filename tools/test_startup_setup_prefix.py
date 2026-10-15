"""Native regression for startup setup and its source-sensitive shared literals."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeStartupSetupTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src, out, 0x1278, 0x12e2, rom=build.fn.ROM,
            shcc=build.SHCC, version=build.SHC_VERSION, flags=build.c_flags(src),
            symbols=build.read_symbols(), assembler=build.AS)

    def test_exact_prefix_calls_and_six_original_literals(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / 'kept.o'
            self.compile(build.ROOT / 'src/startup_setup_1278.c', out)
            data = out.with_suffix('.prefix.bin').read_bytes()
            self.assertEqual(data, build.fn.ROM[0x1278:0x12e2])
            manifest = json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['functions'], ['_f_1278'])
            self.assertEqual(manifest['literal_dependencies'], [
                dict(pc=0x1288,target=0x13a8,size=4,value='06064898'),
                dict(pc=0x128e,target=0x13ac,size=4,value='10000010'),
                dict(pc=0x1290,target=0x139c,size=2,value='03b4'),
                dict(pc=0x1294,target=0x139e,size=2,value='030e'),
                dict(pc=0x12ac,target=0x139c,size=2,value='03b4'),
                dict(pc=0x12b0,target=0x139e,size=2,value='030e')])
            self.assertFalse(manifest.get('tail_proofs'))
            with out.with_suffix('.context.elf').open('rb') as f:
                elf = ELFFile(f)
                roots = {s.name:s['st_value']-0x5fff880 for s in elf.get_section_by_name('.symtab').iter_symbols()
                         if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            for name,pc in [('_f_1278',0x1278),('_f_12e2',0x12e2),('_f_1340',0x1340)]:
                self.assertEqual(roots[name],pc)
            for pc in [0x12a8,0x12c6]:
                w=int.from_bytes(data[pc-0x1278:pc-0x1278+2],'big')
                self.assertEqual(w>>12,11)
                d=w&0xfff
                if d&0x800:d-=0x1000
                self.assertEqual(pc+4+d*2,roots['_f_1340'])
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                exports={s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                         if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(exports,{'_f_1278'})

    def test_widened_peer_index_rejects_shifted_word_literals(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/startup_setup_1278.c').read_text()
        needle='assign_peer(f, &g_6064898[(unsigned char)(id == 0)]);'
        self.assertEqual(source.count(needle),1)
        source=source.replace(needle,'assign_peer(f, &g_6064898[(short)(id == 0)]);')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x1291'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
