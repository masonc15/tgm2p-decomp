"""BF/BRA switch proof: independent fixtures and actual compiler regression."""
from dataclasses import replace
import hashlib
import json
import os
from pathlib import Path
import tempfile
import unittest
from c_prefix import PrefixError, validate_prefix, compile_prefix
from switch_proof import OriginalText, GuardedSwitches
from test_guarded_switch import put, branch

START, END, GUARD, MOVA, JUMP, SLOT, TABLE = 0x1000, 0x10c0, 0x1010, 0x101e, 0x1022, 0x1024, 0x1040


def fixture():
    rom = bytearray(0x1100)
    for pc in range(START, END + 4, 2):
        put(rom, pc, 0x0009)
    shape = [0xe10d, 0x3012, 0x8b01, branch(GUARD + 6, 0x1090), 0x0009,
             0x4000, 0x6103, 0xc708, 0x001d, 0x0023, 0x0009]
    for i, word in enumerate(shape):
        put(rom, GUARD + i * 2, word)
    put(rom, JUMP + 4, 0xc700)  # BRAF must have no fallthrough.
    for i in range(13):
        target = 0x105c + i * 4
        put(rom, TABLE + i * 2, target - JUMP - 4)
        put(rom, target, 0x000b)
    put(rom, 0x1090, 0x000b)
    put(rom, END, 0x000b)
    return rom


