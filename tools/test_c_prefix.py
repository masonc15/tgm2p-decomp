import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from c_prefix import PrefixError, compile_prefix, validate_prefix


def words(*xs):
    return b''.join(x.to_bytes(2, 'big') for x in xs)


class PrefixValidationTests(unittest.TestCase):
    def check(self, code, *, compiled=None, end=6, entries=None):
        return validate_prefix(code if compiled is None else compiled, code, 0, end,
                               entries if entries is not None else {0: '_f_0', end: f'_f_{end:x}'})

    def test_cross_boundary_word(self):
        code = words(0x9003, 0x000b, 0x0009, 0x000b, 0x0009, 0x0364)
        self.assertEqual(self.check(code), [dict(pc=0, target=10, size=2, value='0364')])

    def test_cross_boundary_long(self):
        code = words(0xd102, 0x000b, 0x0009, 0x000b, 0x0009, 0xffff, 0x0606, 0x475a)
        self.assertEqual(self.check(code)[0]['value'], '0606475a')

    def test_changed_prefix_rejected(self):
        code = words(0xe000, 0x000b, 0x0009, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.check(code, compiled=words(0xe001) + code[2:])

    def test_changed_discarded_pool_rejected(self):
        code = words(0x9003, 0x000b, 0x0009, 0x000b, 0x0009, 0x0364)
        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.check(code, compiled=code[:-2] + words(0x0365))

    def test_missing_pool_rejected(self):
        code = words(0x9003, 0x000b, 0x0009, 0xffff, 0xffff, 0x0364)
        with self.assertRaisesRegex(PrefixError, 'outside compiled context'):
            self.check(code, compiled=code[:6])

    def test_cut_inside_function_rejected(self):
        code = words(0xe000, 0x000b, 0x0009, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'function boundaries'):
            self.check(code, entries={0: '_f_0'})

    def test_complete_function_code_range_accepts_only_reachable_body(self):
        code = words(0x000b, 0x0009, 0xffff)
        proofs = []
        self.assertEqual(validate_prefix(code, code, 0, 4, {0: '_f_0'},
                                         function_code_range=True,
                                         function_code_proofs=proofs), [])
        self.assertEqual(proofs[0]['instruction_pcs'], [0])
        self.assertEqual(proofs[0]['delay_slots'], [dict(pc=2, owners=[0])])
        self.assertTrue(proofs[0]['complete_single_entry'])

        with self.assertRaisesRegex(PrefixError, 'not completely reachable'):
            validate_prefix(code, code, 0, 6, {0: '_f_0'},
                            function_code_range=True)

    def test_complete_function_code_range_rejects_early_fallthrough_cut(self):
        code = words(0xe000, 0xe101, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'falls through retained end'):
            validate_prefix(code, code, 0, 4, {0: '_f_0'},
                            function_code_range=True)

    def test_delay_slot_literal_rejected(self):
        code = words(0x000b, 0x9000, 0xffff, 0x1234)
        with self.assertRaisesRegex(PrefixError, 'delay-slot'):
            self.check(code, end=4)

    def test_indirect_jump_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'indirect jump'):
            self.check(words(0x412b, 0x0009, 0xffff, 0xffff))

    def test_mova_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'MOVA'):
            self.check(words(0xc701, 0x000b, 0x0009, 0xffff))

    def test_control_transfer_in_slot_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'control transfer'):
            self.check(words(0x000b, 0x000b, 0x0009, 0xffff))

    def test_conditional_reaches_both_literal_loads(self):
        code = words(0x8902, 0x9005, 0xa001, 0x0009, 0x9104, 0x000b, 0x0009,
                     0x000b, 0x1234, 0xffff, 0x5678)
        self.assertEqual([v['target'] for v in self.check(code, end=14)], [16,20])

    def test_unknown_exit_rejected(self):
        code = words(0xa004, 0x0009, 0x0009, 0xffff, 0xffff, 0xffff, 0xffff)
        with self.assertRaisesRegex(PrefixError, 'unverified control-flow exit'):
            self.check(code)

    def test_misplaced_context_function_rejected(self):
        code = words(0xa001, 0x0009, 0x0009, 0x000b, 0x0009)
        with self.assertRaisesRegex(PrefixError, 'unverified control-flow exit'):
            self.check(code, entries={0: '_f_0', 6: '_f_8'})

    def test_verified_context_tailcall_allowed(self):
        code = words(0xa001, 0x0009, 0x0009, 0x000b, 0x0009)
        self.assertEqual(self.check(code), [])

    def test_unreachable_pool_not_decoded_as_code(self):
        code = words(0x000b, 0x0009, 0xc700, 0x412b, 0xffff)
        self.assertEqual(self.check(code, end=8), [])

    def middle(self, compiled=None, entries=None, context_start=0):
        code = words(0xe001, 0x000b, 0x0009, 0xbffb, 0x0009,
                     0x9003, 0x000b, 0x0009, 0xffff, 0xffff, 0x1234)
        return validate_prefix(code if compiled is None else compiled, code, 6, 16,
                               entries if entries is not None else
                               {0: '_f_0', 6: '_f_6', 16: '_f_10'},
                               context_start=context_start)

    def test_middle_range_verifies_backward_call_and_tail_literal(self):
        self.assertEqual(self.middle(), [dict(pc=10, target=20, size=2, value='1234')])

    def test_middle_range_ignores_unused_preceding_draft_bytes(self):
        code = words(0xe002, 0x000b, 0x0009, 0xbffb, 0x0009,
                     0x9003, 0x000b, 0x0009, 0xffff, 0xffff, 0x1234)
        self.assertEqual(len(self.middle(compiled=code)), 1)

    def test_middle_changed_retained_bytes_rejected(self):
        code = words(0xe001, 0x000b, 0x0009, 0xbffa, 0x0009,
                     0x9003, 0x000b, 0x0009, 0xffff, 0xffff, 0x1234)
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.middle(compiled=code)

    def test_middle_changed_external_literal_rejected(self):
        code = words(0xe001, 0x000b, 0x0009, 0xbffb, 0x0009,
                     0x9003, 0x000b, 0x0009, 0xffff, 0xffff, 0x1235)
        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.middle(compiled=code)

    def test_middle_misplaced_backward_call_entry_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'unverified control-flow exit'):
            self.middle(entries={0: '_f_2', 6: '_f_6', 16: '_f_10'})

    def test_middle_invalid_context_origin_rejected(self):
        for origin in (-2, 1, 8):
            with self.subTest(origin=origin), self.assertRaisesRegex(PrefixError, 'context'):
                self.middle(context_start=origin)

    def test_middle_truncated_context_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'shorter than prefix'):
            self.middle(compiled=words(0xe001, 0x000b, 0x0009, 0xbffb, 0x0009))

    def test_middle_indirect_jump_still_rejected(self):
        code = words(0x000b, 0x0009, 0x432b, 0x0009)
        with self.assertRaisesRegex(PrefixError, 'indirect jump'):
            validate_prefix(code, code, 4, 8, {4: '_f_4'}, context_start=0)


