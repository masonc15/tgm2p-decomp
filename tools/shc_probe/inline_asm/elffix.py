#!/usr/bin/env python3
"""Mark global NOTYPE symbols in .text as functions with sizes.

asmsh's objects leave global functions as STT_NOTYPE with size 0, and
build.py and funcscore.py look for STT_FUNC. Each one becomes STT_FUNC with a
size running to the next global in .text (or the end of .text). Plain struct
parsing of the big-endian ELF32 file, so it needs no pyelftools.

usage: elffix.py <file.o>
"""
import struct
import sys


def main():
    path = sys.argv[1]
    buf = bytearray(open(path, "rb").read())
    shoff, = struct.unpack_from(">I", buf, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", buf, 0x2E)
    secs = [struct.unpack_from(">IIIIIIIIII", buf, shoff + i * shentsize) for i in range(shnum)]
    # Section header: name type flags addr offset size link info addralign entsize
    shstr = secs[shstrndx][4]
    name = lambda s: bytes(buf[shstr + s[0]:buf.index(0, shstr + s[0])]).decode()
    text = next(i for i, s in enumerate(secs) if name(s) == ".text")
    tsize = secs[text][5]
    symtab = next(s for s in secs if s[1] == 2)  # SHT_SYMTAB
    off, size, ent = symtab[4], symtab[5], symtab[9]
    # Elf32_Sym: name(4) value(4) size(4) info(1) other(1) shndx(2)
    syms = [struct.unpack_from(">IIIBBH", buf, off + i * ent) for i in range(size // ent)]
    is_global_text = lambda s: s[5] == text and s[3] >> 4 == 1  # STB_GLOBAL
    starts = sorted(s[1] for s in syms if is_global_text(s))
    for i, s in enumerate(syms):
        if is_global_text(s) and s[3] & 0xF == 0:  # STT_NOTYPE
            nxt = min([a for a in starts if a > s[1]] + [tsize])
            base = off + i * ent
            struct.pack_into(">I", buf, base + 8, nxt - s[1])
            buf[base + 12] = (1 << 4) | 2  # STB_GLOBAL, STT_FUNC
    open(path, "wb").write(buf)


if __name__ == "__main__":
    main()
