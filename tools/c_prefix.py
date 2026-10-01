#!/usr/bin/env python3
"""Compile contextual C, verify a prefix, retain only its unmodified bytes.

No bytes are sourced from the reference ROM: it is a comparison oracle only.
This intentionally supports a conservative subset of control flow. Unsupported
indirect jumps and PC-relative delay-slot loads fail closed.
"""
import hashlib
import json
import re
import subprocess
from pathlib import Path


class PrefixError(ValueError):
    pass


def validate_prefix(compiled, rom, start, end, entries):
    """Verify linked bytes and reachable cross-boundary literal dependencies.

Addresses are ROM offsets; compiled[0] corresponds to start. entries maps all
contextual function starts to names. The cut must be at a function boundary.
Returns the external literal dependency manifest. No instruction is rewritten.
"""
    size = end - start
    if start < 0 or start % 2 or end % 2 or size <= 0 or end > len(rom):
        raise PrefixError('invalid prefix bounds')
    if size > len(compiled):
        raise PrefixError('compiled context shorter than prefix')
    if start not in entries or (end not in entries and size != len(compiled)):
        raise PrefixError('prefix must start/end at contextual function boundaries')
    for addr, name in entries.items():
        m = re.fullmatch(r'_?f_([0-9a-f]+)', name)
        if start <= addr < end and m and int(m[1], 16) != addr:
            raise PrefixError(f'kept function {name} is at the wrong address {addr:#x}')
    if compiled[:size] != rom[start:end]:
        first = next(i for i, (a, b) in enumerate(zip(compiled[:size], rom[start:end])) if a != b)
        raise PrefixError(f'compiled prefix differs from ROM at {start + first:#x}')

    def word(pc):
        off = pc - start
        if off < 0 or off + 2 > size:
            raise PrefixError(f'instruction crosses prefix boundary at {pc:#x}')
        return int.from_bytes(compiled[off:off + 2], 'big')

    def literal(w, pc):
        if w >> 12 == 9:
            return pc + 4 + (w & 255) * 2, 2
        if w >> 12 == 13:
            return ((pc + 4) & ~3) + (w & 255) * 4, 4
        return None

    def sext(n, bits):
        return n - (1 << bits) if n & (1 << (bits - 1)) else n

    dependencies = {}
    visited = set()
    pending = [a for a in entries if start <= a < end]

    def follow(pc):
        if start <= pc < end:
            pending.append(pc)
        else:
            # Direct calls/tail falls may reach retained assembly functions,
            # but never an arbitrary byte in discarded context.
            name = entries.get(pc, '')
            m = re.fullmatch(r'_?f_([0-9a-f]+)', name)
            if not m or int(m[1], 16) != pc:
                raise PrefixError(f'unverified control-flow exit to {pc:#x}')

    def check_instruction(pc, delay=False):
        w = word(pc)
        pool = literal(w, pc)
        if delay and (pool or w & 0xff00 == 0xc700):
            raise PrefixError(f'PC-relative delay-slot instruction at {pc:#x} is unsupported')
        if pool:
            target, width = pool
            if target + width > end:
                off = target - start
                if off < 0 or off + width > len(compiled) or target + width > len(rom):
                    raise PrefixError(f'literal outside compiled context at {pc:#x}')
                value = compiled[off:off + width]
                if value != rom[target:target + width]:
                    raise PrefixError(f'discarded literal differs at {target:#x} (load {pc:#x})')
                dependencies[pc] = dict(pc=pc, target=target, size=width, value=value.hex())
        # MOVA can expose an arbitrary-length table. Do not guess its extent.
        if w & 0xff00 == 0xc700:
            raise PrefixError(f'MOVA/table addressing at {pc:#x} is unsupported')
        if w & 0xf0ff in (0x0023, 0x0003, 0x402b) or w in (0x002b, 0x001b) or w & 0xff00 == 0xc300:
            raise PrefixError(f'indirect jump/exception control flow at {pc:#x} is unsupported')
        if delay and (w == 0x000b or w >> 12 in (10, 11) or
                      w & 0xff00 in (0x8900, 0x8b00, 0x8d00, 0x8f00) or w & 0xf0ff == 0x400b):
            raise PrefixError(f'control transfer in delay slot at {pc:#x}')
        return w

    while pending:
        pc = pending.pop()
        if pc in visited:
            continue
        visited.add(pc)
        w = check_instruction(pc)
        if w == 0x000b:  # RTS, including its executed delay slot
            check_instruction(pc + 2, True)
        elif w >> 12 in (10, 11):  # BRA / BSR
            check_instruction(pc + 2, True)
            follow(pc + 4 + sext(w & 0xfff, 12) * 2)
            if w >> 12 == 11:
                follow(pc + 4)
        elif w & 0xff00 in (0x8900, 0x8b00, 0x8d00, 0x8f00):
            delayed = w & 0xff00 in (0x8d00, 0x8f00)
            if delayed:
                check_instruction(pc + 2, True)
            follow(pc + 4 + sext(w & 255, 8) * 2)
            follow(pc + (4 if delayed else 2))
        elif w & 0xf0ff == 0x400b:  # JSR; all contextual callees are also roots
            check_instruction(pc + 2, True)
            follow(pc + 4)
        else:
            follow(pc + 2)
    return [dependencies[pc] for pc in sorted(dependencies)]


