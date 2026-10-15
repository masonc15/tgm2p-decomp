"""Opt-in proof for a complete seven-instruction field-flag tail leaf.

This proves the machine effect flags308 |= 0x20; tail(field). It does not
admit arbitrary stores, extra instructions, stack frames or computed targets.
The caller supplies an ordinary valid field object, as in the compiled C body.
"""

def prove_field_flag_tail(jump, *, word, literal, entries, start, end, context_start,
                          compiled, rom, visited, predecessors, delay_owners,
                          literal_reads, tail_provenance, error):
    roots = [a for a in entries if start <= a <= jump]
    root = max(roots)
    function_end = min((a for a in entries if a > root),
                       default=context_start + len(compiled))
    if jump != root + 10 or function_end != root + 14 or function_end > end:
        raise error(f'field-flag tail requires a complete seven-instruction leaf at {jump:#x}')
    pcs = list(range(root, function_end, 2))
    words = [word(pc) for pc in pcs]
    if (words[0] != 0xe320 or words[1] & 0xff00 != 0xd100 or
            words[2] & 0xff00 != 0x9000 or
            words[3:] != [0x024e, 0x223b, 0x412b, 0x0426]):
        raise error(f'unsupported field-flag leaf effects at {root:#x}')
    for pc in pcs[:-1]:
        expected = {None} if pc == root else {pc - 2}
        if pc not in visited or predecessors.get(pc) != expected or delay_owners.get(pc):
            raise error(f'alternate entry in field-flag leaf at {pc:#x}')
    slot = jump + 2
    if slot in visited or predecessors.get(slot) or delay_owners.get(slot) != {jump}:
        raise error(f'unsafe field-flag delay ownership at {slot:#x}')
    # Complete-CFG check: neither this leaf nor another retained root may
    # execute any bytes used as literal data, even if discovered after the tail.
    executable = visited | set(delay_owners)
    for pool, width in literal_reads.values():
        if any(pool < pc + 2 and pc < pool + width for pc in executable):
            raise error(f'field-flag literal overlaps reachable code at {pool:#x}')
    offset_pool, width = literal(words[2], root + 4)
    off = offset_pool - context_start
    if (width != 2 or off < 0 or off + 2 > len(compiled) or offset_pool + 2 > len(rom) or
            compiled[off:off+2] != b'\x03\x08' or rom[offset_pool:offset_pool+2] != b'\x03\x08'):
        raise error(f'field-flag offset must be original signed word 0x308 at {offset_pool:#x}')
    if any(pool < offset_pool + 2 and offset_pool < pool + 4 for pool in tail_provenance):
        raise error(f'field-flag offset has an overlapping relocation at {offset_pool:#x}')
    return root + 2, dict(
        proof_kind='field_flag_tail', function_end=function_end,
        instruction_count=7, whole_function_single_entry=True,
        field_write=dict(base_register=4, offset=0x308, size=4, operation='or', mask=0x20, pc=slot),
        offset_literal=offset_pool, unchanged_arguments=[4, 5, 6, 7],
        caller_clobbered=[0, 1, 2, 3],
        preserved_registers=list(range(4, 16)),
        unchanged_special=['PR', 'MACH', 'MACL', 'SR', 'GBR', 'VBR'],
        initial_stack_displacement=0, final_stack_displacement=0,
        target_unique_definition=True,
        instructions=[dict(pc=pc, word=f'{w:04x}') for pc, w in zip(pcs, words)])
