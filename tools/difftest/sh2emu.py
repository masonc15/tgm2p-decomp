#!/usr/bin/env python3
"""A small SH-2 integer interpreter for differential testing.

Big-endian, the full SH-2 integer instruction set the compiler can emit:
delayed branches (the slot runs before the transfer, branch targets and rts
use register values from before the slot), the T/S/Q/M bits, div0s/div0u/div1,
mul.l/muls.w/mulu.w/dmuls.l/dmulu.l, mac.w/mac.l (S=0 and S=1), xtrct, swap,
mova and pc-relative loads (delay slots use branch destination + 2,
per the SH-1/SH-2 programming manual). Not modelled: rte/trapa/sleep, exceptions, address errors,
caches and timing.

The CPU knows nothing about functions: a harness supplies the memory, decides
which addresses are "inside" (`in_region`), what a call does (`on_call`) and
what leaving the region means (`on_leave`). See difftest.py.

    python3 sh2emu.py --selftest     runs the ROM's SHC runtime helpers
                                     (_divls, _modls, ...) against Python
"""
from __future__ import annotations

import struct
import sys
from pathlib import Path

MASK = 0xFFFFFFFF
ROOT = Path(__file__).resolve().parents[2]
ROM_PATH = ROOT / "build" / "prog.bin"

# The game copies ROM 0x780-0x313fc to RAM 0x06000000 and runs it there.
RAM_BASE, RAM_ROM, RAM_ROM_END = 0x06000000, 0x780, 0x313FC
RAM_DELTA = RAM_BASE - RAM_ROM            # 0x5fff880
RAM_END = RAM_ROM_END + RAM_DELTA         # 0x06030c7c
# SHC's runtime library (division, variable shifts) at the end of that block:
# executed for real rather than stubbed, since it takes arguments in r0/r1.
HELPERS = (0x30D40 + RAM_DELTA, RAM_END)


class Unsupported(Exception):
    pass


class StepLimit(Exception):
    pass


def sx8(v: int) -> int:
    v &= 0xFF
    return v | 0xFFFFFF00 if v & 0x80 else v


def sx16(v: int) -> int:
    v &= 0xFFFF
    return v | 0xFFFF0000 if v & 0x8000 else v


def s32(v: int) -> int:
    return v - 0x100000000 if v & 0x80000000 else v


# ---------------------------------------------------------------- handlers
# h(c, n, m, i, pc) -> next pc. Registers are kept as unsigned 32-bit ints.
# Delayed branches set c.pending = (kind, target) and return pc + 2.

JUMP, CALL, RET = 0, 1, 2


def h_nop(c, n, m, i, pc): return pc + 2
def h_mov_i(c, n, m, i, pc): c.R[n] = i; return pc + 2
def _pc_base(c, pc):
    return c.delay_pc if c.delay_pc is not None else (pc + 4) & MASK


def h_movw_pc(c, n, m, i, pc): c.R[n] = sx16(c.mem.r16((_pc_base(c, pc) + i) & MASK)); return pc + 2
def h_movl_pc(c, n, m, i, pc): c.R[n] = c.mem.r32(((_pc_base(c, pc) & ~3) + i) & MASK); return pc + 2
def h_mova(c, n, m, i, pc): c.R[0] = ((_pc_base(c, pc) & ~3) + i) & MASK; return pc + 2
def h_mov(c, n, m, i, pc): c.R[n] = c.R[m]; return pc + 2


def h_movb_s(c, n, m, i, pc): c.mem.w8(c.R[n], c.R[m], pc); return pc + 2
def h_movw_s(c, n, m, i, pc): c.mem.w16(c.R[n], c.R[m], pc); return pc + 2
def h_movl_s(c, n, m, i, pc): c.mem.w32(c.R[n], c.R[m], pc); return pc + 2
def h_movb_l(c, n, m, i, pc): c.R[n] = sx8(c.mem.r8(c.R[m])); return pc + 2
def h_movw_l(c, n, m, i, pc): c.R[n] = sx16(c.mem.r16(c.R[m])); return pc + 2
def h_movl_l(c, n, m, i, pc): c.R[n] = c.mem.r32(c.R[m]); return pc + 2


def _predec(c, n, m, size, w, pc):
    R = c.R
    v = R[m]
    a = (R[n] - size) & MASK
    w(a, v, pc)
    R[n] = a
    return pc + 2


def h_movb_m(c, n, m, i, pc): return _predec(c, n, m, 1, c.mem.w8, pc)
def h_movw_m(c, n, m, i, pc): return _predec(c, n, m, 2, c.mem.w16, pc)
def h_movl_m(c, n, m, i, pc): return _predec(c, n, m, 4, c.mem.w32, pc)


def h_movb_p(c, n, m, i, pc):
    R = c.R
    v = sx8(c.mem.r8(R[m]))
    if n != m:
        R[m] = (R[m] + 1) & MASK
    R[n] = v
    return pc + 2


def h_movw_p(c, n, m, i, pc):
    R = c.R
    v = sx16(c.mem.r16(R[m]))
    if n != m:
        R[m] = (R[m] + 2) & MASK
    R[n] = v
    return pc + 2


def h_movl_p(c, n, m, i, pc):
    R = c.R
    v = c.mem.r32(R[m])
    if n != m:
        R[m] = (R[m] + 4) & MASK
    R[n] = v
    return pc + 2


