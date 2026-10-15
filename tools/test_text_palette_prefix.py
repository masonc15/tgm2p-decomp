"""Bounded two-call palette wrapper and adversarial tail-proof controls."""
import json,os,tempfile,unittest
from pathlib import Path
from c_prefix import validate_prefix,compile_prefix,PrefixError
START=0x1000
def fixture():
    values=[0x4f22,0xe501,0xd307,0xd606,0x430b,0xe451,0xd604,0xe501,0xd204,0xe452,0x422b,0x4f26,
            0x0603,0x076c,0x0603,0x3014,0x0006,0x7910,0x0600,0x1880]
    return b''.join(v.to_bytes(2,'big') for v in values)
class PairedCallTailTests(unittest.TestCase):
    def check(self,code=None,**kw):
        code=fixture() if code is None else code
        provenance=dict(symbol='_f_2000',relocation_type='R_SH_DIR32',addend=0,embedded_addend=0,
                        binding='STB_GLOBAL',section='SHN_UNDEF',relocation_count=1,
                        value=0x06001880)
        provenance.update(kw.pop('provenance',{}));proofs=[]
        args=dict(context_start=START,tail_provenance={START+36:provenance},
                  canonical_functions={'_f_2000':0x2000},tail_proofs=proofs)
        args.update(kw)
        rom=bytes(START)+code
        validate_prefix(code,rom,START,START+len(code),args.pop('entries',{START:'_f_1000'}),**args)
        return proofs
    def test_full_wrapper_and_shared_pool(self):
        p=self.check()[0]
        self.assertEqual(p['proof_kind'],'paired_call_argument_tail')
        self.assertEqual(p['first_call_literal'],START+36)
        self.assertEqual(p['stack_proof']['final_displacement'],0)
        self.assertEqual(p['delay_slots'],[START+10,START+22])
    def test_requires_explicit_tail_provenance(self):
        with self.assertRaises(PrefixError):self.check(tail_provenance=None,canonical_functions=None)
    def test_rejects_unsafe_instruction_mutations_even_when_rom_agrees(self):
        for off,word in [(0,0x0009),(2,0x7ffc),(2,0xe801),(4,0xd306),
                         (8,0x440b),(10,0x2f46),(12,0xd603),(18,0xe252),(22,0x6ef6)]:
            with self.subTest(off=off,word=word):
                c=bytearray(fixture());c[off:off+2]=word.to_bytes(2,'big')
                with self.assertRaises(PrefixError):self.check(bytes(c))
    def test_argument_destination_and_stack_control_faults(self):
        cases=[(18,0xe052 | (r<<8)) for r in range(16) if r!=4]
        cases += [(off,op) for off in (0,2,4,6,8,10,12,14,16,18,20,22)
                  for op in (0x7ffc,0x7f04,0x4f22,0x4f26,0x2fe6,0x68f6,0x402a,0xa000,0x8900,0x400b)
                  if not (off==0 and op==0x4f22) and not (off==22 and op==0x4f26)]
        for off,op in cases:
            with self.subTest(off=off,op=op):
                c=bytearray(fixture());c[off:off+2]=op.to_bytes(2,'big')
                if off==18 and op in (0x7f04,0x4f26,0x68f6):
                    # Existing short-restore tails already admit these restores.
                    # They must never acquire the new complete-wrapper ABI proof.
                    self.assertNotEqual(self.check(bytes(c))[0].get('proof_kind'),
                                        'paired_call_argument_tail')
                else:
                    with self.assertRaises(PrefixError):self.check(bytes(c))
    def test_signed_immediate_edges_preserve_shape(self):
        for value in (0,127,128,255):
            c=bytearray(fixture());c[19]=value
            self.assertEqual(self.check(bytes(c))[0]['proof_kind'],'paired_call_argument_tail')
    def test_rejects_entry_into_spine_or_delay(self):
        for off in [8,10,16,18,20,22]:
            with self.subTest(off=off):
                with self.assertRaises(PrefixError):
                    self.check(entries={START:'_f_1000',START+off:f'_f_{START+off:x}'})
    def test_rejects_untrusted_target_provenance(self):
        for patch in [dict(addend=1),dict(embedded_addend=1),dict(binding='STB_LOCAL'),
                      dict(section=1),dict(relocation_count=2),dict(overlap=True),
                      dict(value=0),dict(symbol='_f_2002'),dict(embedded_addend=None),dict(relocation_type='R_SH_REL32')]:
            with self.subTest(patch=patch):
                with self.assertRaises(PrefixError):self.check(provenance=patch)
        with self.assertRaises(PrefixError):self.check(canonical_functions={})
@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS')=='1','real compiler tests opt in')
class NativeTextPaletteTests(unittest.TestCase):
    def compile(self,source,out):
        import build
        return compile_prefix(source,out,0xe3b8,0xe3e0,rom=build.fn.ROM,shcc=build.SHCC,
            version=build.SHC_VERSION,flags=build.c_flags(source),symbols=build.read_symbols(),
            assembler=build.AS,context_start=0xe074,literal_tail_calls=True,
            canonical_functions=build.reviewed_tail_functions())
    def test_full40_and_excluded_context(self):
        import build
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/text_palette_e3b8.c',out)
            raw=out.with_suffix('.context.bin').read_bytes()
            self.assertEqual(len(raw),876)
            self.assertNotEqual(raw[:0xe3b8-0xe074],build.fn.ROM[0xe074:0xe3b8])
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0xe3b8:0xe3e0])
            p=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(p['functions'],['_f_e3b8'])
            self.assertEqual(p['kept_bytes'],40)
            self.assertEqual(p['literal_dependencies'],[])
            self.assertEqual(p['tail_proofs'][0]['proof_kind'],'paired_call_argument_tail')
            self.assertEqual(p['tail_proofs'][0]['target_rom'],0x2af5e)
    def test_missing_final_pointer_use_rejects_without_publishing(self):
        import build
        s=(build.ROOT/'src/text_palette_e3b8.c').read_text()
        self.assertEqual(s.count('if ((*buttons & 14)'),1)
        s=s.replace('if ((*buttons & 14)','if ((g_6079304 & 14)')
        with tempfile.TemporaryDirectory() as td:
            source=Path(td)/'negative.c';source.write_text(s);out=Path(td)/'kept.o'
            with self.assertRaises(PrefixError):self.compile(source,out)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
