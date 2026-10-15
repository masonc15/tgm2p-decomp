"""Opt-in SHC/binutils regression checks: PERM_SETUP_TESTS=1 on nuada."""
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile
import unittest


@unittest.skipUnless(os.environ.get('PERM_SETUP_TESTS') == '1', 'requires nuada SHC/binutils')
class PermuterTargetTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import objdiff_case as oc
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        cls.oc = oc
        cls.temp = tempfile.TemporaryDirectory(prefix='perm-setup-test-')
        cls.addClassCleanup(cls.temp.cleanup)
        cls.root = Path(cls.temp.name)
        cases = cls.root / 'cases'
        cases.mkdir()
        # Grow an early function: every following compiled symbol/relocation
        # can move, but the target must still describe the unchanged ROM.
        source = (oc.ROOT / 'src/f_2f68.c').read_text()
        old = 'p->w35c |= s;'
        assert old in source
        source = source.replace(old, old + '\n\tp->w300 = s;', 1)
        # Deliberately wrong following bodies give both objects explicit
        # function boundaries. DAS reads a shared literal at 0x3f0c, so its
        # target context must extend beyond its own instructions.
        source += '\nvoid f_3e2e(void) {}\nextern volatile unsigned char g_6060000;\nlong f_3f92(void) {\n'
        source += '\n'.join(f'g_6060000 = {i};' for i in range(200))
        source += '\nreturn 0;\n}\n'
        (cases / 'shifted.c').write_text(source)
        cls.setup = cls.root / 'setup'
        subprocess.run([sys.executable, str(oc.ROOT / 'tools/perm_setup.py'),
                        'shifted', 'f_3c88', '--cases', str(cases),
                        '--dir', str(cls.setup), '--no-lineswap'],
                       check=True, capture_output=True, text=True)
        cls.target = cls.setup / 'target.o'
        cls.base = cls.setup / 'base.o'
        with cls.target.open('rb') as f:
            elf = ELFFile(f)
            syms = elf.get_section_by_name('.symtab')
            cls.symbols = {s.name: s['st_value'] for s in syms.iter_symbols()
                           if s['st_info']['type'] == 'STT_FUNC' and s['st_shndx'] != 'SHN_UNDEF'}
            cls.target_bytes = elf.get_section_by_name('.text').data()
            cls.relocs = {}
            for section in elf.iter_sections():
                if isinstance(section, RelocationSection):
                    for r in section.iter_relocations():
                        cls.relocs[0x2f68 + r['r_offset']] = syms.get_symbol(r['r_info_sym']).name
            cls.undefined = [s.name for s in syms.iter_symbols()
                             if s.name and s['st_shndx'] == 'SHN_UNDEF']

    def test_missing_function_is_not_scored_as_the_whole_object(self):
        from perm_objdump import function_bounds
        with self.assertRaisesRegex(ValueError, 'expected one defined function'):
            function_bounds(self.base, '_not_a_function')

    def test_symbols_use_rom_addresses_not_candidate_offsets(self):
        from elftools.elf.elffile import ELFFile
        with self.base.open('rb') as f:
            syms = ELFFile(f).get_section_by_name('.symtab')
            shifted = {s.name: s['st_value'] for s in syms.iter_symbols()}
        self.assertNotEqual(shifted['_f_2f7a'], 0x2f7a - 0x2f68)
        for name, offset in self.symbols.items():
            if re.fullmatch('_f_[0-9a-f]+', name):
                self.assertEqual(offset + 0x2f68, int(name[3:], 16), name)

    def test_literal_relocations_do_not_overwrite_instructions(self):
        self.assertEqual(self.relocs[0x3dfc], '_g_606475a')
        self.assertEqual(self.relocs[0x3e00], '_g_606475e')
        self.assertNotIn(0x3e04, self.relocs)

    def test_target_end_does_not_grow_with_candidate(self):
        candidate, _, _ = self.oc.load_base(self.base)
        self.assertGreater(len(candidate), 0x4044 - 0x2f68)
        self.assertEqual(len(self.target_bytes), 0x4044 - 0x2f68)

    def test_target_relinks_to_the_original_rom(self):
        symbols = self.oc.known_symbols()
        defs = []
        for name in self.undefined:
            value = self.oc.symbol_addr(name, symbols, {})
            self.assertIsNotNone(value, name)
            defs.append(f'{name} = {value:#x};')
        script = self.root / 'target.ld'
        script.write_text('\n'.join(defs) +
                          '\nSECTIONS { .text 0x60027e8 : { *(.text) } /DISCARD/ : { *(*) } }\n')
        linked = self.root / 'target.elf'
        binary = self.root / 'target.bin'
        subprocess.run(['sh-elf-ld', '-T', str(script), '-o', str(linked), str(self.target)], check=True)
        subprocess.run(['sh-elf-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(binary)], check=True)
        self.assertEqual(binary.read_bytes(), self.oc.fn.ROM[0x2f68:0x4044])

    def test_exact_rotation_scores_zero_despite_unmatched_tail(self):
        # The real C prefix matches through rotation; DAS after it does not.
        # A whole-unit scorer cannot give this requested function score zero.
        import toolchain
        sys.path.insert(0, str(toolchain.PERMUTER))
        from src.scorer import Scorer
        setup = self.root / 'exact-control'
        subprocess.run([sys.executable, str(self.oc.ROOT / 'tools/perm_setup.py'),
                        'f_2f68', 'f_3a02', '--cases', str(self.oc.ROOT / 'src'),
                        '--dir', str(setup), '--no-lineswap'],
                       check=True, capture_output=True, text=True)
        settings = (setup / 'settings.toml').read_text()
        command = json.loads(re.search(r'^objdump_command = (.+)$', settings, re.M)[1])
        scorer = Scorer(str(setup / 'target.o'), stack_differences=True,
                        algorithm='difflib', debug_mode=False,
                        ign_branch_targets=True, objdump_command=command)
        self.assertEqual(len(scorer.target_seq), 323)
        self.assertEqual(scorer.score(str(setup / 'base.o'))[0], 0)

    def test_scoring_disassembles_only_the_requested_function(self):
        settings = (self.setup / 'settings.toml').read_text()
        command = json.loads(re.search(r'^objdump_command = (.+)$', settings, re.M)[1])
        output = subprocess.check_output([*shlex.split(command), str(self.target)], text=True)
        labels = re.findall(r'^\s*[0-9a-f]+ <([^>]+)>:', output, re.M)
        self.assertEqual(labels, ['_f_3c88'])
        rows = re.findall(r'^\s*([0-9a-f]+):\s+[0-9a-f]{2} [0-9a-f]{2}\s', output, re.M)
        self.assertEqual(len(rows), 211)
        reloc_offsets = re.findall(r'^\s*([0-9a-f]+):\s+R_SH_', output, re.M)
        self.assertTrue(reloc_offsets)
        self.assertTrue(all(0xd20 <= int(a, 16) < 0xec6 for a in reloc_offsets))


if __name__ == '__main__':
    unittest.main()
