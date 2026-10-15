"""Native range authentication and strict boundary rejection for the VM tail."""
import json
import os
from pathlib import Path
import tempfile
import unittest


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeVMTailTests(unittest.TestCase):
    def compile(self, source, output):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(source, output, 0xee9c, 0xef04,
            context_start=0xec0c, rom=build.fn.ROM, shcc=build.SHCC,
            version=build.SHC_VERSION, flags=build.c_flags(source),
            symbols=build.read_symbols(), assembler=build.AS)

    def test_complete_tail_is_exact_and_context_is_excluded(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            output = Path(td)/'kept.o'
            self.compile(build.ROOT/'src/vm_tail_ee9c.c', output)
            self.assertEqual(output.with_suffix('.prefix.bin').read_bytes(), build.fn.ROM[0xee9c:0xef04])
            context = output.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(context), 760)
            self.assertNotEqual(context[:656], build.fn.ROM[0xec0c:0xee9c])
            proof = json.loads(output.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['kept_bytes'], 104)
            self.assertEqual(proof['context_bytes'], 760)
            self.assertEqual(proof['functions'], ['_f_ee9c','_f_eeb0','_f_eeb4','_f_eeb8','_f_eebc'])
            self.assertFalse(proof['literal_dependencies'])
            self.assertFalse(proof.get('tail_proofs', []))
            with output.open('rb') as f:
                elf = ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, set(proof['functions']))

    def assert_rejected(self, old, new):
        import build
        from c_prefix import PrefixError
        source = (build.ROOT/'src/vm_tail_ee9c.c').read_text()
        self.assertIn(old, source)
        with tempfile.TemporaryDirectory() as td:
            src = Path(td)/'negative.c'
            src.write_text(source.replace(old,new))
            output = Path(td)/'kept.o'
            with self.assertRaises(PrefixError):
                self.compile(src,output)
            self.assertFalse(output.exists())
            self.assertFalse(output.with_suffix('.prefix.json').exists())

    def test_widened_completion_counter_rejects_shifted_tail(self):
        self.assert_rejected('\tunsigned char done;', '\tshort done;')

    def test_separate_counter_initialization_rejects_shifted_tail(self):
        self.assert_rejected('\ti = row = col = 0;\n\tx = 24;',
                             '\ti = 0;\n\tx = 24;\n\trow = 0;\n\tcol = 0;')