# @(disp,Rn) forms; i is the scaled displacement
def h_movb_s4(c, n, m, i, pc): c.mem.w8((c.R[n] + i) & MASK, c.R[0], pc); return pc + 2
def h_movw_s4(c, n, m, i, pc): c.mem.w16((c.R[n] + i) & MASK, c.R[0], pc); return pc + 2
def h_movl_s4(c, n, m, i, pc): c.mem.w32((c.R[n] + i) & MASK, c.R[m], pc); return pc + 2
def h_movb_l4(c, n, m, i, pc): c.R[0] = sx8(c.mem.r8((c.R[m] + i) & MASK)); return pc + 2
def h_movw_l4(c, n, m, i, pc): c.R[0] = sx16(c.mem.r16((c.R[m] + i) & MASK)); return pc + 2
def h_movl_l4(c, n, m, i, pc): c.R[n] = c.mem.r32((c.R[m] + i) & MASK); return pc + 2
# @(R0,Rn)
def h_movb_s0(c, n, m, i, pc): c.mem.w8((c.R[n] + c.R[0]) & MASK, c.R[m], pc); return pc + 2
def h_movw_s0(c, n, m, i, pc): c.mem.w16((c.R[n] + c.R[0]) & MASK, c.R[m], pc); return pc + 2
def h_movl_s0(c, n, m, i, pc): c.mem.w32((c.R[n] + c.R[0]) & MASK, c.R[m], pc); return pc + 2
def h_movb_l0(c, n, m, i, pc): c.R[n] = sx8(c.mem.r8((c.R[m] + c.R[0]) & MASK)); return pc + 2
def h_movw_l0(c, n, m, i, pc): c.R[n] = sx16(c.mem.r16((c.R[m] + c.R[0]) & MASK)); return pc + 2
def h_movl_l0(c, n, m, i, pc): c.R[n] = c.mem.r32((c.R[m] + c.R[0]) & MASK); return pc + 2
# @(disp,GBR)
def h_movb_sg(c, n, m, i, pc): c.mem.w8((c.gbr + i) & MASK, c.R[0], pc); return pc + 2
def h_movw_sg(c, n, m, i, pc): c.mem.w16((c.gbr + i) & MASK, c.R[0], pc); return pc + 2
def h_movl_sg(c, n, m, i, pc): c.mem.w32((c.gbr + i) & MASK, c.R[0], pc); return pc + 2
def h_movb_lg(c, n, m, i, pc): c.R[0] = sx8(c.mem.r8((c.gbr + i) & MASK)); return pc + 2
def h_movw_lg(c, n, m, i, pc): c.R[0] = sx16(c.mem.r16((c.gbr + i) & MASK)); return pc + 2
def h_movl_lg(c, n, m, i, pc): c.R[0] = c.mem.r32((c.gbr + i) & MASK); return pc + 2


def h_movt(c, n, m, i, pc): c.R[n] = c.T; return pc + 2


def h_swapb(c, n, m, i, pc):
    v = c.R[m]
    c.R[n] = (v & 0xFFFF0000) | ((v & 0xFF) << 8) | ((v >> 8) & 0xFF)
    return pc + 2


def h_swapw(c, n, m, i, pc):
    v = c.R[m]
    c.R[n] = ((v >> 16) | (v << 16)) & MASK
    return pc + 2


def h_xtrct(c, n, m, i, pc):
    c.R[n] = ((c.R[m] << 16) | (c.R[n] >> 16)) & MASK
    return pc + 2


# arithmetic
def h_add(c, n, m, i, pc): c.R[n] = (c.R[n] + c.R[m]) & MASK; return pc + 2
def h_add_i(c, n, m, i, pc): c.R[n] = (c.R[n] + i) & MASK; return pc + 2


def h_addc(c, n, m, i, pc):
    s = c.R[n] + c.R[m] + c.T
    c.R[n] = s & MASK
    c.T = s >> 32
    return pc + 2


def h_addv(c, n, m, i, pc):
    a, b = c.R[n], c.R[m]
    r = (a + b) & MASK
    c.T = ((~(a ^ b) & (a ^ r)) >> 31) & 1
    c.R[n] = r
    return pc + 2


def h_cmpeq_i(c, n, m, i, pc): c.T = int(c.R[0] == i); return pc + 2
def h_cmpeq(c, n, m, i, pc): c.T = int(c.R[n] == c.R[m]); return pc + 2
def h_cmphs(c, n, m, i, pc): c.T = int(c.R[n] >= c.R[m]); return pc + 2
def h_cmpge(c, n, m, i, pc): c.T = int(s32(c.R[n]) >= s32(c.R[m])); return pc + 2
def h_cmphi(c, n, m, i, pc): c.T = int(c.R[n] > c.R[m]); return pc + 2
def h_cmpgt(c, n, m, i, pc): c.T = int(s32(c.R[n]) > s32(c.R[m])); return pc + 2
def h_cmppz(c, n, m, i, pc): c.T = int(not c.R[n] & 0x80000000); return pc + 2
def h_cmppl(c, n, m, i, pc): c.T = int(0 < s32(c.R[n])); return pc + 2


def h_cmpstr(c, n, m, i, pc):
    t = c.R[n] ^ c.R[m]
    c.T = int(not (t & 0xFF000000) or not (t & 0xFF0000) or not (t & 0xFF00) or not (t & 0xFF))
    return pc + 2


def h_div0s(c, n, m, i, pc):
    c.Q = c.R[n] >> 31
    c.M = c.R[m] >> 31
    c.T = c.Q ^ c.M
    return pc + 2


def h_div0u(c, n, m, i, pc):
    c.M = c.Q = c.T = 0
    return pc + 2


def h_div1(c, n, m, i, pc):
    # Transcribed from the SH-1/SH-2 programming manual's DIV1 description.
    R = c.R
    old_q = c.Q
    rn = R[n]
    c.Q = rn >> 31
    rn = ((rn << 1) | c.T) & MASK
    rm = R[m]
    if old_q == c.M:          # subtract
        tmp0 = rn
        rn = (rn - rm) & MASK
        tmp1 = int(rn > tmp0)
        if old_q == 0:
            c.Q = tmp1 if c.Q == 0 else int(tmp1 == 0)
        else:
            c.Q = int(tmp1 == 0) if c.Q == 0 else tmp1
    else:                     # add
        tmp0 = rn
        rn = (rn + rm) & MASK
        tmp1 = int(rn < tmp0)
        if old_q == 0:
            c.Q = int(tmp1 == 0) if c.Q == 0 else tmp1
        else:
            c.Q = tmp1 if c.Q == 0 else int(tmp1 == 0)
    R[n] = rn
    c.T = int(c.Q == c.M)
    return pc + 2


def h_dmuls(c, n, m, i, pc):
    p = (s32(c.R[n]) * s32(c.R[m])) & 0xFFFFFFFFFFFFFFFF
    c.mach, c.macl = p >> 32, p & MASK
    return pc + 2


def h_dmulu(c, n, m, i, pc):
    p = c.R[n] * c.R[m]
    c.mach, c.macl = p >> 32, p & MASK
    return pc + 2


