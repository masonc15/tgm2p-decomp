"""Real SHC matching and negative gates for the complete board update."""
import json
import os
from pathlib import Path
import tempfile
import unittest

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeBoardUpdateTests(unittest.TestCase):
    def compile(self, src, out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src, out, 0x259ea, 0x25f6e, rom=build.fn.ROM,
            shcc=build.SHCC, version=build.SHC_VERSION, flags=build.c_flags(src),
            symbols=build.read_symbols(), assembler=build.AS, context_start=0x255dc)

    def test_complete_function_original_roots_exports_and_shared_pool(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out = Path(td)/'kept.o'
            self.compile(build.ROOT/'src/board_update_259ea.c', out)
            kept = out.with_suffix('.prefix.bin').read_bytes()
            self.assertEqual(kept, build.fn.ROM[0x259ea:0x25f6e])
            manifest = json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(set(manifest['functions']), {'_f_259ea'})
            self.assertEqual((manifest['kept_bytes'], manifest['context_bytes']), (1412, 2564))
            expected = [(0x25e94,0x25f9a,2), (0x25e98,0x25f9c,2),
                        (0x25eb0,0x25f9e,2), (0x25ec4,0x25fa0,2),
                        (0x25edc,0x25fa2,2), (0x25f1c,0x25fa8,4),
                        (0x25f3c,0x25fac,4), (0x25f40,0x25fb0,4),
                        (0x25f44,0x25fb8,4), (0x25f48,0x25fb4,4)]
            self.assertEqual(manifest['literal_dependencies'], [
                dict(pc=pc, target=target, size=size, value=build.fn.ROM[target:target+size].hex())
                for pc, target, size in expected])
            self.assertFalse(manifest.get('tail_proofs'))
            self.assertFalse(manifest.get('switch_proofs'))
            with out.with_suffix('.context.o').open('rb') as f:
                e = ELFFile(f)
                roots = {s.name:s['st_value']+0x255dc for s in e.get_section_by_name('.symtab').iter_symbols()
                         if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF'}
            self.assertEqual(roots, {'_f_'+format(a,'x'):a for a in
                [0x255dc,0x25658,0x257f8,0x25806,0x25938,0x259bc,0x259ea,0x25f6e]})
            with out.open('rb') as f:
                e = ELFFile(f)
                self.assertEqual(e.get_section_by_name('.text').data(), kept)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in e.iter_sections()))
                self.assertEqual({s.name for s in e.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}, {'_f_259ea'})

    def test_separate_copy_rejects_four_byte_register_mismatch(self):
        import build
        from c_prefix import PrefixError
        source = (build.ROOT/'src/board_update_259ea.c').read_text()
        needle = 'old = k = r13->w84;'
        self.assertEqual(source.count(needle), 2)
        left, right = source.rsplit(needle, 1)
        source = left+'old = r13->w84;\n\t\tk = old;'+right
        with tempfile.TemporaryDirectory() as td:
            src = Path(td)/'negative.c'
            src.write_text(source)
            out = Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError, 'differs from ROM at 0x25e48'):
                self.compile(src, out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
