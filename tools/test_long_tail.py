"""Independent complete-oracle structural fixtures for ordered tail forms."""
import unittest
from c_prefix import PrefixError, validate_prefix

START, END, POOL, TARGET, DELTA = 0x1000, 0x1080, 0x1070, 0x2000, 0x5fff880
FORMS = {
    'A': [0xd31b, 0x314c, 0x2100, 0xe412, 0x4f16, 0x4f26,
          0x69f6, 0x6af6, 0x6bf6, 0x6cf6, 0x6df6, 0x432b, 0x6ef6],
    'B': [0xd11b, 0xe501, 0x02ed, 0x64e3, 0x223b, 0x0e25,
          0x4f16, 0x4f26, 0x412b, 0x6ef6],
    'C': [0xd31b, 0xe113, 0x2010, 0xd11b, 0x2120, 0x4f26,
          0x6cf6, 0x6df6, 0x432b, 0x6ef6],
    'D': [0xd31b, 0x4f16, 0x4f26, 0x68f6, 0x69f6, 0x6af6, 0x6bf6,
          0x6cf6, 0x6df6, 0x432b, 0x6ef6],
    'E': [0xd31b, 0x7f58, 0x4f26, 0x68f6, 0x69f6, 0x6af6, 0x6bf6,
          0x6cf6, 0x6df6, 0x432b, 0x6ef6],
    'F': [0xd31b, 0x7f10, 0x4f16, 0x4f26, 0x68f6, 0x69f6, 0x6af6, 0x6bf6,
          0x6cf6, 0x6df6, 0x432b, 0x6ef6],
    'G': [0xd11b, 0x4f26, 0x68f6, 0x69f6, 0x6af6, 0x6bf6,
          0x6cf6, 0x6df6, 0x412b, 0x6ef6],
}
# Signed argument immediates and form C's store-address literal displacement.
WILDCARDS = {'A': (3,), 'B': (1,), 'C': (1, 3), 'D': (), 'E': (1,), 'F': (1,), 'G': ()}
IMMEDIATE = {'A': 3, 'B': 1, 'C': 1}
STACK = {'A': 32, 'B': 12, 'C': 16, 'D': 36, 'E': 120, 'F': 52, 'G': 32}


