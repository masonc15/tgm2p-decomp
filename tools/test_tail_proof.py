"""Independent structural negatives: compiled and oracle deliberately agree."""
import unittest
from c_prefix import PrefixError, validate_prefix

START, END, POOL, TARGET = 0x1000, 0x1040, 0x1030, 0x2000
DELTA = 0x5fff880


def fixture(ops=None):
    rom = bytearray(0x3000)
    # Four permitted instructions between the defining load and tail JMP.
    ops = ops or [0xd30b, 0x7f08, 0x4f26, 0x68f6, 0x6df6, 0x432b, 0x6ef6]
    for i, op in enumerate(ops):
        rom[START+i*2:START+i*2+2] = op.to_bytes(2, 'big')
    rom[POOL:POOL+4] = (TARGET+DELTA).to_bytes(4, 'big')
    return rom


class TailProofTests(unittest.TestCase):
    def run_fixture(self, ops=None, entries=None, provenance=None, canonical=None,
                    compiled_change=None, start=START, end=END):
        rom = fixture(ops)
        compiled = bytearray(rom[START:END])
        if compiled_change:
            compiled_change(compiled)
        proofs = []
        provenance = self.provenance() if provenance is None else provenance
        result = validate_prefix(compiled, rom, start, end,
                                 entries or {START: '_f_1000', end: f'_f_{end:x}'},
                                 context_start=START, tail_provenance=provenance,
                                 canonical_functions={'_f_2000': TARGET} if canonical is None else canonical,
                                 tail_proofs=proofs)
        return result, proofs

    def provenance(self):
        # Shape is coordinated with the proposed extractor; these are the
        # independently supplied facts, not numeric pointers alone.
        return {POOL: dict(symbol='_f_2000', relocation_type='R_SH_DIR32', addend=0,
                           binding='STB_GLOBAL', section='SHN_UNDEF',
                           value=TARGET+DELTA)}

    def test_proven_tail_and_manifest(self):
        deps, proofs = self.run_fixture()
        self.assertEqual(deps, [])
        self.assertEqual(len(proofs), 1)
        self.assertEqual(proofs[0]['jump_pc'], START+10)
        self.assertEqual(proofs[0]['load_pc'], START)
        self.assertEqual(proofs[0]['target_rom'], TARGET)

    def test_short_r2_window_and_external_literal(self):
        deps, proofs = self.run_fixture([0xd20b, 0x422b, 0x6ef6], end=0x1020)
        self.assertEqual(deps, [dict(pc=START, target=POOL, size=4,
                                    value=(TARGET+DELTA).to_bytes(4, 'big').hex())])
        self.assertEqual(len(proofs), 1)

    def test_r1_literal_tail_uses_same_proof(self):
        _, proofs = self.run_fixture([0xd10b, 0x7f08, 0x4f26, 0x68f6,
                                     0x6df6, 0x412b, 0x6ef6])
        self.assertEqual(proofs[0]['register'], 1)

    def test_r1_overwrite_or_delay_write_rejected(self):
        for op in [0x61e3, 0x7101, 0x2119, 0x211a, 0x611e, 0x61f6,
                   0x6816, 0x410b, 0xd10a]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.run_fixture([0xd10b, op, 0x4f26, 0x68f6, 0x6df6, 0x412b, 0x6ef6])
        with self.assertRaises(PrefixError):
            self.run_fixture([0xd10b, 0x7f08, 0x4f26, 0x68f6, 0x6df6, 0x412b, 0x61f6])

    def test_pool_alignment_uses_real_pc_in_middle_context(self):
        rom = fixture([0x0009, 0xd30b, 0x432b, 0x6ef6])
        proofs = []
        deps = validate_prefix(rom[START:END], rom, START+2, 0x1020,
                               {START+2:'_f_1002', 0x1020:'_f_1020'},
                               context_start=START, tail_provenance=self.provenance(),
                               canonical_functions={'_f_2000':TARGET}, tail_proofs=proofs)
        self.assertEqual(deps[0]['target'], POOL)
        self.assertEqual(proofs[0]['load_pc'], START+2)

    def test_corrupt_external_tail_literal(self):
        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.run_fixture([0xd20b, 0x422b, 0x6ef6], end=0x1020,
                             compiled_change=lambda b: b.__setitem__(POOL-START, 7))

    def test_bypass_load_rejoins_valid_path(self):
        # Taken outcome bypasses1004's load and joins its safe-looking suffix.
        with self.assertRaises(PrefixError):
            self.run_fixture([0x8902, 0x0009, 0xd30a, 0x7f08, 0x4f26,
                              0x432b, 0x6ef6])

    def test_five_intervening_restores_are_outside_initial_scope(self):
        with self.assertRaises(PrefixError):
            self.run_fixture([0xd30b, 0x7f08, 0x4f26, 0x68f6, 0x6df6,
                              0x69f6, 0x432b, 0x6ef6])

    def test_extra_roots_in_spine_or_delay_fail(self):
        for pc in range(START+2, START+14, 2):
            entries = {START: '_f_1000', END: '_f_1040', pc: f'_f_{pc:x}'}
            with self.subTest(pc=pc), self.assertRaises(PrefixError):
                self.run_fixture(entries=entries)

    def test_overwrite_call_store_and_unknown_spines_fail(self):
        # Includes target write, target postincrement, call, branches, memory
        # store, unknown, negative SP adjustment, and a second literal load.
        for op in [0x63e3, 0x7301, 0x2339, 0x233a, 0x633e, 0x63f6,
                   0x6836, 0x430b, 0xb001, 0xa001, 0x8901, 0x2f32,
                   0xffff, 0x7ff8, 0xd30a]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.run_fixture([0xd30b, op, 0x4f26, 0x68f6, 0x6df6, 0x432b, 0x6ef6])

    def test_bad_delay_slots_fail(self):
        for op in [0x63f6, 0x7301, 0xd30a, 0x9001, 0x432b, 0x000b,
                   0xc701, 0xffff, 0x2f32, 0x6836]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.run_fixture([0xd30b, 0x7f08, 0x4f26, 0x68f6, 0x6df6, 0x432b, op])

    def test_wrong_load_register_and_unsupported_jump_register_fail(self):
        for load, jump in [(0xd20b, 0x432b), (0xdf0b, 0x4f2b),
                           (0xd40b, 0x442b), (0xd00b, 0x402b), (0x930b, 0x432b)]:
            with self.subTest(load=hex(load)), self.assertRaises(PrefixError):
                self.run_fixture([load, 0x7f08, 0x4f26, 0x68f6, 0x6df6, jump, 0x6ef6])

    def test_provenance_missing_or_wrong_identity_fail(self):
        with self.assertRaises(PrefixError):
            self.run_fixture(provenance={})
        for key, values in {
            'symbol': ['_g_2000', '_d_2000', '_alias', '_f_2002', 'f_2000'],
            'relocation_type': ['R_SH_NONE', 'R_SH_REL32'], 'addend': [1, -1],
            'binding': ['STB_LOCAL', 'STB_WEAK'], 'section': [1, '.text'],
            'value': [TARGET+DELTA+2, TARGET],
            'relocation_count': [0, 2], 'overlap': [True],
        }.items():
            for value in values:
                p = self.provenance()
                p[POOL][key] = value
                with self.subTest(key=key, value=value), self.assertRaises(PrefixError):
                    self.run_fixture(provenance=p)

    def test_noncanonical_or_internal_target_fail(self):
        for canonical in [{}, {'_f_2000':TARGET+2}, {'_f_2000':POOL}, {'_f_2000':START}]:
            with self.subTest(canonical=canonical), self.assertRaises(PrefixError):
                self.run_fixture(canonical=canonical)

    def test_byte_corruption_fails_before_proof(self):
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_fixture(compiled_change=lambda b: b.__setitem__(1, b[1]^1))

    def test_independent_branches_into_window_fail(self):
        # Separate root at1010 has a conditional/BRA/BSR/backedge that enters
        # the good path after the defining load. Oracle still agrees.
        for branch in [0x89f7, 0x8bf7, 0xaff7, 0xbff7, 0x8df7, 0x8ff7]:
            ops = [0xd30b, 0x7f08, 0x4f26, 0x68f6, 0x6df6, 0x432b, 0x6ef6,
                   0x0009, branch, 0x0009, 0x000b, 0x0009]
            with self.subTest(branch=hex(branch)), self.assertRaises(PrefixError):
                self.run_fixture(ops, entries={START:'_f_1000', START+16:'_f_1010', END:'_f_1040'})

    def test_generic_indirects_remain_rejected(self):
        for op in [0x0023, 0x0003, 0x002b, 0x001b, 0xc300, 0xc700]:
            with self.subTest(op=hex(op)), self.assertRaises(PrefixError):
                self.run_fixture([op, 0x0009])


if __name__ == '__main__':
    unittest.main()
