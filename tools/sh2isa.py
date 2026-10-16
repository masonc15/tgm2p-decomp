#!/usr/bin/env python3
"""Table-driven SH-2 instruction decoder with register def/use sets.

decode(word, pc) describes one 16-bit instruction: its mnemonic and operand
text, the registers it reads and writes (r0-r15, pr, macl, mach, t, sr, gbr,
vbr), how it transfers control and where, whether it has a delay slot, and
which literal-pool word a pc-relative load reads. Used by asm_scan.py and
shiftable.py; it is a static description, not an emulator.

PC-relative loads in a delay slot use the branch destination, not the slot's
own address (SH-1/SH-2 programming manual, MOV immediate-data notes); decode()
reports the ordinary address and callers that care must special-case slots.
"""
from __future__ import annotations

from dataclasses import dataclass, field

R = [f"r{i}" for i in range(16)]


def _sx(v: int, bits: int) -> int:
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


@dataclass(frozen=True)
class Insn:
    pc: int
    word: int
    mnemonic: str
    ops: str = ""
    uses: frozenset = frozenset()
    defs: frozenset = frozenset()
    # control transfer: None, "cond" (bt/bf), "jump" (bra/braf/jmp),
    # "call" (bsr/bsrf/jsr), "ret" (rts), "rte"
    flow: str | None = None
    target: int | None = None        # static branch target, when known
    delay: bool = False
    pool: tuple | None = None        # (address, size) read by mov.w/mov.l @(disp,pc)
    mova: int | None = None          # address computed by mova
    mem: tuple | None = None         # (base register, mode, size, is_store)
    valid: bool = True
    extra: dict = field(default_factory=dict, compare=False)

    def text(self) -> str:
        return f"{self.mnemonic} {self.ops}".strip()


def _i(pc, w, mn, ops="", uses=(), defs=(), **kw):
    return Insn(pc, w, mn, ops, frozenset(uses), frozenset(defs), **kw)


