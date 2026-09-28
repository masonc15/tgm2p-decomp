#!/usr/bin/env python3
"""Find SH-2 function extents in build/prog.bin and print annotated listings.

A function's code ends at the first rts/bra/jmp (plus its delay slot) that no
earlier forward branch jumps past; its literal pool is every pc-relative load
target beyond that point. The extent runs to the end of the pool (or the code,
if there is no pool) and is what probe cases use as `len:`.

usage: fn.py 0x2f852 [0x3e2e ...]        annotated listing
       fn.py --scan                       one line per call target: addr len shape
"""
import argparse
import struct
from pathlib import Path

import capstone as cs

ROOT = Path(__file__).resolve().parent.parent
TEXT_END = 0x31000
# Boot code copies ROM 0x780-0x313fc to RAM at 0x06000000 and runs it there, so
# calls through literal pools name RAM addresses (see the table at 0x313fc).
RAM_BASE = 0x06000000
RAM_ROM = 0x780
RAM_END = RAM_BASE + 0x313fc - RAM_ROM
ROM = (ROOT / "build" / "prog.bin").read_bytes()
MD = cs.Cs(cs.CS_ARCH_SH, cs.CS_MODE_SH2 | cs.CS_MODE_BIG_ENDIAN)
MD.skipdata = True


def hw(a: int) -> int:
    return ROM[a] << 8 | ROM[a + 1]


def sdisp(v: int, bits: int) -> int:
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def extent(start: int, limit: int = 0x2000):
    """Return (code_end, extent_end, pool_refs, branch_targets)."""
    pc = start
    far = start
    pool = set()
    targets = set()
    resume = None  # first address after a jump that doesn't end the function
    after_jump = False
    while pc < start + limit:
        if pc == resume:
            after_jump = True
        # A pool can sit mid-function after a bra, holding words that only
        # earlier functions load, so skip anything any load reads there.
        if pc in pool or (after_jump and (pc in all_pool() or (hw(pc) in (0x0009, 0xFFFF, 0x0000)
                                                               and pc + 2 in all_pool()))):
            pc += 2
            continue
        after_jump = False
        w = hw(pc)
        op = w >> 12
        if op == 0xD:  # mov.l @(disp,pc)
            a = ((pc + 4) & ~3) + (w & 0xFF) * 4
            pool.update((a, a + 2))
        elif op == 0x9:  # mov.w @(disp,pc)
            pool.add(pc + 4 + (w & 0xFF) * 2)
        elif (w & 0xFF00) == 0xC700:  # mova
            pool.add(((pc + 4) & ~3) + (w & 0xFF) * 4)
        elif op == 0x8 and (w >> 8) & 0xF in (0x9, 0xB, 0xD, 0xF):  # bt/bf(/s)
            t = pc + 4 + sdisp(w & 0xFF, 8) * 2
            targets.add(t)
            far = max(far, t)
        elif op == 0xA:  # bra
            t = pc + 4 + sdisp(w & 0xFFF, 12) * 2
            targets.add(t)
            far = max(far, t)
            if pc + 2 >= far:
                pc += 4
                break
            resume = pc + 4
        elif w == 0x000B or (op == 0x4 and (w & 0xFF) == 0x2B):  # rts / jmp @Rn
            if pc + 2 >= far:
                pc += 4
                break
            resume = pc + 4
        pc += 2
    code_end = pc
    end = code_end
    beyond = [a for a in pool if a >= code_end]
    if beyond:
        end = max(beyond) + 2
    # a function followed by padding to 4 bytes
    if end % 4 and hw(end) == 0x0009:
        end += 2
    return code_end, end, pool, targets


_ALL_POOL = None


def all_pool():
    """Every address some pc-relative load in the code reads."""
    global _ALL_POOL
    if _ALL_POOL is None:
        out = set()
        for pc in range(0x400, TEXT_END, 2):
            w = hw(pc)
            if w >> 12 == 0xD or w & 0xFF00 == 0xC700:
                a = ((pc + 4) & ~3) + (w & 0xFF) * 4
                out.update((a, a + 2))
            elif w >> 12 == 0x9:
                out.add(pc + 4 + (w & 0xFF) * 2)
        _ALL_POOL = out
    return _ALL_POOL


def shape(start: int, code_end: int) -> str:
    """Rough codegen classification: O0 spills incoming args to the frame."""
    ins = list(MD.disasm(ROM[start:code_end], start))
    txt = [f"{i.mnemonic} {i.op_str}" for i in ins[:10]]
    pushes = sum(1 for t in txt if t.startswith("mov.l r") and "@-r15" in t)
    spills = sum(1 for t in txt if t.startswith(("mov.b r4", "mov.w r4", "mov.l r4,@(", "mov.l r4,@r15"))
                 or t.startswith(("mov.b r5", "mov.w r5", "mov.l r5,@")))
    if pushes == 0 and spills and any("r15" in t for t in txt[:4]):
        return "O0"
    return "opt"


def listing(start: int) -> None:
    code_end, end, pool, targets = extent(start)
    print(f"--- {start:#x}  code {code_end - start:#x}  extent {end - start:#x}  shape {shape(start, code_end)}")
    for i in MD.disasm(ROM[start:code_end], start):
        extra = ""
        if i.mnemonic in ("mov.l", "mov.w") and i.op_str.startswith("0x"):
            t = int(i.op_str.split(",")[0], 16)
            v = struct.unpack(">I", ROM[t:t + 4])[0] if i.mnemonic == "mov.l" else struct.unpack(">h", ROM[t:t + 2])[0]
            extra = f"   ; ={v:#x}"
        mark = ">" if i.address in targets else " "
        print(f"{mark}{i.address:06x} {i.bytes.hex():4s}  {i.mnemonic:7s} {i.op_str}{extra}")
    for a in range(code_end, end, 2):
        print(f" {a:06x} {hw(a):04x}  .word")


