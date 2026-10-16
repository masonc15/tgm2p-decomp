#!/usr/bin/env python3
"""Differential execution: compare sampled behavior of SH-2 functions.

Compiles a case the way funcscore.py does, then runs the ROM function and the
compiled one side by side in an SH-2 interpreter (sh2emu.py) from the same
randomized state, many times, and compares what each one does to the world:

  - the memory writes between calls (address, size, value; as the final value
    of each byte per stretch between calls, so store scheduling is allowed);
  - every call in order: target, the argument registers the callee reads, and
    the stack words it reads (callee arity is measured by running the callee);
  - on return: r0 (narrowed to the declared return type; skipped for void),
    r8-r15 and MACL.

Writes into the function's own frame (below the entry sp) are internal and not
compared, and any value that points into that frame compares as "sp" without
its offset, so a different frame layout is fine. Unwritten memory reads as a
deterministic pseudo-random value of (seed, address, size): mostly small ints,
zero, -1 and pointers into a fake heap. Unwritten frame slots read the same
value regardless of address. Calls are stubbed: the result in r0 (and junk in
r1-r7, MACH, T) comes from (seed, call index, target), identical for both.
SHC's runtime helpers (division, variable shifts) are executed for real.

Verdicts: MATCHED_TRIALS (all executed trials completed without an observed
mismatch), DIFFERENT (a divergence under this harness, with replay input),
INCONCLUSIVE (any unsupported or truncated trial, or no completed trials).
Exit status 0 / 1 / 2, 3 for a compile/setup error.

This is a heuristic testing aid, NOT a proof of equivalence or register-only
mismatches. Calls are stubbed; arity is inferred from sampled callee paths;
stack pointers lose their offsets in comparisons; writes between calls are
compared by their final values. Hardware effects and callee memory side effects
are not modeled. Low coverage, invalid randomized pointers, relocation mistakes
and wrong function extents can mislead either verdict. Confirm divergences
against valid inputs and disassembly. Always use byte-exact build verification
before linking. --rom-end overrides the heuristic ROM boundary for one function;
it is necessary when fn.py mistakes an internal indirect jump for a tail call.

Run on nuada (SHC lives there):
  difftest.py <case.c> [func] [--at name=0xaddr] [--trials 200] [--trace SEED]

usage: difftest.py <case.c> [func] [--at name=0xaddr]... [--flags "..."] [--version v5.0r32]
                   [--trials N] [--seed S] [--steps N] [--ret auto|none|8|16|32] [--obj file.o]
                   [--trace SEED] [-v]
"""
from __future__ import annotations

import argparse
import hashlib
import json
import random
import re
import struct
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))

import sh2emu as emu  # noqa: E402
from sh2emu import MASK, RAM_DELTA, RAM_BASE, RAM_END, HELPERS  # noqa: E402

ROM = emu.ROM_PATH.read_bytes()
ELF = ROOT / "build" / "tgm2p.elf"
CACHE = Path.home() / ".cache" / "tgm2p-difftest" / "arity-v4.json"

CAND_BASE = 0x0E000000        # where the compiled .text is placed
DATA_BASE = 0x0D000000        # other sections of the object, if referenced
MAGIC = 0x0DEAD000            # return address handed to the function
SP0 = 0x063FFF00              # entry sp
SP_LO = SP0 - 0x10000         # [SP_LO, SP0) is the function's own frame
HEAP = 0x06100000             # fake pointers land in [HEAP, HEAP + 1 MB)


def rom_name(a: int) -> str:
    if isinstance(a, tuple):
        return f"cand+{a[1]:#x}"
    if RAM_BASE <= a < RAM_END:
        return f"f_{a - RAM_DELTA:x}"
    return f"{a:#010x}"


def mix(*xs) -> int:
    """splitmix64 over a few ints."""
    h = 0x9E3779B97F4A7C15
    for x in xs:
        h = (h ^ (x & 0xFFFFFFFFFFFFFFFF)) * 0xBF58476D1CE4E5B9 & 0xFFFFFFFFFFFFFFFF
        h ^= h >> 31
        h = h * 0x94D049BB133111EB & 0xFFFFFFFFFFFFFFFF
        h ^= h >> 29
    return h


# Interesting values: the ROM function's immediates and pool constants, each
# +-1 (set by main() before any run). Boundary tests like `x < 3` vs `x <= 3`
# only differ when x is exactly 3, which uniform noise almost never produces.
DICT: list = []
TINY = {4: (0, 1, 2, MASK, 0x06100000, 0x06100100), 2: (0, 1, 2, 0xFFFF), 1: (0, 1, 2, 0xFF)}


def profile(seed: int) -> int:
    """Trials rotate through four fill profiles: 0 plain, 1 zero-heavy,
    2 dictionary-heavy, 3 a tiny pool (0, 1, 2, -1 and two pointers, so equal
    values and aliasing pointers are common)."""
    return seed % 4


def fill_value(h: int, size: int, prof: int = 0) -> int:
    """A plausible value for an unwritten location."""
    k, r = h & 0xFF, h >> 8
    mk = (1 << 8 * size) - 1
    if prof == 3 and k < 180:
        t = TINY[size]
        return t[r % len(t)]
    if DICT and k >= 256 - (128 if prof == 2 else 48 if prof == 3 else 40):
        return DICT[r % len(DICT)] & mk
    z = 80 if prof == 1 else 0
    if size == 4:
        if k < 26 + z:
            return 0
        k -= z
        if k < 90:
            return r % 16
        if k < 115:
            return (-1 - r % 8) & MASK
        if k < 175:
            return HEAP + (r & 0xFFFFC)
        return r & MASK
    if size == 2:
        if k < 38 + z:
            return 0
        k -= z
        if k < 115:
            return r % 16
        if k < 140:
            return (-1 - r % 8) & 0xFFFF
        return r & 0xFFFF
    if k < 50 + z:
        return 0
    k -= z
    if k < 130:
        return r % 8
    if k < 150:
        return 0xFF
    return r & 0xFF


def call_result(h: int) -> int:
    """What a stubbed call returns: mostly 0 or 1, so both ways out of
    `if (f(...))` get taken."""
    k, r = h & 0xFF, h >> 8
    if k < 100:
        return 0
    if k < 160:
        return 1
    if k < 190:
        return r % 16
    if k < 205:
        return MASK
    if k < 230 and DICT:
        return DICT[r % len(DICT)]
    return fill_value(r, 4)


