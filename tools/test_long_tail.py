"""Independent complete-oracle structural fixtures for ordered tail forms."""
import unittest
from c_prefix import PrefixError, validate_prefix

START, END, POOL, TARGET, DELTA = 0x1000, 0x1080, 0x1070, 0x2000, 0x5fff880
FORMS = {
    'A': [0xd31b, 0x314c, 0x2100, 0xe412, 0x4f16, 0x4f26,
          0x69f6, 0x6af6, 0x6bf6, 0x6cf6, 0x6df6, 0x432b, 0x6ef6],
    'B': [0xd11b, 0xe501, 0x02ed, 0x64e3, 0x223b, 0x0e25,
          0x4f16, 0x4f26, 0x412b, 0x6ef6],
}


class LongTailTests(unittest.TestCase):
    def validate(self, ops, *, entries=None, extra=None, enabled=True, provenance=None,
                 proofs_out=None):
        rom = bytearray(0x3000)
        for i,w in enumerate(ops):
            rom[START+i*2:START+i*2+2] = w.to_bytes(2, 'big')
        for pc,w in (extra or {}).items():
            rom[pc:pc+2] = w.to_bytes(2, 'big')
        rom[POOL:POOL+4] = (TARGET+DELTA).to_bytes(4, 'big')
        p = {POOL: dict(symbol='_f_2000', relocation_type='R_SH_DIR32', addend=0,
                       binding='STB_GLOBAL', section='SHN_UNDEF', value=TARGET+DELTA)}
        proofs = [] if proofs_out is None else proofs_out
        kw = dict(context_start=START, tail_proofs=proofs)
        if enabled:
            kw.update(tail_provenance=p if provenance is None else provenance,
                      canonical_functions={'_f_2000': TARGET})
        validate_prefix(rom[START:END], rom, START, END,
                        entries or {START: '_f_1000', END: '_f_1080'}, **kw)
        return proofs

    def test_both_forms_are_proven(self):
        for name,ops in FORMS.items():
            with self.subTest(form=name):
                proof, = self.validate(ops)
                self.assertEqual(proof['load_pc'], START)
                self.assertEqual(proof['jump_pc'], START+(len(ops)-2)*2)
                self.assertEqual(proof['target_rom'], TARGET)
                self.assertEqual(proof['form'],name)
                self.assertEqual(proof['stack_increment'],32 if name=='A' else 12)
                self.assertEqual([e['pc'] for e in proof['effects']],
                                 list(range(START,START+len(ops)*2,2)))

    def test_default_still_rejects_both(self):
        for ops in FORMS.values():
            with self.assertRaisesRegex(PrefixError, 'indirect jump'):
                self.validate(ops, enabled=False)

    def test_every_non_immediate_opcode_bit_mutation_fails(self):
        # The only wildcard is the signed argument immediate; mutating any
        # other instruction bit must not discover an arbitrary safe pattern.
        for name,ops in FORMS.items():
            immediate = 3 if name == 'A' else 1
            for index in range(1,len(ops)):
                if index == len(ops)-2:
                    # JSR/RTS are supported by the preexisting validator;
                    # they must not produce a literal-tail proof, but need
                    # not invalidate an otherwise exact ordinary range.
                    continue
                for bit in range(16):
                    if index == immediate and bit < 8:
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
            index = 3 if name == 'A' else 1
            for value in (0,1,127,128,255):
                changed = list(ops)
                changed[index] = (changed[index] & 0xff00) | value
                with self.subTest(form=name,value=value):
                    self.assertEqual(len(self.validate(changed)),1)

    def test_restore_order_and_extra_instructions_fail(self):
        for name,ops in FORMS.items():
            for index in range(1,len(ops)-2):
                for replacement in (0x0009,0x7f04,0x4f36,0xffff):
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


if __name__ == '__main__':
    unittest.main()
