#!/usr/bin/env python3
"""Classify unresolved assembly as SHC-shaped or likely hand-written.

Every unresolved (asm, non-exempt) range from splits.txt is cut into
functions at the discovered call targets. Each function is decoded by
recursive descent from its entry (following SHC's braf switch tables, never
decoding literal pools or table words) and checked for evidence that SHC
v5.0r32 did not produce it:

  scratch-input     r0-r3, T, MACH or MACL read before any definition on some
                    path from the entry: arguments in a private register ABI
  callee-clobber    r8-r14 written without being saved in the function
  scratch-save      r0-r3 saved by the prologue and restored before return,
                    outside an rte-ending interrupt handler (SHC pushes
                    scratch registers only as outgoing stack arguments)
  pr-unsaved-call   a call followed by rts with PR never saved
  sp-imbalance      r15 offset differs between paths or is nonzero at return
  sp-write          r15 loaded or copied from a general register
  shared-code       a branch into another function's body (not its entry),
                    or a conditional branch to another function's entry
  zero-idiom        xor/sub of a register with itself (SHC uses mov #0)
  system-op         SR/GBR/VBR control, rte, trapa, sleep, division steps,
                    rotate-through-carry, mac, tas, clrmac or sett, unless
                    the matched C controls contain the same operand form
  callee-input      r8-r14 read as a value before the function defines it,
                    other than to save it (weak: SHC does this for an
                    uninitialized local, as in f_2c878)
  unseen-form       an operand form (mnemonic plus addressing mode) that no
                    matched C function uses (weak: reported, never decisive)
  prologue-order    saves out of SHC's order (callee-saved descending, then
                    pr, then macl), learned from the controls

The rules are fixed in advance; the matched C functions (SHC output, the
negatives) and the SHC runtime library in the two exempt ranges (hand-written,
the positives) only calibrate the learned vocabularies. Precision is reported
with the vocabularies learned on four fifths of the negatives and applied to
the held-out fifth, so a negative never vouches for itself.

The verdict is a hypothesis for routing work, not proof of authorship: SHC
inline assembly (#pragma inline_asm) or intrinsics can produce some of these
shapes, and a function with no evidence may still be hand-written.

usage: asm_scan.py [--json build/asm_scan.json] [--routes] [--functions]
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import build  # noqa: E402
import fn  # noqa: E402
from sh2isa import decode  # noqa: E402

EXEMPT = [(0x30D40, 0x3127C), (0x312BA, 0x313E4)]
SCRATCH = {"r0", "r1", "r2", "r3"}
ARGS = {"r4", "r5", "r6", "r7"}
CALLEE = {f"r{i}" for i in range(8, 15)}
SPECIAL_IN = {"t", "macl", "mach"}
SYSTEM = {"ldc", "stc", "ldc.l", "stc.l", "rte", "trapa", "sleep", "div0s", "div0u", "div1",
          "rotcl", "rotcr", "mac.w", "mac.l", "tas.b", "clrmac", "sett", "negc", "subv", "addv"}
STRONG = ("scratch-input", "callee-clobber", "scratch-save", "pr-unsaved-call",
          "sp-imbalance", "sp-write", "shared-code", "zero-idiom", "system-op", "prologue-order")
WEAK = ("callee-input", "unseen-form")
CONTEXT_FREE = ("sp-write", "zero-idiom", "system-op", "unseen-form")
FOLDS = 5


def rom_of(a: int) -> int:
    return fn.rom_addr(a)


# ---------------------------------------------------------------- discovery

def entry_points() -> set:
    """Function entries with code evidence: bsr targets, jsr/jmp targets
    loaded from a literal pool, and RAM code pointers (in literal pools or
    the data after the code) that follow a return or a pool. Unlike
    fn.call_targets(), halfwords inside literal pools never seed an entry."""
    import struct
    pool = fn.all_pool()
    seeds = set()
    lit = {}
    pointers = set()
    for pc in range(0x400, fn.TEXT_END, 2):
        if pc in pool:
            continue
        w = fn.hw(pc)
        if w >> 12 == 0xB:
            seeds.add(pc + 4 + fn.sdisp(w & 0xFFF, 12) * 2)
        elif w >> 12 == 0xD:
            a = ((pc + 4) & ~3) + (w & 0xFF) * 4
            v = struct.unpack(">I", fn.ROM[a:a + 4])[0]
            lit[(w >> 8) & 0xF] = (fn.rom_addr(v), pc)
            if fn.RAM_BASE <= v < fn.RAM_END:
                pointers.add(fn.rom_addr(v))
        elif w >> 12 == 0x4 and (w & 0xFF) in (0x0B, 0x2B):
            n = (w >> 8) & 0xF
            if n in lit and pc - lit[n][1] <= 16 and 0x400 <= lit[n][0] < fn.TEXT_END:
                seeds.add(lit[n][0])
    for a in range(build.RAM_ROM_END, len(fn.ROM) - 3, 4):
        v = struct.unpack(">I", fn.ROM[a:a + 4])[0]
        if fn.RAM_BASE <= v < fn.RAM_END:
            pointers.add(fn.rom_addr(v))
    seeds |= {p for p in pointers if 0x400 < p < fn.TEXT_END and fn.after_return(p)}
    # exception vectors: ROM handlers in the boot block, RAM handlers in C
    for a in range(0, 0x400, 4):
        v = struct.unpack(">I", fn.ROM[a:a + 4])[0]
        if 0x400 <= v < fn.RAM_ROM or fn.RAM_BASE <= v < fn.RAM_END:
            seeds.add(fn.rom_addr(v))
    return {s for s in seeds if 0x400 <= s < fn.TEXT_END and s % 2 == 0}


def c_entries() -> dict:
    """ROM address -> name of every function a linked C object defines."""
    elf = ROOT / "build" / "tgm2p.elf"
    out = {}
    nm = subprocess.run(["sh-elf-nm", "-n", str(elf)], capture_output=True, text=True, check=True).stdout
    segs = [(a, b) for a, b, k, _ in build.read_splits() if k in ("c", "c-prefix", "c-range")]
    for line in nm.splitlines():
        p = line.split()
        if len(p) == 3 and p[1] in "Tt" and not p[2].startswith("func_"):
            a = rom_of(int(p[0], 16))
            if any(x <= a < y for x, y in segs):
                out[a] = p[2]
    return out


def runtime_entries() -> dict:
    out = {}
    for line in (ROOT / "symbols.txt").read_text().splitlines():
        line = line.split("#", 1)[0].split()
        if len(line) >= 2:
            a = rom_of(int(line[1], 16))
            if any(x <= a < y for x, y in EXEMPT):
                out.setdefault(a, line[0])
    for a in entry_points():
        if any(x <= a < y for x, y in EXEMPT):
            out.setdefault(a, f"f_{a:x}")
    return out


def unresolved_ranges():
    out = []
    for a, b, kind, _ in build.read_splits():
        if kind != "asm":
            continue
        exempt = sum(max(0, min(b, y) - max(a, x)) for x, y in EXEMPT)
        out.append((a, b, exempt))
    return out


# ---------------------------------------------------------------- decoding

def switch_targets(braf_pc: int, lo: int, hi: int):
    """Targets of SHC's `mova T,r0; mov.w @(r0,rI),r0; braf r0` dispatch.

    The table holds 16-bit offsets from braf+4. Its length comes from the
    bound tested before the dispatch (`mov #N,rB; cmp/hs rB,rX`) when that is
    visible; otherwise entries are read until the table runs into code."""
    back = [decode(fn.hw(p), p) for p in range(braf_pc - 2, max(lo, braf_pc - 24) - 2, -2)]
    if len(back) < 2 or back[0].mnemonic != "mov.w" or "@(r0," not in back[0].ops or back[1].mova is None:
        # also accept `mova T,r0; mov.w @(r0,rI),rJ; ...; braf rJ`
        movas = [i for i in back[:6] if i.mova is not None]
        if not movas:
            return None, None
        table = movas[0].mova
    else:
        table = back[1].mova
    bound = None
    for i in back:
        if i.mnemonic == "cmp/hs" or i.mnemonic == "cmp/hi":
            reg = i.ops.split(",")[0]
            for j in back[back.index(i) + 1: back.index(i) + 6]:
                if j.mnemonic == "mov" and j.ops.startswith("#") and j.ops.endswith("," + reg):
                    bound = j.extra["imm"] + (1 if i.mnemonic == "cmp/hi" else 0)
                    break
            break
    targets = []
    limit = hi
    k = 0
    while table + 2 * k < limit:
        if bound is not None and k >= bound:
            break
        e = fn.hw(table + 2 * k)
        e = e - 0x10000 if e & 0x8000 else e
        t = braf_pc + 4 + e
        if not (lo <= t < hi) or t % 2:
            break
        targets.append(t)
        if t > table:
            limit = min(limit, t)
        k += 1
        if k > 512:
            break
    return table, targets


class Func:
    def __init__(self, entry, region_hi, entries, lo, hi, orphan=False):
        self.entry = entry
        self.orphan = orphan
        self.region_hi = region_hi
        self.entries = entries
        self.lo, self.hi = lo, hi       # decoding bounds (the whole code block)
        self.insns = {}
        self.succ = defaultdict(list)   # pc -> [(succ_pc, after_call)]
        self.tables = set()             # table word addresses
        self.out_edges = []             # (pc, target, kind)
        self.unknown_jumps = []
        self.walk()

    def walk(self):
        work = [self.entry]
        seen = set()
        while work:
            pc = work.pop()
            if pc in seen or not (self.lo <= pc < self.hi):
                continue
            seen.add(pc)
            ins = decode(fn.hw(pc), pc)
            self.insns[pc] = ins
            if not ins.valid:
                continue
            if ins.delay:
                slot = pc + 2
                sins = decode(fn.hw(slot), slot)
                self.insns[slot] = sins
                seen.add(slot)
                self.succ[pc].append((slot, False))
                nxt = self._targets(pc, ins)
                for t, after_call in nxt:
                    self.succ[slot].append((t, after_call))
                    work.append(t)
                continue
            if ins.flow == "cond":
                for t in (ins.target, pc + 2):
                    if t == ins.target and self._outside(pc, t, cond=True):
                        continue
                    self.succ[pc].append((t, False))
                    work.append(t)
                continue
            if ins.flow == "call":  # trapa
                self.succ[pc].append((pc + 2, True))
                work.append(pc + 2)
                continue
            if ins.flow in ("ret", "rte"):
                continue
            if pc + 2 in self.entries and pc + 2 != self.entry:
                # SHC drops a tail call to the next function and falls into it
                self.out_edges.append((pc, pc + 2, "fallthrough"))
                continue
            self.succ[pc].append((pc + 2, False))
            work.append(pc + 2)

    def _outside(self, pc, t, cond=False):
        """Record a transfer that leaves this function. True if not followed."""
        if t == self.entry:
            return False
        if t in self.entries:
            self.out_edges.append((pc, t, "cond-to-entry" if cond else "tail"))
            return True
        return False

    def _targets(self, pc, ins):
        slot_end = pc + 4
        if ins.flow == "cond":
            out = [(slot_end, False)]
            if not self._outside(pc, ins.target, cond=True):
                out.append((ins.target, False))
            return out
        if ins.flow == "call":
            return [(slot_end, True)]
        if ins.flow in ("ret", "rte"):
            return []
        if ins.mnemonic == "bra":
            return [] if self._outside(pc, ins.target) else [(ins.target, False)]
        if ins.mnemonic == "braf":
            table, targets = switch_targets(pc, self.lo, self.hi)
            if targets:
                self.tables.update(range(table, table + 2 * len(targets), 2))
                return [(t, False) for t in targets]
            self.unknown_jumps.append(pc)
            return []
        # jmp @rn: a tail call through a register
        self.out_edges.append((pc, None, "tail-jmp"))
        return []

    # ---------------------------------------------------------- analyses

    def preds(self):
        p = defaultdict(list)
        for a, ss in self.succ.items():
            for b, call in ss:
                if b in self.insns:
                    p[b].append((a, call))
        return p

    def must_defined(self):
        """Registers defined on every path from the entry, before each pc."""
        universe = SCRATCH | ARGS | CALLEE | SPECIAL_IN | {"r15", "pr", "gbr", "vbr", "sr"}
        start = ARGS | {"r15", "pr", "gbr", "vbr", "sr"}
        state = {pc: set(universe) for pc in self.insns}
        state[self.entry] = set(start)
        preds = self.preds()
        changed = True
        order = sorted(self.insns)
        while changed:
            changed = False
            for pc in order:
                if pc == self.entry:
                    continue
                ps = preds.get(pc)
                if not ps:
                    continue
                new = None
                for a, call in ps:
                    out = state[a] | self.insns[a].defs
                    if call:
                        out = out | SCRATCH | ARGS | {"t", "pr"}
                    new = out if new is None else new & out
                if new != state[pc]:
                    state[pc] = new
                    changed = True
        return state

    def plausible(self) -> bool:
        """Whether an orphan fragment decodes as code: at least three valid
        instructions ending in a return or jump, rather than stray data."""
        ins = list(self.insns.values())
        if len(ins) < 3 or any(not i.valid for i in ins):
            return False
        return any(i.flow in ("ret", "rte", "jump") for i in ins) or bool(self.out_edges)

    def features(self, forms_ok=None, system_ok=None):
        ev = self._features(forms_ok, system_ok)
        if self.orphan:
            # A fragment no entry reaches starts in an unknown state, so only
            # context-free evidence applies to it.
            ev = {k: v for k, v in ev.items() if k in CONTEXT_FREE}
        return ev

    def _features(self, forms_ok=None, system_ok=None):
        ev = defaultdict(list)
        ins_list = [self.insns[p] for p in sorted(self.insns)]
        valid = [i for i in ins_list if i.valid]
        defined = self.must_defined()
        saved = set()
        for i in valid:
            if i.mnemonic == "mov.l" and i.mem and i.mem[1] == "predec" and i.mem[0] == "r15":
                saved.add(i.ops.split(",")[0])
            if i.mnemonic in ("sts.l", "stc.l") and i.mem and i.mem[0] == "r15":
                saved.add(i.ops.split(",")[0])
            if i.mnemonic == "sts" and i.ops.startswith("pr,"):
                saved.add("pr")
        is_irq = any(i.flow == "rte" for i in valid)
        for i in valid:
            pre = defined.get(i.pc, set())
            is_save = (i.mem and i.mem[1] == "predec" and i.mem[0] == "r15" and i.mem[3])
            reads = set(i.uses)
            if is_save:
                reads -= {i.ops.split(",")[0]}
            for r in sorted(reads - pre):
                if r in SCRATCH or r in SPECIAL_IN:
                    ev["scratch-input"].append(f"{i.pc:05x} {i.text()} reads {r}")
                elif r in CALLEE:
                    ev["callee-input"].append(f"{i.pc:05x} {i.text()} reads {r}")
            for r in sorted(i.defs & CALLEE):
                if r not in saved and not (i.mem and i.mem[1] == "postinc" and i.mem[0] == "r15"):
                    ev["callee-clobber"].append(f"{i.pc:05x} {i.text()}")
                elif r not in saved:
                    ev["callee-clobber"].append(f"{i.pc:05x} {i.text()} (pop without push)")
            if "r15" in i.defs and not (i.mem and i.mem[0] == "r15") and \
                    not (i.mnemonic == "add" and i.ops.endswith(",r15")) and i.mnemonic not in ("rte",) and \
                    not (i.mnemonic == "sub" and i.ops.endswith(",r15")):
                ev["sp-write"].append(f"{i.pc:05x} {i.text()}")
            if i.mnemonic in ("xor", "sub") and len(set(i.ops.split(","))) == 1:
                ev["zero-idiom"].append(f"{i.pc:05x} {i.text()}")
            f = form(i)
            if i.mnemonic in SYSTEM and (system_ok is None or f not in system_ok):
                ev["system-op"].append(f"{i.pc:05x} {i.text()}")
            elif forms_ok is not None and f not in forms_ok:
                ev["unseen-form"].append(f"{i.pc:05x} {i.text()} [{f}]")
        calls = [i for i in valid if i.flow == "call"]
        rets = [i for i in valid if i.flow == "ret"]
        if calls and rets and "pr" not in saved:
            ev["pr-unsaved-call"].append(f"{calls[0].pc:05x} {calls[0].text()} with no pr save")
        ev.update(self.sp_check())
        for pc, t, kind in self.out_edges:
            if kind == "cond-to-entry":
                ev["shared-code"].append(f"{pc:05x} conditional branch to entry {t:05x}")
        mine = set(self.insns)
        for other in self.entries:
            if other != self.entry and other in mine and other > self.entry:
                pass  # handled by overlap() across functions
        po = prologue(self)
        if not is_irq:
            restored = {i.ops.split(",")[1] for i in valid
                        if i.mem and i.mem[0] == "r15" and i.mem[1] == "postinc" and "r15" in i.defs}
            for r in po:
                if r in SCRATCH and r in restored:
                    ev["scratch-save"].append(f"prologue saves {r} and restores it")
        order = []
        for r in po:
            if r in order:
                break
            if r not in SCRATCH | ARGS:
                order.append(r)
        if order and not canonical_saves(order):
            ev["prologue-order"].append(" ".join(order))
        return dict(ev)

    def sp_check(self):
        ev = defaultdict(list)
        off = {self.entry: 0}
        consts = {}
        work = [self.entry]
        bad = False
        visits = Counter()
        while work and not bad:
            pc = work.pop()
            visits[pc] += 1
            if visits[pc] > 50:
                break
            i = self.insns[pc]
            o = off[pc]
            d = None
            if o is not None:
                if i.mem and i.mem[0] == "r15" and i.mem[1] == "predec":
                    d = o - 4 if i.mnemonic in ("sts.l", "stc.l") else o - i.mem[2]
                elif i.mem and i.mem[0] == "r15" and i.mem[1] == "postinc" and "r15" in i.defs:
                    d = o + i.mem[2]
                elif i.mnemonic == "add" and i.ops.endswith(",r15"):
                    src = i.ops.split(",")[0]
                    if src.startswith("#"):
                        d = o + int(src[1:])
                    else:
                        d = o + consts[src] if src in consts else None
                elif i.mnemonic == "sub" and i.ops.endswith(",r15"):
                    src = i.ops.split(",")[0]
                    d = o - consts[src] if src in consts else None
                elif "r15" in i.defs and i.mnemonic != "rte":
                    d = None
                else:
                    d = o
            # track small constants loaded into registers for frame setup
            if i.mnemonic == "mov" and i.ops.startswith("#"):
                consts[i.ops.split(",")[1]] = i.extra["imm"]
            elif i.pool and i.pool[1] == 2:
                v = fn.hw(i.pool[0])
                consts[i.ops.split(",")[1]] = v - 0x10000 if v & 0x8000 else v
            elif i.pool and i.pool[1] == 4:
                v = int.from_bytes(fn.ROM[i.pool[0]:i.pool[0] + 4], "big")
                consts[i.ops.split(",")[1]] = v - (1 << 32) if v & 0x80000000 else v
            if i.flow in ("ret", "rte") or (i.delay and False):
                pass
            for s, _ in self.succ.get(pc, []):
                if s not in self.insns:
                    continue
                if s in off and off[s] != d:
                    if off[s] is not None and d is not None:
                        ev["sp-imbalance"].append(f"{s:05x} reached with sp {off[s]:+d} and {d:+d}")
                        bad = True
                    continue
                if s not in off:
                    off[s] = d
                    work.append(s)
        for pc, i in self.insns.items():
            if i.flow in ("ret", "rte") and pc + 2 in off:
                o = off.get(pc + 2)
                # the delay slot executes before the return completes
                slot = self.insns.get(pc + 2)
                if o is not None and slot is not None and slot.mem and slot.mem[0] == "r15" \
                        and slot.mem[1] == "postinc":
                    o += slot.mem[2]
                elif o is not None and slot is not None and slot.mnemonic == "add" and slot.ops.endswith(",r15") \
                        and slot.ops.startswith("#"):
                    o += int(slot.ops.split(",")[0][1:])
                if o not in (0, None):
                    ev["sp-imbalance"].append(f"{pc:05x} {i.mnemonic} with sp {o:+d}")
        return ev


def form(i) -> str:
    """Operand form: registers abstracted, r0 and r15 kept, same-register pairs marked."""
    ops = i.ops
    ops = re.sub(r"@\(0x[0-9a-f]+\)", "@(pc)", ops)
    ops = re.sub(r"#-?\d+", "#i", ops)
    ops = re.sub(r"@\((\d+),", "@(d,", ops)
    ops = re.sub(r"0x[0-9a-f]+", "L", ops)
    regs = re.findall(r"\br(\d+)\b", ops)
    same = len(regs) == 2 and regs[0] == regs[1]

    def repl(m):
        r = m.group(1)
        return {"0": "r0", "15": "sp"}.get(r, "rA" if same else "r")
    ops = re.sub(r"\br(\d+)\b", repl, ops)
    return f"{i.mnemonic} {ops}".strip()


def prologue(f: Func):
    """Registers saved by pushes in the entry block, in order."""
    out = []
    pc = f.entry
    while pc in f.insns:
        i = f.insns[pc]
        if not i.valid or i.flow:
            break
        if i.mem and i.mem[0] == "r15" and i.mem[1] == "predec" and i.mem[3]:
            out.append(i.ops.split(",")[0])
        pc += 2
    return out


def canonical_saves(seq) -> bool:
    """SHC v5.0 pushes callee-saved registers in descending order, then pr,
    then mach/macl; arguments it keeps as locals (r4-r7) come after."""
    rank = []
    for r in seq:
        if r in CALLEE:
            rank.append((0, -int(r[1:])))
        elif r == "pr":
            rank.append((1, 0))
        elif r in ("mach", "macl"):
            rank.append((2, r == "macl"))
        else:
            rank.append((3, int(r[1:]) if r.startswith("r") else 99))
    return rank == sorted(rank)


# ---------------------------------------------------------------- ranges

def code_blocks():
    """Contiguous runs of code (C, asm) between data, for decoding bounds."""
    return [(0x400, fn.RAM_ROM), (fn.RAM_ROM, build.RAM_ROM_END)]


def functions_in(lo, hi, entries, all_entries):
    """Func objects for entries in [lo, hi), plus orphan entries for any code
    in the range that no entry reaches."""
    block = next((a, b) for a, b in code_blocks() if a <= lo < b)
    out = []
    ents = sorted(e for e in entries if lo <= e < hi)
    if (not ents or ents[0] != lo) and lo not in fn.all_pool() and fn.hw(lo) not in (0x0009, 0xFFFF, 0x0000):
        ents = sorted(set(ents) | {lo})
    covered = set()
    pool = fn.all_pool()
    i = 0
    orphan = set()
    while True:
        for e in ents:
            if any(f.entry == e for f in out):
                continue
            nxt = next((x for x in sorted(all_entries | set(ents)) if x > e), hi)
            f = Func(e, nxt, (all_entries | set(ents)) - {e}, *block, orphan=e in orphan)
            out.append(f)
            covered |= set(f.insns) | f.tables
        # orphans: first non-pool, non-padding halfword not covered
        gap = None
        for a in range(lo, hi, 2):
            if a in covered or a in pool or a in orphan:
                continue
            if fn.hw(a) in (0x0009, 0xFFFF, 0x0000):
                continue
            gap = a
            break
        if gap is None:
            break
        orphan.add(gap)
        ents = sorted(set(ents) | {gap})
        i += 1
        if i > 200:
            break
    return out, orphan


def overlaps(funcs):
    ev = defaultdict(list)
    owner = {}
    for f in sorted(funcs, key=lambda f: (f.orphan, f.entry)):
        if f.orphan:
            continue
        for pc in f.insns:
            if pc in owner and owner[pc] != f.entry:
                ev[f.entry].append(f"shares {pc:05x} with {owner[pc]:05x}")
            else:
                owner.setdefault(pc, f.entry)
    return ev


# ---------------------------------------------------------------- calibration

def vocab(funcs):
    forms = set()
    system = set()
    for f in funcs:
        for i in f.insns.values():
            if i.valid:
                forms.add(form(i))
                if i.mnemonic in SYSTEM:
                    system.add(form(i))
    return forms, system


def verdict(ev) -> str:
    return "hand-written" if any(k in ev for k in STRONG) else "shc-shaped"


def analyse():
    all_entries = entry_points()
    cent = c_entries()
    rent = runtime_entries()
    all_entries |= set(cent) | set(rent)
    # negatives: matched C functions
    neg = []
    cseg = [(a, b) for a, b, k, _ in build.read_splits() if k in ("c", "c-prefix", "c-range")]
    for a in sorted(cent):
        block = next((x, y) for x, y in code_blocks() if x <= a < y)
        nxt = next((x for x in sorted(all_entries) if x > a), block[1])
        neg.append(Func(a, nxt, all_entries - {a}, *block))
    pos = []
    for lo, hi in EXEMPT:
        fs, orph = functions_in(lo, hi, set(rent), all_entries)
        pos += [f for f in fs if f.entry not in orph]
    # cross-validated control results
    rows = []
    folds = [neg[k::FOLDS] for k in range(FOLDS)]
    for k in range(FOLDS):
        train = [f for j, fold in enumerate(folds) if j != k for f in fold]
        forms_ok, system_ok = vocab(train)
        for f in folds[k]:
            ev = f.features(forms_ok, system_ok)
            rows.append(("negative", f, ev))
    forms_ok, system_ok = vocab(neg)
    ov = overlaps(pos)
    for f in pos:
        ev = f.features(forms_ok, system_ok)
        if ov.get(f.entry):
            ev.setdefault("shared-code", []).extend(ov[f.entry])
        rows.append(("positive", f, ev))
    controls = {"negatives": len(neg), "positives": len(pos)}
    fp = [(f, ev) for kind, f, ev in rows if kind == "negative" and verdict(ev) == "hand-written"]
    tp = [(f, ev) for kind, f, ev in rows if kind == "positive" and verdict(ev) == "hand-written"]
    fn_ = [(f, ev) for kind, f, ev in rows if kind == "positive" and verdict(ev) != "hand-written"]
    controls.update({
        "true_positive": len(tp), "false_positive": len(fp), "false_negative": len(fn_),
        "true_negative": len(neg) - len(fp),
        "precision": len(tp) / (len(tp) + len(fp)) if tp or fp else None,
        "recall": len(tp) / len(pos) if pos else None,
        "false_positive_functions": [{"entry": f"0x{f.entry:05x}", "name": cent.get(f.entry),
                                      "evidence": {k: v[:3] for k, v in ev.items() if k in STRONG}}
                                     for f, ev in fp],
        "false_negative_functions": [{"entry": f"0x{f.entry:05x}", "name": rent.get(f.entry)} for f, _ in fn_],
        "feature_rates": feature_rates(rows),
        "positive_functions": [{"entry": f"0x{f.entry:05x}", "name": rent.get(f.entry, "orphan"),
                                "verdict": verdict(ev),
                                "evidence": {k: v[:3] for k, v in ev.items()}} for kind, f, ev in rows
                               if kind == "positive"],
    })
    # unresolved ranges, judged with the full negative vocabulary
    ranges = []
    for lo, hi, exempt in unresolved_ranges():
        if exempt == hi - lo:
            continue
        fs, orphans = functions_in(lo, hi, all_entries, all_entries)
        ov = overlaps(fs)
        funcs = []
        data = []
        for f in sorted(fs, key=lambda f: f.entry):
            if f.orphan and not f.plausible():
                data.append(f"0x{f.entry:05x}")
                continue
            ev = f.features(forms_ok, system_ok)
            if ov.get(f.entry):
                ev.setdefault("shared-code", []).extend(ov[f.entry])
            code = sum(2 for p in f.insns if lo <= p < hi)
            funcs.append({"entry": f"0x{f.entry:05x}", "orphan": f.entry in orphans,
                          "decoded_bytes": code, "verdict": verdict(ev),
                          "strong": sorted(k for k in ev if k in STRONG),
                          "weak": sorted(k for k in ev if k in WEAK),
                          "evidence": {k: v[:4] for k, v in ev.items()},
                          "unknown_jumps": [f"0x{p:05x}" for p in f.unknown_jumps]})
        covered = set()
        for f in fs:
            if f"0x{f.entry:05x}" not in data:
                covered |= {p for p in set(f.insns) | f.tables if lo <= p < hi}
        hand = [x for x in funcs if x["verdict"] == "hand-written"]
        hand_bytes = sum(x["decoded_bytes"] for x in hand)
        code_bytes = sum(x["decoded_bytes"] for x in funcs)
        if not funcs:
            rv = "pool-data"
        elif not hand:
            rv = "shc-shaped"
        elif len(hand) == len(funcs):
            rv = "hand-written"
        else:
            rv = "mixed"
        ranges.append({"start": f"0x{lo:06x}", "end": f"0x{hi:06x}", "bytes": hi - lo - exempt,
                       "verdict": rv, "functions": len(funcs), "hand_written_functions": len(hand),
                       "hand_written_decoded_bytes": hand_bytes, "decoded_bytes": code_bytes,
                       "undecoded_bytes": hi - lo - len(covered) * 2,
                       "data_fragments": data, "functions_detail": funcs})
    return {"schema": 1, "tool": "tools/asm_scan.py",
            "tool_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
            "program_sha256": hashlib.sha256(fn.ROM).hexdigest(),
            "splits_sha256": hashlib.sha256((ROOT / "splits.txt").read_bytes()).hexdigest(),
            "rules": {"strong": list(STRONG), "weak": list(WEAK), "folds": FOLDS},
            "controls": controls, "ranges": ranges}


def feature_rates(rows):
    out = {}
    for kind in ("negative", "positive"):
        sub = [ev for k, _, ev in rows if k == kind]
        c = Counter(f for ev in sub for f in ev)
        out[kind] = {f: c[f] for f in (*STRONG, *WEAK)}
    return out


def routes(report):
    """Per-range hypotheses for config/frontier_routes.json's asm_scan key."""
    out = []
    for r in report["ranges"]:
        hw = [f for f in r["functions_detail"] if f["verdict"] == "hand-written"]
        if r["verdict"] == "pool-data":
            obs = (f"No code: {r['bytes']} bytes of literal pool or table data, covered with the "
                   "functions that load them.")
        elif r["verdict"] == "shc-shaped":
            obs = (f"{r['functions']} functions decoded; no strong hand-written evidence. "
                   f"{r['undecoded_bytes']} bytes are pools, tables or unreached.")
        else:
            parts = [f"{f['entry']}: {', '.join(f['strong'])}" for f in hw[:6]]
            obs = (f"{len(hw)}/{r['functions']} functions carry strong evidence ("
                   + "; ".join(parts) + ("; ..." if len(hw) > 6 else "") + ").")
        out.append({"start": r["start"], "end": r["end"], "verdict": r["verdict"],
                    "observation": obs,
                    "hand_written_functions": [f["entry"] for f in hw]})
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--json", type=Path, default=ROOT / "build" / "asm_scan.json")
    ap.add_argument("--routes", action="store_true",
                    help="write the per-range verdicts to config/frontier_routes.json (key asm_scan)")
    ap.add_argument("--functions", action="store_true", help="print every function")
    args = ap.parse_args()
    report = analyse()
    args.json.parent.mkdir(parents=True, exist_ok=True)
    args.json.write_text(json.dumps(report, indent=1) + "\n")
    c = report["controls"]
    prec = "n/a" if c["precision"] is None else f"{c['precision']:.3f}"
    rec = "n/a" if c["recall"] is None else f"{c['recall']:.3f}"
    print(f"controls: {c['negatives']} matched C functions (negatives, {FOLDS}-fold), "
          f"{c['positives']} runtime functions (positives)")
    print(f"  hand-written verdicts: {c['true_positive']} true, {c['false_positive']} false; "
          f"precision {prec}, recall {rec}")
    for f in c["false_positive_functions"]:
        print(f"  false positive {f['entry']} {f['name']}: {f['evidence']}")
    for f in c["false_negative_functions"]:
        print(f"  missed positive {f['entry']} {f['name']}")
    print(f"feature counts (negatives / positives):")
    for k in (*STRONG, *WEAK):
        print(f"  {k:16s} {c['feature_rates']['negative'][k]:4d} / {c['feature_rates']['positive'][k]:3d}")
    print()
    for r in report["ranges"]:
        print(f"{r['start']}-{r['end']} {r['bytes']:5d} B  {r['verdict']:12s} "
              f"{r['hand_written_functions']}/{r['functions']} hand-written functions")
        for f in r["functions_detail"]:
            if args.functions or f["verdict"] == "hand-written":
                ev = "; ".join(f"{k}: {v[0]}" for k, v in f["evidence"].items() if k in STRONG)
                weak = f"  weak: {', '.join(f['weak'])}" if f["weak"] else ""
                print(f"    {f['entry']} {f['decoded_bytes']:5d} B {f['verdict']:12s} {ev}{weak}")
    if args.routes:
        path = ROOT / "config" / "frontier_routes.json"
        routes_json = json.loads(path.read_text())
        routes_json["asm_scan"] = {
            "scope": "hypotheses from tools/asm_scan.py; routing only, not an authorship proof "
                     "or an exemption. See docs/research/2026-10-16-handwritten-asm-scan.md.",
            "program_sha256": report["program_sha256"],
            "controls": {k: c[k] for k in ("negatives", "positives", "true_positive", "false_positive",
                                           "false_negative", "precision", "recall")},
            "ranges": routes(report)}
        path.write_text(json.dumps(routes_json, indent=2) + "\n")
        print(f"wrote {len(routes_json['asm_scan']['ranges'])} range hypotheses to {path.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
