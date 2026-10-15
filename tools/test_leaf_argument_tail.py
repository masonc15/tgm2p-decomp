"""Parent-authored ABI/CFG negatives; each mutated oracle agrees with its code."""
import unittest
from c_prefix import PrefixError, validate_prefix

START, END, POOL, TARGET = 0x1000, 0x1080, 0x107c, 0x2000
DELTA = 0x5fff880
BASE = [0x2f46, 0xd11e, 0xe202, 0x320c, 0x342c, 0x6442, 0x412b, 0x7f04]
SAVED = [0x2fe6, 0xd11e, 0xe202, 0x6e43, 0x64e3, 0x412b, 0x6ef6]


class IndependentLeafTests(unittest.TestCase):
    def run_leaf(self, ops=BASE, *, enabled=True, extra_entries=None, provenance_change=None,
                 data_literals=None):
        rom = bytearray(0x3000)
        for i, op in enumerate(ops):
            rom[START + i*2:START + i*2 + 2] = op.to_bytes(2, 'big')
        rom[POOL:POOL+4] = (TARGET + DELTA).to_bytes(4, 'big')
        for address, value in (data_literals or {}).items():
            rom[address:address+4] = value.to_bytes(4, 'big')
        facts = dict(symbol='_f_2000', relocation_type='R_SH_DIR32', addend=0,
                     binding='STB_GLOBAL', section='SHN_UNDEF', value=TARGET + DELTA,
                     relocation_count=1, overlap=False)
        if provenance_change:
            facts.update(provenance_change)
        entries = {START: '_f_1000', END: '_f_1080'}
        entries.update(extra_entries or {})
        proofs = []
        kwargs = dict(context_start=START, tail_provenance={POOL: facts},
                      canonical_functions={'_f_2000': TARGET}, tail_proofs=proofs)
        if enabled:
            kwargs['leaf_argument_tails'] = True
        validate_prefix(rom[START:END], rom, START, END, entries, **kwargs)
        return proofs

    def test_argument_leaf_manifest_and_full_stack_proof(self):
        proof, = self.run_leaf()
        self.assertEqual(proof['proof_kind'], 'whole_leaf_argument_tail')
        self.assertEqual(proof['function_root'], START)
        self.assertEqual(proof['load_pc'], START+2)
        self.assertEqual(proof['jump_pc'], START+12)
        self.assertEqual(proof['target_rom'], TARGET)
        self.assertEqual(len(proof['effects']), len(BASE))

    def test_saved_register_restored_in_owned_slot(self):
        proof, = self.run_leaf(SAVED)
        self.assertEqual(proof['proof_kind'], 'whole_leaf_argument_tail')
        self.assertEqual(len(proof['effects']), len(SAVED))

    def test_new_class_requires_separate_opt_in(self):
        with self.assertRaises(PrefixError):
            self.run_leaf(enabled=False)

    def test_private_frame_and_callee_register_failures(self):
        variants = {
            'stack_leak': BASE[:-1] + [0x0009],
            'release_too_far': BASE[:-1] + [0x7f08],
            'release_backwards': BASE[:-1] + [0x7ffc],
            'no_push': [0xe400] + BASE[1:],
            'wrong_saved_value': [0x2fd6] + SAVED[1:],
            'restore_wrong_register': SAVED[:-1] + [0x6df6],
            'callee_register_clobber': BASE[:2] + [0xee01] + BASE[2:],
            'callee_register_copy': BASE[:2] + [0x6e43] + BASE[2:],
            'save_special': [0x4f22] + BASE[1:],
            'restore_special': BASE[:-1] + [0x4f26],
        }
        for name, ops in variants.items():
            with self.subTest(name=name), self.assertRaises(PrefixError):
                self.run_leaf(ops)

    def test_argument_side_effects_and_aliases_fail(self):
        for op in [0x2422, 0x2426, 0x64f3, 0x6f43, 0x7ffc, 0x7f04,
                   0x6122, 0x6143, 0x7101, 0x6416, 0x0008, 0xffff,
                   0x440b, 0x0023, 0x0003, 0xc300]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.run_leaf(BASE[:3] + [op] + BASE[3:])

    def test_every_alternate_function_entry_fails(self):
        for i in range(1, len(BASE)):
            pc = START+i*2
            with self.subTest(pc=hex(pc)), self.assertRaises(PrefixError):
                self.run_leaf(extra_entries={pc: f'_f_{pc:x}'})

    def test_branches_calls_and_returns_inside_whole_leaf_fail(self):
        for op in [0x8900, 0x8b00, 0x8d00, 0x8f00, 0xa000, 0xb000,
                   0x0009]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.run_leaf(BASE[:3] + [op] + BASE[3:])

    def test_return_before_tail_does_not_fabricate_leaf_proof(self):
        # Existing RTS handling intentionally leaves subsequent pool/data unvisited.
        self.assertEqual(self.run_leaf(BASE[:3] + [0x000b, 0x0009] + BASE[3:]), [])

    def test_whole_leaf_instruction_limit(self):
        # Including push, defining load, final jump and its slot: 22 passes, 23 fails.
        valid = BASE[:2] + [0xe200] * 18 + BASE[-2:]
        self.assertEqual(len(valid), 22)
        self.assertEqual(self.run_leaf(valid)[0]['proof_kind'], 'whole_leaf_argument_tail')
        with self.assertRaises(PrefixError):
            self.run_leaf(valid[:2] + [0xe200] + valid[2:])

    def test_original_relocation_identity_remains_required(self):
        for change in [dict(symbol='_f_2002'), dict(symbol='_g_2000'),
                       dict(addend=4), dict(binding='STB_LOCAL'), dict(section='.text'),
                       dict(relocation_count=2), dict(overlap=True),
                       dict(value=TARGET), dict(relocation_type='R_SH_REL32')]:
            with self.subTest(change=change), self.assertRaises(PrefixError):
                self.run_leaf(provenance_change=change)

    def test_duplicate_literal_target_alias_cannot_be_dereferenced(self):
        for extra in [[], [0x7204], [0x4208], [0xe000, 0x320c],
                      [0x6323, 0x6233], [0x6503, 0x325c]]:
            # R2's independent PC load aliases R1's reviewed target address.
            ops = [0x2f46, 0xd11e, 0xd21d] + extra + [0x6422, 0x412b, 0x7f04]
            with self.subTest(extra=extra), self.assertRaises(PrefixError):
                self.run_leaf(ops)

    def test_independent_literal_can_construct_target_alias(self):
        # The data load uses a different pool word, then computes target address.
        ops = [0x2f46, 0xd11e, 0xd21c, 0x4208, 0x6422, 0x412b, 0x7f04]
        with self.assertRaises(PrefixError):
            self.run_leaf(ops, data_literals={POOL-4: (TARGET+DELTA)//4})
        # An indexed read can form the same address from two non-alias operands.
        indexed = [0x2f46, 0xd11e, 0xd21c, 0xe004, 0x042e, 0x412b, 0x7f04]
        with self.assertRaises(PrefixError):
            self.run_leaf(indexed, data_literals={POOL-4: TARGET+DELTA-4})


if __name__ == '__main__':
    unittest.main()
