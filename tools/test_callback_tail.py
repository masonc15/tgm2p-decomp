"""Adversarial coverage for complete dynamic callback-tail ownership."""
from dataclasses import replace
import json, os, tempfile, unittest
from pathlib import Path
from c_prefix import PrefixError, validate_prefix
from switch_proof import OriginalText

class CallbackProofTests(unittest.TestCase):
    def setUp(self):
        import build
        self.start,self.end=0x2769e,0x276e8
        self.raw=bytearray(build.fn.ROM[self.start:self.end]);self.rom=bytearray(build.fn.ROM)
        self.entries={self.start:'_f_2769e'}
        original=bytearray(self.raw);original[70:74]=bytes(4)
        self.original=OriginalText(self.start,bytes(original),self.entries,((self.start+70,4),),'fixture')
        self.provenance={self.start+70:dict(symbol='_g_60ad6a0',relocation_type='R_SH_DIR32',
            addend=0,embedded_addend=0,binding='STB_GLOBAL',section='SHN_UNDEF',
            relocation_count=1,overlap=False,value=0x60ad6a0)}
    def validate(self,**kwargs):
        opts=dict(context_start=self.start,callback_tails=True,callback_original=self.original,
                  callback_provenance=self.provenance);opts.update(kwargs)
        return validate_prefix(bytes(self.raw),bytes(self.rom),self.start,self.end,self.entries,**opts)
    def alter(self,offset,value):
        # Mutate oracle AND original too: structural rejects cannot rely on equality.
        b=bytes.fromhex(value);self.raw[offset:offset+len(b)]=b
        self.rom[self.start+offset:self.start+offset+len(b)]=b
        d=bytearray(self.original.data);d[offset:offset+len(b)]=b
        self.original=replace(self.original,data=bytes(d))
    def test_complete_dispatcher(self):
        p=[];self.assertEqual(self.validate(callback_proofs=p),[]);self.assertEqual(len(p),1)
        self.assertEqual(p[0]['target']['kind'],'runtime_function_pointer')
        self.assertEqual(p[0]['target']['offset'],52)
        self.assertEqual(p[0]['stack_proof']['final_displacement'],0)
        self.assertEqual(p[0]['argument_proof'],{'r4':'selected_record'})
    def test_default_and_mixed_modes_reject(self):
        with self.assertRaises(PrefixError):self.validate(callback_tails=False)
        for mode in ('local_loop_jumps','guarded_switches','leaf_argument_tails','field_flag_tails',
                     'startup_reset_tails','function_code_range'):
            with self.subTest(mode=mode),self.assertRaises(PrefixError):self.validate(**{mode:True})
        with self.assertRaises(PrefixError):self.validate(tail_provenance={})
    def test_raw_difference_still_rejects(self):
        self.raw[36]^=1
        with self.assertRaises(PrefixError):self.validate()
    def test_mutated_control_stack_target_argument_and_store_effects(self):
        cases={0:'2fd6',4:'4f22',8:'24ee',14:'6eed',18:'52ed',22:'8906',26:'430b',28:'65e3',
               30:'e301',32:'a008',34:'1e3d',36:'52ed',40:'8b05',42:'52ec',44:'64d3',46:'4f26',
               48:'4f16',50:'432b',52:'6df6',54:'0009',58:'002b',60:'6df6'}
        for offset,value in cases.items():
            with self.subTest(offset=offset):
                self.setUp();self.alter(offset,value)
                with self.assertRaises(PrefixError):self.validate()
    def test_literal_load_ownership(self):
        for offset,value in ((6,'9e00'),(6,'9eff'),(10,'d300'),(10,'d3ff')):
            with self.subTest(offset=offset,value=value):
                self.setUp();self.alter(offset,value)
                with self.assertRaises(PrefixError):self.validate()
    def test_stride_constraints(self):
        for value in ('0000','ffff','0034','0e51'):
            with self.subTest(value=value):
                self.setUp();self.alter(64,value)
                with self.assertRaises(PrefixError):self.validate()
        self.setUp();self.original=replace(self.original,relocations=self.original.relocations+((self.start+64,4),))
        with self.assertRaises(PrefixError):self.validate()
    def test_original_object_authentication(self):
        for obj in (None,replace(self.original,origin=self.start+2),replace(self.original,entries={}),
                    replace(self.original,data=b'\0\0'+self.original.data[2:]),
                    replace(self.original,relocations=((self.start,4),)+self.original.relocations)):
            with self.subTest(obj=obj),self.assertRaises(PrefixError):self.validate(callback_original=obj)
    def test_unique_zero_addend_undefined_data_relocation(self):
        changes={'symbol':'_f_2769e','relocation_type':'R_SH_REL32','addend':4,'embedded_addend':4,
                 'binding':'STB_LOCAL','section':1,'relocation_count':2,'overlap':True,'value':0x60ad6a4}
        for field,value in changes.items():
            with self.subTest(field=field):
                self.setUp();self.provenance[self.start+70][field]=value
                with self.assertRaises(PrefixError):self.validate()
        self.setUp()
        for p in (None,{}, {self.start+70:{}}):
            with self.subTest(p=p),self.assertRaises(PrefixError):self.validate(callback_provenance=p)
        for relocs in ((),((self.start+70,4),(self.start+70,4)),((self.start+70,4),(self.start+68,4))):
            with self.subTest(relocs=relocs),self.assertRaises(PrefixError):
                self.validate(callback_original=replace(self.original,relocations=relocs))
    def test_base_ram_range_and_alignment(self):
        for addr in (0x2769e,0x05000000,0x06100000,0x060ad6a1):
            with self.subTest(addr=addr):
                self.setUp();self.raw[70:74]=addr.to_bytes(4,'big');self.rom[self.start+70:self.end]=self.raw[70:74]
                self.provenance[self.start+70].update(value=addr,symbol=f'_g_{addr:x}')
                with self.assertRaises(PrefixError):self.validate()
    def test_extra_entry_and_truncation(self):
        self.entries[self.start+42]='_f_276c8'
        with self.assertRaises(PrefixError):self.validate()
        self.setUp();self.end-=2
        with self.assertRaises(PrefixError):self.validate()
    def test_header_marker(self):
        import build
        with tempfile.TemporaryDirectory() as td:
            p=Path(td)/'test.c';p.write_text('/* flags: -speed */\n');self.assertFalse(build.c_callback_tails(p))
            p.write_text('/* callback-tails: 1 */\n');self.assertTrue(build.c_callback_tails(p))
            for h in ('callback-tails: 0','callback-tails: 2','callback-tails: 1 callback-tails: 1',
                      'callback-tails: 1 literal-tail-calls: 1','callback-tails: 1 guarded-switches: 1',
                      'callback-tails: 1 local-loop-jumps: 1','callback-tails: 1 function-code-range: 1'):
                p.write_text('/* '+h+' */\n')
                with self.subTest(h=h),self.assertRaises(ValueError):build.c_callback_tails(p)

