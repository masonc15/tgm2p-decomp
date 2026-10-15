"""Paired synthetic/oracle mutations test the exact reset-wrapper ABI proof."""
import json
import os
from pathlib import Path
import tempfile
import unittest
from c_prefix import PrefixError, validate_prefix

START=0x1002
ROOT=START+52
END=ROOT+74
TARGET=0x2000
POOL=START+0x6e
OFFSET=START+0x64
MASK=START+0x7a
RESET=[0xe500,0x962d,0xe704,0x6053,0x364c,0x2650,0x7002,0x7601,
       0x630f,0x2650,0x3373,0x8ff8,0x7601,0x9622,0x6053,0x364c,
       0x7002,0x2651,0x630f,0x7602,0x3373,0x2651,0x8ff8,0x7602,0x000b,0x0009]
WRAPPER=[0x2fe6,0x4f22,0xbfe2,0x6e43,0x64e3,0xd10b,0x9010,
         0xd30d,0x02ee,0x223b,0x0e26,0x4f26,0x412b,0x6ef6]

class StartupResetTailTests(unittest.TestCase):
    def check(self, *, enabled=True, changes=None, entries_extra=None, extra_facts=None,
              facts_change=None, reverse=False, start=START, end=END, proofs=None,
              literal_mode=True, mixed=None, linked_change=None):
        rom=bytearray(0x3000)
        for base,words in [(START,RESET),(ROOT,WRAPPER)]:
            for i,w in enumerate(words):rom[base+i*2:base+i*2+2]=w.to_bytes(2,'big')
        for a,w in [(START+0x60,0x2fc),(START+0x62,0x300),(OFFSET,0x308)]:rom[a:a+2]=w.to_bytes(2,'big')
        rom[POOL:POOL+4]=(TARGET+0x5fff880).to_bytes(4,'big')
        rom[MASK:MASK+4]=(0x58000).to_bytes(4,'big')
        for a,w in (changes or {}).items():rom[a:a+2]=w.to_bytes(2,'big')
        entries={START:f'_f_{START:x}',ROOT:f'_f_{ROOT:x}',END:f'_f_{END:x}'}
        entries.update(entries_extra or {})
        facts=dict(symbol='_f_2000',relocation_type='R_SH_DIR32',addend=0,
                   binding='STB_GLOBAL',section='SHN_UNDEF',value=TARGET+0x5fff880,
                   relocation_count=1,overlap=False,embedded_addend=0)
        facts.update(facts_change or {})
        provenance={POOL:facts};provenance.update(extra_facts or {})
        data=bytearray(rom[START:max(end,END)+8])
        if linked_change:
            pc,value=linked_change;data[pc-START]=value
        out=[] if proofs is None else proofs
        kw=dict(context_start=START,tail_proofs=out)
        if enabled:kw['startup_reset_tails']=True
        if literal_mode:kw.update(tail_provenance=provenance,canonical_functions={'_f_2000':TARGET})
        kw.update(mixed or {})
        validate_prefix(data,rom,start,end,dict(reversed(list(entries.items()))) if reverse else entries,**kw)
        return out

    def test_exact_pair_proves_reset_call_flag_store_and_balanced_tail(self):
        proof,=self.check()
        self.assertEqual(proof['proof_kind'],'startup_reset_tail')
        self.assertEqual(proof['reset_call'],dict(pc=ROOT+4,target=START,delay_pc=ROOT+6))
        self.assertEqual(proof['field_write'],dict(base_register=14,offset=0x308,size=4,operation='or',mask=0x58000,pc=ROOT+20))
        self.assertEqual(proof['stack_proof']['final_displacement'],0)
        self.assertEqual(proof['target_rom'],TARGET)
        self.assertEqual(proof['argument_proof'],{'r4':'incoming_field'})
        self.assertEqual(proof['preserved_registers'],list(range(8,16)))

    def test_explicit_mode_and_provenance_required(self):
        for kw in [dict(enabled=False),dict(literal_mode=False),dict(mixed={'field_flag_tails':True}),
                   dict(mixed={'leaf_argument_tails':True}),dict(mixed={'guarded_switches':True}),
                   dict(mixed={'local_loop_jumps':True})]:
            with self.subTest(kw=kw),self.assertRaises(PrefixError):self.check(**kw)

    def test_helper_must_be_complete_and_retained(self):
        with self.assertRaises(PrefixError):self.check(start=ROOT)
        for i in range(len(RESET)):
            with self.subTest(i=i),self.assertRaises(PrefixError):self.check(changes={START+2*i:0x0009 if RESET[i]!=0x0009 else 0x0008})

    def test_frame_call_argument_store_and_restore_mutations_reject(self):
        for i in range(len(WRAPPER)):
            if i==12:continue  # removing the tail is checked separately
            with self.subTest(i=i),self.assertRaises(PrefixError):self.check(changes={ROOT+2*i:0x0009})
        for word in [0xbfe1,0xbfe3,0xb000]:
            with self.subTest(call=word),self.assertRaises(PrefixError):self.check(changes={ROOT+4:word})
        for word in [0x0e25,0x0e36,0x0126,0x6ef6]:
            with self.subTest(store=word),self.assertRaises(PrefixError):self.check(changes={ROOT+20:word})
        self.assertEqual(self.check(changes={ROOT+24:0x000b}),[])
        # Replacing the call with a branch removes the continuation/tail path.
        # Ordinary unreachable bytes must not fabricate a specialized proof.
        self.assertEqual(self.check(changes={ROOT+4:0xafe2}),[])

    def test_wrong_constants_and_relocated_data_reject(self):
        for pc in [START+0x60,START+0x62,OFFSET,MASK,MASK+2]:
            with self.subTest(pc=pc),self.assertRaises(PrefixError):self.check(changes={pc:0xffff})
            with self.subTest(relocated=pc),self.assertRaises(PrefixError):self.check(extra_facts={pc:{}})
        for pc in [OFFSET-2,MASK-2]:
            with self.subTest(overlap=pc),self.assertRaises(PrefixError):self.check(extra_facts={pc:{}})

    def test_interior_entries_and_delay_ownership_reject(self):
        for base,words in [(START,RESET),(ROOT,WRAPPER)]:
            for pc in range(base+2,base+2*len(words),2):
                with self.subTest(pc=pc),self.assertRaises(PrefixError):self.check(entries_extra={pc:f'_f_{pc:x}'})

    def test_late_edges_and_executed_literals_reject(self):
        for target in [START,START+10,ROOT,ROOT+10,ROOT+20,ROOT+26,OFFSET,POOL,MASK]:
            op=0xa000|(((target-(END+4))//2)&0xfff)
            for reverse in [False,True]:
                with self.subTest(target=target,reverse=reverse),self.assertRaises(PrefixError):
                    self.check(changes={END:op,END+2:0x0009},end=END+4,
                               entries_extra={END+4:f'_f_{END+4:x}'},reverse=reverse)

    def test_target_relocation_and_linked_bytes_reject_atomically(self):
        for change in [dict(symbol='_f_2002'),dict(binding='STB_LOCAL'),dict(section='.text'),
                       dict(addend=4),dict(embedded_addend=4),dict(embedded_addend=None),
                       dict(relocation_type='R_SH_REL32'),dict(relocation_count=2),dict(overlap=True),dict(value=TARGET)]:
            proofs=[]
            with self.subTest(change=change),self.assertRaises(PrefixError):self.check(facts_change=change,proofs=proofs)
            self.assertEqual(proofs,[])
        for pc in [START,ROOT,POOL,OFFSET,MASK]:
            with self.subTest(pc=pc),self.assertRaises(PrefixError):self.check(linked_change=(pc,255))

class StartupResetBuildTests(unittest.TestCase):
    def test_explicit_exclusive_marker(self):
        import build
        with tempfile.TemporaryDirectory() as d:
            p=Path(d)/'case.c';p.write_text('/* ordinary */\n')
            self.assertTrue(hasattr(build,'c_startup_reset_tails'))
            self.assertFalse(build.c_startup_reset_tails(p))
            p.write_text('/* startup-reset-tails: 1 literal-tail-calls: 1 */\n')
            self.assertTrue(build.c_startup_reset_tails(p))
            for header in ['startup-reset-tails: 1','startup-reset-tails: 0 literal-tail-calls: 1',
                           'startup-reset-tails: 1 startup-reset-tails: 1 literal-tail-calls: 1',
                           'startup-reset-tails: 1 literal-tail-calls: 1 leaf-argument-tails: 1',
                           'startup-reset-tails: 1 literal-tail-calls: 1 field-flag-tails: 1']:
                p.write_text('/* '+header+' */\n')
                with self.subTest(header=header),self.assertRaises(ValueError):build.c_startup_reset_tails(p)

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS')=='1','real compiler tests opt in')
class NativeStartupResetTests(unittest.TestCase):
    def test_exact_native_pair_requires_own_mode_and_reviewed_target(self):
        import build
        from c_prefix import compile_prefix
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as d:
            out=Path(d)/'kept.o';src=build.ROOT/'src/startup_reset_1896.c'
            kw=dict(rom=build.fn.ROM,shcc=build.SHCC,version=build.SHC_VERSION,
                    flags=build.c_flags(src),symbols=build.read_symbols(),assembler=build.AS,
                    context_start=0x1278,literal_tail_calls=True,canonical_functions=build.reviewed_tail_functions())
            with self.assertRaises(PrefixError):compile_prefix(src,out,0x1896,0x1914,**kw)
            self.assertFalse(out.exists())
            compile_prefix(src,out,0x1896,0x1914,startup_reset_tails=True,**kw)
            manifest=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(manifest['functions'],['_f_1896','_f_18ca'])
            self.assertEqual(manifest['literal_dependencies'],[])
            self.assertEqual(manifest['tail_proofs'][0]['target_rom'],0x1175c)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x1896:0x1914])
            with out.open('rb') as f:
                e=ELFFile(f);self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') for s in e.iter_sections()))
            kw['canonical_functions']={k:v for k,v in kw['canonical_functions'].items() if k!='_f_1175c'}
            with self.assertRaises(PrefixError):compile_prefix(src,out,0x1896,0x1914,startup_reset_tails=True,**kw)
            self.assertFalse(out.exists())
            self.assertFalse(out.with_suffix('.prefix.json').exists())