@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS') == '1', 'requires nuada SHC/binutils')
class PrefixCompileTests(unittest.TestCase):
    def run_case(self, filename, start, end, mutation=None, context_start=None,
                 literal_tail_calls=False, canonical_functions=None, leaf_argument_tails=False,
                 function_code_range=False):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            source = (build.ROOT / 'src' / filename).read_text()
            if mutation:
                old, new = mutation
                self.assertIn(old, source)
                source = source.replace(old, new, 1)
            src = td / 'case.c'
            src.write_text(source)
            obj = td / 'kept.o'
            compile_prefix(src, obj, start, end, rom=build.fn.ROM, shcc=build.SHCC,
                           version=build.SHC_VERSION, flags=build.c_flags(src),
                           symbols=build.read_symbols(), assembler=build.AS,
                           context_start=context_start,
                           function_code_range=function_code_range,
                           **(dict(leaf_argument_tails=True) if leaf_argument_tails else {}),
                           **(dict(literal_tail_calls=True,
                                   canonical_functions=(build.reviewed_tail_functions() if canonical_functions is None
                                                        else canonical_functions))
                              if literal_tail_calls else {}))
            with obj.open('rb') as f:
                elf = ELFFile(f)
                self.assertEqual(elf.get_section_by_name('.text').data(), build.fn.ROM[start:end])
                names = {s.name for s in elf.get_section_by_name('.symtab').iter_symbols()}
            return names, json.loads(obj.with_suffix('.prefix.json').read_text())

    def test_settings_helpers_keep_readonly_predecessor_context_outside_package(self):
        names, reset = self.run_case('settings_reset_9c0c.c', 0xb2f4, 0xb352,
                                     context_start=0x9c0c)
        self.assertEqual(reset['functions'], ['_f_b2f4'])
        self.assertIn('_f_b2f4', names)
        self.assertNotIn('_f_af8c', names)
        self.assertNotIn('_f_b352', names)
        self.assertEqual(reset['kept_bytes'], 94)
        self.assertEqual(reset['context_bytes'], 19684)
        self.assertEqual(reset['discarded_readonly_sections'], [
            dict(name='.rodata', size=1182)])
        self.assertEqual(len(reset['literal_dependencies']), 2)

        names, copy = self.run_case('settings_reset_9c0c.c', 0xb352, 0xb36e,
                                    context_start=0x9c0c)
        self.assertEqual(copy['functions'], ['_f_b352'])
        self.assertIn('_f_b352', names)
        self.assertNotIn('_f_b2f4', names)
        self.assertNotIn('_f_b36e', names)
        self.assertEqual(copy['kept_bytes'], 28)
        self.assertEqual(copy['discarded_readonly_sections'], [
            dict(name='.rodata', size=1182)])
        self.assertEqual(copy['literal_dependencies'], [
            dict(pc=0xb354, target=0xb3b8, size=4, value='06032b6a'),
            dict(pc=0xb358, target=0xb3b0, size=4, value='06060034')])

    def test_settings_checksum_is_complete_function_code_range(self):
        names, manifest = self.run_case(
            'settings_reset_9c0c.c', 0xb36e, 0xb39a,
            context_start=0x9c0c, literal_tail_calls=True,
            function_code_range=True)
        self.assertEqual(manifest['functions'], ['_f_b36e'])
        self.assertIn('_f_b36e', names)
        self.assertEqual(manifest['kept_bytes'], 44)
        self.assertTrue(manifest['function_code_range'])
        proof = manifest['function_code_proofs'][0]
        self.assertEqual((proof['start'], proof['end']), (0xb36e, 0xb39a))
        self.assertEqual(len(proof['instruction_pcs']), 20)
        self.assertEqual(proof['delay_slots'], [
            dict(pc=0xb38e, owners=[0xb38c]),
            dict(pc=0xb398, owners=[0xb396]),
        ])
        tail = manifest['tail_proofs'][0]
        self.assertEqual(tail['proof_kind'], 'complete_function_code_tail')
        self.assertEqual((tail['load_pc'], tail['jump_pc'], tail['literal']),
                         (0xb390, 0xb396, 0xb3bc))
        self.assertEqual(tail['symbol'], '_f_3036e')

        with self.assertRaisesRegex(PrefixError, '(crosses prefix boundary|not completely reachable)'):
            self.run_case('settings_reset_9c0c.c', 0xb36e, 0xb398,
                          context_start=0x9c0c, literal_tail_calls=True,
                          function_code_range=True)

        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.run_case('settings_reset_9c0c.c', 0xb36e, 0xb39a,
                          ('\tf_3036e();', '\tf_2f36e();'),
                          context_start=0x9c0c, literal_tail_calls=True,
                          function_code_range=True)

    def test_shuffle_copy_uses_genuine_context_and_exact_external_literals(self):
        names, manifest = self.run_case(
            'shuffle_225e4.c', 0x225e4, 0x225fa,
            context_start=0x21d94, function_code_range=True)
        self.assertEqual(manifest['functions'], ['_f_225e4'])
        self.assertIn('_f_225e4', names)
        self.assertNotIn('_f_21d94', names)
        self.assertEqual(manifest['kept_bytes'], 22)
        self.assertEqual(manifest['context_bytes'], 2184)
        self.assertTrue(manifest['function_code_range'])
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x225e8, target=0x22614, size=4, value='06035670'),
            dict(pc=0x225ec, target=0x22618, size=4, value='060308ec'),
        ])
        proof = manifest['function_code_proofs'][0]
        self.assertEqual((proof['start'], proof['end']), (0x225e4, 0x225fa))
        self.assertTrue(proof['complete_single_entry'])
        self.assertFalse(proof['falls_through_end'])

        with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
            self.run_case(
                'shuffle_225e4.c', 0x225e4, 0x225fa,
                ('\tx = g_6035670;', '\tx = *(const struct s76 *)&g_6035618;'),
                context_start=0x21d94, function_code_range=True)

    def test_item_callback_middle_range_uses_genuine_context(self):
        names, manifest = self.run_case('item_8164.c', 0x8164, 0x81cc, context_start=0x7fa0)
        self.assertIn('_f_8164', names)
        self.assertNotIn('_f_7fa0', names)
        self.assertNotIn('_f_8156', names)
        self.assertEqual(manifest['kept_bytes'], 104)
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_gfx_callback_middle_range_uses_genuine_context(self):
        names, manifest = self.run_case('gfx_2ab56.c', 0x2ab56, 0x2ac4c, context_start=0x2a0d4)
        exports = {'_f_2ab56', '_f_2abce', '_f_2abd6', '_f_2abde'}
        self.assertEqual(set(manifest['functions']), exports)
        self.assertTrue(exports.issubset(names))
        self.assertNotIn('_f_2a0d4', names)
        self.assertEqual(manifest['kept_bytes'], 246)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_gfx_blend_task_setter_retains_only_real_callback(self):
        names, manifest = self.run_case('gfxblend_2af5e.c', 0x2af5e, 0x2afc0,
                                        context_start=0x2ac4c)
        self.assertEqual(manifest['functions'], ['_f_2af5e'])
        self.assertIn('_f_2af5e', names)
        self.assertNotIn('_f_2ac4c', names)
        self.assertEqual(manifest['kept_bytes'], 98)
        self.assertEqual(manifest['context_bytes'], 884)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('gfxblend_2af5e.c', 0x2ac4c, 0x2afc0,
                          context_start=0x2ac4c)

    def test_board_setters_use_genuine_toggle_pool_context(self):
        names, manifest = self.run_case('board_setters_2774a.c', 0x2774a, 0x277d0,
                                        context_start=0x276e8)
        exports = ['_f_2774a', '_f_2775c', '_f_2776e', '_f_27780',
                   '_f_27792', '_f_277ac', '_f_277be']
        self.assertEqual(manifest['functions'], exports)
        self.assertTrue(set(exports).issubset(names))
        self.assertNotIn('_f_276e8', names)
        self.assertNotIn('_f_277d0', names)
        self.assertNotIn('_f_27862', names)
        self.assertEqual(manifest['kept_bytes'], 134)
        self.assertEqual(manifest['context_bytes'], 476)
        self.assertEqual(len(manifest['literal_dependencies']), 14)
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_save_statistics_reset_counter_use_real_writer_context(self):
        names, manifest = self.run_case('savestats_211a2.c', 0x211a2, 0x21206,
                                        context_start=0x21170)
        self.assertEqual(manifest['functions'], ['_f_211a2', '_f_211e8'])
        self.assertTrue({'_f_211a2', '_f_211e8'}.issubset(names))
        self.assertNotIn('_f_21170', names)
        self.assertNotIn('_f_21206', names)
        self.assertNotIn('_f_213da', names)
        self.assertEqual(manifest['kept_bytes'], 100)
        self.assertEqual(manifest['context_bytes'], 732)
        self.assertEqual(len(manifest['literal_dependencies']), 8)
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('savestats_211a2.c', 0x21170, 0x21206,
                          context_start=0x21170)

    def test_board_tile_callback_uses_real_queue_and_writer_context(self):
        names, manifest = self.run_case('board_tile_callback_27310.c', 0x27310, 0x2733a,
                                        context_start=0x27310)
        self.assertEqual(manifest['functions'], ['_f_27310'])
        self.assertIn('_f_27310', names)
        self.assertNotIn('_f_2733a', names)
        self.assertNotIn('_f_274c0', names)
        self.assertNotIn('_f_2769e', names)
        self.assertEqual(manifest['kept_bytes'], 42)
        self.assertEqual(manifest['context_bytes'], 980)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        # The callback must sign-extend the packed high word for the helper's
        # long selector argument. Removing it shortens the compiled callback,
        # so the nominal 42-byte end is no longer a function boundary.
        with self.assertRaisesRegex(PrefixError, 'contextual function boundaries'):
            self.run_case('board_tile_callback_27310.c', 0x27310, 0x2733a,
                          context_start=0x27310, mutation=(
                              '(short)(packed >> 16)', 'packed >> 16'))
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('board_tile_callback_27310.c', 0x27310, 0x274c0,
                          context_start=0x27310)

    def test_board_queue_address_carrier_agrees_with_retained_vm_caller(self):
        import build
        caller = (build.ROOT / 'src/vm_f0e8.c').read_text()
        declaration = next(line for line in caller.splitlines()
                           if line.startswith('extern void f_2733a('))
        # Compile the genuine producer with its independently retained caller's
        # declaration. Equal ABI widths do not make incompatible C types valid.
        names, manifest = self.run_case(
            'board_tile_callback_27310.c', 0x27310, 0x2733a,
            context_start=0x27310,
            mutation=('void f_2733a(', declaration + '\nvoid f_2733a('))
        self.assertEqual(manifest['functions'], ['_f_27310'])
        self.assertEqual(manifest['kept_bytes'], 42)
        self.assertEqual(manifest['context_bytes'], 980)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertNotIn('_f_2733a', names)

    def test_field_flag_counter_uses_genuine_consumed_id_context(self):
        names, manifest = self.run_case('field_flag_counter_907c.c', 0x907c, 0x9098,
                                        context_start=0x8b20)
        self.assertEqual(manifest['functions'], ['_f_907c'])
        self.assertIn('_f_907c', names)
        self.assertNotIn('_f_8b20', names)
        self.assertNotIn('_f_8b86', names)
        self.assertNotIn('_f_8c96', names)
        self.assertNotIn('_f_9098', names)
        self.assertNotIn('_f_9180', names)
        self.assertEqual(manifest['kept_bytes'], 28)
        self.assertEqual(manifest['context_bytes'], 4180)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x907c, target=0x90d0, size=4, value='06064880'),
            dict(pc=0x907e, target=0x90be, size=2, value='dfff'),
            dict(pc=0x9086, target=0x90c0, size=2, value='4000'),
            dict(pc=0x908e, target=0x90d4, size=4, value='0606488a')])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('field_flag_counter_907c.c', 0x907c, 0x9180,
                          context_start=0x8b20)

    def test_field_match_9098_authenticates_predecessor_owned_pool(self):
        names, manifest = self.run_case('field_match_9098.c', 0x9098, 0x9180,
                                        context_start=0x8b20)
        self.assertEqual(manifest['functions'], ['_f_9098'])
        self.assertIn('_f_9098', names)
        self.assertNotIn('_f_8c96', names)
        self.assertNotIn('_f_907c', names)
        self.assertNotIn('_f_9180', names)
        self.assertEqual(manifest['kept_bytes'], 232)
        self.assertEqual(manifest['context_bytes'], 4180)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('field_match_9098.c', 0x9098, 0x9180,
                          context_start=0x8b20,
                          mutation=('0x06065698;', '0xfffff;'))

    def test_save_checksum_uses_complete_real_writer_reader_context(self):
        names, manifest = self.run_case('save_checksum_131e8.c', 0x131e8, 0x1326c,
                                        context_start=0x131e8)
        self.assertEqual(manifest['functions'], ['_f_131e8'])
        self.assertIn('_f_131e8', names)
        self.assertNotIn('_f_1326c', names)
        self.assertNotIn('_f_13378', names)
        self.assertNotIn('_f_13380', names)
        self.assertEqual(manifest['kept_bytes'], 132)
        self.assertEqual(manifest['context_bytes'], 668)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x131f0, target=0x132c4, size=4, value='06065650'),
            dict(pc=0x13220, target=0x132be, size=2, value='00dc'),
            dict(pc=0x13244, target=0x132c0, size=2, value='00f4')])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('save_checksum_131e8.c', 0x131e8, 0x13378,
                          context_start=0x131e8)

    def test_save_dirty_setter_keeps_real_direct_branch_destination(self):
        names, manifest = self.run_case('save_checksum_131e8.c', 0x13378, 0x13380,
                                        context_start=0x131e8)
        self.assertEqual(manifest['functions'], ['_f_13378'])
        self.assertIn('_f_13378', names)
        self.assertNotIn('_f_131e8', names)
        self.assertNotIn('_f_1326c', names)
        self.assertNotIn('_f_13380', names)
        self.assertEqual(manifest['kept_bytes'], 8)
        self.assertEqual(manifest['context_bytes'], 668)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x13378, target=0x13414, size=4, value='0007ffff'),
            dict(pc=0x1337a, target=0x13418, size=4, value='060794bc')])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_reviewed_effect_tail_target_has_real_entry_and_context(self):
        import build
        self.assertEqual(build.reviewed_tail_functions()['_f_20250'], 0x20250)
        names, manifest = self.run_case('effect_20250.c', 0x20250, 0x202b0)
        self.assertEqual(manifest['functions'], ['_f_20250'])
        self.assertIn('_f_20250', names)
        self.assertNotIn('_f_202b0', names)
        self.assertEqual(manifest['kept_bytes'], 96)
        self.assertEqual(manifest['context_bytes'], 928)
        self.assertEqual(len(manifest['literal_dependencies']), 7)
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_field_setup_helpers_use_genuine_vs_context(self):
        names, manifest = self.run_case('field_helpers_881a.c', 0x881a, 0x88fe,
                                        context_start=0x85d8)
        self.assertEqual(manifest['functions'], ['_f_881a', '_f_88b4'])
        self.assertTrue({'_f_881a', '_f_88b4'}.issubset(names))
        self.assertNotIn('_f_85d8', names)
        self.assertNotIn('_f_87b8', names)
        self.assertNotIn('_f_88fe', names)
        self.assertEqual(manifest['kept_bytes'], 228)
        self.assertEqual(manifest['context_bytes'], 5524)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x88b4, target=0x8990, size=4, value='06079292'),
            dict(pc=0x88b8, target=0x8994, size=4, value='06079290'),
            dict(pc=0x88be, target=0x8998, size=4, value='06064880'),
            dict(pc=0x88c0, target=0x8984, size=2, value='ff3f'),
            dict(pc=0x88c4, target=0x8988, size=2, value='1000'),
            dict(pc=0x88ca, target=0x8986, size=2, value='02f4'),
            dict(pc=0x88d8, target=0x8986, size=2, value='02f4'),
            dict(pc=0x88dc, target=0x898a, size=2, value='0308')])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_field_bgm_thresholds_use_genuine_picker_context(self):
        names, manifest = self.run_case('field_helpers_881a.c', 0x95b8, 0x96ac,
                                        context_start=0x85d8)
        self.assertEqual(manifest['functions'], ['_f_95b8', '_f_9648', '_f_966c'])
        self.assertTrue({'_f_95b8', '_f_9648', '_f_966c'}.issubset(names))
        self.assertNotIn('_f_9180', names)
        self.assertNotIn('_f_96ac', names)
        self.assertEqual(manifest['kept_bytes'], 244)
        self.assertEqual(manifest['context_bytes'], 5524)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x963c, target=0x970e, size=2, value='0384'),
            dict(pc=0x9648, target=0x9710, size=2, value='0118'),
            dict(pc=0x9656, target=0x9712, size=2, value='012c'),
            dict(pc=0x9660, target=0x9712, size=2, value='012c'),
            dict(pc=0x966c, target=0x9710, size=2, value='0118'),
            dict(pc=0x967a, target=0x9712, size=2, value='012c'),
            dict(pc=0x9680, target=0x9712, size=2, value='012c'),
            dict(pc=0x9686, target=0x9714, size=2, value='01e0'),
            dict(pc=0x9690, target=0x9714, size=2, value='01e0'),
            dict(pc=0x9696, target=0x9716, size=2, value='01f4'),
            dict(pc=0x96a0, target=0x9716, size=2, value='01f4'),
        ])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_menu_lookup_uses_real_menu_table_context(self):
        names, manifest = self.run_case('menu_lookup_be04.c', 0xbe04, 0xbe28)
        self.assertEqual(manifest['functions'], ['_f_be04'])
        self.assertIn('_f_be04', names)
        self.assertNotIn('_f_be28', names)
        self.assertNotIn('_f_c21a', names)
        self.assertEqual(manifest['kept_bytes'], 36)
        self.assertEqual(manifest['context_bytes'], 3220)
        self.assertEqual(manifest['literal_dependencies'],
                         [dict(pc=0xbe04, target=0xbf00, size=4, value='0003a8e8')])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_hud_middle_ranges_preserve_real_exports_and_literal_dependencies(self):
        cases = [
            (0x103d0, 0x1050e, ['_f_103d0'],
             [dict(pc=0x104f0, target=0x105f0, size=4, value='000a6d4c')]),
            (0x1065e, 0x10788, ['_f_1065e', '_f_106a6', '_f_106ee'], []),
            (0x10942, 0x10b28, ['_f_10942'], []),
            (0x10d94, 0x11254, ['_f_10d94', '_f_10de6'], []),
        ]
        for start, end, exports, dependencies in cases:
            with self.subTest(start=hex(start)):
                names, manifest = self.run_case('hud_ranges_f958.c', start, end,
                                                context_start=0xf958)
                self.assertEqual(manifest['functions'], exports)
                self.assertTrue(set(exports).issubset(names))
                self.assertNotIn('_f_f958', names)
                self.assertNotIn('_f_1050e', names)
                self.assertNotIn('_f_10788', names)
                self.assertEqual(manifest['kept_bytes'], end - start)
                self.assertEqual(manifest['context_bytes'], 7104)
                self.assertEqual(manifest['literal_dependencies'], dependencies)
                self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_field_dispatch_full_width_return_and_real_context(self):
        import build
        names, manifest = self.run_case('field_dispatch_87b8.c', 0x87b8, 0x881a,
                                       context_start=0x85d8)
        self.assertEqual(manifest['functions'], ['_f_87b8'])
        self.assertIn('_f_87b8', names)
        self.assertNotIn('_f_85d8', names)
        self.assertNotIn('_f_9180', names)
        self.assertEqual(manifest['kept_bytes'], 98)
        self.assertEqual(manifest['context_bytes'], 5512)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x87c8, target=0x8880, size=4, value='06060008'),
            dict(pc=0x87cc, target=0x887c, size=4, value='06064878'),
            dict(pc=0x8808, target=0x8884, size=4, value='06079538'),
        ])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        source = (build.ROOT / 'src/field_dispatch_87b8.c').read_text()
        self.assertEqual(source.count('int f_9180(void)'), 2)
        # Change both declaration and complete definition, preserving C type
        # consistency. Narrowing inserts EXTS.W at87ec and must fail raw matching.
        with self.assertRaisesRegex(PrefixError, 'differs from ROM at 0x87ec'):
            self.run_case('field_dispatch_87b8.c', 0x87b8, 0x881a,
                          context_start=0x85d8, mutation=(source,
                              source.replace('int f_9180(void)', 'short f_9180(void)')))

    def test_startup_reset_retains_exact_leaf_and_following_literals(self):
        names, manifest = self.run_case('startup_reset_1896.c', 0x1896, 0x18ca,
                                       context_start=0x1278)
        self.assertEqual(manifest['functions'], ['_f_1896'])
        self.assertIn('_f_1896', names)
        self.assertNotIn('_f_18ca', names)
        self.assertEqual(manifest['kept_bytes'], 52)
        self.assertEqual(manifest['context_bytes'], 1692)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0x1898, target=0x18f6, size=2, value='02fc'),
            dict(pc=0x18b0, target=0x18f8, size=2, value='0300'),
        ])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaises(PrefixError):
            self.run_case('startup_reset_1896.c', 0x1896, 0x18ca,
                          mutation=('void f_1896(struct field *f)\n{\n\tshort i;\n\n\tfor (i = 0; i < 4; i++)\n\t\tf->b2fc[i] = 0;',
                                    'void f_1896(struct field *f)\n{\n\tshort i;\n\n\tfor (i = 0; i < 4; i++)\n\t\tf->b2fc[i] = 1;'),
                          context_start=0x1278)

    def test_startup_input_complete_pair_and_shared_pool(self):
        names, manifest = self.run_case('startup_input_2efc.c', 0x2e4c, 0x2f3c,
                                       context_start=0x2e4c, literal_tail_calls=True)
        self.assertEqual(manifest['functions'], ['_f_2e4c', '_f_2efc'])
        self.assertIn('_f_2e4c', names)
        self.assertIn('_f_2efc', names)
        self.assertEqual(manifest['kept_bytes'], 240)
        self.assertEqual(manifest['context_bytes'], 240)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual(len(manifest['tail_proofs']), 1)
        self.assertEqual(manifest['tail_proofs'][0]['function_root'], 0x2e4c)
        self.assertEqual(manifest['tail_proofs'][0]['target_rom'], 0x2e6fc)
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('startup_input_2efc.c', 0x2e4c, 0x2f3c,
                          mutation=('f->mode |= 0x200;', 'f->mode |= 0x100;'),
                          context_start=0x2e4c, literal_tail_calls=True)

    def test_startup_input_predecessor_mutation_is_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('startup_input_2efc.c', 0x2e4c, 0x2f3c,
                          mutation=('count /= 10L;', 'count /= 9L;'),
                          context_start=0x2e4c, literal_tail_calls=True)

    def test_hud_marker_preserves_export_and_external_y_literal(self):
        names, manifest = self.run_case('hud_ranges_f958.c', 0x105ba, 0x1065e,
                                       context_start=0xf958)
        self.assertEqual(manifest['functions'], ['_f_105ba'])
        self.assertIn('_f_105ba', names)
        self.assertNotIn('_f_1050e', names)
        self.assertNotIn('_f_1065e', names)
        self.assertEqual(manifest['kept_bytes'], 164)
        self.assertEqual(manifest['context_bytes'], 7104)
        self.assertEqual(manifest['literal_dependencies'],
                         [dict(pc=0x10644, target=0x1076c, size=2, value='011d')])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_hud_unmatched_predecessor_stays_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case('hud_ranges_f958.c', 0x1050e, 0x1065e,
                          context_start=0xf958)

    def test_checksum_callback_middle_range_uses_genuine_context(self):
        names, manifest = self.run_case('checksum_30c2e.c', 0x30c2e, 0x30d40, context_start=0x30880)
        self.assertEqual(manifest['functions'], ['_f_30c2e'])
        self.assertIn('_f_30c2e', names)
        self.assertNotIn('_f_30880', names)
        self.assertNotIn('_f_30a44', names)
        self.assertEqual(manifest['kept_bytes'], 274)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual(manifest.get('tail_proofs', []), [])

    def test_real_vm_renderer_prefix_preserves_allocator_and_pool_context(self):
        names, manifest = self.run_case(
            'vm_empty_f292.c', 0xf200, 0xf292,
            literal_tail_calls=True, leaf_argument_tails=True)
        self.assertEqual(manifest['functions'], ['_f_f200'])
        self.assertIn('_f_f200', names)
        self.assertNotIn('_f_f292', names)
        self.assertEqual(manifest['kept_bytes'], 146)
        self.assertEqual(manifest['context_bytes'], 972)
        self.assertEqual(manifest['literal_dependencies'], [
            dict(pc=0xf204, target=0xf310, size=4, value='0606005c'),
            dict(pc=0xf20c, target=0xf30a, size=2, value='0080'),
            dict(pc=0xf240, target=0xf314, size=4, value='06060004'),
            dict(pc=0xf252, target=0xf318, size=4, value='0602bc4a'),
            dict(pc=0xf258, target=0xf31c, size=4, value='0602c6d4'),
            dict(pc=0xf268, target=0xf320, size=4, value='0602b8c2'),
            dict(pc=0xf270, target=0xf324, size=4, value='0606006c')])
        self.assertEqual(manifest.get('tail_proofs', []), [])
        with self.assertRaisesRegex(PrefixError, 'differs from ROM'):
            self.run_case(
                'vm_empty_f292.c', 0xf200, 0xf292,
                mutation=('register short t = (a & 0xff0000) >> 16;',
                          'short t = (a & 0xff0000) >> 16;'),
                literal_tail_calls=True, leaf_argument_tails=True)

    def test_real_vm_whole_leaf_argument_tails(self):
        for start, end, size in [(0xf4c6, 0xf4dc, 22), (0xf574, 0xf5cc, 88),
                                 (0xf4c6, 0xf5cc, 262)]:
            with self.subTest(start=hex(start), end=hex(end)):
                names, manifest = self.run_case('vm_empty_f292.c', start, end,
                    context_start=0xf200, literal_tail_calls=True, leaf_argument_tails=True)
                self.assertEqual(manifest['kept_bytes'], size)
                self.assertTrue(manifest['leaf_argument_tails'])
                self.assertEqual(len(manifest['tail_proofs']), 2 if size == 262 else 1)
                self.assertTrue(all(p['proof_kind'] == 'whole_leaf_argument_tail'
                                    for p in manifest['tail_proofs']))

    def test_vm_argument_tails_require_separate_mode_and_reviewed_targets(self):
        for start, end in [(0xf4c6, 0xf4dc), (0xf574, 0xf5cc)]:
            for literal in (False, True):
                with self.subTest(start=hex(start), literal=literal), self.assertRaises(PrefixError):
                    self.run_case('vm_empty_f292.c', start, end, context_start=0xf200,
                                  literal_tail_calls=literal)
            with self.subTest(start=hex(start)), self.assertRaises(PrefixError):
                self.run_case('vm_empty_f292.c', start, end, context_start=0xf200,
                              literal_tail_calls=True, leaf_argument_tails=True, canonical_functions={})

    def test_rotation_prefix_has_no_das_export(self):
        names, manifest = self.run_case('f_2f68.c', 0x2f68, 0x3c88)
        self.assertIn('_f_3a02', names)
        self.assertNotIn('_f_3c88', names)
        self.assertEqual(manifest['kept_bytes'], 3360)
        self.assertEqual(manifest['literal_dependencies'],
                         [dict(pc=0x3c4c, target=0x3d14, size=2, value='0364')])

    def test_draw_prefix_has_no_context_export(self):
        names, manifest = self.run_case('draw_15f10.c', 0x15f10, 0x15fae)
        self.assertIn('_f_15f10', names)
        self.assertNotIn('_f_15fae', names)
        self.assertEqual(manifest['kept_bytes'], 158)
        self.assertEqual(len(manifest['literal_dependencies']), 13)

    def test_changed_field_layout_fails(self):
        # This changes an earlier copy of the offset as well as the external
        # pool word, so the retained-byte check rejects it first.
        with self.assertRaisesRegex(PrefixError, 'compiled prefix differs'):
            self.run_case('f_2f68.c', 0x2f68, 0x3c88, ('char pad6;', 'char pad6[5];'))

    def test_corrupt_compiler_literal_fails(self):
        import subprocess
        real_run = subprocess.run
        def corrupt_pool(command, **kwargs):
            result = real_run(command, **kwargs)
            if command[0] == 'sh-elf-objcopy':
                output = Path(command[-1])
                data = bytearray(output.read_bytes())
                data[0x3d14 - 0x2f68] ^= 1
                output.write_bytes(data)
            return result
        with patch('c_prefix.subprocess.run', side_effect=corrupt_pool):
            with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
                self.run_case('f_2f68.c', 0x2f68, 0x3c88)

    def test_changed_retained_c_fails(self):
        with self.assertRaisesRegex(PrefixError, 'compiled prefix differs'):
            self.run_case('f_2f68.c', 0x2f68, 0x3c88, ('k &= 15;', 'k &= 7;'))

    def test_middle_ranges_export_only_selected_functions(self):
        for start, end, name, loads in [(0x1e64, 0x1ede, '_f_1e64', 16),
                                       (0x1fce, 0x2058, '_f_1fce', 0)]:
            with self.subTest(start=start):
                names, manifest = self.run_case('startup_tail.c', start, end,
                                                context_start=0x1278)
                self.assertEqual({n for n in names if n.startswith('_f_')}, {name})
                self.assertEqual(manifest['functions'], [name])
                self.assertEqual(manifest['context_start'], 0x1278)
                self.assertEqual(manifest['kept_bytes'], end-start)
                self.assertEqual(len(manifest['literal_dependencies']), loads)
                self.assertEqual(manifest['mode'], 'c-range')

    def test_middle_full_candidate_still_rejects_indirect_jumps(self):
        with self.assertRaisesRegex(PrefixError, 'indirect jump'):
            self.run_case('startup_tail.c', 0x1c8c, 0x20be, context_start=0x1278)

    def test_full_startup_range_has_three_proven_literal_tails(self):
        names, manifest = self.run_case('startup_tail.c', 0x1c8c, 0x20be,
                                       context_start=0x1278, literal_tail_calls=True)
        self.assertEqual({n for n in names if n.startswith('_f_')},
                         {'_f_1c8c', '_f_1e64', '_f_1ede', '_f_1fce', '_f_2058'})
        self.assertEqual(manifest['kept_bytes'], 1074)
        self.assertEqual([(p['jump_pc'], p['target_rom']) for p in manifest['tail_proofs']],
                         [(0x1e0e, 0x10942), (0x1fbc, 0x2e6fc), (0x20ba, 0x2e6fc)])
        self.assertTrue(all(p['load_dominates_jump'] and p['single_entry_spine']
                            for p in manifest['tail_proofs']))
        self.assertIn(dict(pc=0x20b8, target=0x2164, size=4, value='0602df7c'),
                      manifest['literal_dependencies'])

    def test_effect_prefix_has_proven_r1_tail(self):
        names, manifest = self.run_case('effect_209a4.c', 0x209a4, 0x20a9c,
                                       literal_tail_calls=True)
        self.assertEqual({n for n in names if n.startswith('_f_')},
                         {'_f_209a4', '_f_20a32'})
        self.assertEqual(manifest['kept_bytes'], 248)
        self.assertEqual([(p['jump_pc'],p['register'],p['target_rom'])
                          for p in manifest['tail_proofs']], [(0x20a90,1,0x17638)])
        self.assertIn(dict(pc=0x20a8c, target=0x20b1c, size=4, value='06016eb8'),
                      manifest['literal_dependencies'])

    def test_gravity_getter_caller_preserves_fixed_point_high_word(self):
        import build
        import subprocess
        from elftools.elf.elffile import ELFFile
        from difftest import sh2emu as emu
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            source = td / 'gravity.c'
            source.write_text((build.ROOT / 'src/field_81cc.c').read_text() +
                              '\nlong gravity_api_probe(struct field *f) { return f_837a(f) >> 16; }\n')
            obj = td / 'gravity.o'
            subprocess.run([str(build.SHCC), build.SHC_VERSION, str(obj), str(source),
                            *build.c_flags(source)], check=True)
            with obj.open('rb') as stream:
                elf = ELFFile(stream)
                text = elf.get_section_by_name('.text').data()
                symbols = {x.name: x['st_value'] for x in elf.get_section_by_name('.symtab').iter_symbols()
                           if x['st_info']['type'] == 'STT_FUNC'}
            base = 0x1000
            entry, callee = base + symbols['_gravity_api_probe'], base + symbols['_f_837a']
            # These inputs model actual ROM table values, independently of
            # the C declaration: 1G and 20G must survive a C caller's ABI.
            for value, expected in [(0x4000, 0), (0x10000, 1), (0x140000, 20)]:
                with self.subTest(value=hex(value)):
                    memory = emu._FlatMem(b'')
                    for i, byte in enumerate(text):
                        memory.w8(base + i, byte, 0)
                    cpu = emu.CPU(memory, memory.r16)
                    cpu.region = (entry, base + len(text))
                    cpu.in_region = lambda pc: False
                    cpu.on_leave = lambda pc: None
                    calls = []
                    def getter(target, ret, pc):
                        self.assertEqual(target, callee)
                        calls.append(target)
                        cpu.R[0] = value
                        return ret
                    cpu.on_call = getter
                    cpu.R[4], cpu.R[15], cpu.pr = 0x50000, 0x100000, 0x4000
                    cpu.run(entry, 100)
                    self.assertEqual(calls, [callee])
                    self.assertEqual(cpu.R[0], expected)
                    self.assertEqual(cpu.R[15], 0x100000)

    def test_real_palette_completed_prefix(self):
        exports, manifest = self.run_case('palette_29bf8.c', 0x29bf8, 0x29f94,
                                         literal_tail_calls=True)
        self.assertEqual(manifest['kept_bytes'], 924)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertIn('_f_29cc6', exports)
        self.assertNotIn('_f_29f94', exports)
        self.assertEqual(len(manifest['functions']), 11)
        self.assertEqual(len(manifest['tail_proofs']), 2)

    def test_real_vm_callbacks_and_drawing_range(self):
        exports, manifest = self.run_case('vm_empty_f292.c', 0xf4dc, 0xf574,
                                         context_start=0xf200)
        expected = {f'_f_{a:x}' for a in range(0xf4dc, 0xf518, 4)} | {'_f_f518'}
        self.assertEqual({n for n in exports if n.startswith('_f_')}, expected)
        self.assertEqual(manifest['kept_bytes'], 152)
        self.assertEqual(len(manifest['literal_dependencies']), 4)
        self.assertEqual(manifest.get('tail_proofs', []), [])
        old_exports, old = self.run_case('vm_empty_f292.c', 0xf292, 0xf29a,
                                        context_start=0xf200)
        self.assertEqual({n for n in old_exports if n.startswith('_f_')},
                         {'_f_f292', '_f_f296'})
        self.assertEqual(old['kept_bytes'], 8)

    def test_real_palette_middle_with_reviewed_task_cleanup(self):
        exports, manifest = self.run_case('palette_29bf8.c', 0x29e8c, 0x29f94,
                                         context_start=0x29bf8, literal_tail_calls=True)
        self.assertEqual({n for n in exports if n.startswith('_f_')},
                         {'_f_29e8c', '_f_29ede', '_f_29f24', '_f_29f70'})
        self.assertEqual(manifest['kept_bytes'], 264)
        self.assertEqual(manifest['literal_dependencies'], [])
        self.assertEqual([(p['jump_pc'], p['register'], p['target_rom'])
                          for p in manifest['tail_proofs']],
                         [(0x29eb6, 1, 0x247b0), (0x29ec4, 1, 0x247b0)])

    def test_real_palette_middle_requires_opt_in_and_reviewed_target(self):
        with self.assertRaisesRegex(PrefixError, 'indirect jump'):
            self.run_case('palette_29bf8.c', 0x29e8c, 0x29f94, context_start=0x29bf8)
        with self.assertRaisesRegex(PrefixError, 'noncanonical literal tail function'):
            self.run_case('palette_29bf8.c', 0x29e8c, 0x29f94, context_start=0x29bf8,
                          literal_tail_calls=True, canonical_functions={})

    def test_real_ordered_long_tail_ranges(self):
        for filename,start,end,origin,names,form,stack in [
                ('field_7004.c',0x7004,0x7264,None,{'_f_7004'},'A',32),
                ('field_7004.c',0x7004,0x7324,None,{'_f_7004','_f_7264'},'A',32),
                ('field_81cc.c',0x8430,0x8518,0x81cc,{'_f_8430'},'B',12)]:
            with self.subTest(start=start,end=end):
                exports,manifest=self.run_case(filename,start,end,context_start=origin,
                                               literal_tail_calls=True)
                self.assertEqual({n for n in exports if n.startswith('_f_')},names)
                self.assertEqual(manifest['kept_bytes'],end-start)
                self.assertEqual([(p['form'],p['stack_increment'])
                                  for p in manifest['tail_proofs']],[(form,stack)])

    def test_real_ordered_long_tails_default_rejected(self):
        for filename,start,end,origin in [('field_7004.c',0x7004,0x7324,None),
                                          ('field_81cc.c',0x8430,0x8518,0x81cc)]:
            with self.subTest(start=start), self.assertRaisesRegex(PrefixError,'indirect jump'):
                self.run_case(filename,start,end,context_start=origin)

    def test_proof_mode_preserves_previous_middle_spans(self):
        for start, end in [(0x1e64, 0x1ede), (0x1fce, 0x2058)]:
            _, old = self.run_case('startup_tail.c', start, end, context_start=0x1278)
            _, new = self.run_case('startup_tail.c', start, end, context_start=0x1278,
                                   literal_tail_calls=True)
            self.assertEqual(new['tail_proofs'], [])
            for key in ('prefix_sha256', 'functions', 'literal_dependencies'):
                self.assertEqual(old[key], new[key])

    def test_proof_mode_corrupt_discarded_tail_literal_rejected(self):
        import subprocess
        real_run = subprocess.run
        def corrupt_pool(command, **kwargs):
            result = real_run(command, **kwargs)
            if command[0] == 'sh-elf-objcopy':
                output = Path(command[-1])
                data = bytearray(output.read_bytes())
                data[0x2164 - 0x1278 + 3] ^= 1
                output.write_bytes(data)
            return result
        with patch('c_prefix.subprocess.run', side_effect=corrupt_pool):
            with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
                self.run_case('startup_tail.c', 0x1c8c, 0x20be,
                              context_start=0x1278, literal_tail_calls=True)

    def test_original_elf_tail_relocation_identity_rejected(self):
        # Compile a fresh genuine object for every mutation. Restore a prior
        # compiler-generated linked context in the test-only objcopy hook so
        # byte inequality cannot hide an incomplete provenance check.
        import build
        import io
        import struct
        import subprocess
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            source = build.ROOT / 'src/startup_tail.c'
            kwargs = dict(rom=build.fn.ROM, shcc=build.SHCC, version=build.SHC_VERSION,
                          flags=build.c_flags(source), symbols=build.read_symbols(),
                          assembler=build.AS, context_start=0x1278, literal_tail_calls=True,
                          canonical_functions=build.reviewed_tail_functions())
            baseline = td / 'baseline.o'
            compile_prefix(source, baseline, 0x1c8c, 0x20be, **kwargs)
            original_bytes = baseline.with_suffix('.context.bin').read_bytes()
            for mutation in ('duplicate', 'overlap', 'type', 'addend', 'local'):
                state = dict(object=False, bytes=False)
                real_run = subprocess.run
                def injected(command, **run_kwargs):
                    result = real_run(command, **run_kwargs)
                    if command[0] == str(build.SHCC):
                        path = Path(command[2])
                        data = bytearray(path.read_bytes())
                        elf = ELFFile(io.BytesIO(data))
                        section = elf.get_section_by_name('.rela.text')
                        self.assertEqual(section['sh_entsize'], 12)
                        records = list(section.iter_relocations())
                        index = next(i for i,r in enumerate(records) if r['r_offset'] == 0xbe8)
                        other = next(i for i,r in enumerate(records) if r['r_offset'] == 0xef0)
                        at = section['sh_offset'] + index*12
                        other_at = section['sh_offset'] + other*12
                        off, info, addend = struct.unpack_from('>IIi', data, at)
                        self.assertEqual((off, info & 255, addend), (0xbe8, 1, 0))
                        if mutation == 'duplicate':
                            data[other_at:other_at+12] = data[at:at+12]
                        elif mutation == 'overlap':
                            struct.pack_into('>IIi', data, other_at, off+2, info & ~255, 0)
                        elif mutation == 'type':
                            struct.pack_into('>IIi', data, at, off, info & ~255, 0)
                        elif mutation == 'addend':
                            struct.pack_into('>IIi', data, at, off, info, 1)
                        else:
                            symbols = elf.get_section(section['sh_link'])
                            local = next(i for i,s in enumerate(symbols.iter_symbols())
                                         if s.name == '_f_20be')
                            self.assertNotEqual(symbols.get_symbol(local)['st_shndx'], 'SHN_UNDEF')
                            struct.pack_into('>IIi', data, at, off, (local << 8) | 1, 0)
                        path.write_bytes(data)
                        state['object'] = True
                    if command[0] == 'sh-elf-objcopy':
                        Path(command[-1]).write_bytes(original_bytes)
                        state['bytes'] = True
                    return result
                output = td / f'{mutation}.o'
                with self.subTest(mutation=mutation), patch('c_prefix.subprocess.run', side_effect=injected):
                    with self.assertRaisesRegex(PrefixError, 'literal tail (relocation|function)'):
                        compile_prefix(source, output, 0x1c8c, 0x20be, **kwargs)
                    self.assertEqual(state, dict(object=True, bytes=True))
                    self.assertFalse(output.exists())
                    self.assertFalse(output.with_suffix('.prefix.json').exists())

    def test_middle_changed_retained_c_rejected(self):
        with self.assertRaisesRegex(PrefixError, 'compiled prefix differs'):
            self.run_case('startup_tail.c', 0x1fce, 0x2058,
                          ('f->w30c &= 0xef78;', 'f->w30c &= 0xef70;'),
                          context_start=0x1278)

    def test_middle_corrupt_external_literal_rejected(self):
        import subprocess
        real_run = subprocess.run
        def corrupt_pool(command, **kwargs):
            result = real_run(command, **kwargs)
            if command[0] == 'sh-elf-objcopy':
                output = Path(command[-1])
                data = bytearray(output.read_bytes())
                data[0x1f2a - 0x1278] ^= 1
                output.write_bytes(data)
            return result
        with patch('c_prefix.subprocess.run', side_effect=corrupt_pool):
            with self.assertRaisesRegex(PrefixError, 'discarded literal differs'):
                self.run_case('startup_tail.c', 0x1e64, 0x1ede, context_start=0x1278)


if __name__ == '__main__':
    unittest.main()
