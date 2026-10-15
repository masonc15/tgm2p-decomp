"""Native EEPROM writer/reader pair and offset-result lifetime regression."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeSaveIOTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x21206,0x213da,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x21170)

    def test_complete_pair_roots_exports_and_literals(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/saveio_21206.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x21206:0x213da])
            m=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(set(m['functions']),{'_f_21206','_f_212f8'})
            self.assertEqual((m['kept_bytes'],m['context_bytes']),(468,740))
            self.assertEqual(m['literal_dependencies'],[
                dict(pc=0x21304,target=0x2143c,size=4,value='06065650'),
                dict(pc=0x21308,target=0x21440,size=4,value='0602ff0c'),
                dict(pc=0x2131c,target=0x21444,size=4,value='06065640'),
                dict(pc=0x21390,target=0x21448,size=4,value='060657a0')])
            self.assertFalse(m.get('tail_proofs'))
            self.assertFalse(m.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                e=ELFFile(f);roots={s.name:s['st_value']+0x21170 for s in e.get_section_by_name('.symtab').iter_symbols()
                      if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            self.assertEqual(roots,{'_f_21170':0x21170,'_f_211a2':0x211a2,'_f_211e8':0x211e8,
                '_f_21206':0x21206,'_f_212f8':0x212f8,'_f_213da':0x213da,'_f_2140e':0x2140e})
            with out.open('rb') as f:
                e=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in e.iter_sections()))
                self.assertEqual({s.name for s in e.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_21206','_f_212f8'})

    def test_direct_helper_return_rejects_unmatched_register_schedule(self):
        import build
        from c_prefix import PrefixError
        s=(build.ROOT/'src/saveio_21206.c').read_text()
        needle='static short io_difference(void *b, void *p) { short offset = (long)p - (long)b; return offset; }'
        self.assertEqual(s.count(needle),1)
        s=s.replace(needle,'static short io_difference(void *b, void *p) { return (long)p - (long)b; }')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(s);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x2122a'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