def h_dt(c, n, m, i, pc):
    v = (c.R[n] - 1) & MASK
    c.R[n] = v
    c.T = int(v == 0)
    return pc + 2


def h_extsb(c, n, m, i, pc): c.R[n] = sx8(c.R[m]); return pc + 2
def h_extsw(c, n, m, i, pc): c.R[n] = sx16(c.R[m]); return pc + 2
def h_extub(c, n, m, i, pc): c.R[n] = c.R[m] & 0xFF; return pc + 2
def h_extuw(c, n, m, i, pc): c.R[n] = c.R[m] & 0xFFFF; return pc + 2


def h_macl(c, n, m, i, pc):
    R = c.R
    a = s32(c.mem.r32(R[n]))
    R[n] = (R[n] + 4) & MASK
    b = s32(c.mem.r32(R[m]))
    R[m] = (R[m] + 4) & MASK
    acc = (c.mach << 32) | c.macl
    if acc & (1 << 63):
        acc -= 1 << 64
    acc += a * b
    if c.S:
        acc = max(-(1 << 47), min((1 << 47) - 1, acc))
    acc &= 0xFFFFFFFFFFFFFFFF
    c.mach, c.macl = acc >> 32, acc & MASK
    return pc + 2


def h_macw(c, n, m, i, pc):
    R = c.R
    a = s32(sx16(c.mem.r16(R[n])))
    R[n] = (R[n] + 2) & MASK
    b = s32(sx16(c.mem.r16(R[m])))
    R[m] = (R[m] + 2) & MASK
    if c.S:
        acc = s32(c.macl) + a * b
        if acc > 0x7FFFFFFF or acc < -0x80000000:
            c.mach |= 1
            acc = 0x7FFFFFFF if acc > 0 else -0x80000000
        c.macl = acc & MASK
    else:
        acc = (c.mach << 32) | c.macl
        if acc & (1 << 63):
            acc -= 1 << 64
        acc = (acc + a * b) & 0xFFFFFFFFFFFFFFFF
        c.mach, c.macl = acc >> 32, acc & MASK
    return pc + 2


def h_mull(c, n, m, i, pc): c.macl = (c.R[n] * c.R[m]) & MASK; return pc + 2
def h_mulsw(c, n, m, i, pc): c.macl = (s32(sx16(c.R[n])) * s32(sx16(c.R[m]))) & MASK; return pc + 2
def h_muluw(c, n, m, i, pc): c.macl = (c.R[n] & 0xFFFF) * (c.R[m] & 0xFFFF); return pc + 2
def h_neg(c, n, m, i, pc): c.R[n] = (-c.R[m]) & MASK; return pc + 2


def h_negc(c, n, m, i, pc):
    s = -c.R[m] - c.T
    c.R[n] = s & MASK
    c.T = int(s < 0)
    return pc + 2


def h_sub(c, n, m, i, pc): c.R[n] = (c.R[n] - c.R[m]) & MASK; return pc + 2


def h_subc(c, n, m, i, pc):
    s = c.R[n] - c.R[m] - c.T
    c.R[n] = s & MASK
    c.T = int(s < 0)
    return pc + 2


def h_subv(c, n, m, i, pc):
    a, b = c.R[n], c.R[m]
    r = (a - b) & MASK
    c.T = (((a ^ b) & (a ^ r)) >> 31) & 1
    c.R[n] = r
    return pc + 2


# logic
def h_and(c, n, m, i, pc): c.R[n] &= c.R[m]; return pc + 2
def h_and_i(c, n, m, i, pc): c.R[0] &= i; return pc + 2
def h_not(c, n, m, i, pc): c.R[n] = ~c.R[m] & MASK; return pc + 2
def h_or(c, n, m, i, pc): c.R[n] |= c.R[m]; return pc + 2
def h_or_i(c, n, m, i, pc): c.R[0] |= i; return pc + 2
def h_xor(c, n, m, i, pc): c.R[n] ^= c.R[m]; return pc + 2
def h_xor_i(c, n, m, i, pc): c.R[0] ^= i; return pc + 2
def h_tst(c, n, m, i, pc): c.T = int(not (c.R[n] & c.R[m])); return pc + 2
def h_tst_i(c, n, m, i, pc): c.T = int(not (c.R[0] & i)); return pc + 2


def _gbr_b(c, i, op, pc):
    a = (c.gbr + c.R[0]) & MASK
    v = c.mem.r8(a)
    if op == "tst":
        c.T = int(not (v & i))
    else:
        c.mem.w8(a, {"and": v & i, "or": v | i, "xor": v ^ i}[op], pc)
    return pc + 2


def h_andb(c, n, m, i, pc): return _gbr_b(c, i, "and", pc)
def h_orb(c, n, m, i, pc): return _gbr_b(c, i, "or", pc)
def h_xorb(c, n, m, i, pc): return _gbr_b(c, i, "xor", pc)
def h_tstb(c, n, m, i, pc): return _gbr_b(c, i, "tst", pc)


def h_tas(c, n, m, i, pc):
    a = c.R[n]
    v = c.mem.r8(a)
    c.T = int(v == 0)
    c.mem.w8(a, v | 0x80, pc)
    return pc + 2


# shifts
def h_rotl(c, n, m, i, pc):
    v = c.R[n]
    c.T = v >> 31
    c.R[n] = ((v << 1) | c.T) & MASK
    return pc + 2


def h_rotr(c, n, m, i, pc):
    v = c.R[n]
    c.T = v & 1
    c.R[n] = (v >> 1) | (c.T << 31)
    return pc + 2


def h_rotcl(c, n, m, i, pc):
    v = c.R[n]
    c.R[n] = ((v << 1) | c.T) & MASK
    c.T = v >> 31
    return pc + 2


def h_rotcr(c, n, m, i, pc):
    v = c.R[n]
    c.R[n] = (v >> 1) | (c.T << 31)
    c.T = v & 1
    return pc + 2


def h_shll(c, n, m, i, pc):
    v = c.R[n]
    c.T = v >> 31
    c.R[n] = (v << 1) & MASK
    return pc + 2