class LongTailTests(unittest.TestCase):
    def validate(self, ops, *, entries=None, extra=None, enabled=True, provenance=None,
                 proofs_out=None, boot=None, value=TARGET+DELTA, symbol='_f_2000'):
        rom = bytearray(0x3000)
        for i,w in enumerate(ops):
            rom[START+i*2:START+i*2+2] = w.to_bytes(2, 'big')
        for pc,w in (extra or {}).items():
            rom[pc:pc+2] = w.to_bytes(2, 'big')
        rom[POOL:POOL+4] = value.to_bytes(4, 'big')
        p = {POOL: dict(symbol=symbol, relocation_type='R_SH_DIR32', addend=0,
                       binding='STB_GLOBAL', section='SHN_UNDEF', value=value)}
        proofs = [] if proofs_out is None else proofs_out
        kw = dict(context_start=START, tail_proofs=proofs)
        if enabled:
            kw.update(tail_provenance=p if provenance is None else provenance,
                      canonical_functions={'_f_2000': TARGET}, boot_functions=boot)
        validate_prefix(rom[START:END], rom, START, END,
                        entries or {START: '_f_1000', END: '_f_1080'}, **kw)
        return proofs

    def test_all_forms_are_proven(self):
        for name,ops in FORMS.items():
            with self.subTest(form=name):
                proof, = self.validate(ops)
                self.assertEqual(proof['load_pc'], START)
                self.assertEqual(proof['jump_pc'], START+(len(ops)-2)*2)
                self.assertEqual(proof['target_rom'], TARGET)
                self.assertEqual(proof['form'],name)
                self.assertEqual(proof['stack_increment'],STACK[name])
                self.assertEqual([e['pc'] for e in proof['effects']],
                                 list(range(START,START+len(ops)*2,2)))

    def test_default_still_rejects_all(self):
        for ops in FORMS.values():
            with self.assertRaisesRegex(PrefixError, 'indirect jump'):
                self.validate(ops, enabled=False)

    def test_every_non_immediate_opcode_bit_mutation_fails(self):
        # The only wildcards are the signed argument immediate and form C's
        # literal displacement; mutating any other instruction bit must not
        # discover an arbitrary safe pattern.
        for name,ops in FORMS.items():
            for index in range(1,len(ops)):
                if index == len(ops)-2:
                    # JSR/RTS are supported by the preexisting validator;
                    # they must not produce a literal-tail proof, but need
                    # not invalidate an otherwise exact ordinary range.
                    continue
                for bit in range(16):
                    if index in WILDCARDS[name] and bit < 8:
                        continue
                    changed = list(ops)
                    changed[index] ^= 1 << bit
                    with self.subTest(form=name,index=index,bit=bit), self.assertRaises(PrefixError):
                        self.validate(changed)

    def test_a_jsr_is_not_reported_as_a_proven_tail(self):
        for ops in FORMS.values():
            changed=list(ops)
            changed[-2] &= ~0x20  # JMP @Rn -> JSR @Rn
            changed += [0x000b,0x0009]
            self.assertEqual(self.validate(changed),[])

    def test_generic_five_restore_window_still_fails(self):
        with self.assertRaises(PrefixError):
            self.validate([0xd31b,0x69f6,0x6af6,0x6bf6,0x6cf6,0x6df6,0x432b,0x6ef6])

    def test_immediate_argument_values_are_not_address_specific(self):
        for name,ops in FORMS.items():
            if name not in IMMEDIATE:
                continue  # D-F carry no argument immediate
            index = IMMEDIATE[name]
            for value in (0,1,127,128,255):
                changed = list(ops)
                changed[index] = (changed[index] & 0xff00) | value
                with self.subTest(form=name,value=value):
                    self.assertEqual(len(self.validate(changed)),1)

    def test_restore_order_and_extra_instructions_fail(self):
        for name,ops in FORMS.items():
            for index in range(1,len(ops)-2):
                for replacement in (0x0009,0x7f04,0x4f36,0xffff):
                    if name in 'DEF' and index == 1 and replacement == 0x7f04:
                        continue  # this turns one full-frame epilogue into another (E or F)
                    changed=list(ops)
                    changed[index]=replacement
                    with self.subTest(form=name,index=index,op=replacement), self.assertRaises(PrefixError):
                        self.validate(changed)
            for left in range(1,len(ops)-3):
                changed=list(ops)
                changed[left],changed[left+1]=changed[left+1],changed[left]
                with self.subTest(form=name,swap=left), self.assertRaises(PrefixError):
                    self.validate(changed)
            for index in range(1,len(ops)-1):
                changed=list(ops)
                changed.insert(index,0x0009)
                with self.subTest(form=name,insert=index), self.assertRaises(PrefixError):
                    self.validate(changed)

    def test_extra_roots_and_late_branch_entries_fail(self):
        root=START+0x40
        for name,ops in FORMS.items():
            for pc in range(START+2,START+len(ops)*2,2):
                entries={START:'_f_1000',END:'_f_1080',pc:f'_f_{pc:x}'}
                with self.subTest(form=name,root=pc), self.assertRaises(PrefixError):
                    self.validate(ops, entries=entries)
                entries={START:'_f_1000',END:'_f_1080',root:'_f_1040'}
                displacement=(pc-(root+4))//2
                for base,mask in [(0xa000,0xfff),(0xb000,0xfff),(0x8900,0xff),(0x8d00,0xff)]:
                    extra={root:base|(displacement&mask),root+2:0x0009,
                           root+4:0x000b,root+6:0x0009}
                    with self.subTest(form=name,pc=pc,branch=base), self.assertRaises(PrefixError):
                        self.validate(ops,entries=entries,extra=extra)

    def test_no_provenance_is_not_a_numeric_pointer_proof(self):
        for ops in FORMS.values():
            with self.assertRaises(PrefixError):
                self.validate(ops,provenance={})

    def test_later_bad_candidate_publishes_no_partial_proof(self):
        proofs=[]
        root=START+0x40
        extra={root:0xd40b,root+2:0x442b,root+4:0x6ef6}
        with self.assertRaises(PrefixError):
            self.validate(FORMS['A'],entries={START:'_f_1000',root:'_f_1040',END:'_f_1080'},
                          extra=extra,proofs_out=proofs)
        self.assertEqual(proofs,[])


    def test_form_c_is_only_for_r3(self):
        for register in (1, 2):
            changed = list(FORMS['C'])
            changed[0] = 0xd01b | register << 8
            changed[-2] = 0x402b | register << 8
            with self.subTest(register=register), self.assertRaises(PrefixError):
                self.validate(changed)

    def test_form_c_store_literal_moves_with_its_displacement(self):
        changed = list(FORMS['C'])
        changed[3] = 0xd11a
        proof, = self.validate(changed)
        load, = [e for e in proof['effects'] if e['pc'] == START+6]
        self.assertEqual(load['memory']['address'], POOL)
        self.assertEqual(load['writes'], [1])

    def test_full_frame_forms_reject_reservations_and_bad_sizes(self):
        for name in ('E', 'F'):
            for imm in (0x00, 0x80, 0xf0, 0x5a, 0x7f):
                changed = list(FORMS[name])
                changed[1] = 0x7f00 | imm
                with self.subTest(form=name, imm=imm), self.assertRaises(PrefixError):
                    self.validate(changed)

    def test_full_frame_forms_need_every_restore(self):
        for name in ('D', 'E', 'F'):
            ops = FORMS[name]
            # Dropping the optional MACL/frame release gives another member of
            # the family; dropping PR or any GPR restore must fail.
            for index in range(ops.index(0x4f26), len(ops) - 2):
                changed = ops[:index] + ops[index + 1:]
                with self.subTest(form=name, drop=index), self.assertRaises(PrefixError):
                    self.validate(changed)

    def test_full_frame_forms_are_only_for_r3(self):
        for name in ('D', 'E', 'F'):
            for register in (1, 2):
                changed = list(FORMS[name])
                changed[0] = 0xd01b | register << 8
                changed[-2] = 0x402b | register << 8
                with self.subTest(form=name, register=register), self.assertRaises(PrefixError):
                    self.validate(changed)

    BOOT = dict(boot={'_soft_reset': 0x686}, value=0x686, symbol='_soft_reset')

    def test_boot_rom_tail_after_full_frame_epilogues(self):
        for name in ('D', 'E', 'F', 'G'):
            ops = list(FORMS[name])
            with self.subTest(form=name):
                proof, = self.validate(ops, **self.BOOT)
                self.assertTrue(proof['boot_rom_target'])
                self.assertEqual((proof['target_rom'], proof['value']), (0x686, 0x686))

    def test_boot_rom_tail_needs_its_reviewed_entry_and_in_place_value(self):
        ops = FORMS['G']
        for kw in [dict(self.BOOT, boot=None), dict(self.BOOT, boot={}),
                   dict(self.BOOT, value=0x686 + DELTA), dict(self.BOOT, value=0x688),
                   dict(self.BOOT, boot={'_soft_reset': 0x780}),
                   dict(self.BOOT, boot={'_soft_reset': 0x687}),
                   dict(self.BOOT, symbol='_other')]:
            with self.subTest(kw=kw), self.assertRaises(PrefixError):
                self.validate(ops, **kw)

    def test_boot_rom_tail_rejects_the_older_forms(self):
        for name in ('A', 'B', 'C'):
            with self.subTest(form=name), self.assertRaises(PrefixError):
                self.validate(FORMS[name], **self.BOOT)

    def test_boot_entries_do_not_admit_ram_targets_differently(self):
        proof, = self.validate(FORMS['G'], boot={'_soft_reset': 0x686})
        self.assertNotIn('boot_rom_target', proof)

    def test_full_frame_family_members(self):
        d, e, f = FORMS['D'], FORMS['E'], FORMS['F']
        self.assertEqual(self.validate(f[:1] + f[2:])[0]['form'], 'D')
        self.assertEqual(self.validate(f[:2] + f[3:])[0]['form'], 'E')
        with self.assertRaises(PrefixError):  # MACL must follow the frame release
            self.validate([f[0], f[2], f[1]] + f[3:])

    def test_form_a_restores_after_form_c_stores_fail(self):
        a, c = FORMS['A'], FORMS['C']
        with self.assertRaises(PrefixError):
            self.validate(c[:5] + a[4:])

if __name__ == '__main__':
    unittest.main()