def compile_prefix(src, out, start, end, *, rom, shcc, version, flags, symbols, assembler):
    """Produce a relocation-free object with only verified compiler bytes.

Full-context compilation/linking is deliberately repeated each build: a stale
manifest or changed literal dependency must never bypass validation.
"""
    from elftools.elf.elffile import ELFFile
    src, out = Path(src), Path(out)
    context = out.with_suffix('.context.o')
    linked = out.with_suffix('.context.elf')
    script = out.with_suffix('.context.ld')
    binary = out.with_suffix('.context.bin')
    prefix = out.with_suffix('.prefix.bin')
    wrapper = out.with_suffix('.prefix.s')
    ram_delta = 0x5fff880
    if not 0x780 <= start < end <= 0x313fc:
        raise PrefixError('c-prefix currently supports only the copied RAM code block')
    subprocess.run([str(shcc), version, str(context), str(src), *flags], check=True)
    definitions = {}
    exports = []
    unresolved = []
    with context.open('rb') as f:
        elf = ELFFile(f)
        text_index = next(i for i, sec in enumerate(elf.iter_sections()) if sec.name == '.text')
        for sec in elf.iter_sections():
            if sec.name != '.text' and sec['sh_flags'] & 2 and sec['sh_size']:
                raise PrefixError(f'context contains unsupported allocated section {sec.name}')
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            if symbol['st_shndx'] == 'SHN_UNDEF' and symbol.name:
                unresolved.append(symbol.name)
            if symbol['st_shndx'] == text_index and symbol['st_info']['type'] == 'STT_FUNC':
                addr = start + symbol['st_value']
                definitions[addr] = symbol.name
                if start <= addr < end:
                    exports.append((symbol.name, symbol['st_value'], symbol['st_info']['bind']))
    resolved = {}
    for name in unresolved:
        key = name[1:] if name.startswith('_') else name
        addr = symbols.get(key)
        if addr is None:
            m = re.fullmatch(r'(f|d|g)_([0-9a-f]+)', key)
            if m:
                addr = int(m[2], 16)
                if m[1] == 'f' and 0x780 <= addr < 0x313fc:
                    addr += ram_delta
        if addr is None:
            raise PrefixError(f'unresolved contextual symbol {name}')
        resolved[name] = addr
    script.write_text(
        f'SECTIONS {{ .text 0x{start + ram_delta:x} : {{ *(.text) }} /DISCARD/ : {{ *(.comment) }} }}\n' +
        '\n'.join(f'{name} = 0x{addr:x};' for name, addr in sorted(resolved.items())) + '\n')
    subprocess.run(['sh-elf-ld', '-T', str(script), '-o', str(linked), str(context)], check=True)
    subprocess.run(['sh-elf-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(binary)], check=True)
    compiled = binary.read_bytes()
    deps = validate_prefix(compiled, rom, start, end, definitions)
    kept = compiled[:end - start]
    prefix.write_bytes(kept)
    # This generated wrapper only packages SHC output; it has no instructions
    # or ROM-derived .short data. Export no context-only functions.
    lines = ['.text', '.Lcompiled_prefix:']
    for name, offset, binding in exports:
        if binding == 'STB_GLOBAL':
            lines.append(f'.global {name}')
        elif binding == 'STB_WEAK':
            lines.append(f'.weak {name}')
        lines += [f'.type {name}, @function', f'{name} = .Lcompiled_prefix + {offset}']
    lines.append(f'.incbin "{prefix.resolve()}"')
    wrapper.write_text('\n'.join(lines) + '\n')
    subprocess.run([*assembler, '-no-pad-sections', '-o', str(out), str(wrapper)], check=True)
    manifest = dict(source=str(src), start=start, end=end, kept_bytes=len(kept),
                    context_bytes=len(compiled), source_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),
                    context_sha256=hashlib.sha256(compiled).hexdigest(),
                    prefix_sha256=hashlib.sha256(kept).hexdigest(),
                    functions=[name for name, _, _ in exports], literal_dependencies=deps)
    out.with_suffix('.prefix.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(f'c-prefix: {src.name} {start:#x}-{end:#x}: {len(kept)} verified C bytes; '
          f'{len(compiled) - len(kept)} context-only bytes excluded; {len(deps)} external literal loads')
