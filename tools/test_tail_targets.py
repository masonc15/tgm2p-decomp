"""Tail targets require reviewed entry evidence, never discovery heuristics."""
import hashlib
import importlib
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch


class TailTargetTests(unittest.TestCase):
    def fixture(self):
        rom = bytes(range(256)) * 48
        entry = dict(symbol='_f_2000', start=0x2000, end=0x2020,
                     sha256=hashlib.sha256(rom[0x2000:0x2020]).hexdigest(),
                     evidence='Independently reviewed function frame and callers.')
        return rom, dict(rom_sha256=hashlib.sha256(rom).hexdigest(), entries=[entry])

    def load(self, rom, registry):
        module = importlib.import_module('tail_targets')
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'targets.json'
            p.write_text(json.dumps(registry))
            return module.load_tail_targets(p, rom)

    def reject(self, rom, registry):
        module = importlib.import_module('tail_targets')
        with self.assertRaises(module.TailTargetError):
            self.load(rom, registry)

    def test_reviewed_entry_loads(self):
        rom, registry = self.fixture()
        self.assertEqual(self.load(rom, registry), {'_f_2000': 0x2000})

    def test_unreviewed_pool_seed_is_not_a_build_tail_target(self):
        import build
        import c_prefix
        class StopBuild(Exception):
            pass
        seen = {}
        def capture(*args, **kwargs):
            seen.update(kwargs['canonical_functions'])
            raise StopBuild
        with tempfile.TemporaryDirectory() as d:
            with patch.object(build, 'OBJ', Path(d)), \
                 patch.object(build, 'read_splits', return_value=[(0x1000, 0x1040, 'c-prefix', 'src/startup_tail.c')]), \
                 patch.object(build, 'read_symbols', return_value={}), \
                 patch.object(build, 'c_flags', return_value=[]), \
                 patch.object(build, 'c_literal_tail_calls', return_value=True), \
                 patch.object(build.fn, 'call_targets', return_value=[0x64b0]), \
                 patch.object(c_prefix, 'compile_prefix', side_effect=capture):
                with self.assertRaises(StopBuild):
                    build.build()
        self.assertNotIn('_f_64b0', seen)
        self.assertEqual(set(seen), {'_f_31dc', '_f_7fa0', '_f_10942', '_f_12fa8', '_f_17638', '_f_247b0',
                                    '_f_29c50', '_f_2af5e', '_f_2c6fc', '_f_2e6fc', '_f_3036e',
                                    '_f_20250', '_f_21d94', '_f_1175c', '_f_2b4e8'})

    def test_pool_seed_rejected_by_structural_tail_proof(self):
        import build
        from c_prefix import PrefixError, validate_prefix
        # Synthetic compiler/oracle agreement isolates caller-input trust.
        # This is not real SHC output and earns no matching credit.
        start, end, pool, target = 0x1000, 0x1006, 0x1008, 0x64b0
        oracle = bytearray(0x7000)
        compiled = bytes.fromhex('d301432b6ef60009') + (target + 0x5fff880).to_bytes(4, 'big')
        oracle[start:start + len(compiled)] = compiled
        facts = {pool: dict(symbol='_f_64b0', relocation_type='R_SH_DIR32',
                            addend=0, binding='STB_GLOBAL', section='SHN_UNDEF',
                            value=target + 0x5fff880)}
        args = (compiled, oracle, start, end, {start: '_f_1000', end: '_f_1006'})
        validate_prefix(*args, context_start=start, tail_provenance=facts,
                        canonical_functions={'_f_64b0': target})
        with self.assertRaisesRegex(PrefixError, 'noncanonical literal tail function'):
            validate_prefix(*args, context_start=start, tail_provenance=facts,
                            canonical_functions=build.reviewed_tail_functions())
        self.assertEqual(build.fn.ROM[target:target + 4].hex(), '060027e8')

    def test_changed_rom_rejected(self):
        rom, registry = self.fixture()
        self.reject(bytes([rom[0] ^ 1]) + rom[1:], registry)

    def test_changed_body_with_updated_whole_hash_rejected(self):
        rom, registry = self.fixture()
        rom = bytearray(rom)
        rom[0x2000] ^= 1
        registry['rom_sha256'] = hashlib.sha256(rom).hexdigest()
        self.reject(rom, registry)

    def test_symbol_identity_and_address_shape(self):
        rom, original = self.fixture()
        for key, values in {
            'symbol': ['_f_2002', '_f_02000', '_alias', '_F_2000', '_f_2000junk', 42],
            'start': [True, '8192', -2, 0x2001, len(rom)],
            'end': [True, '8224', 0x2000, 0x2001, len(rom) + 2],
            'sha256': ['0' * 64, 'bad', 42],
            'evidence': ['', ' ', [], None],
        }.items():
            for value in values:
                registry = json.loads(json.dumps(original))
                registry['entries'][0][key] = value
                with self.subTest(key=key, value=value):
                    self.reject(rom, registry)

    def test_reviewed_spans_stay_in_copied_code(self):
        rom = bytes(range(256)) * 1024
        for start, end in [(0x400, 0x420), (0x77e, 0x782),
                           (0x313fc, 0x31400), (0x313fa, 0x313fe)]:
            registry = dict(rom_sha256=hashlib.sha256(rom).hexdigest(), entries=[
                dict(symbol=f'_f_{start:x}', start=start, end=end,
                     sha256=hashlib.sha256(rom[start:end]).hexdigest(),
                     evidence='Synthetic boundary fixture only.')])
            with self.subTest(start=hex(start), end=hex(end)):
                self.reject(rom, registry)
        for start, end in [(0x780, 0x782), (0x313fa, 0x313fc)]:
            registry = dict(rom_sha256=hashlib.sha256(rom).hexdigest(), entries=[
                dict(symbol=f'_f_{start:x}', start=start, end=end,
                     sha256=hashlib.sha256(rom[start:end]).hexdigest(),
                     evidence='Synthetic boundary fixture only.')])
            self.assertEqual(self.load(rom, registry), {f'_f_{start:x}': start})

    def test_duplicate_entries_and_schema_errors(self):
        rom, registry = self.fixture()
        registry['entries'].append(dict(registry['entries'][0]))
        self.reject(rom, registry)
        for registry in [[], {}, {'rom_sha256': '0' * 64, 'entries': []},
                         dict(self.fixture()[1], entries=[]),
                         dict(self.fixture()[1], extra='typo')]:
            self.reject(rom, registry)
        registry = self.fixture()[1]
        registry['entries'][0]['extra'] = 'typo'
        self.reject(rom, registry)

    def test_duplicate_json_keys_rejected(self):
        module = importlib.import_module('tail_targets')
        rom, registry = self.fixture()
        text = json.dumps(registry)
        text = text.replace('"start": 8192', '"start": 8192, "start": 8192')
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'targets.json'
            p.write_text(text)
            with self.assertRaises(module.TailTargetError):
                module.load_tail_targets(p, rom)

    def test_missing_or_malformed_registry_rejected(self):
        module = importlib.import_module('tail_targets')
        rom, _ = self.fixture()
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'missing.json'
            with self.assertRaises(module.TailTargetError):
                module.load_tail_targets(p, rom)
            p.write_text('{')
            with self.assertRaises(module.TailTargetError):
                module.load_tail_targets(p, rom)

    def test_real_registry_all_current_targets(self):
        import build
        module = importlib.import_module('tail_targets')
        result = module.load_tail_targets(build.ROOT / 'tail_targets.json', build.fn.ROM)
        self.assertEqual(result, {f'_f_{a:x}': a for a in [0x31dc, 0x7fa0, 0x10942, 0x1175c, 0x12fa8, 0x17638,
                                                        0x20250, 0x21d94, 0x247b0, 0x29c50, 0x2af5e, 0x2b4e8,
                                                        0x2c6fc, 0x2e6fc, 0x3036e]})


if __name__ == '__main__':
    unittest.main()