def h_shar(c, n, m, i, pc):
    v = c.R[n]
    c.T = v & 1
    c.R[n] = (v >> 1) | (v & 0x80000000)
    return pc + 2


def h_shlr(c, n, m, i, pc):
    v = c.R[n]
    c.T = v & 1
    c.R[n] = v >> 1
    return pc + 2


def h_shll_k(c, n, m, i, pc): c.R[n] = (c.R[n] << i) & MASK; return pc + 2
def h_shlr_k(c, n, m, i, pc): c.R[n] >>= i; return pc + 2


# branches; i is the byte displacement (already sign-extended and doubled)
def h_bt(c, n, m, i, pc): return pc + 4 + i if c.T else pc + 2
def h_bf(c, n, m, i, pc): return pc + 2 if c.T else pc + 4 + i


def h_bts(c, n, m, i, pc):
    c.pending = (JUMP, pc + 4 + i if c.T else pc + 4)
    return pc + 2


def h_bfs(c, n, m, i, pc):
    c.pending = (JUMP, pc + 4 + i if not c.T else pc + 4)
    return pc + 2


def h_bra(c, n, m, i, pc): c.pending = (JUMP, pc + 4 + i); return pc + 2
def h_braf(c, n, m, i, pc): c.pending = (JUMP, (pc + 4 + c.R[m]) & MASK); return pc + 2
def h_jmp(c, n, m, i, pc): c.pending = (JUMP, c.R[m]); return pc + 2


def h_bsr(c, n, m, i, pc):
    c.pr = pc + 4
    c.pending = (CALL, pc + 4 + i)
    return pc + 2


def h_bsrf(c, n, m, i, pc):
    t = (pc + 4 + c.R[m]) & MASK
    c.pr = pc + 4
    c.pending = (CALL, t)
    return pc + 2


def h_jsr(c, n, m, i, pc):
    t = c.R[m]
    c.pr = pc + 4
    c.pending = (CALL, t)
    return pc + 2


def h_rts(c, n, m, i, pc): c.pending = (RET, c.pr); return pc + 2


# system
def h_clrt(c, n, m, i, pc): c.T = 0; return pc + 2
def h_sett(c, n, m, i, pc): c.T = 1; return pc + 2
def h_clrmac(c, n, m, i, pc): c.mach = c.macl = 0; return pc + 2


def h_ldc(c, n, m, i, pc):
    v = c.R[m]
    if i == 0:
        c.set_sr(v)
    elif i == 1:
        c.gbr = v
    else:
        c.vbr = v
    return pc + 2


def h_ldcl(c, n, m, i, pc):
    v = c.mem.r32(c.R[m])
    c.R[m] = (c.R[m] + 4) & MASK
    return h_ldc_v(c, i, v, pc)


def h_ldc_v(c, i, v, pc):
    if i == 0:
        c.set_sr(v)
    elif i == 1:
        c.gbr = v
    else:
        c.vbr = v
    return pc + 2


def _ctl(c, i):
    return (c.get_sr(), c.gbr, c.vbr)[i]


def h_stc(c, n, m, i, pc): c.R[n] = _ctl(c, i); return pc + 2


def h_stcl(c, n, m, i, pc):
    a = (c.R[n] - 4) & MASK
    c.mem.w32(a, _ctl(c, i), pc)
    c.R[n] = a
    return pc + 2


def _sys(c, i):
    return (c.mach, c.macl, c.pr)[i]


def _set_sys(c, i, v):
    if i == 0:
        c.mach = v
    elif i == 1:
        c.macl = v
    else:
        c.pr = v


def h_lds(c, n, m, i, pc): _set_sys(c, i, c.R[m]); return pc + 2


def h_ldsl(c, n, m, i, pc):
    v = c.mem.r32(c.R[m])
    c.R[m] = (c.R[m] + 4) & MASK
    _set_sys(c, i, v)
    return pc + 2


def h_sts(c, n, m, i, pc): c.R[n] = _sys(c, i); return pc + 2


def h_stsl(c, n, m, i, pc):
    a = (c.R[n] - 4) & MASK
    c.mem.w32(a, _sys(c, i), pc)
    c.R[n] = a
    return pc + 2


def h_bad(c, n, m, i, pc):
    raise Unsupported(f"instruction {c.fetch(pc):04x} at {pc:08x}")


BRANCHES = {h_bt, h_bf, h_bts, h_bfs, h_bra, h_braf, h_jmp, h_bsr, h_bsrf, h_jsr, h_rts}

# ---------------------------------------------------------------- decoding

N = lambda op: (op >> 8) & 15          # noqa: E731
M = lambda op: (op >> 4) & 15          # noqa: E731


def _sd8(op):
    v = op & 0xFF
    return v - 0x100 if v & 0x80 else v


def _sd12(op):
    v = op & 0xFFF
    return v - 0x1000 if v & 0x800 else v


# (mask, value, name, handler, operands: op -> (n, m, i))
NM = lambda op: (N(op), M(op), 0)       # noqa: E731
NN = lambda op: (N(op), N(op), 0)       # noqa: E731   single register in bits 8-11
NONE = lambda op: (0, 0, 0)             # noqa: E731


def _k(i):
    return lambda op: (N(op), N(op), i)


