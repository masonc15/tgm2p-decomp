"""Synthetic paired code/oracle mutations isolate control-flow and ABI checks."""
import unittest
from c_prefix import PrefixError, validate_prefix

START, STOP, POOL, OFFSET, TARGET = 0x1000, 0x100e, 0x1040, 0x1044, 0x2000
OPS = [0xe320, 0xd10f, 0x901e, 0x024e, 0x223b, 0x412b, 0x0426]

class FieldFlagTailTests(unittest.TestCase):
    def check(self, *, enabled=True, ops=OPS, offset=0x308, facts_change=None,
              entries_change=None, suffix=None, linked_change=None, proofs=None, reverse=False, end_override=None, extra_provenance=None):
        rom=bytearray(0x3000)
        for i,w in enumerate(ops): rom[START+i*2:START+i*2+2]=w.to_bytes(2,'big')
        rom[POOL:POOL+4]=(TARGET+0x5fff880).to_bytes(4,'big')
        rom[OFFSET:OFFSET+2]=offset.to_bytes(2,'big')
        entries={START:'_f_1000',STOP:'_f_100e'};end=STOP
        if entries_change: entries.update(entries_change)
        if suffix:
            for pc,w in suffix.items():rom[pc:pc+2]=w.to_bytes(2,'big')
            end=end_override or 0x1020;entries[end]=f'_f_{end:x}'
        facts=dict(symbol='_f_2000',relocation_type='R_SH_DIR32',addend=0,
                   binding='STB_GLOBAL',section='SHN_UNDEF',value=TARGET+0x5fff880,
                   relocation_count=1,overlap=False,embedded_addend=0)
        facts.update(facts_change or {})
        raw=bytearray(rom[START:0x1060])
        if linked_change:
            pc,value=linked_change;raw[pc-START]=value
        out=[] if proofs is None else proofs
        provenance={POOL:facts};provenance.update(extra_provenance or {})
        kw=dict(context_start=START,tail_provenance=provenance,
                canonical_functions={'_f_2000':TARGET},tail_proofs=out)
        if enabled:kw['field_flag_tails']=True
        validate_prefix(raw,rom,START,end,dict(reversed(list(entries.items()))) if reverse else entries,**kw)
        return out

    def test_exact_leaf_proves_store_then_same_argument_tail(self):
        try: result=self.check()
        except (PrefixError,TypeError) as e:self.fail(f'field-flag proof unavailable: {e}')
        proof,=result
        self.assertEqual(proof['proof_kind'],'field_flag_tail')
        self.assertEqual(proof['field_write'],dict(base_register=4,offset=0x308,size=4,operation='or',mask=0x20,pc=0x100c))
        self.assertEqual(proof['target_rom'],TARGET)
        self.assertEqual(proof['unchanged_arguments'],[4,5,6,7])

    def test_default_rejects_store_delay(self):
        with self.assertRaises(PrefixError):self.check(enabled=False)

    def test_mutated_registers_operations_and_delay_reject(self):
        for i,words in {0:[0xe321,0xe420,0x2fe6],1:[0xd20f],2:[0x911e],
                        3:[0x024d,0x025e,0x021e],4:[0x223a,0x243b],
                        5:[0x422b],6:[0x0009,0x0425,0x0436,0x0526,0x4f26]}.items():
            for word in words:
                ops=OPS.copy();ops[i]=word
                with self.subTest(i=i,word=hex(word)),self.assertRaises(PrefixError):self.check(ops=ops)

    def test_return_does_not_fabricate_tail_proof(self):
        ops=OPS.copy();ops[5]=0x000b
        self.assertEqual(self.check(ops=ops),[])

    def test_wrong_field_offset_rejects(self):
        for n in [0,4,0x30a,0xffff]:
            with self.subTest(offset=n),self.assertRaises(PrefixError):self.check(offset=n)

    def test_all_alternate_entries_reject(self):
        for pc in range(START+2,STOP,2):
            with self.subTest(pc=pc),self.assertRaises(PrefixError):self.check(entries_change={pc:f'_f_{pc:x}'})

    def test_late_foreign_branch_into_leaf_rejects(self):
        # second root is visited first by default; reverse insertion tests the late-edge case.
        for target in [START,START+2,START+8,START+12]:
            op=0xa000|(((target-(STOP+4))//2)&0xfff)
            for reverse in [False,True]:
                with self.subTest(target=target,reverse=reverse),self.assertRaises(PrefixError):
                    self.check(suffix={STOP:op,STOP+2:0x0009},reverse=reverse)

    def test_offset_literal_may_not_be_relocated_or_executed(self):
        with self.assertRaises(PrefixError):self.check(extra_provenance={OFFSET:{}})
        with self.assertRaises(PrefixError):
            self.check(entries_change={OFFSET:'_f_1044'},end_override=0x1060,
                       suffix={STOP:0x000b,STOP+2:0x0009,OFFSET+2:0x000b,OFFSET+4:0x0009})

    def test_original_provenance_and_separate_mode_required(self):
        rom=bytes(0x3000)
        with self.assertRaises(PrefixError):
            validate_prefix(rom[START:STOP],rom,START,STOP,{START:'_f_1000',STOP:'_f_100e'},
                            field_flag_tails=True)

    def test_changed_pool_or_relocation_rejects_without_partial_proof(self):
        for change in [dict(symbol='_f_2002'),dict(binding='STB_LOCAL'),dict(section='.text'),
                       dict(addend=4),dict(embedded_addend=4),dict(relocation_type='R_SH_REL32'),
                       dict(relocation_count=2),dict(overlap=True),dict(value=TARGET)]:
            proofs=[]
            with self.subTest(change=change),self.assertRaises(PrefixError):self.check(facts_change=change,proofs=proofs)
            self.assertEqual(proofs,[])
        for pc in [POOL,OFFSET,START]:
            with self.subTest(pc=pc),self.assertRaises(PrefixError):self.check(linked_change=(pc,255))

class FieldFlagBuildTests(unittest.TestCase):
    def test_build_requires_explicit_marker_and_literal_mode(self):
        import build
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as d:
            p=Path(d)/'case.c'
            p.write_text('/* ordinary */\n')
            self.assertTrue(hasattr(build,'c_field_flag_tails'),'build flag parser is missing')
            self.assertFalse(build.c_field_flag_tails(p))
            p.write_text('/* field-flag-tails: 1 literal-tail-calls: 1 */\n')
            self.assertTrue(build.c_field_flag_tails(p))
            for header in ['field-flag-tails: 1','field-flag-tails: 0 literal-tail-calls: 1',
                           'field-flag-tails: 1 field-flag-tails: 1 literal-tail-calls: 1',
                           'field-flag-tails: 1 literal-tail-calls: 1 leaf-argument-tails: 1']:
                p.write_text('/* '+header+' */\n')
                with self.subTest(header=header),self.assertRaises(ValueError):build.c_field_flag_tails(p)

import os
@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS')=='1','real compiler tests opt in')
class NativeFieldFlagTests(unittest.TestCase):
    def test_real_compiler_leaf_and_default_rejection(self):
        import build, tempfile, json
        from pathlib import Path
        from c_prefix import compile_prefix
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as d:
            out=Path(d)/'leaf.o'
            kw=dict(rom=build.fn.ROM,shcc=build.SHCC,version=build.SHC_VERSION,
                    flags=['-macsave=1','-optimize=1','-speed'],symbols=build.read_symbols(),
                    assembler=build.AS,context_start=0x7fa0,literal_tail_calls=True,
                    canonical_functions=build.reviewed_tail_functions())
            src=build.ROOT/'src/item_8164.c'
            with self.assertRaises(PrefixError):compile_prefix(src,out,0x8156,0x8164,**kw)
            self.assertFalse(out.exists())
            compile_prefix(src,out,0x8156,0x8164,field_flag_tails=True,**kw)
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),bytes.fromhex('e320d1189020024e223b412b0426'))
            self.assertEqual(proof['functions'],['_f_8156'])
            self.assertEqual(proof['tail_proofs'][0]['target_rom'],0x21d94)
            self.assertEqual(proof['tail_proofs'][0]['offset_literal'],0x819e)
            with out.open('rb') as f:
                elf=ELFFile(f)
                self.assertEqual(elf.get_section_by_name('.text').data(),build.fn.ROM[0x8156:0x8164])
                self.assertFalse(any(sec['sh_type'] in ['SHT_REL','SHT_RELA'] for sec in elf.iter_sections()))

if __name__=='__main__':unittest.main()