class StartupSwitchShapeTests(unittest.TestCase):
    def check(self, mutate=None, *, enabled=True, entries=None, original_change=None, proofs=None, recognize_only=False):
        rom = fixture()
        if mutate:
            mutate(rom)
        code = bytes(rom[START:END + 4])
        entries = entries or {START: '_f_1000', END: '_f_10c0'}
        original = OriginalText(START, code, dict(entries), (), 'fixture-object')
        if original_change:
            original = original_change(original)
        if recognize_only:
            def word(pc):
                if pc < START or pc + 2 > END:
                    raise PrefixError('fixture instruction boundary')
                return int.from_bytes(rom[pc:pc+2], 'big')
            return GuardedSwitches(original,code,rom,START,END,START,entries,word,PrefixError).candidate(MOVA)
        output = [] if proofs is None else proofs
        validate_prefix(code, rom, START, END, entries, context_start=START,
                        guarded_switches=enabled, switch_original=original, switch_proofs=output)
        return output

    def test_positive_guard_polarity_default_and_both_delay_owners(self):
        proof, = self.check()
        self.assertEqual(proof['family'], 'sh2-guarded13-r0-r1-bf-bra-v1')
        self.assertEqual(proof['default'], 0x1090)
        self.assertEqual(proof['targets'], list(range(0x105c, 0x108d, 4)))
        self.assertEqual(proof['spine_predecessors'][str(GUARD + 10)], [GUARD + 4])
        self.assertEqual(proof['default_delay_owner'], GUARD + 6)
        self.assertTrue(proof['guard_dominates_dispatch'])

    def test_default_disabled(self):
        with self.assertRaisesRegex(PrefixError, 'MOVA'):
            self.check(enabled=False)

    def test_fixed_instruction_bit_changes_fail_closed(self):
        baseline = fixture()
        for pc in [GUARD, GUARD + 2, GUARD + 4, GUARD + 8, GUARD + 10, GUARD + 12, MOVA + 2, JUMP, SLOT]:
            word = int.from_bytes(baseline[pc:pc + 2], 'big')
            for bit in range(16):
                with self.subTest(pc=hex(pc), bit=bit), self.assertRaises(PrefixError):
                    self.check(lambda r, pc=pc, v=word ^ (1 << bit): put(r, pc, v), recognize_only=True)

    def test_default_transfer_must_be_bra(self):
        for word in [0x0009, 0xb000, 0x8901, 0x8b01, 0x0023, 0x412b]:
            with self.subTest(word=hex(word)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, GUARD + 6, word))

    def test_every_interior_root_rejects(self):
        for pc in range(GUARD + 2, SLOT + 2, 2):
            with self.subTest(pc=hex(pc)), self.assertRaises(PrefixError):
                self.check(entries={START:'_f_1000', pc:f'_f_{pc:x}', END:'_f_10c0'})

    def test_late_edges_cannot_bypass_guard_or_enter_delays(self):
        for target in range(GUARD + 2, SLOT + 2, 2):
            for op in [0xa000, 0xb000, 0x8900]:
                with self.subTest(target=hex(target), op=hex(op)), self.assertRaises(PrefixError):
                    self.check(lambda r: put(r, 0x105c, branch(0x105c, target, op)))

    def test_early_edges_cannot_bypass_guard(self):
        for target in range(GUARD + 2, SLOT + 2, 2):
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, START, branch(START, target, 0x8900)))

    def test_default_cannot_enter_spine_delay_data_or_leave_function(self):
        for target in [START-2, *range(GUARD, SLOT+2, 2), TABLE, TABLE+24, 0x105e, END, END+2]:
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(lambda r: put(r, GUARD+6, branch(GUARD+6, target)))

    def test_backward_default_sign_extension(self):
        def change(r):
            put(r, START, branch(START, GUARD))
            put(r, 0x1004, 0x000b)
            put(r, GUARD+6, branch(GUARD+6, 0x1004))
        proof, = self.check(change)
        self.assertEqual(proof['default'], 0x1004)

    def test_table_provenance_relocations_and_data_aliases(self):
        for address in range(TABLE-3, TABLE+26):
            with self.subTest(address=hex(address)), self.assertRaises(PrefixError):
                self.check(original_change=lambda o:replace(o,relocations=((address,4),)))
        with self.assertRaises(PrefixError):
            self.check(original_change=lambda o:replace(o,data=o.data[:TABLE-START]+b'xx'+o.data[TABLE-START+2:]))
        with self.assertRaises(PrefixError):
            self.check(lambda r:put(r,START,0xd300|((TABLE-((START+4)&~3))//4)))

    def test_case_targets_reject_nonlocal_data_and_delay(self):
        for target in [START-2, GUARD+8, SLOT, TABLE, TABLE+24, 0x105d, 0x1062, END, END+2]:
            with self.subTest(target=hex(target)), self.assertRaises(PrefixError):
                self.check(lambda r:put(r,TABLE,(target-JUMP-4)&65535))

    def test_failure_never_publishes_partial_proof(self):
        proofs=[{'old':'sentinel'}]
        with self.assertRaises(PrefixError):
            self.check(lambda r:put(r,0x108c,0xc700),proofs=proofs)
        self.assertEqual(proofs,[{'old':'sentinel'}])


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'real compiler tests opt in')
class NativeStartupSwitchTests(unittest.TestCase):
    def compile(self, src, out, enabled=True):
        import build
        return compile_prefix(src,out,0x1914,0x1c8c,rom=build.fn.ROM,
            shcc=build.SHCC,version=build.SHC_VERSION,flags=build.c_flags(src),
            symbols=build.read_symbols(),assembler=build.AS,context_start=0x1278,
            guarded_switches=enabled)

    def test_exact_function_and_original_switch_provenance(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o'
            self.compile(build.ROOT/'src/startup_dispatch_1914.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x1914:0x1c8c])
            manifest=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['functions'],['_f_1914'])
            self.assertEqual(manifest['kept_bytes'],888)
            proof,=manifest['switch_proofs']
            self.assertEqual((proof['guard'],proof['mova'],proof['jump'],proof['table']),(0x19b2,0x19c0,0x19c4,0x19ec))
            self.assertEqual(proof['default'],0x1bb2)
            self.assertEqual((proof['owner'],proof['owner_end']),(0x1914,0x1c8c))
            self.assertEqual(manifest['original_object_sha256'],hashlib.sha256(out.with_suffix('.context.o').read_bytes()).hexdigest())
            self.assertEqual([p['pc'] for p in manifest['literal_dependencies']],
                [0x1c0c,0x1c10,0x1c26,0x1c36,0x1c3e,0x1c40,0x1c4c,0x1c52,0x1c54,0x1c5e,0x1c66,0x1c68,0x1c70])
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
                self.assertEqual({s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                    if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'},{'_f_1914'})
            with self.assertRaisesRegex(PrefixError,'MOVA'):
                self.compile(build.ROOT/'src/startup_dispatch_1914.c',out,False)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())

    def test_logical_not_changes_bytes_and_cannot_publish(self):
        import build
        source=(build.ROOT/'src/startup_dispatch_1914.c').read_text()
        needle='f->b39a = f->b39a ? 0 : 1;'
        self.assertEqual(source.count(needle),1)
        with tempfile.TemporaryDirectory() as td:
            src=Path(td)/'negative.c';src.write_text(source.replace(needle,'f->b39a = !f->b39a;'))
            out=Path(td)/'kept.o'
            with self.assertRaisesRegex(PrefixError,'contextual function boundaries'):
                self.compile(src,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