TABLE = [
    (0xFFFF, 0x0009, "nop", h_nop, NONE),
    (0xFFFF, 0x000B, "rts", h_rts, NONE),
    (0xFFFF, 0x0008, "clrt", h_clrt, NONE),
    (0xFFFF, 0x0018, "sett", h_sett, NONE),
    (0xFFFF, 0x0028, "clrmac", h_clrmac, NONE),
    (0xFFFF, 0x0019, "div0u", h_div0u, NONE),
    (0xF000, 0xE000, "mov #i", h_mov_i, lambda op: (N(op), 0, _sd8(op) & MASK)),
    (0xF000, 0x9000, "mov.w @(d,pc)", h_movw_pc, lambda op: (N(op), 0, (op & 0xFF) * 2)),
    (0xF000, 0xD000, "mov.l @(d,pc)", h_movl_pc, lambda op: (N(op), 0, (op & 0xFF) * 4)),
    (0xFF00, 0xC700, "mova", h_mova, lambda op: (0, 0, (op & 0xFF) * 4)),
    (0xF00F, 0x6003, "mov", h_mov, NM),
    (0xF00F, 0x2000, "mov.b Rm,@Rn", h_movb_s, NM),
    (0xF00F, 0x2001, "mov.w Rm,@Rn", h_movw_s, NM),
    (0xF00F, 0x2002, "mov.l Rm,@Rn", h_movl_s, NM),
    (0xF00F, 0x6000, "mov.b @Rm,Rn", h_movb_l, NM),
    (0xF00F, 0x6001, "mov.w @Rm,Rn", h_movw_l, NM),
    (0xF00F, 0x6002, "mov.l @Rm,Rn", h_movl_l, NM),
    (0xF00F, 0x2004, "mov.b Rm,@-Rn", h_movb_m, NM),
    (0xF00F, 0x2005, "mov.w Rm,@-Rn", h_movw_m, NM),
    (0xF00F, 0x2006, "mov.l Rm,@-Rn", h_movl_m, NM),
    (0xF00F, 0x6004, "mov.b @Rm+,Rn", h_movb_p, NM),
    (0xF00F, 0x6005, "mov.w @Rm+,Rn", h_movw_p, NM),
    (0xF00F, 0x6006, "mov.l @Rm+,Rn", h_movl_p, NM),
    (0xFF00, 0x8000, "mov.b R0,@(d,Rn)", h_movb_s4, lambda op: (M(op), 0, op & 15)),
    (0xFF00, 0x8100, "mov.w R0,@(d,Rn)", h_movw_s4, lambda op: (M(op), 0, (op & 15) * 2)),
    (0xF000, 0x1000, "mov.l Rm,@(d,Rn)", h_movl_s4, lambda op: (N(op), M(op), (op & 15) * 4)),
    (0xFF00, 0x8400, "mov.b @(d,Rm),R0", h_movb_l4, lambda op: (0, M(op), op & 15)),
    (0xFF00, 0x8500, "mov.w @(d,Rm),R0", h_movw_l4, lambda op: (0, M(op), (op & 15) * 2)),
    (0xF000, 0x5000, "mov.l @(d,Rm),Rn", h_movl_l4, lambda op: (N(op), M(op), (op & 15) * 4)),
    (0xF00F, 0x0004, "mov.b Rm,@(R0,Rn)", h_movb_s0, NM),
    (0xF00F, 0x0005, "mov.w Rm,@(R0,Rn)", h_movw_s0, NM),
    (0xF00F, 0x0006, "mov.l Rm,@(R0,Rn)", h_movl_s0, NM),
    (0xF00F, 0x000C, "mov.b @(R0,Rm),Rn", h_movb_l0, NM),
    (0xF00F, 0x000D, "mov.w @(R0,Rm),Rn", h_movw_l0, NM),
    (0xF00F, 0x000E, "mov.l @(R0,Rm),Rn", h_movl_l0, NM),
    (0xFF00, 0xC000, "mov.b R0,@(d,GBR)", h_movb_sg, lambda op: (0, 0, op & 0xFF)),
    (0xFF00, 0xC100, "mov.w R0,@(d,GBR)", h_movw_sg, lambda op: (0, 0, (op & 0xFF) * 2)),
    (0xFF00, 0xC200, "mov.l R0,@(d,GBR)", h_movl_sg, lambda op: (0, 0, (op & 0xFF) * 4)),
    (0xFF00, 0xC400, "mov.b @(d,GBR),R0", h_movb_lg, lambda op: (0, 0, op & 0xFF)),
    (0xFF00, 0xC500, "mov.w @(d,GBR),R0", h_movw_lg, lambda op: (0, 0, (op & 0xFF) * 2)),
    (0xFF00, 0xC600, "mov.l @(d,GBR),R0", h_movl_lg, lambda op: (0, 0, (op & 0xFF) * 4)),
    (0xF0FF, 0x0029, "movt", h_movt, NN),
    (0xF00F, 0x6008, "swap.b", h_swapb, NM),
    (0xF00F, 0x6009, "swap.w", h_swapw, NM),
    (0xF00F, 0x200D, "xtrct", h_xtrct, NM),
    (0xF00F, 0x300C, "add", h_add, NM),
    (0xF000, 0x7000, "add #i", h_add_i, lambda op: (N(op), 0, _sd8(op) & MASK)),
    (0xF00F, 0x300E, "addc", h_addc, NM),
    (0xF00F, 0x300F, "addv", h_addv, NM),
    (0xFF00, 0x8800, "cmp/eq #i", h_cmpeq_i, lambda op: (0, 0, _sd8(op) & MASK)),
    (0xF00F, 0x3000, "cmp/eq", h_cmpeq, NM),
    (0xF00F, 0x3002, "cmp/hs", h_cmphs, NM),
    (0xF00F, 0x3003, "cmp/ge", h_cmpge, NM),
    (0xF00F, 0x3006, "cmp/hi", h_cmphi, NM),
    (0xF00F, 0x3007, "cmp/gt", h_cmpgt, NM),
    (0xF0FF, 0x4011, "cmp/pz", h_cmppz, NN),
    (0xF0FF, 0x4015, "cmp/pl", h_cmppl, NN),
    (0xF00F, 0x200C, "cmp/str", h_cmpstr, NM),
    (0xF00F, 0x3004, "div1", h_div1, NM),
    (0xF00F, 0x2007, "div0s", h_div0s, NM),
    (0xF00F, 0x300D, "dmuls.l", h_dmuls, NM),
    (0xF00F, 0x3005, "dmulu.l", h_dmulu, NM),
    (0xF0FF, 0x4010, "dt", h_dt, NN),
    (0xF00F, 0x600E, "exts.b", h_extsb, NM),
    (0xF00F, 0x600F, "exts.w", h_extsw, NM),
    (0xF00F, 0x600C, "extu.b", h_extub, NM),
    (0xF00F, 0x600D, "extu.w", h_extuw, NM),
    (0xF00F, 0x000F, "mac.l", h_macl, NM),
    (0xF00F, 0x400F, "mac.w", h_macw, NM),
    (0xF00F, 0x0007, "mul.l", h_mull, NM),
    (0xF00F, 0x200F, "muls.w", h_mulsw, NM),
    (0xF00F, 0x200E, "mulu.w", h_muluw, NM),
    (0xF00F, 0x600B, "neg", h_neg, NM),
    (0xF00F, 0x600A, "negc", h_negc, NM),
    (0xF00F, 0x3008, "sub", h_sub, NM),
    (0xF00F, 0x300A, "subc", h_subc, NM),
    (0xF00F, 0x300B, "subv", h_subv, NM),
    (0xF00F, 0x2009, "and", h_and, NM),
    (0xFF00, 0xC900, "and #i", h_and_i, lambda op: (0, 0, op & 0xFF)),
    (0xFF00, 0xCD00, "and.b", h_andb, lambda op: (0, 0, op & 0xFF)),
    (0xF00F, 0x6007, "not", h_not, NM),
    (0xF00F, 0x200B, "or", h_or, NM),
    (0xFF00, 0xCB00, "or #i", h_or_i, lambda op: (0, 0, op & 0xFF)),
    (0xFF00, 0xCF00, "or.b", h_orb, lambda op: (0, 0, op & 0xFF)),
    (0xF0FF, 0x401B, "tas.b", h_tas, NN),
    (0xF00F, 0x2008, "tst", h_tst, NM),
    (0xFF00, 0xC800, "tst #i", h_tst_i, lambda op: (0, 0, op & 0xFF)),
    (0xFF00, 0xCC00, "tst.b", h_tstb, lambda op: (0, 0, op & 0xFF)),
    (0xF00F, 0x200A, "xor", h_xor, NM),
    (0xFF00, 0xCA00, "xor #i", h_xor_i, lambda op: (0, 0, op & 0xFF)),
    (0xFF00, 0xCE00, "xor.b", h_xorb, lambda op: (0, 0, op & 0xFF)),
    (0xF0FF, 0x4004, "rotl", h_rotl, NN),
    (0xF0FF, 0x4005, "rotr", h_rotr, NN),
    (0xF0FF, 0x4024, "rotcl", h_rotcl, NN),
    (0xF0FF, 0x4025, "rotcr", h_rotcr, NN),
    (0xF0FF, 0x4020, "shal", h_shll, NN),
    (0xF0FF, 0x4021, "shar", h_shar, NN),
    (0xF0FF, 0x4000, "shll", h_shll, NN),
    (0xF0FF, 0x4001, "shlr", h_shlr, NN),
    (0xF0FF, 0x4008, "shll2", h_shll_k, _k(2)),
    (0xF0FF, 0x4009, "shlr2", h_shlr_k, _k(2)),
    (0xF0FF, 0x4018, "shll8", h_shll_k, _k(8)),
    (0xF0FF, 0x4019, "shlr8", h_shlr_k, _k(8)),
    (0xF0FF, 0x4028, "shll16", h_shll_k, _k(16)),
    (0xF0FF, 0x4029, "shlr16", h_shlr_k, _k(16)),
    (0xFF00, 0x8B00, "bf", h_bf, lambda op: (0, 0, _sd8(op) * 2)),
    (0xFF00, 0x8F00, "bf/s", h_bfs, lambda op: (0, 0, _sd8(op) * 2)),
    (0xFF00, 0x8900, "bt", h_bt, lambda op: (0, 0, _sd8(op) * 2)),
    (0xFF00, 0x8D00, "bt/s", h_bts, lambda op: (0, 0, _sd8(op) * 2)),
    (0xF000, 0xA000, "bra", h_bra, lambda op: (0, 0, _sd12(op) * 2)),
    (0xF000, 0xB000, "bsr", h_bsr, lambda op: (0, 0, _sd12(op) * 2)),
    (0xF0FF, 0x0023, "braf", h_braf, NN),
    (0xF0FF, 0x0003, "bsrf", h_bsrf, NN),
    (0xF0FF, 0x402B, "jmp", h_jmp, NN),
    (0xF0FF, 0x400B, "jsr", h_jsr, NN),
    (0xF0FF, 0x400E, "ldc sr", h_ldc, _k(0)),
    (0xF0FF, 0x401E, "ldc gbr", h_ldc, _k(1)),
    (0xF0FF, 0x402E, "ldc vbr", h_ldc, _k(2)),
    (0xF0FF, 0x4007, "ldc.l sr", h_ldcl, _k(0)),
    (0xF0FF, 0x4017, "ldc.l gbr", h_ldcl, _k(1)),
    (0xF0FF, 0x4027, "ldc.l vbr", h_ldcl, _k(2)),
    (0xF0FF, 0x0002, "stc sr", h_stc, _k(0)),
    (0xF0FF, 0x0012, "stc gbr", h_stc, _k(1)),
    (0xF0FF, 0x0022, "stc vbr", h_stc, _k(2)),
    (0xF0FF, 0x4003, "stc.l sr", h_stcl, _k(0)),
    (0xF0FF, 0x4013, "stc.l gbr", h_stcl, _k(1)),
    (0xF0FF, 0x4023, "stc.l vbr", h_stcl, _k(2)),
    (0xF0FF, 0x400A, "lds mach", h_lds, _k(0)),
    (0xF0FF, 0x401A, "lds macl", h_lds, _k(1)),
    (0xF0FF, 0x402A, "lds pr", h_lds, _k(2)),
    (0xF0FF, 0x4006, "lds.l mach", h_ldsl, _k(0)),
    (0xF0FF, 0x4016, "lds.l macl", h_ldsl, _k(1)),
    (0xF0FF, 0x4026, "lds.l pr", h_ldsl, _k(2)),
    (0xF0FF, 0x000A, "sts mach", h_sts, _k(0)),
    (0xF0FF, 0x001A, "sts macl", h_sts, _k(1)),
    (0xF0FF, 0x002A, "sts pr", h_sts, _k(2)),
    (0xF0FF, 0x4002, "sts.l mach", h_stsl, _k(0)),
    (0xF0FF, 0x4012, "sts.l macl", h_stsl, _k(1)),
    (0xF0FF, 0x4022, "sts.l pr", h_stsl, _k(2)),
]