def rom_addr(v: int) -> int:
    """ROM offset of a code address as the running program sees it."""
    return v - RAM_BASE + RAM_ROM if RAM_BASE <= v < RAM_END else v


def call_targets():
    seeds = set()
    for pc in range(0x400, 0x31000, 2):
        w = hw(pc)
        if w >> 12 == 0xB:
            seeds.add(pc + 4 + sdisp(w & 0xFFF, 12) * 2)
    lit = {}
    pointers = set()
    for pc in range(0x400, 0x31000, 2):
        w = hw(pc)
        if w >> 12 == 0xD:
            a = ((pc + 4) & ~3) + (w & 0xFF) * 4
            v = struct.unpack(">I", ROM[a:a + 4])[0]
            lit[(w >> 8) & 0xF] = (rom_addr(v), pc)
            if RAM_BASE <= v < RAM_END:
                pointers.add(rom_addr(v))
        elif w >> 12 == 0x4 and (w & 0xFF) == 0x0B:
            n = (w >> 8) & 0xF
            if n in lit and pc - lit[n][1] <= 16 and 0x400 <= lit[n][0] < 0x31000:
                seeds.add(lit[n][0])
    # Function pointers: RAM code addresses held in literal pools (often
    # hoisted into registers far from the jsr) and in the data after the code.
    for a in range(0x313fc, len(ROM) - 3, 4):
        v = struct.unpack(">I", ROM[a:a + 4])[0]
        if RAM_BASE <= v < RAM_END:
            pointers.add(rom_addr(v))
    seeds |= {p for p in pointers if 0x400 < p < 0x31000 and after_return(p)}
    return sorted(s for s in seeds if 0x400 <= s < 0x31000 and s % 2 == 0)


def after_return(a: int) -> bool:
    """Whether a could start a function: it follows a return's delay slot,
    alignment padding, or a literal pool."""
    back = hw(a - 4)
    if back == 0x000B or (back >> 12 == 0x4 and back & 0xFF == 0x2B):  # rts / jmp @Rn
        return True
    if hw(a - 2) == 0x0009 and hw(a - 6) == 0x000B:  # padded after rts; slot
        return True
    return extent_pool_end(a)


def extent_pool_end(a: int) -> bool:
    """Whether a pc-relative load earlier in the ROM reads the word just before a."""
    return a in _pool_ends()


_POOL_ENDS = None


def _pool_ends():
    global _POOL_ENDS
    if _POOL_ENDS is None:
        ends = set()
        for pc in range(0x400, 0x31000, 2):
            w = hw(pc)
            if w >> 12 == 0xD:
                ends.add(((pc + 4) & ~3) + (w & 0xFF) * 4 + 4)
            elif w >> 12 == 0x9:
                t = pc + 4 + (w & 0xFF) * 2
                ends.update((t + 2, t + 4 if t % 4 == 2 and hw(t + 2) in (0x0000, 0x0009, 0xFFFF) else t + 2))
        _POOL_ENDS = ends
    return _POOL_ENDS


def units():
    """Split text into pool groups: runs of functions sharing one literal pool.

    SHC emits a single literal pool after a run of functions (per translation
    unit, or earlier when mov.w/mov.l displacement range forces it), so a pool
    group is the smallest span that can be recompiled byte-identically.
    Returns [(start, pool_start, end, [function starts])].
    """
    groups = []
    pc = unit_start = 0x400
    members = []
    refs = set()
    while pc < TEXT_END:
        code_end, _, r, _ = extent(pc)
        members.append(pc)
        refs |= r
        q = code_end
        pad = (0x0000, 0x0009)
        if q in refs or (hw(q) in pad and q + 2 in refs):
            while q < TEXT_END and (q in refs or (hw(q) in pad and q + 2 in refs)):
                q += 2
            groups.append((unit_start, code_end, q, members))
            members, refs, unit_start = [], set(), q
        elif hw(q) == 0x0009 and q % 4:  # alignment nop between functions
            q += 2
        pc = q
    if members:
        groups.append((unit_start, pc, pc, members))
    return groups


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("addrs", nargs="*")
    ap.add_argument("--scan", action="store_true")
    ap.add_argument("--units", action="store_true")
    args = ap.parse_args()
    if args.units:
        for start, pstart, end, members in units():
            shapes = "".join("0" if shape(m, extent(m)[0]) == "O0" else "o" for m in members)
            gap = "" if members[0] == start else f" (starts {members[0]:x})"
            print(f"{start:06x}-{end:06x} len {end - start:5x} pool {end - pstart:4x} funcs {len(members):2d} {shapes}{gap}")
    if args.scan:
        for s in call_targets():
            code_end, end, _, _ = extent(s)
            ins = list(MD.disasm(ROM[s:code_end], s))
            calls = sum(i.mnemonic in ("jsr", "bsr") for i in ins)
            print(f"{s:06x} {end - s:4x} {len(ins):4d} {shape(s, code_end):3s} calls={calls}")
    for a in args.addrs:
        listing(int(a, 16))


if __name__ == "__main__":
    main()