def build_dict(rom: int, code_end: int) -> list:
    import fn
    _, _, pool, _ = fn.extent(rom)
    vals = set()
    for a in range(rom, code_end, 2):
        if a in pool:
            continue
        w = fn.hw(a)
        h = emu.decode(w)[0]
        if h in (emu.h_mov_i, emu.h_cmpeq_i, emu.h_add_i):
            vals.add(emu.sx8(w))
        elif h in (emu.h_tst_i, emu.h_and_i, emu.h_or_i, emu.h_xor_i):
            vals.add(w & 0xFF)
        elif h is emu.h_movw_pc:
            vals.add(emu.sx16(fn.hw(a + 4 + (w & 0xFF) * 2)))
        elif h is emu.h_movl_pc:
            t = ((a + 4) & ~3) + (w & 0xFF) * 4
            v = struct.unpack(">I", ROM[t:t + 4])[0]
            if not (0x02000000 <= v < 0x30000000 or 0x30000 <= v < 0x100000):   # not an address
                vals.add(v)
    out = set()
    for v in vals:
        for d in (-1, 0, 1):
            out.add((v + d) & MASK)
    return sorted(out)


def norm(v):
    """Values that point into the function's own frame compare without offset."""
    return "sp" if isinstance(v, int) and SP_LO <= v < SP0 else v


# ---------------------------------------------------------------- memory

class Mem:
    """Sparse byte memory. Reads of bytes nobody wrote come from the ROM image,
    the candidate's code, or the pseudo-random fill, and are then remembered,
    so overlapping reads agree. Writes outside the frame are logged."""

    def __init__(self, seed: int, static, log, ovr=None):
        self.b: dict[int, int] = {}
        self.seed = seed
        self.prof = profile(seed)
        self.static = static
        self.log = log
        self.ovr = ovr or {}        # (addr, size) -> value, from input-to-state mutation
        self.inputs = []            # fresh fill reads: (addr, size, value)

    def _slow(self, a: int, size: int) -> int:
        b = self.b
        fv = None
        out = 0
        for k in range(size):
            x = (a + k) & MASK
            v = b.get(x)
            if v is None:
                v = self.static(x)
                if v is None:
                    if fv is None:
                        if SP_LO <= a < SP0:
                            fv = fill_value(mix(self.seed, 0x5354, size), size, self.prof)
                        else:
                            fv = self.ovr.get((a, size))
                            if fv is None:
                                fv = fill_value(mix(self.seed, a, size), size, self.prof)
                            self.inputs.append((a, size, fv))
                    v = (fv >> (8 * (size - 1 - k))) & 0xFF
                b[x] = v
            out = out << 8 | v
        return out

    def r8(self, a):
        try:
            return self.b[a]
        except KeyError:
            return self._slow(a, 1)

    def r16(self, a):
        b = self.b
        try:
            return b[a] << 8 | b[a + 1]
        except KeyError:
            return self._slow(a, 2)

    def r32(self, a):
        b = self.b
        try:
            return b[a] << 24 | b[a + 1] << 16 | b[a + 2] << 8 | b[a + 3]
        except KeyError:
            return self._slow(a, 4)

    def _w(self, a, v, size, pc):
        b = self.b
        for k in range(size):
            b[(a + k) & MASK] = (v >> (8 * (size - 1 - k))) & 0xFF
        if not SP_LO <= a < SP0 and self.log is not None:
            self.log.append(("W", a, size, v & ((1 << 8 * size) - 1), pc))

    def w8(self, a, v, pc): self._w(a, v, 1, pc)
    def w16(self, a, v, pc): self._w(a, v, 2, pc)
    def w32(self, a, v, pc): self._w(a, v, 4, pc)


# ---------------------------------------------------------------- programs

class Program:
    """One side of the comparison: its code, its region and how to name the
    addresses it calls."""

    def __init__(self, label, start, end, static, entry_name, cand_funcs=None, rom_of=None):
        self.label = label
        self.start, self.end = start, end
        self.static = static
        self.entry_name = entry_name
        self.cand_funcs = cand_funcs or {}   # candidate .text offset -> name
        self.rom_of = rom_of                 # name -> RAM address or None

    def fetch(self, a: int) -> int:
        hi, lo = self.static(a), self.static(a + 1)
        if hi is None or lo is None:
            raise emu.Unsupported(f"executing unmapped address {a:#x}")
        return hi << 8 | lo

    def target(self, t: int):
        """A call/jump target in the ROM's address space (or a tag)."""
        if self.rom_of is not None and CAND_BASE <= t < CAND_BASE + len(CAND_TEXT):
            off = t - CAND_BASE
            name = self.cand_funcs.get(off)
            if name:
                r = self.rom_of(name)
                if r is not None:
                    return r
            return ("cand", off)
        return t

    def rel(self, pc) -> str:
        """pc as an offset into the function, comparable across the two sides."""
        if pc is None:
            return "tail"
        if self.start <= pc < self.end:
            return f"+{pc - self.start:#x}"
        if HELPERS[0] <= pc < HELPERS[1]:
            return f"helper {pc - RAM_DELTA:#x}"
        return f"{pc:#x}"


# Compare instructions whose operands feed input-to-state mutation:
# handler -> how to read (x, y) before it runs.
_CMPS = {emu.h_cmpeq: "nm", emu.h_cmphs: "nm", emu.h_cmpge: "nm", emu.h_cmphi: "nm",
         emu.h_cmpgt: "nm", emu.h_tst: "tst_nm", emu.h_cmpeq_i: "0i", emu.h_tst_i: "tst_0i",
         emu.h_cmppz: "n0", emu.h_cmppl: "n0", emu.h_dt: "dt"}


def _cmp_watch(h, kind):
    def w(c, n, m, i, pc):
        R = c.R
        if kind in ("nm", "tst_nm"):
            x, y = R[n], R[m]
        elif kind in ("0i", "tst_0i"):
            x, y = R[0], i
        elif kind == "n0":
            x, y = R[n], 0
        else:                       # dt: Rn - 1 == 0
            x, y = R[n], 1
        r = h(c, n, m, i, pc)
        c.cmps.append((pc, x, y, kind, c.T, len(c.mem.inputs), len(c.call_inputs)))
        return r
    return w