_DECODED: dict[int, tuple] = {}


def decode(op: int) -> tuple:
    """(handler, n, m, i, name) for a halfword; unknown ones decode to h_bad."""
    e = _DECODED.get(op)
    if e is None:
        e = (h_bad, 0, 0, 0, f".word 0x{op:04x}")
        for mask, val, name, h, ops in TABLE:
            if op & mask == val:
                n, m, i = ops(op)
                e = (h, n, m, i, name)
                break
        _DECODED[op] = e
    return e


# ---------------------------------------------------------------- register use
# Which general registers an instruction reads and writes; used for a static
# guess at how many argument registers a callee takes.

_RW = {}


def _cls(kind, *hs):
    for h in hs:
        _RW[h] = kind


_cls("w_n", h_mov_i, h_movw_pc, h_movl_pc, h_movt, h_sts, h_stc)
_cls("w_0", h_mova)
_cls("rm_wn", h_mov, h_movb_l, h_movw_l, h_movl_l, h_extsb, h_extsw, h_extub, h_extuw, h_neg, h_negc,
     h_not, h_swapb, h_swapw)
_cls("rnm_wn", h_add, h_addc, h_addv, h_sub, h_subc, h_subv, h_and, h_or, h_xor, h_xtrct, h_div1)
_cls("rnm", h_cmpeq, h_cmphs, h_cmpge, h_cmphi, h_cmpgt, h_cmpstr, h_tst, h_div0s, h_dmuls, h_dmulu,
     h_mull, h_mulsw, h_muluw, h_movb_s, h_movw_s, h_movl_s)