def decode(w: int, pc: int = 0) -> Insn:
    n = (w >> 8) & 0xF
    m = (w >> 4) & 0xF
    rn, rm = R[n], R[m]
    lo4 = w & 0xF
    lo8 = w & 0xFF
    top = w >> 12
    if top == 0x0:
        if lo4 == 0x2:
            sr = {0: "sr", 1: "gbr", 2: "vbr"}.get(m)
            if sr:
                return _i(pc, w, "stc", f"{sr},{rn}", [sr], [rn])
        if lo4 == 0xA:
            sr = {0: "mach", 1: "macl", 2: "pr"}.get(m)
            if sr:
                return _i(pc, w, "sts", f"{sr},{rn}", [sr], [rn])
        if lo8 == 0x23:
            return _i(pc, w, "braf", rn, [rn], [], flow="jump", delay=True)
        if lo8 == 0x03:
            return _i(pc, w, "bsrf", rn, [rn], ["pr"], flow="call", delay=True)
        if lo4 in (0x4, 0x5, 0x6):
            sz = {0x4: 1, 0x5: 2, 0x6: 4}[lo4]
            return _i(pc, w, "mov." + "bwl"[sz // 2], f"{rm},@(r0,{rn})", [rm, rn, "r0"], [],
                      mem=(rn, "r0idx", sz, True))
        if lo4 == 0x7:
            return _i(pc, w, "mul.l", f"{rm},{rn}", [rm, rn], ["macl"])
        if w == 0x0008:
            return _i(pc, w, "clrt", "", [], ["t"])
        if w == 0x0018:
            return _i(pc, w, "sett", "", [], ["t"])
        if w == 0x0028:
            return _i(pc, w, "clrmac", "", [], ["macl", "mach"])
        if w == 0x0009:
            return _i(pc, w, "nop")
        if w == 0x0019:
            return _i(pc, w, "div0u", "", [], ["t", "sr"])
        if lo8 == 0x29:
            return _i(pc, w, "movt", rn, ["t"], [rn])
        if w == 0x000B:
            return _i(pc, w, "rts", "", ["pr"], [], flow="ret", delay=True)
        if w == 0x001B:
            return _i(pc, w, "sleep")
        if w == 0x002B:
            return _i(pc, w, "rte", "", ["r15"], ["r15", "sr"], flow="rte", delay=True)
        if lo4 in (0xC, 0xD, 0xE):
            sz = {0xC: 1, 0xD: 2, 0xE: 4}[lo4]
            return _i(pc, w, "mov." + "bwl"[sz // 2], f"@(r0,{rm}),{rn}", [rm, "r0"], [rn],
                      mem=(rm, "r0idx", sz, False))
        if lo4 == 0xF:
            return _i(pc, w, "mac.l", f"@{rm}+,@{rn}+", [rm, rn, "macl", "mach"], [rm, rn, "macl", "mach"],
                      mem=(rm, "postinc", 4, False))
    elif top == 0x1:
        return _i(pc, w, "mov.l", f"{rm},@({lo4 * 4},{rn})", [rm, rn], [], mem=(rn, "disp", 4, True))
    elif top == 0x2:
        sub = lo4
        if sub in (0, 1, 2):
            sz = 1 << sub
            return _i(pc, w, "mov." + "bwl"[sub], f"{rm},@{rn}", [rm, rn], [], mem=(rn, "ind", sz, True))
        if sub in (4, 5, 6):
            sz = 1 << (sub - 4)
            return _i(pc, w, "mov." + "bwl"[sub - 4], f"{rm},@-{rn}", [rm, rn], [rn],
                      mem=(rn, "predec", sz, True))
        names = {7: "div0s", 8: "tst", 9: "and", 0xA: "xor", 0xB: "or", 0xC: "cmp/str", 0xD: "xtrct",
                 0xE: "mulu.w", 0xF: "muls.w"}
        if sub in names:
            mn = names[sub]
            if mn == "div0s":
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], ["t", "sr"])
            if mn in ("tst", "cmp/str"):
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], ["t"])
            if mn in ("mulu.w", "muls.w"):
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], ["macl"])
            return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], [rn])
    elif top == 0x3:
        names = {0: "cmp/eq", 2: "cmp/hs", 3: "cmp/ge", 4: "div1", 5: "dmulu.l", 6: "cmp/hi", 7: "cmp/gt",
                 8: "sub", 0xA: "subc", 0xB: "subv", 0xC: "add", 0xD: "dmuls.l", 0xE: "addc", 0xF: "addv"}
        mn = names.get(lo4)
        if mn:
            if mn.startswith("cmp/"):
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], ["t"])
            if mn == "div1":
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn, "t", "sr"], [rn, "t", "sr"])
            if mn in ("dmulu.l", "dmuls.l"):
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], ["macl", "mach"])
            if mn in ("subc", "addc"):
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn, "t"], [rn, "t"])
            if mn in ("subv", "addv"):
                return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], [rn, "t"])
            return _i(pc, w, mn, f"{rm},{rn}", [rm, rn], [rn])
    elif top == 0x4:
        if lo4 == 0xF:
            return _i(pc, w, "mac.w", f"@{rm}+,@{rn}+", [rm, rn, "macl", "mach"], [rm, rn, "macl", "mach"],
                      mem=(rm, "postinc", 2, False))
        one = {
            0x00: ("shll", ["t"]), 0x10: ("dt", ["t"]), 0x20: ("shal", ["t"]),
            0x01: ("shlr", ["t"]), 0x11: ("cmp/pz", ["t"]), 0x21: ("shar", ["t"]),
            0x04: ("rotl", ["t"]), 0x24: ("rotcl", ["t"]), 0x05: ("rotr", ["t"]),
            0x15: ("cmp/pl", ["t"]), 0x25: ("rotcr", ["t"]),
            0x08: ("shll2", []), 0x18: ("shll8", []), 0x28: ("shll16", []),
            0x09: ("shlr2", []), 0x19: ("shlr8", []), 0x29: ("shlr16", []),
        }
        if lo8 in one:
            mn, extra_defs = one[lo8]
            uses = [rn] + (["t"] if mn in ("rotcl", "rotcr") else [])
            defs = ([] if mn.startswith("cmp/") else [rn]) + extra_defs
            return _i(pc, w, mn, rn, uses, defs)
        stl = {0x02: "mach", 0x12: "macl", 0x22: "pr"}
        if lo8 in stl:
            return _i(pc, w, "sts.l", f"{stl[lo8]},@-{rn}", [stl[lo8], rn], [rn], mem=(rn, "predec", 4, True))
        stcl = {0x03: "sr", 0x13: "gbr", 0x23: "vbr"}
        if lo8 in stcl:
            return _i(pc, w, "stc.l", f"{stcl[lo8]},@-{rn}", [stcl[lo8], rn], [rn], mem=(rn, "predec", 4, True))
        ldsl = {0x06: "mach", 0x16: "macl", 0x26: "pr"}
        if lo8 in ldsl:
            return _i(pc, w, "lds.l", f"@{rn}+,{ldsl[lo8]}", [rn], [rn, ldsl[lo8]], mem=(rn, "postinc", 4, False))
        ldcl = {0x07: "sr", 0x17: "gbr", 0x27: "vbr"}
        if lo8 in ldcl:
            d = [rn, ldcl[lo8]] + (["t"] if ldcl[lo8] == "sr" else [])
            return _i(pc, w, "ldc.l", f"@{rn}+,{ldcl[lo8]}", [rn], d, mem=(rn, "postinc", 4, False))
        lds = {0x0A: "mach", 0x1A: "macl", 0x2A: "pr"}
        if lo8 in lds:
            return _i(pc, w, "lds", f"{rn},{lds[lo8]}", [rn], [lds[lo8]])
        ldc = {0x0E: "sr", 0x1E: "gbr", 0x2E: "vbr"}
        if lo8 in ldc:
            d = [ldc[lo8]] + (["t"] if ldc[lo8] == "sr" else [])
            return _i(pc, w, "ldc", f"{rn},{ldc[lo8]}", [rn], d)
        if lo8 == 0x0B:
            return _i(pc, w, "jsr", f"@{rn}", [rn], ["pr"], flow="call", delay=True)
        if lo8 == 0x2B:
            return _i(pc, w, "jmp", f"@{rn}", [rn], [], flow="jump", delay=True)
        if lo8 == 0x1B:
            return _i(pc, w, "tas.b", f"@{rn}", [rn], ["t"], mem=(rn, "ind", 1, True))
    elif top == 0x5:
        return _i(pc, w, "mov.l", f"@({lo4 * 4},{rm}),{rn}", [rm], [rn], mem=(rm, "disp", 4, False))
    elif top == 0x6:
        sub = lo4
        if sub in (0, 1, 2):
            return _i(pc, w, "mov." + "bwl"[sub], f"@{rm},{rn}", [rm], [rn], mem=(rm, "ind", 1 << sub, False))
        if sub == 3:
            return _i(pc, w, "mov", f"{rm},{rn}", [rm], [rn])
        if sub in (4, 5, 6):
            d = [rn] if rn == rm else [rn, rm]
            return _i(pc, w, "mov." + "bwl"[sub - 4], f"@{rm}+,{rn}", [rm], d,
                      mem=(rm, "postinc", 1 << (sub - 4), False))
        names = {7: "not", 8: "swap.b", 9: "swap.w", 0xA: "negc", 0xB: "neg", 0xC: "extu.b", 0xD: "extu.w",
                 0xE: "exts.b", 0xF: "exts.w"}
        mn = names[sub]
        if mn == "negc":
            return _i(pc, w, mn, f"{rm},{rn}", [rm, "t"], [rn, "t"])
        return _i(pc, w, mn, f"{rm},{rn}", [rm], [rn])
    elif top == 0x7:
        return _i(pc, w, "add", f"#{_sx(lo8, 8)},{rn}", [rn], [rn], extra={"imm": _sx(lo8, 8)})
    elif top == 0x8:
        if n in (0x0, 0x1):
            sz = 1 << n
            return _i(pc, w, "mov." + "bw"[n], f"r0,@({lo4 * sz},{rm})", ["r0", rm], [],
                      mem=(rm, "disp", sz, True))
        if n in (0x4, 0x5):
            sz = 1 << (n - 4)
            return _i(pc, w, "mov." + "bw"[n - 4], f"@({lo4 * sz},{rm}),r0", [rm], ["r0"],
                      mem=(rm, "disp", sz, False))
        if n == 0x8:
            return _i(pc, w, "cmp/eq", f"#{_sx(lo8, 8)},r0", ["r0"], ["t"])
        if n in (0x9, 0xB, 0xD, 0xF):
            mn = {0x9: "bt", 0xB: "bf", 0xD: "bt/s", 0xF: "bf/s"}[n]
            t = pc + 4 + _sx(lo8, 8) * 2
            return _i(pc, w, mn, f"0x{t:x}", ["t"], [], flow="cond", target=t, delay=mn.endswith("/s"))
    elif top in (0xA, 0xB):
        t = pc + 4 + _sx(w & 0xFFF, 12) * 2
        if top == 0xA:
            return _i(pc, w, "bra", f"0x{t:x}", [], [], flow="jump", target=t, delay=True)
        return _i(pc, w, "bsr", f"0x{t:x}", [], ["pr"], flow="call", target=t, delay=True)
    elif top == 0x9:
        a = pc + 4 + lo8 * 2
        return _i(pc, w, "mov.w", f"@(0x{a:x}),{rn}", [], [rn], pool=(a, 2))
    elif top == 0xC:
        if n in (0, 1, 2):
            sz = 1 << n
            return _i(pc, w, "mov." + "bwl"[n], f"r0,@({lo8 * sz},gbr)", ["r0", "gbr"], [],
                      mem=("gbr", "disp", sz, True))
        if n == 3:
            return _i(pc, w, "trapa", f"#{lo8}", ["r15", "sr", "vbr"], ["r15", "pr"], flow="call")
        if n in (4, 5, 6):
            sz = 1 << (n - 4)
            return _i(pc, w, "mov." + "bwl"[n - 4], f"@({lo8 * sz},gbr),r0", ["gbr"], ["r0"],
                      mem=("gbr", "disp", sz, False))
        if n == 7:
            a = ((pc + 4) & ~3) + lo8 * 4
            return _i(pc, w, "mova", f"@(0x{a:x}),r0", [], ["r0"], mova=a)
        if n == 8:
            return _i(pc, w, "tst", f"#{lo8},r0", ["r0"], ["t"])
        if n in (9, 0xA, 0xB):
            mn = {9: "and", 0xA: "xor", 0xB: "or"}[n]
            return _i(pc, w, mn, f"#{lo8},r0", ["r0"], ["r0"])
        if n in (0xC, 0xD, 0xE, 0xF):
            mn = {0xC: "tst.b", 0xD: "and.b", 0xE: "xor.b", 0xF: "or.b"}[n]
            return _i(pc, w, mn, f"#{lo8},@(r0,gbr)", ["r0", "gbr"], ["t"] if n == 0xC else [],
                      mem=("gbr", "r0idx", 1, n != 0xC))
    elif top == 0xD:
        a = ((pc + 4) & ~3) + lo8 * 4
        return _i(pc, w, "mov.l", f"@(0x{a:x}),{rn}", [], [rn], pool=(a, 4))
    elif top == 0xE:
        return _i(pc, w, "mov", f"#{_sx(lo8, 8)},{rn}", [], [rn], extra={"imm": _sx(lo8, 8)})
    return Insn(pc, w, ".word", f"0x{w:04x}", valid=False)


def is_delayed_branch(w: int) -> bool:
    return decode(w).delay
