"""Complete skill-medal update with genuine shared literal context."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeMedalSkillsTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x217a2,0x21862,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x216d0)

    def test_complete_function_roots_exports_and_shared_literals(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/medal_skills_217a2.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x217a2:0x21862])
            manifest=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(set(manifest['functions']),{'_f_217a2'})
            self.assertEqual((manifest['kept_bytes'],manifest['context_bytes']),(192,572))
            self.assertEqual(manifest['literal_dependencies'],[
                dict(pc=0x21808,target=0x218f2,size=2,value='033c'),
                dict(pc=0x21810,target=0x218f4,size=2,value='0340'),
                dict(pc=0x2181c,target=0x218f2,size=2,value='033c'),
                dict(pc=0x21820,target=0x21900,size=4,value='0607cd48'),
                dict(pc=0x21842,target=0x218f6,size=2,value='030e')])
            self.assertFalse(manifest.get('tail_proofs'))
            self.assertFalse(manifest.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                e=ELFFile(f)
                roots={s.name:s['st_value']+0x216d0 for s in e.get_section_by_name('.symtab').iter_symbols()
                       if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            self.assertEqual(roots,{'_f_216d0':0x216d0,'_f_217a2':0x217a2,'_f_21862':0x21862})
            with out.open('rb') as f:
                e=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in e.iter_sections()))
                self.assertEqual({s.name for s in e.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_217a2'})

    def test_changed_shared_table_pool_is_rejected(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/medal_skills_217a2.c').read_text()
        needle='(t = d_3b58c)'
        self.assertEqual(source.count(needle),1)
        source=source.replace('\tstruct icon *t;\n','').replace(needle,'d_3b58c')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x217f4'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
