"""Native full-function match and rejection gate for the board queue producer."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeBoardQueueTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src, out, 0x2733a, 0x274c0, rom=build.fn.ROM,
            shcc=build.SHCC, version=build.SHC_VERSION, flags=build.c_flags(src),
            symbols=build.read_symbols(), assembler=build.AS, context_start=0x27310)

    def test_complete_producer_native_roots_exports_and_no_outside_literals(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o'
            self.compile(build.ROOT/'src/board_queue_2733a.c',out)
            kept=out.with_suffix('.prefix.bin').read_bytes()
            self.assertEqual(kept,build.fn.ROM[0x2733a:0x274c0])
            manifest=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['functions'],['_f_2733a'])
            self.assertEqual((manifest['kept_bytes'],manifest['context_bytes']),(390,980))
            self.assertEqual(manifest['literal_dependencies'],[])
            self.assertFalse(manifest.get('tail_proofs'))
            self.assertFalse(manifest.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                e=ELFFile(f)
                roots={s.name:s['st_value']+0x27310 for s in e.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            # Unretained trailing dispatch remains shifted by two bytes.
            self.assertEqual(roots,{'_f_27310':0x27310,'_f_2733a':0x2733a,
                '_f_274c0':0x274c0,'_f_2769e':0x2769c})
            with out.open('rb') as f:
                e=ELFFile(f)
                self.assertEqual(e.get_section_by_name('.text').data(),kept)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in e.iter_sections()))
                self.assertEqual({s.name for s in e.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'},{'_f_2733a'})

    def test_nested_record_expression_rejects_nine_byte_temporary_mismatch(self):
        import build
        from c_prefix import PrefixError
        source=(build.ROOT/'src/board_queue_2733a.c').read_text()
        needle='    record = (struct rec104 *)((char *)g_60ad228 + (short)(f->a24[(short)cur] * 104));\n    k = record->w84;'
        self.assertEqual(source.count(needle),1)
        source=source.replace('    struct rec104 *record;\n','').replace(needle,
            '    k = ((struct rec104 *)((char *)g_60ad228 + (short)(f->a24[(short)cur] * 104)))->w84;')
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source);out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM at 0x27376'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
