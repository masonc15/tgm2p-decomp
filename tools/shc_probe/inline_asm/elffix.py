# Mark global NOTYPE symbols in .text as STT_FUNC with sizes (asmsh output lacks them).
import sys, struct
from elftools.elf.elffile import ELFFile
p = sys.argv[1]
buf = bytearray(open(p, 'rb').read())
with open(p, 'rb') as f:
    elf = ELFFile(f)
    tidx = next(i for i, s in enumerate(elf.iter_sections()) if s.name == '.text')
    tsize = elf.get_section(tidx)['sh_size']
    st = elf.get_section_by_name('.symtab')
    off = st['sh_offset']; ent = st['sh_entsize']
    syms = list(st.iter_symbols())
    starts = sorted(s['st_value'] for s in syms if s['st_shndx'] == tidx and s['st_info']['bind'] == 'STB_GLOBAL')
    for i, s in enumerate(syms):
        if s['st_shndx'] == tidx and s['st_info']['bind'] == 'STB_GLOBAL' and s['st_info']['type'] == 'STT_NOTYPE':
            v = s['st_value']
            nxt = min([a for a in starts if a > v] + [tsize])
            base = off + i * ent
            # Elf32_Sym: name(4) value(4) size(4) info(1) other(1) shndx(2); big-endian
            struct.pack_into('>I', buf, base + 8, nxt - v)
            buf[base + 12] = (1 << 4) | 2
open(p, 'wb').write(buf)