class Run(emu.CPU):
    def __init__(self, prog: Program, mem: Mem, ctx, seed: int):
        super().__init__(mem, prog.fetch)
        self.prog = prog
        self.ctx = ctx
        self.seed = seed
        self.events = mem.log
        self.ncalls = 0
        self.limit = 0
        self.region = (prog.start, prog.end)
        self.call_ovr = {}          # call index -> result
        self.call_inputs = []       # (call index, result)
        self.cmps = None            # [(pc, x, y, kind, T)] when exploring

    def _decode_at(self, pc):
        e = self._cache.get(pc)
        if e is None:
            e = emu.decode(self.fetch(pc))
            if self.cmps is not None and e[0] in _CMPS:
                e = (_cmp_watch(e[0], _CMPS[e[0]]),) + e[1:]
            self._cache[pc] = e
        return e

    def in_region(self, pc):
        return self.prog.start <= pc < self.prog.end or HELPERS[0] <= pc < HELPERS[1]

    def on_call(self, t, ret, at):
        if HELPERS[0] <= t < HELPERS[1] or self.prog.start < t < self.prog.end:
            return t
        self.stub(t, at)
        return ret

    def on_leave(self, pc):
        if pc == MAGIC:
            return None
        # A tail call (bra/jmp out, or falling into the next function).
        self.stub(pc, None, tail=True)
        return self.pr

    def stub(self, t, at, tail=False):
        self.steps += 1
        if self.steps > self.limit:
            raise emu.StepLimit("step limit in calls")
        R = self.R
        tgt = self.prog.target(t)
        nregs, nstack = self.ctx.arity(tgt)
        regs = tuple(norm(R[4 + k]) for k in range(nregs))
        sp = R[15]
        stack = tuple(norm(self.mem.r32((sp + 4 * k) & MASK)) for k in range(nstack))
        self.events.append(("C", tgt, regs, stack, at, tail))
        key = tgt if isinstance(tgt, int) else 0xC0DE0000 + tgt[1]
        h = mix(self.seed, 0xCA11, self.ncalls, key)
        r0 = self.call_ovr.get(self.ncalls)
        if r0 is None:
            r0 = call_result(h)
        self.call_inputs.append((self.ncalls, r0))
        self.ncalls += 1
        R[0] = r0
        for k in range(1, 8):
            R[k] = mix(h, k) & MASK
        self.mach = mix(h, 9) & MASK
        self.T = h >> 63


def initial_state(seed: int):
    rng = random.Random(seed)
    R = [rng.getrandbits(32) for _ in range(16)]
    for k in range(4, 8):
        x = rng.random()
        if x < 0.1:
            R[k] = 0
        elif x < 0.35:
            R[k] = rng.randint(-2, 20) & MASK
        elif x < 0.5 and DICT:
            R[k] = rng.choice(DICT)
        elif x < 0.6 and profile(seed) == 3:
            R[k] = rng.choice(TINY[4])
        else:
            R[k] = HEAP + (rng.getrandbits(20) & 0xFFFFC)
    R[15] = SP0
    return dict(R=R, mach=rng.getrandbits(32), macl=rng.getrandbits(32), T=rng.getrandbits(1),
                Q=rng.getrandbits(1), M=rng.getrandbits(1), gbr=HEAP + (rng.getrandbits(20) & 0xFFFFC))


def execute(prog: Program, ctx, seed: int, limit: int, seen=None, ovr=None, explore=False):
    """Run one side from the seed's state (plus any overrides: ('m', addr,
    size) or ('c', call index) -> value). Returns (events, steps, status, cpu)
    where status is 'ok', 'limit' or an Unsupported message."""
    ovr = ovr or {}
    log = []
    mem = Mem(seed, prog.static, log, {k[1:]: v for k, v in ovr.items() if k[0] == "m"})
    c = Run(prog, mem, ctx, seed)
    c.call_ovr = {k[1]: v for k, v in ovr.items() if k[0] == "c"}
    if explore:
        c.cmps = []
    st = initial_state(seed)
    c.R = list(st["R"])
    c.mach, c.macl, c.T, c.Q, c.M, c.gbr = st["mach"], st["macl"], st["T"], st["Q"], st["M"], st["gbr"]
    c.pr = MAGIC
    c.limit = limit
    c.seen = seen
    status = "ok"
    try:
        c.run(prog.start, limit)
    except emu.StepLimit:
        status = "limit"
    except emu.Unsupported as e:
        status = f"unsupported: {e}"
    if status == "ok":
        R = c.R
        log.append(("R", R[0], tuple(R[8:16]) + (c.macl,), None, None, None))
    return log, c.steps, status, c


# ---------------------------------------------------------------- callee arity

class TrackR(list):
    """Register file that notes which of r4-r7 are read before written."""

    def __init__(self, vals):
        super().__init__(vals)
        self.written = set()
        self.read = set()

    def __getitem__(self, i):
        if 4 <= i <= 7 and i not in self.written:
            self.read.add(i)
        return list.__getitem__(self, i)

    def __setitem__(self, i, v):
        self.written.add(i)
        list.__setitem__(self, i, v)