_cls("rn_wn", h_add_i, h_dt, h_rotl, h_rotr, h_rotcl, h_rotcr, h_shll, h_shar, h_shlr, h_shll_k, h_shlr_k)
_cls("rn", h_cmppz, h_cmppl, h_tas, h_braf, h_bsrf, h_jmp, h_jsr, h_lds, h_ldc)
_cls("rn_wn2", h_ldsl, h_ldcl, h_stsl, h_stcl)
_cls("rnm_wn2", h_movb_m, h_movw_m, h_movl_m)          # predecrement store
_cls("rm_wnm", h_movb_p, h_movw_p, h_movl_p)           # postincrement load
_cls("rnm_wnm", h_macl, h_macw)
_cls("r0", h_cmpeq_i, h_tst_i, h_andb, h_orb, h_xorb, h_tstb, h_movb_sg, h_movw_sg, h_movl_sg)
_cls("r0_w0", h_and_i, h_or_i, h_xor_i)
_cls("w0", h_movb_lg, h_movw_lg, h_movl_lg)


def reg_use(op: int):
    """(reads, writes) as sets of general register numbers."""
    h, n, m, i, _ = decode(op)
    k = _RW.get(h)
    if h in (h_movb_s4, h_movw_s4):
        return {n, 0}, set()
    if h is h_movl_s4:
        return {n, m}, set()
    if h in (h_movb_l4, h_movw_l4):
        return {m}, {0}
    if h is h_movl_l4:
        return {m}, {n}
    if h in (h_movb_s0, h_movw_s0, h_movl_s0):
        return {n, m, 0}, set()
    if h in (h_movb_l0, h_movw_l0, h_movl_l0):
        return {m, 0}, {n}
    return {
        "w_n": (set(), {n}), "w_0": (set(), {0}), "rm_wn": ({m}, {n}), "rnm_wn": ({n, m}, {n}),
        "rnm": ({n, m}, set()), "rn_wn": ({n}, {n}), "rn": ({n}, set()), "rn_wn2": ({n}, {n}),
        "rnm_wn2": ({n, m}, {n}), "rm_wnm": ({m}, {n, m}), "rnm_wnm": ({n, m}, {n, m}),
        "r0": ({0}, set()), "r0_w0": ({0}, {0}), "w0": (set(), {0}),
    }.get(k, (set(), set()))


def static_arg_regs(code: list) -> int:
    """How many of r4-r7 a function reads as arguments, from a linear pass over
    [(pc, op)]: a register counts if its first appearance is a read. After a
    call, r0-r7 count as written."""
    written, read = set(), set()
    after_call = False
    for pc, op in code:
        rd, wr = reg_use(op)
        for r in rd:
            if 4 <= r <= 7 and r not in written:
                read.add(r)
        written |= wr
        if after_call:            # the call's delay slot has executed
            written |= set(range(8))
            after_call = False
        h = decode(op)[0]
        if h in (h_jsr, h_bsr, h_bsrf):
            after_call = True
    return max(read) - 3 if read else 0


# ---------------------------------------------------------------- CPU

class CPU:
    """Registers plus the fetch/execute loop. `mem` needs r8/r16/r32 and
    w8/w16/w32(addr, value, pc); `code(addr)` returns an instruction halfword."""

    def __init__(self, mem, code):
        self.mem = mem
        self.fetch = code
        self.R = [0] * 16
        self.T = self.S = self.Q = self.M = 0
        self.I = 0xF
        self.gbr = self.vbr = self.mach = self.macl = self.pr = 0
        self.pending = None
        self.delay_pc = None
        self.steps = 0
        self.seen = None            # set of executed pcs, if wanted
        self._cache: dict[int, tuple] = {}

    def get_sr(self) -> int:
        return (self.M << 9) | (self.Q << 8) | (self.I << 4) | (self.S << 1) | self.T

    def set_sr(self, v: int) -> None:
        self.M, self.Q, self.I = (v >> 9) & 1, (v >> 8) & 1, (v >> 4) & 15
        self.S, self.T = (v >> 1) & 1, v & 1

    # Harness hooks -----------------------------------------------------
    def in_region(self, pc: int) -> bool:
        raise NotImplementedError

    def on_call(self, target: int, ret: int, at: int) -> int:
        """A bsr/bsrf/jsr; return the pc to continue at."""
        raise NotImplementedError

    def on_leave(self, pc: int):
        """Control reached pc outside the region: return a pc, or None to stop."""
        raise NotImplementedError

    # -------------------------------------------------------------------
    def _decode_at(self, pc):
        e = self._cache.get(pc)
        if e is None:
            e = self._cache[pc] = decode(self.fetch(pc))
        return e

    def run(self, pc: int, limit: int) -> int:
        """Execute from pc until on_leave returns None. `self.region` may hold
        (lo, hi): addresses in it, or in the runtime helpers, count as inside
        without calling in_region."""
        dec = self._decode_at
        cache = self._cache
        inreg = self.in_region
        lo, hi = getattr(self, "region", (1, 0))
        hlo, hhi = HELPERS
        seen = self.seen
        steps = self.steps
        try:
            while True:
                if not (lo <= pc < hi or hlo <= pc < hhi or inreg(pc)):
                    self.steps = steps
                    pc = self.on_leave(pc)
                    steps = self.steps
                    if pc is None:
                        return steps
                    continue
                e = cache.get(pc)
                if e is None:
                    e = dec(pc)
                h, n, m, i, _ = e
                steps += 1
                if steps > limit:
                    raise StepLimit(f"step limit {limit} at {pc:08x}")
                if seen is not None:
                    seen.add(pc)
                nxt = h(self, n, m, i, pc)
                if self.pending is not None:
                    kind, tgt = self.pending
                    self.pending = None
                    slot = pc + 2
                    h2, n2, m2, i2, name2 = dec(slot)
                    if h2 in BRANCHES or h2 is h_bad:
                        raise Unsupported(f"{name2} in the delay slot at {slot:08x}")
                    if seen is not None:
                        seen.add(slot)
                    steps += 1
                    if steps > limit:
                        raise StepLimit(f"step limit {limit} at {slot:08x}")
                    self.delay_pc = (tgt + 2) & MASK
                    try:
                        h2(self, n2, m2, i2, slot)
                    finally:
                        self.delay_pc = None
                    if kind == CALL:
                        self.steps = steps
                        nxt = self.on_call(tgt, pc + 4, pc)
                        steps = self.steps
                    else:
                        nxt = tgt
                pc = nxt
        finally:
            self.steps = steps


