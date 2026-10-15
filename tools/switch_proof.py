"""Fixed SH2 guarded switch proof; original compiler ELF is the authority.

Only the 13-entry R0/R1, signed-word, forward-local-target family is supported.
Recognition adds edges; acceptance waits for the complete retained CFG.
"""
from dataclasses import dataclass
import hashlib
import re


@dataclass(frozen=True)
class OriginalText:
    origin: int
    data: bytes
    entries: dict
    relocations: tuple
    object_sha256: str

    @classmethod
    def read(cls, path, origin, error):
        return cls.from_bytes(path.read_bytes(), origin, error)

    @classmethod
    def from_bytes(cls, raw, origin, error):
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        import io
        elf = ELFFile(io.BytesIO(raw))
        index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == '.text')
        section = elf.get_section(index)
        entries = {}
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            if symbol['st_shndx'] == index and symbol['st_info']['type'] == 'STT_FUNC':
                address = origin + symbol['st_value']
                if address in entries:
                    raise error('switch provenance has ambiguous function aliases')
                entries[address] = symbol.name
        relocs = []
        for s in elf.iter_sections():
            if isinstance(s, RelocationSection) and s['sh_info'] == index:
                for relocation in s.iter_relocations():
                    off = relocation['r_offset']
                    # Reject unknown-width relocations instead of guessing overlap.
                    if relocation['r_info_type'] != 1 or not 0 <= off <= len(section.data()) - 4:
                        raise error('switch provenance has unsupported text relocation')
                    relocs.append((origin + off, 4))
        return cls(origin, section.data(), entries, tuple(relocs),
                   hashlib.sha256(raw).hexdigest())


def overlaps(a, b):
    return a[0] < b[1] and b[0] < a[1]


