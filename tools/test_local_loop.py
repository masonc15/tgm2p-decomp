"""Literal-loop proof fixtures independent of the real wave routine."""
from dataclasses import replace
from pathlib import Path
import os
import tempfile
import unittest
from c_prefix import PrefixError, validate_prefix, compile_prefix
from switch_proof import OriginalText
from local_loop_proof import LocalLoopOriginal

START, END, LOAD, JUMP, SLOT, POOL, TARGET = 0x1000, 0x1080, 0x1020, 0x1022, 0x1024, 0x10a0, 0x1004
DELTA = 0x5fff880


def put(data, address, value, width=2):
    data[address:address + width] = (value & ((1 << (8 * width)) - 1)).to_bytes(width, 'big')


def bra(pc, target):
    return 0xa000 | (((target - pc - 4) // 2) & 4095)


class LocalLoopTests(unittest.TestCase):
    def check(self, mutate=None, *, target=TARGET, entries=None, relocation_change=None,
              original_change=None, enabled=True, proofs=None, register=1, **kwargs):
        rom = bytearray(0x1100)
        for pc in range(START, 0x10c0, 2):
            put(rom, pc, 0x0009)
        put(rom, LOAD - 2, 0x8907)  # ordinary exit to 1030, or fall through
        put(rom, LOAD, 0xd01f | register << 8)
        put(rom, JUMP, 0x402b | register << 8)
        put(rom, SLOT + 2, 0xc700)  # unreachable unless JMP gains fallthrough
        put(rom, 0x1030, 0x000b)
        put(rom, END, 0x000b)
        put(rom, POOL, target + DELTA, 4)
        if mutate:
            mutate(rom)
        code = bytes(rom[START:0x10c0])
        original_data = bytearray(code)
        put(original_data, POOL - START, target - START, 4)
        entries = entries or {START: '_f_1000', END: '_f_1080'}
        rel = dict(address=POOL, width=4, type=1, symbol='.text', symbol_type='STT_SECTION',
                   binding='STB_LOCAL', is_text=True, symbol_value=0, explicit_addend=0,
                   embedded_addend=(target - START) & 0xffffffff)
        relocs = [rel]
        if relocation_change:
            relocs = relocation_change(relocs)
        text = OriginalText(START, bytes(original_data), dict(entries),
                            tuple((r['address'], r['width']) for r in relocs), 'fixture-object')
        original = LocalLoopOriginal(text, tuple(relocs))
        if original_change:
            original = original_change(original)
        output = [] if proofs is None else proofs
        validate_prefix(code, rom, START, END, entries, context_start=START,
                        local_loop_jumps=enabled, loop_original=original,
                        loop_proofs=output, **kwargs)
        return output

    def test_original_section_backedge_and_external_pool(self):
        proof, = self.check()
        self.assertEqual(proof['target'], TARGET)
        self.assertEqual(proof['pool'], POOL)
        self.assertEqual(proof['original_relocation']['embedded_addend'], 4)
        self.assertTrue(proof['target_reachable_without_indirect_edges'])
        self.assertTrue(proof['no_fallthrough'])
        self.assertEqual(proof['jump_predecessors'], [LOAD])

    def test_default_and_mixed_modes_reject(self):
        with self.assertRaisesRegex(PrefixError, 'indirect'):
            self.check(enabled=False)
        for kwargs in [dict(guarded_switches=True), dict(tail_provenance={}), dict(leaf_argument_tails=True)]:
            with self.subTest(kwargs=kwargs), self.assertRaisesRegex(PrefixError, 'mixed'):
                    self.check(**kwargs)

    def test_r2_requires_the_same_complete_backedge_proof(self):
        proof, = self.check(register=2)
        self.assertEqual(proof['register'], 2)
        self.assertEqual(proof['family'], 'sh2-local-loop-r2-nop-v1')
        self.assertEqual(proof['target'], TARGET)
        self.assertTrue(proof['complete_cfg_checked'])
        self.assertTrue(proof['target_reachable_without_indirect_edges'])
        self.assertTrue(proof['load_dominates_jump'])
        for target in [START, TARGET + 1, LOAD, END, POOL]:
            with self.subTest(target=target), self.assertRaises(PrefixError):
                self.check(register=2, target=target)
        for change in [dict(type=2), dict(symbol='_f_1004'), dict(binding='STB_GLOBAL'),
                       dict(is_text=False), dict(embedded_addend=8), dict(explicit_addend=4)]:
            with self.subTest(change=change), self.assertRaises(PrefixError):
                self.check(register=2, relocation_change=lambda rs: [dict(rs[0], **change)])
        for pc, op in [(LOAD, 0xd11f), (JUMP, 0x412b), (JUMP, 0x420b),
                       (SLOT, 0x7201), (START, bra(START, JUMP)),
                       (START, bra(START, SLOT)), (START, bra(START, 0x1010))]:
            with self.subTest(pc=pc, op=op), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, pc, op), register=2)
        with self.assertRaises(PrefixError):
            self.check(register=2, entries={START: '_f_1000'})

    def test_other_matched_register_pairs_remain_unsupported(self):
        for register in [0, 3, 4, 14, 15]:
            with self.subTest(register=register), self.assertRaises(PrefixError):
                self.check(register=register)

    def test_wrong_register_width_jump_and_slot_reject(self):
        for pc, ops in [(LOAD, [0x911f, 0xd01f, 0xd21f, 0x6103]),
                        (JUMP, [0x402b, 0x422b, 0x432b, 0x0003, 0x0023, 0x410b]),
                        (SLOT, [0x7101, 0x000b, 0x4f26, 0x9101, 0xc700])]:
            for op in ops:
                with self.subTest(pc=hex(pc), op=hex(op)), self.assertRaises(PrefixError):
                    self.check(lambda r: put(r, pc, op))

    def test_changed_literal_displacement_rejects(self):
        for op in [0xd100, 0xd11e, 0xd120, 0xd1ff]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, LOAD, op))

    def test_early_and_late_alternate_jump_or_slot_entry(self):
        for source in [START, 0x1030]:
            for target in [JUMP, SLOT]:
                with self.subTest(source=hex(source), target=hex(target)), self.assertRaises(PrefixError):
                    self.check(lambda r: put(r, source, bra(source, target)))

    def test_extra_roots_and_foreign_entry_both_orders(self):
        for root in [LOAD, JUMP, SLOT, 0x1060]:
            for reverse in [False, True]:
                items = [(START, '_f_1000'), (root, f'_f_{root:x}'), (END, '_f_1080')]
                if reverse:
                    items.reverse()
                mutate = (lambda r: put(r, 0x1060, bra(0x1060, LOAD))) if root == 0x1060 else None
                with self.subTest(root=hex(root), reverse=reverse), self.assertRaises(PrefixError):
                    self.check(mutate, entries=dict(items))

    def test_defining_load_cannot_be_a_delay_slot(self):
        with self.assertRaises(PrefixError):
            self.check(lambda r: put(r, LOAD - 2, bra(LOAD - 2, JUMP)))

    def test_target_requires_ordinary_reachability(self):
        # The indirect edge would be the only path into 1004; those bytes are
        # otherwise skipped. Traversing the pointer must not invent code.
        with self.assertRaisesRegex(PrefixError, 'ordinary reachability'):
            self.check(lambda r: put(r, START, bra(START, 0x1010)))

    def test_target_cannot_be_executed_delay_slot(self):
        with self.assertRaises(PrefixError):
            self.check(lambda r: put(r, TARGET, bra(TARGET, 0x1010)), target=TARGET + 2)

    def test_forward_odd_root_cross_function_targets_reject(self):
        for target in [START - 2, START, TARGET + 1, LOAD, JUMP, SLOT, END, POOL]:
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(target=target)

    def test_valid_alternate_backward_target(self):
        proof, = self.check(target=TARGET + 4)
        self.assertEqual(proof['target'], TARGET + 4)

    def test_metadata_and_relocation_identity_required(self):
        changes = [dict(type=2), dict(symbol='_f_1004'), dict(symbol_type='STT_FUNC'),
                   dict(binding='STB_GLOBAL'), dict(is_text=False), dict(symbol_value=4),
                   dict(explicit_addend=4), dict(embedded_addend=8), dict(address=POOL + 1), dict(width=2)]
        for change in changes:
            with self.subTest(change=change), self.assertRaises(PrefixError):
                self.check(relocation_change=lambda rs: [dict(rs[0], **change)])
        with self.assertRaises(PrefixError):
            self.check(relocation_change=lambda rs: [])

    def test_duplicate_and_partial_overlap_relocations_reject(self):
        for address in range(POOL - 3, POOL + 4):
            with self.subTest(address=hex(address)), self.assertRaises(PrefixError):
                self.check(relocation_change=lambda rs: rs + [dict(rs[0], address=address)])

    def test_linked_pointer_must_reconstruct_original_offset(self):
        with self.assertRaisesRegex(PrefixError, 'reconstruct'):
            self.check(lambda r: put(r, POOL, TARGET + DELTA + 2, 4))

    def test_original_text_identity_and_complete_owner_required(self):
        for change in [lambda o: None,
                       lambda o: replace(o, text=replace(o.text, origin=START + 2)),
                       lambda o: replace(o, text=replace(o.text, entries={})),
                       lambda o: replace(o, text=replace(o.text, relocations=())),
                       lambda o: replace(o, text=replace(o.text, data=o.text.data[:-1]))]:
            with self.subTest(change=change), self.assertRaises(PrefixError):
                self.check(original_change=change)
        with self.assertRaises(PrefixError):
            self.check(entries={START: '_f_1000'})

    def test_literal_data_cannot_become_loop_code(self):
        # An ordinary long load identifies the otherwise valid target as data.
        with self.assertRaises(PrefixError):
            self.check(lambda r: put(r, START, 0xd300))

    def test_late_failure_leaves_proof_list_unchanged(self):
        output = [{'sentinel': True}]
        with self.assertRaises(PrefixError):
            self.check(lambda r: put(r, 0x1030, bra(0x1030, JUMP)), proofs=output)
        self.assertEqual(output, [{'sentinel': True}])


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'requires nuada SHC/binutils')
class LocalLoopCompileTests(unittest.TestCase):
    def test_complete_wave_range_with_r1_and_r2_backedges(self):
        import build,json
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'wave.o'
            compile_prefix(build.ROOT/'src/wave_loop_27e90.c', out, 0x27e90, 0x28334,
                rom=build.fn.ROM, shcc=build.SHCC, version=build.SHC_VERSION,
                flags=['-macsave=1','-optimize=1','-speed'], symbols=build.read_symbols(),
                assembler=build.AS, context_start=0x27e70, local_loop_jumps=True)
            m=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(m['kept_bytes'],1188)
            self.assertEqual(set(m['functions']),{'_f_27e90','_f_2808a'})
            self.assertFalse(m['literal_dependencies'])
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x27e90:0x28334])
            self.assertEqual([(p['register'],p['jump'],p['target'],p['owner_end']) for p in m['loop_proofs']],
                             [(1,0x2803c,0x27f0e,0x2808a),(2,0x2828c,0x2810c,0x28334)])
            self.assertTrue(all(p['complete_cfg_checked'] and p['target_reachable_without_indirect_edges'] for p in m['loop_proofs']))
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'},set(m['functions']))
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))

    def test_unhinted_wave_still_fails_byte_matching(self):
        import build
        source=(build.ROOT/'src/wave_loop_27e90.c').read_text()
        self.assertEqual(source.count('register long ang;'),1)
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'unhinted.c';src.write_text(source.replace('register long ang;','long ang;'))
            out=Path(td)/'wave.o'
            with self.assertRaisesRegex(PrefixError,'differs from ROM'):
                compile_prefix(src,out,0x27e90,0x28334,rom=build.fn.ROM,shcc=build.SHCC,
                    version=build.SHC_VERSION,flags=['-macsave=1','-optimize=1','-speed'],
                    symbols=build.read_symbols(),assembler=build.AS,context_start=0x27e70,local_loop_jumps=True)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())

    def test_real_wave_and_default_rejection(self):
        import build,json
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / 'wave.o'
            args = dict(rom=build.fn.ROM, shcc=build.SHCC, version=build.SHC_VERSION,
                        flags=['-macsave=1', '-optimize=1', '-speed'], symbols=build.read_symbols(),
                        assembler=build.AS, context_start=0x27e70)
            source = build.ROOT / 'src/wave_loop_27e90.c'
            compile_prefix(source, out, 0x27e90, 0x2808a, local_loop_jumps=True, **args)
            m = json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(m['kept_bytes'], 506)
            self.assertEqual(m['functions'], ['_f_27e90'])
            proof, = m['loop_proofs']
            self.assertEqual((proof['load'], proof['jump'], proof['target'], proof['pool']),
                             (0x2803a, 0x2803c, 0x27f0e, 0x28128))
            self.assertEqual(proof['original_relocation']['embedded_addend'], 0x9e)
            with out.open('rb') as f:
                self.assertEqual(ELFFile(f).get_section_by_name('.text').data(), build.fn.ROM[0x27e90:0x2808a])
            with self.assertRaisesRegex(PrefixError, 'indirect'):
                compile_prefix(source, out, 0x27e90, 0x2808a, **args)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())

    def test_exact_separate_build_header(self):
        import build
        with tempfile.TemporaryDirectory() as td:
            source = Path(td) / 'case.c'
            for text in ['local-loop-jumps: 0', 'local-loop-jumps: 1 local-loop-jumps: 1',
                         'local-loop-jumps: 1 guarded-switches: 1',
                         'local-loop-jumps: 1 literal-tail-calls: 1']:
                source.write_text('/* ' + text + ' */\n')
                with self.subTest(text=text), self.assertRaises(ValueError):
                    build.c_local_loop_jumps(source)
            source.write_text('/* local-loop-jumps: 1 */\n')
            self.assertTrue(build.c_local_loop_jumps(source))
            source.write_text('/* defaults */\n')
            self.assertFalse(build.c_local_loop_jumps(source))


if __name__ == '__main__':
    unittest.main()
