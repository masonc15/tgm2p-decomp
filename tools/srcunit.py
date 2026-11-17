#!/usr/bin/env python3
"""Source units for the mod build: compiled C relinked by symbol.

The shiftable build (tools/shiftable.py) places matching C by its original
byte offsets: a `c` unit's unrelocated address literals are found at their ROM
offsets, and a `c-prefix`/`c-range` unit is the byte range the matching proof
retained from its context object. That is exact, and it only holds while the
source still compiles to the original bytes.

A source unit depends on nothing but the object the compiler just produced.
The functions a unit owns are found by name (`f_<rom offset>`, or a name in
config/function_names.txt), cut out of the object with everything they need, and
written as an assembly package the linker can place anywhere:

- relocated words keep their symbol (`.4byte _g_6060030`);
- pool words that hold an address as a plain integer (`(struct frame *)0xa7130`
  in the source) become symbol expressions;
- a pool load or a branch that leaves the cut gets a local copy of the pool
  word, or a fixup to the function it names. SHC shares pools and emits `bsr`
  between the functions of one file, and a `c-prefix`/`c-range` source holds
  context functions that other units own;
- `.rodata` (and for a whole-file unit `.data`/`.bss`) comes along.

So the source can be edited: functions may grow, shrink or be added, and
references to them follow.
"""
from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

import asm_scan  # noqa: E402
import build  # noqa: E402
import fn  # noqa: E402

DEFAULT_FLAGS = ["-macsave=1", "-optimize=1", "-speed"]
R_SH_DIR32 = 1


def compile_cached(src: Path, obj: Path, flags) -> Path:
    """Compile src with the pinned SHC unless obj is already its output."""
    key = hashlib.sha256(src.read_bytes() + b"\0" + " ".join(flags).encode() + b"\0" +
                         build.SHC_VERSION.encode()).hexdigest()
    stamp = obj.with_suffix(".key")
    if obj.exists() and stamp.exists() and stamp.read_text() == key:
        return obj
    obj.parent.mkdir(parents=True, exist_ok=True)
    stamp.unlink(missing_ok=True)
    obj.unlink(missing_ok=True)
    r = subprocess.run([str(build.SHCC), build.SHC_VERSION, str(obj), str(src), *flags],
                       capture_output=True, text=True)
    if r.returncode or not obj.exists():
        raise SystemExit(f"{src}: SHC failed:\n{(r.stdout + r.stderr)[-3000:]}")
    stamp.write_text(key)
    return obj


class Obj:
    """The allocated sections, symbols and relocations of an SHC object."""

    def __init__(self, path: Path):
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        self.path = path
        self.sections = {}   # index -> dict(name, size, data, nobits, write)
        self.relocs = {}     # section index -> {offset: (name, shndx, value, addend, symbol type)}
        with path.open("rb") as f:
            elf = ELFFile(f)
            for i, s in enumerate(elf.iter_sections()):
                if s["sh_flags"] & 2 and s["sh_size"]:
                    nobits = s["sh_type"] == "SHT_NOBITS"
                    self.sections[i] = dict(name=s.name, size=s["sh_size"], nobits=nobits,
                                            data=b"" if nobits else s.data(),
                                            write=bool(s["sh_flags"] & 1))
            text = [i for i, s in self.sections.items() if s["name"] == ".text"]
            if len(text) != 1:
                raise SystemExit(f"{path}: expected one .text section")
            self.text = text[0]
            self.data = self.sections[self.text]["data"]
            self.syms = [(s.name, s["st_value"], s["st_shndx"], s["st_info"]["type"], s["st_info"]["bind"])
                         for s in elf.get_section_by_name(".symtab").iter_symbols()]
            for sec in elf.iter_sections():
                if not isinstance(sec, RelocationSection) or sec["sh_info"] not in self.sections:
                    continue
                table = elf.get_section(sec["sh_link"])
                data = self.sections[sec["sh_info"]]["data"]
                out = self.relocs.setdefault(sec["sh_info"], {})
                for r in sec.iter_relocations():
                    if r["r_info_type"] != R_SH_DIR32:
                        raise SystemExit(f"{path}: unsupported relocation type {r['r_info_type']}")
                    s = table.get_symbol(r["r_info_sym"])
                    off = r["r_offset"]
                    # R_SH_DIR32 is partial-in-place: SHC keeps its offset in the word
                    add = ((r["r_addend"] if sec.is_RELA() else 0) + int.from_bytes(data[off:off + 4], "big"))
                    add &= 0xFFFFFFFF
                    add = add - (1 << 32) if add & 0x80000000 else add
                    out[off] = (s.name, s["st_shndx"], s["st_value"], add, s["st_info"]["type"])
        # functions of .text in address order: (offset, name, global)
        self.funcs = sorted((v, n, b == "STB_GLOBAL") for n, v, shndx, typ, b in self.syms
                            if shndx == self.text and typ == "STT_FUNC")

    def undefined(self):
        return {n for n, _, shndx, _, _ in self.syms if shndx == "SHN_UNDEF" and n}


