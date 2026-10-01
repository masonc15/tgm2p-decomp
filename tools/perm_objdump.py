#!/usr/bin/env python3
"""Disassemble one ELF function span for decomp-permuter.

SH objdump's --disassemble=SYMBOL still prints relocations before SYMBOL,
which the permuter incorrectly attaches to the first selected instruction.
Use explicit address bounds instead. Compile surrounding functions normally;
only the requested symbol's span participates in scoring.
"""
import subprocess
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile


def function_bounds(obj: Path, name: str):
    with obj.open('rb') as f:
        elf = ELFFile(f)
        text_index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == '.text')
        size = elf.get_section(text_index)['sh_size']
        functions = [s for s in elf.get_section_by_name('.symtab').iter_symbols()
                     if s['st_shndx'] == text_index and s['st_info']['type'] == 'STT_FUNC']
        matches = [s for s in functions if s.name == name]
        if len(matches) != 1:
            raise ValueError(f'{obj}: expected one defined function {name}, found {len(matches)}')
        function = matches[0]
        start = function['st_value']
        end = min((s['st_value'] for s in functions if s['st_value'] > start), default=size)
        if function['st_size']:
            end = min(end, start + function['st_size'])
        if not 0 <= start < end <= size:
            raise ValueError(f'{obj}: invalid bounds for {name}: {start:#x}-{end:#x}')
        return start, end


def main():
    if len(sys.argv) != 3:
        sys.exit('usage: perm_objdump.py _function file.o')
    name, filename = sys.argv[1:]
    start, end = function_bounds(Path(filename), name)
    subprocess.run(['sh-elf-objdump', '-drz', '-j', '.text',
                    f'--start-address={start:#x}', f'--stop-address={end:#x}', filename], check=True)


if __name__ == '__main__':
    main()
