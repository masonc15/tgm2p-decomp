"""Native task allocator proof and rejection of the wrong priority form."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeTaskAllocatorTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x2452e,0x245da,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x23f08)

    def test_allocator_and_genuine_neighbors_match(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/task_23f08.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x2452e:0x245da])
            self.assertEqual(out.with_suffix('.context.bin').read_bytes(),build.fn.ROM[0x23f08:0x248a8])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_2452e'])
            self.assertEqual(proof['kept_bytes'],172)
            self.assertEqual(proof['context_bytes'],2464)
            self.assertEqual(len(proof['literal_dependencies']),5)
            self.assertFalse(proof.get('tail_proofs'))
            self.assertFalse(proof.get('switch_proofs'))
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_2452e'})

    def test_embedded_postincrement_rejects_swapped_pointer_registers(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/task_23f08.c').read_text()
        step='\t\t*p = 0;\n\t\tp += 1;'
        self.assertEqual(source.count(step),1)
        source=source.replace(step,'\t\t*p++ = 0;')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x2454e'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