class Plan:
    """Which part of an object a unit owns: text offsets [lo, hi) and the
    functions in it that other code may name."""

    def __init__(self, unit, obj: Obj, whole: bool, entry_of, siblings=()):
        """siblings: the ROM ranges of the other units cut from the same source."""
        self.unit, self.obj, self.whole, self.entry_of = unit, obj, whole, entry_of
        named = {off: entry_of(name) for off, name, _ in obj.funcs}
        mine = [off for off, a in named.items() if a is not None and unit.start <= a < unit.end]
        if not mine:
            raise SystemExit(f"{unit.arg}: defines no function of {unit.start:#x}-{unit.end:#x} "
                             "(a function is found by its name: f_<rom offset>, "
                             "or a name in config/function_names.txt)")
        if whole:
            self.lo, self.hi = 0, len(obj.data)
        else:
            order = [off for off, _, _ in obj.funcs]
            i, j = order.index(min(mine)), order.index(max(mine))
            # A function with no address name is new, and goes with the
            # unit's function next to it. Between the functions of two units
            # cut from this source it goes with the one before it, so that
            # only one of them takes it.
            while j + 1 < len(order) and named[order[j + 1]] is None:
                j += 1
            k = i
            while k > 0 and named[order[k - 1]] is None:
                k -= 1
            before = named[order[k - 1]] if k > 0 else None
            if before is None or not any(s <= before < e for s, e in siblings):
                i = k
            self.lo = order[i]
            self.hi = order[j + 1] if j + 1 < len(order) else len(obj.data)
            for off in order[i:j + 1]:
                a = named[off]
                if a is not None and not unit.start <= a < unit.end:
                    name = next(n for o, n, _ in obj.funcs if o == off)
                    raise SystemExit(f"{unit.arg}: {name[1:]} lies between the functions of "
                                     f"{unit.start:#x}-{unit.end:#x}, which another unit owns")
        self.entries = {}    # rom offset -> symbol, for every function other code may name
        self.exports = {}    # text offset -> (symbol, global)
        self.alias = {}      # text offset -> f_<rom offset> name, when the function has another
        for off, name, glob in obj.funcs:
            if not self.lo <= off < self.hi:
                continue
            a = named[off]
            if a is not None:
                self.entries[a] = name
                if name != f"_f_{a:x}":
                    # other sources call it by address
                    self.alias[off] = f"_f_{a:x}"
            # an address-named function is global even when the source says
            # static: tables and assembly reach it by address
            self.exports[off] = (name, a is not None or glob)
        self.settle()

    def settle(self):
        """Decode the unit's code, and find its static functions. SHC gives a
        static function no symbol, so one is known only by the call (or the
        pointer) that reaches it. Inside the cut it is decoded like the rest;
        directly before the cut, where a helper defined ahead of the unit's
        first function lands, the cut grows to take it in."""
        obj = self.obj
        symbols = {off for off, _, _ in obj.funcs}
        self.statics = set()
        while True:
            self.insns, self.slots = decode_functions(obj, set(self.exports) | self.statics)
            # what the unit's own code calls or points to, and what the
            # object's tables point to (those only count inside the cut)
            mine = {i.target for pc, i in self.insns.items()
                    if self.lo <= pc < self.hi and i.flow == "call" and i.target is not None}
            tables = set()
            for sec, rels in obj.relocs.items():
                for off, (name, shndx, value, add, styp) in rels.items():
                    if shndx == obj.text:
                        tgt = (0 if styp == "STT_SECTION" else value) + add
                        (mine if sec == obj.text and self.lo <= off < self.hi else tables).add(tgt)
            new, lo = set(), self.lo
            for tgt in sorted((mine | tables) - symbols - self.statics, reverse=True):
                if tgt % 2:
                    continue
                if self.lo <= tgt < self.hi:
                    new.add(tgt)
                elif tgt in mine and tgt < lo and not any(tgt <= s < lo for s in symbols):
                    new.add(tgt)
                    lo = tgt
            if not new:
                return
            self.statics |= new
            self.lo = lo