@unittest.skipUnless(os.environ.get('PREFIX_COMPILE_TESTS')=='1','native compiler opt in')
class NativeCallbackTests(unittest.TestCase):
    def compile(self,src,out):
        import build
        from c_prefix import compile_prefix
        return compile_prefix(src,out,0x2769e,0x276e8,rom=build.fn.ROM,shcc=build.SHCC,
            version=build.SHC_VERSION,flags=build.c_flags(src),symbols=build.read_symbols(),
            assembler=build.AS,context_start=0x27310,callback_tails=True)
    def test_ordinary_compiler_range_and_protected_context(self):
        import build
        from elftools.elf.elffile import ELFFile
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/'kept.o';self.compile(build.ROOT/'src/board_callback_dispatch_2769e.c',out)
            self.assertEqual(out.with_suffix('.prefix.bin').read_bytes(),build.fn.ROM[0x2769e:0x276e8])
            self.assertEqual(out.with_suffix('.context.bin').read_bytes()[:432],build.fn.ROM[0x27310:0x274c0])
            proof=json.loads(out.with_suffix('.prefix.json').read_text())
            self.assertEqual(proof['functions'],['_f_2769e']);self.assertEqual(proof['literal_dependencies'],[])
            self.assertEqual(proof['callback_proofs'][0]['proof_kind'],'record_callback_tail')
            with out.open('rb') as stream:
                elf=ELFFile(stream)
                exports={s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
                         if s['st_info']['bind']=='STB_GLOBAL' and s['st_shndx']!='SHN_UNDEF'}
                self.assertEqual(exports,{'_f_2769e'})
                self.assertFalse(any(s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_size'] for s in elf.iter_sections()))
    def test_early_return_rejects_and_removes_stale_success(self):
        import build
        s=(build.ROOT/'src/board_callback_dispatch_2769e.c').read_text()
        old='''        f->cb48 = 0;
    } else if (f->cb52) {
        f->cb52(f);
    }'''
        new='''        f->cb48 = 0;
        return;
    }
    if (f->cb52)
        f->cb52(f);'''
        self.assertEqual(s.count(old),1)
        with tempfile.TemporaryDirectory() as td:
            p=Path(td)/'negative.c';p.write_text(s.replace(old,new));out=Path(td)/'kept.o'
            out.write_bytes(b'stale');out.with_suffix('.prefix.json').write_text('{}')
            with self.assertRaises(PrefixError):self.compile(p,out)
            self.assertFalse(out.exists());self.assertFalse(out.with_suffix('.prefix.json').exists())
