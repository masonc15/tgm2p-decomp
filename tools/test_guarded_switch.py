"""Independent SH2 fixtures; structural negatives change code AND its oracle."""
from dataclasses import replace
from pathlib import Path
import os
import tempfile
import unittest
from unittest.mock import patch

from c_prefix import PrefixError, validate_prefix, compile_prefix
from switch_proof import OriginalText

START, END, GUARD, MOVA, JUMP, SLOT, TABLE = 0x1000, 0x10c0, 0x1010, 0x101a, 0x101e, 0x1020, 0x1040


def put(data, address, value):
    data[address:address + 2] = value.to_bytes(2, 'big')


def branch(pc, target, opcode=0xa000):
    return opcode | (((target - pc - 4) // 2) & (255 if opcode == 0x8900 else 4095))


def fixture():
    rom = bytearray(0x1100)
    for pc in range(START, END + 4, 2):
        put(rom, pc, 0x0009)
    shape = [0xe10d, 0x3012, branch(GUARD + 4, 0x1090, 0x8900),
             0x4000, 0x6103, 0xc709, 0x001d, 0x0023, 0x0009]
    for i, word in enumerate(shape):
        put(rom, GUARD + i * 2, word)
    # Illegal if BRAF is accidentally given a fallthrough edge.
    put(rom, JUMP + 4, 0xc700)
    targets = [0x105c, 0x1060, 0x1064, 0x1068, 0x1068, 0x1068,
               0x106c, 0x106c, 0x106c, 0x106c, 0x1070, 0x1074, 0x1078]
    for i, target in enumerate(targets):
        put(rom, TABLE + i * 2, target - JUMP - 4)
        put(rom, target, 0x000b)
    put(rom, 0x1090, 0x000b)
    put(rom, END, 0x000b)
    return rom


class GuardedSwitchTests(unittest.TestCase):
    def check(self, mutate=None, *, entries=None, original_change=None, enabled=True,
              proofs=None, compiled_change=None, end=END, **kwargs):
        rom = fixture()
        if mutate:
            mutate(rom)
        code = bytes(rom[START:END + 4])
        entries = entries or {START: '_f_1000', END: '_f_10c0'}
        original = OriginalText(START, code, dict(entries), (), 'fixture-object')
        if original_change:
            original = original_change(original)
        if compiled_change:
            code = compiled_change(code)
        output = [] if proofs is None else proofs
        validate_prefix(code, rom, START, end, entries, context_start=START,
                        guarded_switches=enabled, switch_original=original,
                        switch_proofs=output, **kwargs)
        return output

    def test_positive_thirteen_entries_duplicate_targets_and_owned_delay(self):
        proof, = self.check()
        self.assertEqual(proof['targets'], [0x105c, 0x1060, 0x1064, 0x1068, 0x1068,
                         0x1068, 0x106c, 0x106c, 0x106c, 0x106c, 0x1070, 0x1074, 0x1078])
        self.assertEqual(proof['braf_base'], 0x1022)
        self.assertEqual(proof['table_end'], 0x105a)
        self.assertTrue(proof['guard_dominates_dispatch'])
        self.assertEqual(proof['delay_owner'], JUMP)

    def test_default_still_rejects(self):
        with self.assertRaisesRegex(PrefixError, 'MOVA'):
            self.check(enabled=False)

    def test_all_fixed_shape_bit_changes_reject(self):
        baseline = fixture()
        for pc in [GUARD, GUARD + 2, GUARD + 6, GUARD + 8, MOVA + 2, JUMP, SLOT]:
            word = int.from_bytes(baseline[pc:pc + 2], 'big')
            for bit in range(16):
                with self.subTest(pc=hex(pc), bit=bit), self.assertRaises(PrefixError):
                    self.check(lambda r, pc=pc, v=word ^ (1 << bit): put(r, pc, v))

    def test_guard_branches_wrong_or_delayed(self):
        for op in [0x8b3c, 0x8d3c, 0x8f3c, 0x0009, 0x8900]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, GUARD + 4, op))

    def test_invalid_bound_counts_and_truncated_retention(self):
        for bound in [0, 12, 14, 255]:
            with self.subTest(bound=bound), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, GUARD, 0xe100 | bound))
        for finish in [TABLE, TABLE + 24, TABLE + 25, TABLE + 26]:
            with self.subTest(end=finish), self.assertRaises(PrefixError):
                self.check(end=finish, entries={START: '_f_1000', finish: f'_f_{finish:x}', END: '_f_10c0'})

    def test_every_interior_function_root_rejects(self):
        for pc in range(GUARD + 2, SLOT + 2, 2):
            entries = {START: '_f_1000', pc: f'_f_{pc:x}', END: '_f_10c0'}
            with self.subTest(pc=hex(pc)), self.assertRaises(PrefixError):
                self.check(entries=entries)

    def test_late_case_edges_into_every_spine_point_and_slot(self):
        for target in range(GUARD + 2, SLOT + 2, 2):
            for op in [0xa000, 0xb000, 0x8900]:
                with self.subTest(target=hex(target), op=hex(op)), self.assertRaises(PrefixError):
                    self.check(lambda r: put(r, 0x105c, branch(0x105c, target, op)))

    def test_other_root_late_edges_both_worklist_orders(self):
        for target in range(GUARD + 2, SLOT + 2, 2):
            for reverse in [False, True]:
                pairs = [(START, '_f_1000'), (0x10b0, '_f_10b0'), (END, '_f_10c0')]
                if reverse:
                    pairs.reverse()
                with self.subTest(target=hex(target), reverse=reverse), self.assertRaises(PrefixError):
                    self.check(lambda r: put(r, 0x10b0, branch(0x10b0, target)), entries=dict(pairs))

    def test_early_bypass_and_guard_in_delay_slot(self):
        for target in range(GUARD + 2, SLOT + 2, 2):
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, START, branch(START, target)))
        with self.assertRaises(PrefixError):
            self.check(lambda r: put(r, GUARD - 2, branch(GUARD - 2, GUARD + 2)))

    def test_relocation_overlaps_from_before_inside_and_last_byte(self):
        for address in range(TABLE - 3, TABLE + 26):
            with self.subTest(address=hex(address)), self.assertRaisesRegex(PrefixError, 'relocation'):
                self.check(original_change=lambda o: replace(o, relocations=((address, 4),)))

    def test_nonoverlapping_relocations_allowed(self):
        self.check(original_change=lambda o: replace(o, relocations=((TABLE - 4, 4), (TABLE + 26, 4))))

    def test_original_table_and_original_roots_required(self):
        for change in [lambda o: None, lambda o: replace(o, entries={}),
                       lambda o: replace(o, origin=START + 2),
                       lambda o: replace(o, data=o.data[:-1]),
                       lambda o: replace(o, data=o.data[:TABLE - START] + b'xx' + o.data[TABLE - START + 2:])]:
            with self.subTest(change=change), self.assertRaises(PrefixError):
                self.check(original_change=change)

    def test_following_actual_root_and_nominal_owner_required(self):
        for entries in [{START: '_f_1000'}, {START: '_f_1002', END: '_f_10c0'},
                        {START: 'anonymous', END: '_f_10c0'}]:
            with self.subTest(entries=entries), self.assertRaises(PrefixError):
                self.check(entries=entries)

    def test_target_boundaries_data_spine_and_slot_reject(self):
        for target in [START - 2, GUARD, SLOT, TABLE - 2, TABLE, TABLE + 24,
                       0x105d, 0x1062, END, END + 2, 0x2000]:
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, TABLE, (target - JUMP - 4) & 65535))

    def test_default_destinations_reject(self):
        for target in [START - 2, GUARD, GUARD + 2, SLOT, TABLE, TABLE + 24, 0x105e, END, END + 2]:
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, GUARD + 4, branch(GUARD + 4, target, 0x8900)))

    def test_valid_alternate_default_and_table_target_allowed(self):
        self.check(lambda r: put(r, GUARD + 4, branch(GUARD + 4, 0x105c, 0x8900)))
        self.check(lambda r: put(r, TABLE, 0x1078 - JUMP - 4))

    def test_mova_bad_extents(self):
        for op in [0xc700, 0xc701, 0xc727, 0xc7ff]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, MOVA, op))

    def test_table_literal_alias_rejected(self):
        # Normal PC-relative long load at the entry reads the first table word.
        op = 0xd300 | ((TABLE - ((START + 4) & ~3)) // 4)
        with self.assertRaisesRegex(PrefixError, 'aliases'):
            self.check(lambda r: put(r, START, op))

    def test_case_target_in_normal_literal_pool_rejected(self):
        op = 0xd300 | ((0x105c - ((START + 4) & ~3)) // 4)
        with self.assertRaises(PrefixError):
            self.check(lambda r: put(r, START, op))

    def test_new_unsupported_transfer_and_no_partial_publication(self):
        for word in [0xc700, 0x0023, 0x0003, 0x412b, 0xc300]:
            proofs = [{'old': 'sentinel'}]
            with self.subTest(word=hex(word)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, 0x1078, word), proofs=proofs)
            self.assertEqual(proofs, [{'old': 'sentinel'}])

    def test_raw_mismatch_remains_independent(self):
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.check(compiled_change=lambda b: b'xx' + b[2:])

    def test_mixed_tail_mode_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'mixed'):
            self.check(tail_provenance={}, canonical_functions={})


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'requires nuada SHC/binutils')
class SwitchCompileTests(unittest.TestCase):
    def run_compile(self, td, source=None, **kwargs):
        import build
        source = source or build.ROOT / 'src/rank_switch_14b10.c'
        out = Path(td) / 'switch.o'
        compile_prefix(source, out, 0x14b10, 0x14bce, rom=build.fn.ROM,
                       shcc=build.SHCC, version=build.SHC_VERSION,
                       flags=build.c_flags(source), symbols=build.read_symbols(),
                       assembler=build.AS, context_start=0x149b4, **kwargs)
        return out

    def test_real_candidate_opt_in_and_default_stale_output_rejection(self):
        import json
        with tempfile.TemporaryDirectory() as td:
            out = self.run_compile(td, guarded_switches=True)
            manifest = json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['kept_bytes'], 190)
            self.assertEqual(manifest['functions'], ['_f_14b10'])
            self.assertEqual(len(manifest['switch_proofs']), 1)
            self.assertEqual(manifest['switch_proofs'][0]['table'], 0x14b58)
            with self.assertRaisesRegex(PrefixError, 'MOVA'):
                self.run_compile(td)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())

    def test_original_elf_loader_rejects_unknown_relocation_and_alias(self):
        import io
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        with tempfile.TemporaryDirectory() as td:
            out = self.run_compile(td, guarded_switches=True)
            obj = out.with_suffix('.context.o')
            raw = obj.read_bytes()
            elf = ELFFile(io.BytesIO(raw))
            text_index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == '.text')
            relocs = next(s for s in elf.iter_sections()
                          if isinstance(s, RelocationSection) and s['sh_info'] == text_index)
            self.assertGreater(relocs.num_relocations(), 0)
            modified = bytearray(raw)
            # ELF32 r_info: replace the relocation type with an unknown width.
            modified[relocs['sh_offset'] + 7] = 255
            obj.write_bytes(modified)
            with self.assertRaisesRegex(PrefixError, 'unsupported text relocation'):
                OriginalText.read(obj, 0x149b4, PrefixError)
            symbols = elf.get_section_by_name('.symtab')
            funcs = [(i, s) for i, s in enumerate(symbols.iter_symbols())
                     if s['st_shndx'] == text_index and s['st_info']['type'] == 'STT_FUNC']
            modified = bytearray(raw)
            i, symbol = funcs[1]
            off = symbols['sh_offset'] + i * symbols['sh_entsize'] + 4
            modified[off:off + 4] = funcs[0][1]['st_value'].to_bytes(4, 'big')
            obj.write_bytes(modified)
            with self.assertRaisesRegex(PrefixError, 'ambiguous function aliases'):
                OriginalText.read(obj, 0x149b4, PrefixError)

    def test_source_change_rejected_after_compiling_snapshot(self):
        import build
        import subprocess
        real_run = subprocess.run
        with tempfile.TemporaryDirectory() as td:
            source = Path(td) / 'case.c'
            source.write_bytes((build.ROOT / 'src/rank_switch_14b10.c').read_bytes())
            def run(argv, **kwargs):
                result = real_run(argv, **kwargs)
                if str(argv[0]) == str(build.SHCC):
                    self.assertNotEqual(Path(argv[3]), source)
                    source.write_text('changed while compiler ran')
                return result
            with patch('c_prefix.subprocess.run', side_effect=run):
                with self.assertRaisesRegex(PrefixError, 'source changed'):
                    self.run_compile(td, source=source, guarded_switches=True)
            self.assertFalse((Path(td) / 'switch.o').exists())
            self.assertFalse((Path(td) / 'switch.prefix.json').exists())

    def test_build_header_requires_exact_marker_and_separate_mode(self):
        import build
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / 'marker.c'
            for header in ['guarded-switches: 0', 'guarded-switches: 2',
                           'guarded-switches: 1 guarded-switches: 1',
                           'guarded-switches: 1 literal-tail-calls: 1',
                           'guarded-switches: 1 leaf-argument-tails: 1']:
                p.write_text('/* ' + header + ' */\n')
                with self.subTest(header=header), self.assertRaises(ValueError):
                    build.c_guarded_switches(p)
            p.write_text('/* guarded-switches: 1 */\n')
            self.assertTrue(build.c_guarded_switches(p))
            p.write_text('/* ordinary */\n')
            self.assertFalse(build.c_guarded_switches(p))

    def test_packaging_failure_cannot_publish_old_manifest(self):
        import subprocess
        real_run = subprocess.run
        def run(argv, **kwargs):
            if '-no-pad-sections' in argv:
                raise subprocess.CalledProcessError(1, argv)
            return real_run(argv, **kwargs)
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / 'switch.o'
            out.write_bytes(b'old')
            out.with_suffix('.prefix.json').write_text('{}')
            with patch('c_prefix.subprocess.run', side_effect=run):
                with self.assertRaises(subprocess.CalledProcessError):
                    self.run_compile(td, guarded_switches=True)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())


if __name__ == '__main__':
    unittest.main()