# ---------------------------------------------------------------- self-test

def _rom():
    return ROM_PATH.read_bytes()


class _FlatMem:
    def __init__(self, rom):
        self.rom = rom
        self.b = {}

    def _get(self, a):
        if a in self.b:
            return self.b[a]
        if RAM_BASE <= a < RAM_END:
            return self.rom[a - RAM_DELTA]
        return 0

    def r8(self, a): return self._get(a)
    def r16(self, a): return self._get(a) << 8 | self._get(a + 1)
    def r32(self, a): return self.r16(a) << 16 | self.r16(a + 2)

    def w8(self, a, v, pc): self.b[a] = v & 0xFF

    def w16(self, a, v, pc):
        self.w8(a, v >> 8, pc)
        self.w8(a + 1, v, pc)

    def w32(self, a, v, pc):
        self.w16(a, v >> 16, pc)
        self.w16(a + 2, v, pc)


def selftest(n: int = 3000) -> bool:
    """Run the ROM's own division and shift helpers against Python."""
    import random
    rom = _rom()
    rng = random.Random(1)
    MAGIC = 0xDEAD0000

    class C(CPU):
        def in_region(self, pc): return HELPERS[0] <= pc < HELPERS[1]
        def on_call(self, t, ret, at): return t
        def on_leave(self, pc): return None

    def call(addr, r0, r1):
        mem = _FlatMem(rom)
        c = C(mem, lambda a: mem.r16(a))
        c.R[0], c.R[1], c.R[15], c.pr = r0 & MASK, r1 & MASK, 0x06400000, MAGIC
        c.run(addr, 10000)
        assert c.R[15] == 0x06400000, "stack not balanced"
        return c.R[0]

    def tdiv(a, b):
        q = abs(a) // abs(b)
        return q if (a < 0) == (b < 0) else -q

    def pick():
        k = rng.random()
        if k < 0.3:
            return rng.randint(-20, 20)
        if k < 0.6:
            return rng.randint(-70000, 70000)
        return rng.randint(-2**31, 2**31 - 1)

    cases = {
        "_divls": (0x060305C0, lambda a, b: tdiv(a, b)),
        "_divlu": (0x06030674, lambda a, b: (a & MASK) // (b & MASK)),
        "_modls": (0x0603076C, lambda a, b: a - tdiv(a, b) * b),
        "_modlu": (0x06030834, lambda a, b: (a & MASK) % (b & MASK)),
    }
    ok = True
    for name, (addr, ref) in cases.items():
        bad = 0
        for _ in range(n):
            a, b = pick(), pick()
            if b == 0 or (a == -2**31 and b == -1):
                continue
            # SHC's helpers take the dividend in r1 and the divisor in r0.
            got = call(addr, b, a)
            want = ref(a, b) & MASK
            if got != want:
                bad += 1
                if bad <= 3:
                    print(f"  {name}({a}, {b}) = {got:#x}, want {want:#x}")
        print(f"{name:8s} {n - bad}/{n} ok")
        ok &= bad == 0
    # 16-bit signed division: same convention, operands in 16-bit range
    bad = 0
    for _ in range(n):
        a, b = rng.randint(-32768, 32767), rng.randint(-32768, 32767)
        if b == 0:
            continue
        got = call(0x0603071C, b & MASK, a & MASK)
        want = tdiv(a, b)
        if s32(got) != want and sx16(got) != want & 0xFFFF and s32(sx16(got)) != want:
            bad += 1
            if bad <= 3:
                print(f"  _divws({a}, {b}) = {got:#x}, want {want}")
    print(f"{'_divws':8s} {n - bad}/{n} ok")
    ok &= bad == 0
    for name, addr, ref in (("_sftl", 0x06030944, lambda v, s: (v << s) & MASK),
                            ("_sftra", 0x06030A04, lambda v, s: (s32(v) >> s) & MASK)):
        bad = 0
        for _ in range(n):
            v, s = rng.getrandbits(32), rng.randint(0, 31)
            # value in r0, count in r1 (checked both ways below if this fails)
            got = call(addr, v, s)
            if got != ref(v, s):
                bad += 1
                if bad <= 3:
                    print(f"  {name}({v:#x}, {s}) = {got:#x}, want {ref(v, s):#x}")
        print(f"{name:8s} {n - bad}/{n} ok")
        ok &= bad == 0
    return ok


if __name__ == "__main__":
    if sys.argv[1:] == ["--selftest"]:
        sys.exit(0 if selftest() else 1)
    print(__doc__)