class ArityMem(Mem):
    def __init__(self, seed, static):
        super().__init__(seed, static, None)
        self.stack_words = set()

    def note(self, a, size):
        if SP0 <= a < SP0 + 0x40:
            self.stack_words.add((a - SP0) // 4)
            self.stack_words.add((a + size - 1 - SP0) // 4)


# Loads addressed off r15 itself: the only way SHC code reads stack arguments.
# (A frame pointer copied out of r15 and indexed by data is a local array.)
_SP_LOADS = {emu.h_movl_l4: 4, emu.h_movw_l4: 2, emu.h_movb_l4: 1,
             emu.h_movl_l: 4, emu.h_movw_l: 2, emu.h_movb_l: 1,
             emu.h_movl_l0: 4, emu.h_movw_l0: 2, emu.h_movb_l0: 1}


def _sp_watch(h, size):
    def w(c, n, m, i, pc):
        if h in (emu.h_movl_l0, emu.h_movw_l0, emu.h_movb_l0):
            a = (c.R[m] + c.R[0]) & MASK
        else:
            a = (c.R[m] + i) & MASK
        c.mem.note(a, size)
        return h(c, n, m, i, pc)
    return w


class ArityRun(emu.CPU):
    def __init__(self, start, end, mem):
        super().__init__(mem, lambda a: rom_static(a) << 8 | rom_static(a + 1))
        self.start, self.end = start, end
        self.region = (start, end)

    def _decode_at(self, pc):
        e = self._cache.get(pc)
        if e is None:
            e = emu.decode(self.fetch(pc))
            if e[0] in _SP_LOADS and e[2] == 15:
                e = (_sp_watch(e[0], _SP_LOADS[e[0]]),) + e[1:]
            self._cache[pc] = e
        return e

    def in_region(self, pc):
        return self.start <= pc < self.end or HELPERS[0] <= pc < HELPERS[1]

    def on_call(self, t, ret, at):
        if HELPERS[0] <= t < HELPERS[1] or self.start < t < self.end:
            return t
        self._junk()
        return ret

    def on_leave(self, pc):
        if pc == MAGIC:
            return None
        self._junk()
        return None   # a tail call: the rest is the other function's business

    def _junk(self):
        for k in range(8):
            self.R[k] = mix(k, self.steps) & MASK


def reachable_code(rom, code_end):
    """[(pc, halfword)] in address order for the instructions that control
    flow reaches from `rom` without leaving [rom, code_end), following SHC
    switch tables; None when another indirect jump leaves that set unknown.
    Literal pools are never reached, so a pool word fn.py's pool detection
    missed cannot pass for an argument read (f_2e5aa's pool at 0x2e5c4 holds
    0x1878, which decodes as `mov.l r7,@(32,r8)`)."""
    import asm_scan
    import fn
    f = asm_scan.Func(rom, code_end, set(), rom, code_end)
    if f.unknown_jumps:
        return None
    return [(pc, fn.hw(pc)) for pc in sorted(f.insns) if f.insns[pc].valid]


class Ctx:
    def __init__(self):
        self.cache = {}
        try:
            self.disk = json.loads(CACHE.read_text())
        except (OSError, ValueError):
            self.disk = {}
        self.dirty = False

    def arity(self, tgt):
        """(argument registers, stack argument words) a callee reads."""
        if not isinstance(tgt, int):
            return 1, 0
        if tgt in self.cache:
            return self.cache[tgt]
        key = f"{tgt:08x}"
        if key in self.disk:
            r = tuple(self.disk[key])
        else:
            r = self._measure(tgt)
            self.disk[key] = list(r)
            self.dirty = True
        self.cache[tgt] = r
        return r

    def _measure(self, tgt):
        rom = tgt - RAM_DELTA if RAM_BASE <= tgt < RAM_END else tgt
        if not (emu.RAM_ROM <= rom < emu.RAM_ROM_END) or rom % 2:
            # a function pointer we can't see into, or the boot block, which
            # runs from ROM and has no RAM copy to run (f_a226 leaves through
            # `jmp 0x686`, the warm-reset entry): compare r4 only
            return (1, 0)
        import fn
        code_end = fn.extent(rom)[0]
        start, end = tgt, tgt + (code_end - rom)
        regs, words = set(), set()
        for s in range(48):
            mem = ArityMem(0xA417 + s, static_all)
            c = ArityRun(start, end, mem)
            st = initial_state(0xA417 + s)
            c.R = TrackR(st["R"])
            c.pr = MAGIC
            try:
                c.run(start, 20000)
            except (emu.StepLimit, emu.Unsupported):
                pass
            regs |= c.R.read
            words |= mem.stack_words
        nregs = max(regs) - 3 if regs else 0
        nstack = max(words) + 1 if words else 0
        # Paths the random runs missed can still read an argument; take the
        # static guess too (first appearance of r4-r7 in code order is a read).
        code = reachable_code(rom, code_end)
        if code is None:
            _, _, pool, _ = fn.extent(rom)
            code = [(a, fn.hw(a)) for a in range(rom, code_end, 2) if a not in pool]
        nregs = max(nregs, emu.static_arg_regs(code))
        return (nregs, nstack)

    def save(self):
        """Merge into the on-disk cache and replace it atomically (several
        agents may run difftest at once)."""
        if not self.dirty:
            return
        import os
        import tempfile
        CACHE.parent.mkdir(parents=True, exist_ok=True)
        try:
            merged = json.loads(CACHE.read_text())
        except (OSError, ValueError):
            merged = {}
        merged.update(self.disk)
        fd, tmp = tempfile.mkstemp(dir=CACHE.parent, prefix=".arity.")
        with os.fdopen(fd, "w") as f:
            json.dump(merged, f, indent=0, sort_keys=True)
        os.replace(tmp, CACHE)
        self.dirty = False


# ---------------------------------------------------------------- static memory

def rom_static(a: int):
    """ROM bytes where the game reads them: the code copy in RAM (pools, jump
    tables) and the data after the copied block (d_ symbols), directly or via
    the cache-through mirror. Low ROM (vectors, boot code, the code's ROM
    image) reads as fill, so a null or small-int pointer doesn't see the same
    vector-table bytes in every trial."""
    if RAM_BASE <= a < RAM_END:
        return ROM[a - RAM_DELTA]
    if 0x20000000 <= a < 0x20000000 + len(ROM):     # cache-through mirror
        a -= 0x20000000
    if emu.RAM_ROM_END <= a < len(ROM):
        return ROM[a]
    if 0x26000000 <= a < 0x26000000 + (RAM_END - RAM_BASE):
        return ROM[a - 0x26000000 + emu.RAM_ROM]
    return None


CAND_TEXT = bytearray()
CAND_DATA: dict[int, bytes] = {}


def static_all(a: int):
    if CAND_BASE <= a < CAND_BASE + len(CAND_TEXT):
        return CAND_TEXT[a - CAND_BASE]
    if DATA_BASE <= a < CAND_BASE:
        blk = CAND_DATA.get((a - DATA_BASE) >> 20)
        off = (a - DATA_BASE) & 0xFFFFF
        if blk is not None and off < len(blk):
            return blk[off]
        return None
    return rom_static(a)


# ---------------------------------------------------------------- the object

def elf_symbols():
    """name -> address from the linked build, for names like field_clear_flag."""
    if not ELF.exists():
        return {}
    from elftools.elf.elffile import ELFFile
    with ELF.open("rb") as f:
        st = ELFFile(f).get_section_by_name(".symtab")
        return {s.name.lstrip("_"): s["st_value"] for s in st.iter_symbols()
                if s.name and s["st_shndx"] != "SHN_UNDEF"}


def read_symbols_txt():
    out = {}
    for line in (ROOT / "symbols.txt").read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            n, a = line.split()[:2]
            out[n] = int(a, 16)
    return out


AUTO = re.compile(r"^(f|d|g)_([0-9a-f]+)$")


class Resolver:
    def __init__(self, where: dict):
        self.where = where          # C name -> ROM offset (from header / --at / f_ names)
        self.syms = read_symbols_txt()
        self._elf = None
        self.notes = []

    def __call__(self, name: str):
        """Address the running game sees for a C name, or None."""
        if name in self.where:
            a = self.where[name]
            return a + RAM_DELTA if emu.RAM_ROM <= a < emu.RAM_ROM_END else a
        if name in self.syms:
            return self.syms[name]
        m = AUTO.match(name)
        if m:
            a = int(m[2], 16)
            if m[1] == "f" and emu.RAM_ROM <= a < emu.RAM_ROM_END:
                a += RAM_DELTA
            return a
        if self._elf is None:
            self._elf = elf_symbols()
        return self._elf.get(name)


def load_object(path: Path):
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    with path.open("rb") as f:
        elf = ELFFile(f)
        secs = list(elf.iter_sections())
        tidx = next(i for i, s in enumerate(secs) if s.name == ".text")
        text = bytearray(secs[tidx].data())
        st = elf.get_section_by_name(".symtab")
        syms = list(st.iter_symbols())
        labels = {}
        for s in syms:
            if s["st_shndx"] == tidx and s.name and s["st_info"]["type"] in ("STT_FUNC", "STT_NOTYPE") \
                    and not s.name.startswith("."):
                labels.setdefault(s["st_value"], s.name.lstrip("_"))
        funcs = sorted((s["st_value"], s.name.lstrip("_")) for s in syms
                       if s["st_shndx"] == tidx and s["st_info"]["type"] == "STT_FUNC")
        relocs = []
        for sec in secs:
            if isinstance(sec, RelocationSection) and sec["sh_info"] != tidx:
                if secs[sec["sh_info"]]["sh_flags"] & 2 and sec.num_relocations():
                    raise emu.Unsupported(f"relocations in {secs[sec['sh_info']].name} are not modeled")
            if isinstance(sec, RelocationSection) and sec["sh_info"] == tidx:
                for r in sec.iter_relocations():
                    s = syms[r["r_info_sym"]]
                    shndx = s["st_shndx"]
                    relocs.append(dict(off=r["r_offset"], type=r["r_info_type"],
                                       name=s.name.lstrip("_") if s.name else None,
                                       sect=secs[shndx].name if isinstance(shndx, int) else None,
                                       shndx=shndx if isinstance(shndx, int) else None,
                                       kind=s["st_info"]["type"], value=s["st_value"],
                                       addend=r.entry.get("r_addend", 0) if sec.is_RELA() else None))
        sections = {i: (s.name, s.data() if s["sh_type"] != "SHT_NOBITS" else bytes(s["sh_size"]))
                    for i, s in enumerate(secs) if s["sh_flags"] & 2}   # SHF_ALLOC
    return text, funcs, labels, relocs, sections, tidx


def link(text, funcs, labels, relocs, sections, tidx, resolve, spans):
    """Apply candidate relocations without borrowing expected values from ROM."""
    notes = []
    for i, (name, data) in sections.items():
        if i != tidx:
            CAND_DATA[i] = data
    for r in relocs:
        if r["type"] != 1:   # R_SH_DIR32 is all SHC emits for pool words
            raise emu.Unsupported(f"relocation type {r['type']} at .text+{r['off']:#x}")
        # SH's R_SH_DIR32 is partial-inplace: rof2elf leaves the offset in the
        # word itself (g_6060034+0xc is 0x0000000c with addend 0), so add both.
        add = (r["addend"] or 0) + struct.unpack(">I", text[r["off"]:r["off"] + 4])[0]
        base = None
        if r["kind"] == "STT_SECTION" or not r["name"]:
            if r["shndx"] == tidx:
                # .text+off: a function's address (as the ROM's pool has it)
                # or an internal label the code jumps through (asmsh turns long
                # branches into mov.l/jmp), which stays in the candidate.
                tgt_off = add + (r["value"] or 0)
                if tgt_off in labels and resolve(labels[tgt_off]) is not None:
                    base, add = resolve(labels[tgt_off]), 0
                else:
                    base, add = CAND_BASE, tgt_off
        elif r["shndx"] == tidx and r["value"] in labels and resolve(labels[r["value"]]) is not None:
            base = resolve(labels[r["value"]])
        elif r["shndx"] is None or r["shndx"] == 0:
            base = resolve(r["name"])
        elif r["shndx"] == tidx:
            base = resolve(r["name"])
        if base is None:
            if r["shndx"] == tidx:
                base = CAND_BASE + (r["value"] or 0)
            elif r["shndx"] in sections:
                base = DATA_BASE + (r["shndx"] << 20) + (r["value"] or 0)
            else:
                raise emu.Unsupported(f"unresolved symbol {r['name']!r} at .text+{r['off']:#x}; "
                                      "supply a real symbol mapping instead of borrowing ROM data")
            notes.append(f"pool word .text+{r['off']:#x} ({r['name'] or r['sect']}) unresolved; "
                         f"placed at {base + add:#x} (ROM side will differ if it uses this)")
        struct.pack_into(">I", text, r["off"], (base + add) & MASK)
    return notes


# ---------------------------------------------------------------- comparison

def segments(events):
    segs, cur = [], []
    for e in events:
        if e[0] == "W":
            cur.append(e)
        else:
            segs.append((cur, e))
            cur = []
    return segs, cur


def bytemap(writes):
    out = {}
    for _, a, size, v, _pc in writes:
        if size == 4 and norm(v) == "sp":
            for k in range(4):
                out[(a + k) & MASK] = ("sp", k)
            continue
        for k in range(size):
            out[(a + k) & MASK] = (v >> (8 * (size - 1 - k))) & 0xFF
    return out


def fmt_target(t):
    return rom_name(t)


def fmt_vals(vs):
    return "(" + ", ".join(v if isinstance(v, str) else f"{v:#x}" for v in vs) + ")"


def fmt_w(e, prog):
    _, a, size, v, pc = e
    return f"{ {1: 'b', 2: 'w', 4: 'l'}[size]}[{a:#010x}] = {v:#x}  @{prog.rel(pc)}"


def compare(rev, cev, rprog, cprog, ret_bits, trunc):
    """None if the traces agree (as far as both got), else a description.
    Returns (diff or None, reordered: bool)."""
    rs, rtail = segments(rev)
    cs, ctail = segments(cev)
    reordered = False
    n = min(len(rs), len(cs))
    for i in range(n):
        (rw, re_), (cw, ce) = rs[i], cs[i]
        where = "before the first call" if i == 0 else f"after call #{i}"
        rm, cm = bytemap(rw), bytemap(cw)
        if rm != cm:
            bad = sorted(a for a in set(rm) | set(cm) if rm.get(a) != cm.get(a))
            a0 = bad[0]
            lines = [f"memory writes {where} differ (first at {a0:#010x})"]
            lines += ["  ROM:       " + fmt_w(e, rprog) for e in rw if e[1] <= a0 < e[1] + e[2]][-3:] or \
                     ["  ROM:       (no write)"]
            lines += ["  candidate: " + fmt_w(e, cprog) for e in cw if e[1] <= a0 < e[1] + e[2]][-3:] or \
                     ["  candidate: (no write)"]
            return "\n".join(lines), reordered
        if [(e[1], e[2], norm(e[3])) for e in rw] != [(e[1], e[2], norm(e[3])) for e in cw]:
            reordered = True
        if re_[0] != ce[0]:
            def what(e, p):
                return (f"call {fmt_target(e[1])} @{p.rel(e[4])}" if e[0] == "C" else "return")
            return (f"event #{i}: ROM does {what(re_, rprog)}, candidate does {what(ce, cprog)}"), reordered
        if re_[0] == "C":
            _, rt, rr, rst, rpc, _ = re_
            _, ct, cr, cst, cpc, _ = ce
            if rt != ct:
                return (f"call #{i + 1}: ROM calls {fmt_target(rt)} @{rprog.rel(rpc)}, "
                        f"candidate calls {fmt_target(ct)} @{cprog.rel(cpc)}"), reordered
            if rr != cr or rst != cst:
                return (f"call #{i + 1} to {fmt_target(rt)}: arguments differ\n"
                        f"  ROM:       r4.. {fmt_vals(rr)} stack {fmt_vals(rst)}  @{rprog.rel(rpc)}\n"
                        f"  candidate: r4.. {fmt_vals(cr)} stack {fmt_vals(cst)}  @{cprog.rel(cpc)}"), reordered
        else:
            r0r, r0c = re_[1], ce[1]
            if ret_bits:
                mk = (1 << ret_bits) - 1
                if r0r & mk != r0c & mk:
                    return f"return value differs: ROM r0={r0r & mk:#x}, candidate r0={r0c & mk:#x}", reordered
            names = ["r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "macl"]
            for nm, a, b in zip(names, re_[2], ce[2]):
                if a != b:
                    return f"on return {nm} differs: ROM {a:#x}, candidate {b:#x}", reordered
    if not trunc and len(rs) != len(cs):
        longer, p = (rs, rprog) if len(rs) > len(cs) else (cs, cprog)
        e = longer[n][1]
        who = "ROM" if longer is rs else "candidate"
        return (f"{who} makes an extra call #{n + 1} to {fmt_target(e[1])} @{p.rel(e[4])}"
                if e[0] == "C" else f"{who} returns where the other calls"), reordered
    return None, reordered


# ---------------------------------------------------------------- input-to-state mutation

def cmp_class(x, y, kind, T):
    """What a compare's outcome contributes to coverage: its T result and, for
    ordered compares, whether x was y-1, y or y+1 (boundary values)."""
    if kind.startswith("tst"):
        return (T,)
    d = emu.s32(x) - emu.s32(y)
    return (T, d) if -1 <= d <= 1 else (T,)


def cmp_done(cls: set, kind: str) -> bool:
    ts = {c[0] for c in cls}
    if kind.startswith("tst") or kind == "dt":
        return len(ts) == 2
    return len(ts) == 2 and {c[1] for c in cls if len(c) == 2} >= {-1, 0, 1}


def mutations(c, ovr, outcomes):
    """New override sets that try to flip compares that have only gone one
    way: an input (fill read or call result) whose value shows up as one
    operand is replaced with the other operand (+-1), or bit-flipped for tst."""
    out = []
    done = set()
    for pc, x, y, kind, T, nm, nc in c.cmps:
        if cmp_done(outcomes.get(pc, set()), kind) or (pc, x, y) in done:
            continue
        done.add((pc, x, y))
        # inputs read before this compare, latest first
        inputs = [(("c", k), 4, v) for k, v in reversed(c.call_inputs[:nc])]
        inputs += [(("m", a, size), size, v) for a, size, v in reversed(c.mem.inputs[:nm])]
        for a, b in ((x, y), (y, x)):
            for mk in (0xFF, 0xFFFF, MASK):
                keys = [key for key, size, v in inputs if (1 << 8 * size) - 1 == mk and v & mk == a & mk]
                for key in keys[:3]:
                    if a == b:
                        news = [(b + d) & mk for d in (-1, 1)]
                    elif kind.startswith("tst"):
                        news = [a & ~b & mk, (a | b) & mk] if a & b else [(a | b) & mk, 0]
                    else:
                        news = [(b + d) & mk for d in (-1, 0, 1)]
                    for nv in news:
                        if ovr.get(key) != nv:
                            o = dict(ovr)
                            o[key] = nv
                            out.append((pc, kind, o))
    return out


def ovr_str(seed, ovr):
    parts = [f"m{k[1]:x}.{k[2]}={v:x}" if k[0] == "m" else f"c{k[1]}={v:x}" for k, v in sorted(ovr.items())]
    return f"{seed}" + ("/" + ",".join(parts) if parts else "")


def parse_ovr(spec: str):
    seed, _, rest = spec.partition("/")
    ovr = {}
    for p in filter(None, rest.split(",")):
        k, v = p.split("=")
        if k[0] == "m":
            a, size = k[1:].split(".")
            ovr[("m", int(a, 16), int(size))] = int(v, 16)
        else:
            ovr[("c", int(k[1:]))] = int(v, 16)
    return int(seed), ovr


# ---------------------------------------------------------------- main

def return_bits(src: str, name: str):
    """Bits of r0 to compare, from the function's definition; None for void."""
    m = re.search(r"^[ \t]*((?:[A-Za-z_]\w*[ \t\*]+)+?)\**[ \t]*\b" + re.escape(name)
                  + r"\s*\([^;{]*\)\s*(?:[^;{()]*;\s*)*\{", src, re.M)
    if not m:
        return 32, "no definition found; comparing all of r0"
    t = m[1]
    if "*" in m[0][:m[0].index(name)]:
        return 32, None
    words = set(t.split())
    if "void" in words:
        return None, None
    if "char" in words:
        return 8, None
    if "short" in words:
        return 16, None
    return 32, None


def code_words(rom, code_end):
    """Instruction addresses of the ROM function (pool words inside it excluded)."""
    import fn
    _, _, pool, _ = fn.extent(rom)
    return {a for a in range(rom, code_end, 2) if a not in pool}


def spans_of(funcs, labels, text, resolve, where):
    """[(name, cstart, cend, rom, code_end)] for every function with a ROM address."""
    import fn
    out = []
    for i, (cstart, name) in enumerate(funcs):
        m = AUTO.match(name)
        rom = where.get(name, int(m[2], 16) if m and m[1] == "f" else None)
        if rom is None:
            a = resolve(name)       # a named function the linked build knows
            if a is not None and RAM_BASE <= a < RAM_END:
                rom = a - RAM_DELTA
        if rom is None:
            out.append((name, cstart, None, None, None))
            continue
        cend = funcs[i + 1][0] if i + 1 < len(funcs) else len(text)
        cend = min([cend] + [a for a in labels if a > cstart])
        code_end = fn.extent(rom)[0]
        # Stop at the next function the case defines, if the ROM falls into it.
        if i + 1 < len(funcs):
            a = resolve(funcs[i + 1][1])
            if a is not None:
                a = a - RAM_DELTA if RAM_BASE <= a < RAM_END else a
                if rom < a < code_end:
                    code_end = a
        out.append((name, cstart, cend, rom, code_end))
    return out


def print_trace(rprog, cprog, ctx, spec, steps):
    tseed, tovr = parse_ovr(spec)
    for prog in (rprog, cprog):
        ev, n, status, _ = execute(prog, ctx, tseed, steps * (1 if prog is rprog else 2), None, tovr)
        print(f"--- {prog.label}: {n} steps, {status}")
        for e in ev:
            if e[0] == "W":
                print("  W " + fmt_w(e, prog))
            elif e[0] == "C":
                print(f"  C {fmt_target(e[1])} r4.. {fmt_vals(e[2])} stack {fmt_vals(e[3])} "
                      f"@{prog.rel(e[4])}{' (tail)' if e[5] else ''}")
            else:
                print(f"  R r0={e[1]:#x} r8-r15,macl={fmt_vals(e[2])}")


def check(args, ctx, func, rprog, cprog, rom, code_end, ret_bits, quiet=False):
    """Run the trials for one function; print the report; return the verdict."""
    t0 = time.time()
    seen = set()
    full = partial = unsup = reordered = 0
    first_unsup = None
    rsteps_tot = csteps_tot = 0
    verdict, detail = "MATCHED_TRIALS", None
    outcomes = {}       # compare pc -> set of cmp_class results seen in ROM runs
    kinds = {}
    queue = []          # (compare pc, kind, overrides, seed) from input-to-state mutation
    tried_inputs = set()
    next_seed = args.seed
    mutated = 0
    budget_hit = False
    for t in range(args.trials):
        if t >= 20 and time.time() - t0 > args.time:
            budget_hit = True
            break
        # Random seeds for the first quarter, then two in three trials from
        # the mutation queue while it has work.
        item = None
        inputs = getattr(args, "input", None)
        if inputs:
            item = parse_ovr(inputs[t])
        elif t >= args.trials // 4 and t % 3 and queue:
            while queue:
                pc, kind, o, sd = queue.pop(0)
                if not cmp_done(outcomes.get(pc, set()), kind):
                    item = (sd, o)
                    break
        if item is None:
            item = (next_seed, {})
            next_seed += 1
        elif not inputs:
            mutated += 1
        seed, ovr = item
        rev, rsteps, rstat, rc = execute(rprog, ctx, seed, args.steps, seen, ovr, explore=True)
        rsteps_tot += rsteps
        for pc, x, y, kind, T, _, _ in rc.cmps:
            outcomes.setdefault(pc, set()).add(cmp_class(x, y, kind, T))
            kinds[pc] = kind
        if len(queue) < 5000:
            for pc, kind, o in mutations(rc, ovr, outcomes):
                key = (seed, tuple(sorted(o.items())))
                if key not in tried_inputs:
                    tried_inputs.add(key)
                    queue.append((pc, kind, o, seed))
        if rstat.startswith("unsupported"):
            unsup += 1
            first_unsup = first_unsup or f"ROM, input {ovr_str(seed, ovr)}: {rstat}"
            continue
        # A truncated ROM run is compared as a prefix, so the candidate gets
        # the same budget; a finished one gets room for slower code.
        climit = max(args.steps, 4 * rsteps + 1000) if rstat == "ok" else args.steps
        cev, csteps, cstat, _ = execute(cprog, ctx, seed, climit, None, ovr)
        csteps_tot += csteps
        if cstat.startswith("unsupported"):
            unsup += 1
            first_unsup = first_unsup or f"candidate, input {ovr_str(seed, ovr)}: {cstat}"
            continue
        trunc = rstat != "ok" or cstat != "ok"
        diff, reo = compare(rev, cev, rprog, cprog, ret_bits, trunc)
        reordered += reo
        if diff is None and rstat == "ok" and cstat == "limit":
            diff = f"candidate didn't return within {climit} steps (ROM returned after {rsteps})"
        if diff:
            verdict, detail = "DIFFERENT", f"input {ovr_str(seed, ovr)}: {diff}"
            break
        if trunc:
            partial += 1
        else:
            full += 1
        if args.verbose:
            print(f"  input {ovr_str(seed, ovr)}: ok ({rsteps}/{csteps} steps{', truncated' if trunc else ''})")
    ran = full + partial
    planned = min(args.trials, full + partial + unsup) if budget_hit else args.trials
    if verdict == "MATCHED_TRIALS" and (full == 0 or partial or unsup):
        verdict = "INCONCLUSIVE"
        detail = first_unsup or f"only {full} of {planned} trials ran to completion (step limit {args.steps})"
    words = code_words(rom, code_end)
    cov = len({a - RAM_DELTA if a >= RAM_BASE else a for a in seen} & words) / max(1, len(words))
    tried = full + partial + (verdict == "DIFFERENT")
    both = sum(len({c[0] for c in v}) == 2 for v in outcomes.values())
    done = sum(cmp_done(v, kinds[pc]) for pc, v in outcomes.items())
    if quiet:
        print(f"  {rom:06x} {func:24s} {verdict:11s} cov {100 * cov:3.0f}%  compares {both}/{len(outcomes)}  "
              f"{tried} trials ({partial} truncated)  {time.time() - t0:.1f}s" + (f"\n      {detail}" if detail else ""),
              flush=True)
        return verdict
    print(f"{verdict}" + (f": {detail}" if detail else ""))
    print(f"  trials: {full} complete, {partial} hit the step limit (compared up to it), "
          f"{unsup} unsupported; {tried} run, {mutated} of them mutated inputs"
          + (f"; stopped at the {args.time:g}s time budget" if budget_hit else ""))
    print(f"  ROM compares: {both}/{len(outcomes)} seen both ways, {done} also at their boundary (x = y-1, y, y+1)")
    if reordered:
        print(f"  {reordered} trials stored the same values in a different order between calls")
    print(f"  estimated ROM instruction coverage {100 * cov:.0f}%; avg steps ROM {rsteps_tot // max(1, tried)}, "
          f"candidate {csteps_tot // max(1, tried)}; {time.time() - t0:.1f}s")
    if args.cov:
        import fn
        hit = {a - RAM_DELTA if a >= RAM_BASE else a for a in seen}
        for ins in fn.MD.disasm(ROM[rom:code_end], rom):
            if ins.address in words and ins.address not in hit:
                print(f"    not run: {ins.address:06x} {ins.mnemonic} {ins.op_str}")
    if verdict == "MATCHED_TRIALS":
        print("  sampled behavior only; not a proof of equivalence or byte identity")
    if verdict == "DIFFERENT":
        print(f"  replay: difftest.py {args.case} {func} --trace {detail.split(':')[0].split()[1]}")
    return verdict


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("case", type=Path)
    ap.add_argument("func", nargs="?", help="function to check (default: the header's func)")
    ap.add_argument("--all", action="store_true", help="check every function in the file that has a ROM address")
    ap.add_argument("--at", action="append", default=[], help="name=0xaddr for a function without an f_ name")
    ap.add_argument("--flags")
    ap.add_argument("--version", default="v5.0r32")
    ap.add_argument("--rom-end", type=lambda v: int(v, 0),
                    help="exclusive ROM end for one function; overrides fn.py's heuristic (not with --all)")
    ap.add_argument("--trials", type=int, default=200)
    ap.add_argument("--seed", type=int, default=1, help="first trial seed")
    ap.add_argument("--steps", type=int, default=100000, help="instruction limit per ROM run")
    ap.add_argument("--time", type=float, default=60, help="stop a function's trials after this many seconds "
                    "(at least 20 trials run)")
    ap.add_argument("--ret", default="auto", help="r0 bits to compare: auto, none, 8, 16, 32")
    ap.add_argument("--obj", type=Path, help="use this compiled object instead of compiling")
    ap.add_argument("--trace", metavar="INPUT", help="print both event traces for one input (a seed, or the "
                    "seed/overrides string a DIFFERENT report gives)")
    ap.add_argument("--cov", action="store_true", help="list the ROM instructions no trial executed")
    ap.add_argument("--input", action="append", default=[], metavar="INPUT",
                    help="run exactly these inputs (seed or seed/overrides, as --trace takes) as the trials, "
                    "compared as any trial is; repeatable")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    if args.input:
        args.trials = len(args.input)
        args.time = 1e9 if args.time == 60 else args.time   # every listed input runs
    if args.trials <= 0 or args.steps <= 0 or not (0 < args.time < float("inf")):
        ap.error("--trials, --steps and --time must be positive and finite")
    if args.rom_end is not None and args.all:
        ap.error("--rom-end applies to one function, not --all")

    from probe import HEADER  # noqa: E402
    src = args.case.read_text(errors="replace")
    hdr = HEADER.search(src)
    flags = args.flags or (hdr["flags"].strip() if hdr and hdr["flags"] else "-macsave=1 -optimize=1 -speed")
    where = {}
    if hdr and hdr["func"]:
        where[hdr["func"]] = int(hdr["rom"], 16)
    for a in args.at:
        n, v = a.split("=")
        where[n] = int(v, 0)

    if args.obj:
        obj = args.obj
    else:
        import subprocess
        from funcscore import compile_case
        try:
            obj = compile_case(args.case, flags, args.version)
        except subprocess.CalledProcessError:
            print("compile failed")
            return 3
    try:
        text, funcs, labels, relocs, sections, tidx = load_object(obj)
    except emu.Unsupported as e:
        print(f"INCONCLUSIVE: {e}")
        return 2
    resolve = Resolver(where)
    spans = spans_of(funcs, labels, text, resolve, where)
    if args.rom_end is not None:
        target = args.func or (hdr["func"] if hdr and hdr["func"] else args.case.stem)
        updated = []
        for name, start, end, rom, code_end in spans:
            if name == target and rom is not None:
                if not (rom < args.rom_end <= emu.RAM_ROM_END) or args.rom_end % 2:
                    ap.error("--rom-end must be even, after the function start, and within code ROM")
                code_end = args.rom_end
            updated.append((name, start, end, rom, code_end))
        spans = updated
    try:
        notes = link(text, funcs, labels, relocs, sections, tidx, resolve,
                     [(c, e, r) for _, c, e, r, _ in spans if r is not None])
    except emu.Unsupported as e:
        print(f"INCONCLUSIVE: {e}")
        return 2
    CAND_TEXT[:] = text
    ctx = Ctx()

    if args.all:
        todo = [sp for sp in spans if sp[3] is not None]
    else:
        func = args.func or (hdr["func"] if hdr and hdr["func"] else args.case.stem)
        todo = [sp for sp in spans if sp[0] == func]
        if not todo:
            print(f"{obj}: no function {func} (have {[n for _, n in funcs]})")
            return 3
        if todo[0][3] is None:
            print(f"{func}: no ROM address (name it f_<hex> or pass --at {func}=0x...)")
            return 3
    if args.all:
        for nt in notes:
            print("  note:", nt)
    worst = 0
    for name, cstart, cend, rom, code_end in todo:
        DICT[:] = build_dict(rom, code_end)
        run_base = rom + RAM_DELTA if emu.RAM_ROM <= rom < emu.RAM_ROM_END else rom
        rprog = Program("ROM", run_base, run_base + (code_end - rom), static_all, name)
        cprog = Program("candidate", CAND_BASE + cstart, CAND_BASE + cend, static_all, name,
                        cand_funcs=dict(labels), rom_of=resolve)
        fnotes = []
        if args.ret == "auto":
            ret_bits, rnote = return_bits(src, name)
            if rnote:
                fnotes.append(rnote)
        else:
            ret_bits = None if args.ret == "none" else int(args.ret)
        if not args.all:
            print(f"{name}: ROM {rom:#x}-{code_end:#x} ({code_end - rom} bytes), "
                  f"compiled {cend - cstart} bytes, r0 compared: {ret_bits or 'no (void)'} bits")
            for nt in notes + fnotes:
                print("  note:", nt)
        if args.trace is not None:
            print_trace(rprog, cprog, ctx, args.trace, args.steps)
            ctx.save()
            return 0
        v = check(args, ctx, name, rprog, cprog, rom, code_end, ret_bits, quiet=args.all)
        worst = max(worst, {"MATCHED_TRIALS": 0, "INCONCLUSIVE": 2, "DIFFERENT": 1}[v])
        ctx.save()
    return worst


if __name__ == "__main__":
    sys.exit(main())