def decode_functions(obj: Obj, entries):
    """Instructions reached by recursive descent from the given function
    offsets, using asm_scan's walker on the object's bytes."""
    every = {off for off, _, _ in obj.funcs}
    saved = fn.ROM
    fn.ROM = bytes(obj.data) + bytes(8)
    try:
        insns, slots = {}, set()
        for e in sorted(entries):
            f = asm_scan.Func(e, len(obj.data), every - {e}, 0, len(obj.data))
            for pc, i in f.insns.items():
                if i.valid:
                    insns[pc] = i
                    if i.delay:
                        slots.add(pc + 2)
    finally:
        fn.ROM = saved
    return insns, slots


def render(plan: Plan, addr_expr, section=".text"):
    """Assembly package of a planned unit, the fixups it needs and the names
    of the other sections it carries.

    addr_expr(value) gives the symbol expression for a pool word whose value
    is an address this build moves, or None."""
    u, obj, lo, hi = plan.unit, plan.obj, plan.lo, plan.hi
    text, trel = obj.data, obj.relocs.get(obj.text, {})
    func_at = {off: name for off, name, _ in obj.funcs}
    labels = set()
    aux = {}  # section index -> label, for the sections this unit carries

    def where(off):
        return f"{u.arg} (.text+{off:#x})"

    def other_unit(tgt, at, what):
        name = func_at.get(tgt)
        if name is None or plan.entry_of(name) is None:
            raise SystemExit(f"{at}: {what} .text+{tgt:#x}, which is neither inside this unit nor a "
                             "function another unit defines by address name")
        return name

    def reloc_expr(rel, at):
        name, shndx, value, add, styp = rel
        if shndx == "SHN_UNDEF":
            return f"{name}{add:+d}" if add else name
        base = 0 if styp == "STT_SECTION" else value
        if shndx == obj.text:
            tgt = base + add
            if lo <= tgt < hi:
                labels.add(tgt)
                return f".Ls{tgt:x}"
            return other_unit(tgt, at, "refers to")
        if isinstance(shndx, int) and shndx in obj.sections:
            sec = obj.sections[shndx]
            if not plan.whole and (sec["write"] or sec["nobits"]):
                raise SystemExit(f"{u.arg}: uses writable data of its own ({sec['name']}). A unit cut from a "
                                 "context source cannot own any, since other units compile the same file: "
                                 "define the variable in a new file of the mod directory and declare it extern here")
            aux.setdefault(shndx, f".Lx{shndx}")
            return f"{aux[shndx]}{base + add:+d}" if base + add else aux[shndx]
        raise SystemExit(f"{at}: relocation against {name or shndx} is not supported")

    insns, slots = plan.insns, plan.slots
    fixups = []
    patched = {}    # offset -> (instruction, original pool offset), then the new word
    literal = {}    # offset of a 4-byte pool word -> expression
    islands = {}    # original pool offset -> [size, position, content]
    for pc in sorted(insns):
        i = insns[pc]
        if not lo <= pc < hi:
            continue
        if pc in slots and (i.pool is not None or i.mova is not None):
            # it would count from the branch target; SHC does not emit one
            raise SystemExit(f"{where(pc)}: pc-relative load in a delay slot is not supported")
        if i.mova is not None and not lo <= i.mova < hi:
            raise SystemExit(f"{where(pc)}: mova to .text+{i.mova:#x}, outside the unit")
        if i.pool is not None:
            tgt, size = i.pool
            if tgt + size > len(text):
                raise SystemExit(f"{where(pc)}: pool load past the end of the object")
            if lo <= tgt < hi:
                if size == 4 and tgt not in trel:
                    e = addr_expr(int.from_bytes(text[tgt:tgt + 4], "big"))
                    if e is not None:
                        literal[tgt] = e
                continue
            if tgt < lo:
                raise SystemExit(f"{where(pc)}: pool load before the unit")
            if tgt not in islands:
                if size == 4 and tgt in trel:
                    content = reloc_expr(trel[tgt], where(tgt))
                elif size == 4:
                    v = int.from_bytes(text[tgt:tgt + 4], "big")
                    content = addr_expr(v) or f"0x{v:08x}"
                else:
                    content = f"0x{int.from_bytes(text[tgt:tgt + 2], 'big'):04x}"
                islands[tgt] = [size, None, content]
            patched[pc] = (i, tgt)
            continue
        if i.target is not None and i.flow in ("cond", "jump", "call") and not lo <= i.target < hi:
            name = other_unit(i.target, where(pc), f"{i.mnemonic} to")
            fixups.append({"at_sym": u.sym, "at_off": pc - lo, "target_sym": name,
                           "target": plan.entry_of(name), "kind": i.mnemonic, "insn": i.word})
    # local copies of the pool words that lie past the unit: longwords first
    pos = hi
    tail = []
    if any(v[0] == 4 for v in islands.values()) and pos % 4:
        tail.append("\t.short\t0x0009")
        pos += 2
    for size in (4, 2):
        for tgt in sorted(t for t, v in islands.items() if v[0] == size):
            islands[tgt][1] = pos
            tail.append(f"\t.{size}byte\t{islands[tgt][2]}\t/* copy of .text+{tgt:#x} */")
            pos += size
    for pc, (i, tgt) in list(patched.items()):
        size, at, _ = islands[tgt]
        if size == 4:
            d = at - ((pc + 4) & ~3)
            ok = d % 4 == 0 and 0 <= d < 1024
            w = (i.word & 0xFF00) | (d // 4)
        else:
            d = at - (pc + 4)
            ok = d % 2 == 0 and 0 <= d < 512
            w = (i.word & 0xFF00) | (d // 2)
        if not ok:
            raise SystemExit(f"{where(pc)}: its pool word is out of reach once copied to the end of the unit")
        patched[pc] = w
    words = {}
    for off, rel in trel.items():
        if lo <= off < hi:
            if off + 4 > hi:
                raise SystemExit(f"{where(off)}: relocation straddles the end of the unit")
            words[off] = reloc_expr(rel, where(off))
    for off, e in literal.items():
        words.setdefault(off, e)

    # the sections the functions refer to (for a whole file, all of them),
    # rendered first: their relocations can name labels in the code
    data_lines = []
    done = []
    todo = sorted(i for i in obj.sections if i != obj.text) if plan.whole else sorted(aux)
    while todo:
        idx = todo.pop(0)
        if idx in done:
            continue
        done.append(idx)
        sec = obj.sections[idx]
        label = aux.setdefault(idx, f".Lx{idx}")
        if sec["nobits"]:
            data_lines += [f"\t.section {sec['name']},\"aw\",@nobits", "\t.align 2", f"{label}:",
                           f"\t.space {sec['size']}"]
            continue
        flags = "aw" if sec["write"] else "a"
        data_lines += [f"\t.section {sec['name']},\"{flags}\"", "\t.align 2", f"{label}:"]
        rel = obj.relocs.get(idx, {})
        data = sec["data"]
        before = set(aux)
        o = 0
        run = []
        while o < len(data):
            if o in rel:
                for k in range(0, len(run), 16):
                    data_lines.append("\t.byte\t" + ",".join(f"0x{b:02x}" for b in run[k:k + 16]))
                run = []
                at = f"{u.arg} ({sec['name']}+{o:#x})"
                data_lines.append(f"\t.4byte\t{reloc_expr(rel[o], at)}")
                o += 4
                continue
            run.append(data[o])
            o += 1
        for k in range(0, len(run), 16):
            data_lines.append("\t.byte\t" + ",".join(f"0x{b:02x}" for b in run[k:k + 16]))
        todo += sorted(set(aux) - before)
    if plan.whole:
        # the variables a whole file defines for others
        for name, value, shndx, typ, bind in obj.syms:
            if isinstance(shndx, int) and shndx in aux and bind == "STB_GLOBAL" and name:
                data_lines += [f"\t.global {name}", f"\t.set {name}, {aux[shndx]}+{value}"]

    lines = [f"/* {u.start:#08x}-{u.end:#08x}: {u.arg}, .text+{lo:#x}..{hi:#x} of the compiled object, "
             "generated by tools/srcunit.py */",
             f"\t.section {section},\"ax\"", f"\t.global {u.sym}", f"{u.sym}:"]
    bad = [x for x in labels if not lo <= x < hi or x % 2]
    if bad:
        raise SystemExit(f"{u.arg}: label at odd or outside offset .text+{bad[0]:#x}")
    off = lo
    while off < hi:
        if off in plan.exports:
            name, glob = plan.exports[off]
            if glob:
                lines.append(f"\t.global {name}")
            lines += [f"\t.type {name}, @function", f"{name}:"]
            if off in plan.alias:
                lines += [f"\t.global {plan.alias[off]}", f"{plan.alias[off]}:"]
        if off in labels:
            lines.append(f".Ls{off:x}:")
        if off in words:
            if off + 2 in labels or off + 2 in plan.exports:
                raise SystemExit(f"{where(off)}: label inside a relocated word")
            lines.append(f"\t.4byte\t{words[off]}")
            off += 4
            continue
        if off in patched:
            lines.append(f"\t.short\t0x{patched[off]:04x}\t/* {insns[off].text()} -> local copy */")
        else:
            w = text[off] << 8 | text[off + 1]
            i = insns.get(off)
            lines.append(f"\t.short\t0x{w:04x}" + (f"\t/* {i.text()} */" if i is not None else ""))
        off += 2
    lines += tail + data_lines
    return "\n".join(lines) + "\n", fixups, sorted({obj.sections[i]["name"] for i in done})
