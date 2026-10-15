"""Prove a compiler-generated same-function SH2 literal loop backedge.

Supported families are MOV.L @(disp,PC),Rn; JMP @Rn; NOP for R1 or R2.
The destination must already be reachable without any indirect-loop edges.
"""
from dataclasses import dataclass
import io
import re
from switch_proof import OriginalText, overlaps


@dataclass(frozen=True)
class LocalLoopOriginal:
    text: OriginalText
    relocations: tuple

    @classmethod
    def read(cls, path, origin, error):
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        raw = path.read_bytes()
        original = OriginalText.from_bytes(raw, origin, error)
        elf = ELFFile(io.BytesIO(raw))
        index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == '.text')
        facts = []
        for section in elf.iter_sections():
            if not isinstance(section, RelocationSection) or section['sh_info'] != index:
                continue
            symbols = elf.get_section(section['sh_link'])
            for rel in section.iter_relocations():
                symbol = symbols.get_symbol(rel['r_info_sym'])
                off = rel['r_offset']
                facts.append(dict(address=origin + off, width=4, type=rel['r_info_type'],
                                  symbol=symbol.name, symbol_type=symbol['st_info']['type'],
                                  binding=symbol['st_info']['bind'],
                                  is_text=symbol['st_shndx'] == index,
                                  symbol_value=symbol['st_value'],
                                  explicit_addend=rel['r_addend'] if section.is_RELA() else 0,
                                  embedded_addend=int.from_bytes(original.data[off:off + 4], 'big')))
        return cls(original, tuple(facts))


class LocalLoopJumps:
    def __init__(self, original, compiled, rom, start, end, origin, entries, word, error):
        if (not isinstance(original, LocalLoopOriginal) or original.text.origin != origin or
                original.text.entries != entries or len(original.text.data) != len(compiled)):
            raise error('missing or inconsistent original ELF loop provenance')
        if sorted(original.text.relocations) != sorted((r['address'], r['width']) for r in original.relocations):
            raise error('inconsistent original ELF loop relocation inventory')
        self.original, self.compiled, self.rom = original, compiled, rom
        self.start, self.end, self.origin = start, end, origin
        self.entries, self.word, self.error = entries, word, error
        self.candidates = {}

    def candidate(self, jump):
        if jump in self.candidates:
            return self.candidates[jump]
        load, slot = jump - 2, jump + 2
        w, fail = self.word, self.error
        if load < self.start or slot + 2 > self.end:
            raise fail('unsupported local loop jump shape')
        register = (w(load) >> 8) & 15
        if (register not in (1, 2) or w(load) & 0xf000 != 0xd000 or
                w(jump) != (0x402b | register << 8) or w(slot) != 0x0009):
            raise fail('unsupported local loop jump shape')
        roots = sorted(self.entries)
        owners = [a for a in roots if a <= load]
        if not owners:
            raise fail('local loop has no original function owner')
        owner = owners[-1]
        following = [a for a in roots if a > owner]
        if not following:
            raise fail('local loop requires an actual following function root')
        finish = following[0]
        name = self.entries[owner]
        match = re.fullmatch(r'_?f_([0-9a-f]+)', name)
        if (not match or int(match[1], 16) != owner or
                not self.start <= owner <= load < slot + 2 <= finish <= self.end):
            raise fail('local loop has invalid original function ownership')
        pool = ((load + 4) & ~3) + (w(load) & 255) * 4
        off = pool - self.origin
        if off < 0 or off + 4 > len(self.compiled) or pool + 4 > len(self.rom):
            raise fail('local loop literal outside original context')
        relocations = [r for r in self.original.relocations
                       if overlaps((pool, pool + 4), (r['address'], r['address'] + r['width']))]
        if len(relocations) != 1:
            raise fail('local loop literal needs exactly one original relocation')
        rel, = relocations
        if (rel['address'] != pool or rel['width'] != 4 or rel['type'] != 1 or
                rel['symbol'] != '.text' or rel['symbol_type'] != 'STT_SECTION' or
                rel['binding'] != 'STB_LOCAL' or not rel['is_text'] or
                rel['symbol_value'] != 0 or rel['explicit_addend'] != 0):
            raise fail('local loop requires the original local text-section relocation')
        raw_addend = int.from_bytes(self.original.text.data[off:off + 4], 'big')
        if rel['embedded_addend'] != raw_addend:
            raise fail('local loop embedded addend disagrees with original text')
        # R_SH_DIR32 from this SHC converter keeps the section offset in place,
        # including for its zero-addend RELA records. Check both original and
        # linked values instead of interpreting a linked pointer as provenance.
        target = self.origin + raw_addend
        expected_value = target + 0x5fff880
        value = self.compiled[off:off + 4]
        if (value != self.rom[pool:pool + 4] or
                int.from_bytes(value, 'big') != expected_value):
            raise fail('local loop relocation does not reconstruct the linked ROM pointer')
        if target % 2 or not owner < target < load:
            raise fail('local loop target is not a backward point in the same function')
        c = dict(family=f'sh2-local-loop-r{register}-nop-v1', owner=owner, owner_end=finish,
                 owner_symbol=name, load=load, jump=jump, slot=slot, register=register,
                 words=[f'{w(pc):04x}' for pc in (load, jump, slot)],
                 pool=pool, value=int.from_bytes(value, 'big'), target=target,
                 original_relocation=dict(rel),
                 original_object_sha256=self.original.text.object_sha256)
        self.candidates[jump] = c
        return c

    def finish(self, visited, predecessors, delay_owners, literal_reads):
        fail = self.error
        # Reachability with all indirect-loop edges removed is independent of
        # the destinations we just added. It prevents a pointer into unused data
        # from manufacturing its own purported code entry.
        normal_edges = {}
        for destination, sources in predecessors.items():
            for source in sources:
                if source is not None and source not in self.candidates:
                    normal_edges.setdefault(source, set()).add(destination)
        reachable = set()
        pending = [a for a in self.entries if self.start <= a < self.end]
        while pending:
            pc = pending.pop()
            if pc not in reachable:
                reachable.add(pc)
                pending.extend(normal_edges.get(pc, ()))
        pools = [(a, a + width) for a, width in literal_reads.values()]
        for c in self.candidates.values():
            load, jump, slot, target = (c[k] for k in ('load', 'jump', 'slot', 'target'))
            if (load not in reachable or jump not in reachable or target not in reachable or
                    predecessors.get(jump) != {load} or delay_owners.get(load) or
                    delay_owners.get(jump)):
                raise fail('local loop lacks ordinary reachability or a unique defining load')
            if (slot in visited or predecessors.get(slot) or delay_owners.get(slot) != {jump}):
                raise fail('alternate entry in local loop delay slot')
            if (target in delay_owners or
                    any(target in (other['jump'], other['slot']) for other in self.candidates.values()) or
                    any(a <= target < b for a, b in pools)):
                raise fail('local loop target enters data, another jump spine or a delay slot')
            if any(p is None or not c['owner'] <= p < c['owner_end']
                   for p in predecessors.get(load, ())):
                raise fail('local loop defining load has a foreign function entry')
            c.update(complete_cfg_checked=True, load_dominates_jump=True,
                     target_reachable_without_indirect_edges=True, delay_owner=jump,
                     jump_predecessors=[load], no_fallthrough=True)
        if self.candidates:
            for pc in visited | set(delay_owners):
                if any(overlaps((pc, pc + 2), pool) for pool in pools):
                    raise fail('local loop graph executes literal data')
        return list(self.candidates.values())