class GuardedSwitches:
    def __init__(self, original, compiled, rom, start, end, origin, entries, word, error):
        self.error = error
        if (not isinstance(original, OriginalText) or original.origin != origin or
                original.entries != entries or len(original.data) != len(compiled)):
            raise error('missing or inconsistent original ELF switch provenance')
        self.original, self.compiled, self.rom = original, compiled, rom
        self.start, self.end, self.origin = start, end, origin
        self.entries, self.word = entries, word
        self.candidates = {}

    def candidate(self, mova):
        if mova in self.candidates:
            return self.candidates[mova]
        w, fail = self.word, self.error
        jump, slot = mova + 4, mova + 6
        guard = mova - 10
        if guard < self.start or slot + 2 > self.end:
            raise fail('switch spine outside retained range')
        # Two explicit compiler layouts for the same unsigned bound. In the
        # extended layout BF skips a BRA/NOP default exit, taking only R0<13
        # into the scaling/load/BRAF spine. No general branch inversion is
        # inferred, and both delay slots must have exclusive ownership.
        extended = w(guard) != 0xe10d
        if extended:
            guard = mova - 14
            if guard < self.start:
                raise fail('switch spine outside retained range')
            shape = [0xe10d, 0x3012, 0x8b01, None, 0x0009,
                     0x4000, 0x6103, None, 0x001d, 0x0023, 0x0009]
            transfer_ok = w(guard + 6) >> 12 == 10
            family = 'sh2-guarded13-r0-r1-bf-bra-v1'
        else:
            shape = [0xe10d, 0x3012, None, 0x4000, 0x6103, None, 0x001d, 0x0023, 0x0009]
            transfer_ok = w(guard + 4) & 0xff00 == 0x8900
            family = 'sh2-guarded13-r0-r1-v1'
        pcs = list(range(guard, slot + 2, 2))
        if (any(expected is not None and w(pc) != expected for pc, expected in zip(pcs, shape)) or
                not transfer_ok or w(mova) & 0xff00 != 0xc700):
            raise fail('unsupported guarded switch shape')
        roots = sorted(self.entries)
        owners = [a for a in roots if a <= guard]
        if not owners:
            raise fail('switch has no original function owner')
        owner = owners[-1]
        following = [a for a in roots if a > owner]
        if not following:
            raise fail('switch requires an actual following function root')
        finish = following[0]
        name = self.entries[owner]
        match = re.fullmatch(r'_?f_([0-9a-f]+)', name)
        if (not match or int(match[1], 16) != owner or
                not self.start <= owner <= guard < slot + 2 <= finish <= self.end):
            raise fail('switch spine has invalid original function ownership')
        table = ((mova + 4) & ~3) + (w(mova) & 255) * 4
        table_end = table + 26
        if not owner <= table < table_end <= finish or table % 4:
            raise fail('switch table outside original function')
        off = table - self.origin
        payload = self.original.data[off:off + 26]
        if (len(payload) != 26 or payload != self.compiled[off:off + 26] or
                payload != self.rom[table:table_end]):
            raise fail('switch table differs from original ELF text')
        if any(overlaps((table, table_end), (a, a + width))
               for a, width in self.original.relocations):
            raise fail('switch table overlaps original ELF relocation')
        offsets = [int.from_bytes(payload[i:i + 2], 'big', signed=True) for i in range(0, 26, 2)]
        targets = [jump + 4 + displacement for displacement in offsets]
        if extended:
            disp = w(guard + 6) & 4095
            default = guard + 10 + (disp - 4096 if disp & 2048 else disp) * 2
        else:
            disp = w(guard + 4) & 255
            default = guard + 8 + (disp - 256 if disp & 128 else disp) * 2
        if any(a % 2 or not table_end <= a < finish for a in targets):
            raise fail('switch case target is not forward local code')
        if not owner <= default < finish or default % 2:
            raise fail('switch default outside original function')
        result = dict(family=family, owner=owner, owner_end=finish,
                      owner_symbol=name, guard=guard, mova=mova, jump=jump, slot=slot,
                      spine=[pc for pc in pcs[:-1] if not (extended and pc == guard + 8)],
                      words=[f'{w(pc):04x}' for pc in pcs],
                      bound=13, comparison='unsigned >=', index_range=[0, 12],
                      byte_offset_range=[0, 24], default=default, table=table,
                      table_end=table_end, table_hex=payload.hex(),
                      table_sha256=hashlib.sha256(payload).hexdigest(),
                      table_offsets=offsets, targets=targets, braf_base=jump + 4,
                      mova_base=((mova + 4) & ~3), table_symbol_required=False,
                      original_object_sha256=self.original.object_sha256)
        if extended:
            result.update(default_jump=guard + 6, default_slot=guard + 8)
        self.candidates[mova] = result
        return result

    def finish(self, visited, predecessors, delay_owners, literal_reads):
        fail = self.error
        candidates = list(self.candidates.values())
        tables = [(c['table'], c['table_end']) for c in candidates]
        pools = [(a, a + width) for a, width in literal_reads.values()]
        for i, c in enumerate(candidates):
            table, spine = tables[i], c['spine']
            if any(pc not in visited or delay_owners.get(pc) for pc in spine):
                raise fail('unreachable or delay-owned switch spine')
            for pc in spine[1:]:
                expected = {c['guard'] + 4} if 'default_jump' in c and pc == c['guard'] + 10 else {pc - 2}
                if predecessors.get(pc) != expected:
                    raise fail(f'alternate entry in switch spine at {pc:#x}')
            slot, jump = c['slot'], c['jump']
            if (slot in visited or predecessors.get(slot) or delay_owners.get(slot) != {jump}):
                raise fail('alternate entry in switch delay slot')
            if 'default_jump' in c:
                ds, dj = c['default_slot'], c['default_jump']
                if ds in visited or predecessors.get(ds) or delay_owners.get(ds) != {dj}:
                    raise fail('alternate entry in switch default delay slot')
                c['default_delay_owner'] = dj
            if any(overlaps(table, (pc, pc + 2)) for pc in visited | set(delay_owners)):
                raise fail('switch table overlaps reachable code or delay slot')
            if any(overlaps(table, pool) for pool in pools):
                raise fail('ordinary literal read aliases switch table')
            if any(i != j and overlaps(table, other) for j, other in enumerate(tables)):
                raise fail('overlapping switch tables')
            for target in c['targets'] + [c['default']]:
                if (target in delay_owners or target in spine or target == slot or
                        any(a <= target < b for a, b in pools + tables) or target not in visited):
                    raise fail('switch destination enters data, spine or delay slot')
            # The unique checked predecessor chain proves the guard/branch dominate
            # dispatch from every retained root, including late discovered edges.
            c['complete_cfg_checked'] = True
            c['guard_dominates_dispatch'] = True
            c['spine_predecessors'] = {str(pc): sorted(predecessors[pc]) for pc in spine[1:]}
            c['delay_owner'] = jump
            c['table_relocations'] = []
        if candidates:
            for pc in visited | set(delay_owners):
                if any(overlaps((pc, pc + 2), pool) for pool in pools):
                    raise fail('reachable instruction overlaps ordinary literal data')
        return candidates
