"""Explicit proof for the complete startup reset and flag-update wrapper.

The field is an ordinary valid C object, disjoint from the callee's private
save cells. The fixed helper writes four bytes/four shorts and preserves R4,
R8-R15 and PR. The wrapper saves/restores R14 and PR around that one local call,
then writes flags308 |= 0x58000 and tails with the original field in R4.
No other call, frame, store, computed target or opcode family is admitted.
"""

RESET = (0xe500, '96', 0xe704, 0x6053, 0x364c, 0x2650, 0x7002, 0x7601,
         0x630f, 0x2650, 0x3373, 0x8ff8, 0x7601, '96', 0x6053, 0x364c,
         0x7002, 0x2651, 0x630f, 0x7602, 0x3373, 0x2651, 0x8ff8, 0x7602,
         0x000b, 0x0009)
WRAPPER = (0x2fe6, 0x4f22, 'bsr', 0x6e43, 0x64e3, 'd1', '90', 'd3',
           0x02ee, 0x223b, 0x0e26, 0x4f26, 0x412b, 0x6ef6)


def prove_startup_reset_tail(jump, *, word, literal, entries, start, end, context_start,
                             compiled, rom, visited, predecessors, delay_owners,
                             literal_reads, tail_provenance, error):
    root = max(a for a in entries if start <= a <= jump)
    reset = root - 52
    function_end = min((a for a in entries if a > root),
                       default=context_start + len(compiled))
    if (jump != root + 24 or function_end != root + 74 or function_end > end or
            reset < start or entries.get(reset) not in (f'_f_{reset:x}', f'f_{reset:x}') or
            any(reset < a < root for a in entries)):
        raise error(f'startup-reset tail requires the complete retained pair at {jump:#x}')

    def matches(actual, spec):
        if spec == 'bsr':
            return actual >> 12 == 11
        return actual >> 8 == int(spec, 16) if isinstance(spec, str) else actual == spec

    for base, pattern in [(reset, RESET), (root, WRAPPER)]:
        for i, spec in enumerate(pattern):
            if not matches(word(base + i * 2), spec):
                raise error(f'unsupported startup-reset effect at {base + i * 2:#x}')
    displacement = word(root + 4) & 0xfff
    if displacement & 0x800:
        displacement -= 0x1000
    if root + 8 + displacement * 2 != reset:
        raise error(f'startup-reset call must reach its verified helper at {root + 4:#x}')

    # Check the complete CFG after all roots/branches were visited. The only
    # non-linear edges are two fixed counted loops and the one helper call.
    slots = {reset + 24: reset + 22, reset + 46: reset + 44,
             reset + 50: reset + 48, root + 6: root + 4, root + 26: jump}
    expected = {}
    for pc in range(reset, root, 2):
        if pc in slots:
            continue
        pred = {None, root + 4} if pc == reset else {pc - 2}
        if pc in (reset + 26, reset + 48):
            pred = {pc - 4}
        if pc == reset + 10:
            pred.add(reset + 22)
        if pc == reset + 32:
            pred.add(reset + 44)
        expected[pc] = pred
    for pc in range(root, root + 28, 2):
        if pc in slots:
            continue
        expected[pc] = {None} if pc == root else {root + 4} if pc == root + 8 else {pc - 2}
    for pc, pred in expected.items():
        if pc not in visited or predecessors.get(pc) != pred or delay_owners.get(pc):
            raise error(f'alternate entry in startup-reset pair at {pc:#x}')
    for pc, owner in slots.items():
        if pc in visited or predecessors.get(pc) or delay_owners.get(pc) != {owner}:
            raise error(f'unsafe startup-reset delay ownership at {pc:#x}')
    executable = visited | set(delay_owners)
    if any(root + 28 <= pc < function_end for pc in executable):
        raise error(f'executable startup-reset trailing pool at {root + 28:#x}')
    for pool, width in literal_reads.values():
        if any(pool < pc + 2 and pc < pool + width for pc in executable):
            raise error(f'startup-reset literal overlaps reachable code at {pool:#x}')

    constants = []
    for pc, value, width in [(reset + 2, 0x2fc, 2), (reset + 26, 0x300, 2),
                             (root + 12, 0x308, 2), (root + 14, 0x58000, 4)]:
        pool, actual_width = literal(word(pc), pc)
        off = pool - context_start
        raw = value.to_bytes(width, 'big')
        if (actual_width != width or off < 0 or off + width > len(compiled) or
                pool + width > len(rom) or compiled[off:off+width] != raw or rom[pool:pool+width] != raw):
            raise error(f'incorrect startup-reset literal at {pool:#x}')
        if any(p < pool + width and pool < p + 4 for p in tail_provenance):
            raise error(f'relocated startup-reset constant at {pool:#x}')
        constants.append(dict(pc=pc, pool=pool, size=width, value=value))

    return root + 10, dict(
        proof_kind='startup_reset_tail', function_end=function_end,
        reset_root=reset, reset_end=root, reset_instruction_count=26,
        instruction_count=14, complete_pair_single_entry=True,
        reset_call=dict(pc=root + 4, target=reset, delay_pc=root + 6),
        reset_writes=[dict(offset=0x2fc, size=4, value=0), dict(offset=0x300, size=8, value=0)],
        field_write=dict(base_register=14, offset=0x308, size=4, operation='or', mask=0x58000, pc=root + 20),
        constant_literals=constants, argument_proof={'r4': 'incoming_field'},
        preserved_registers=list(range(8, 16)),
        unchanged_special=['MACH', 'MACL', 'GBR', 'VBR'],
        restored_special=['PR'], condition_bit='caller-clobbered T',
        stack_proof=dict(initial_displacement=0, deepest_displacement=-8, final_displacement=0,
                         cells=[dict(value='incoming_r14', push_pc=root, pop_pc=root + 26),
                                dict(value='incoming_pr', push_pc=root + 2, pop_pc=root + 22)]),
        target_unique_definition=True,
        instructions=[dict(pc=pc, word=f'{word(pc):04x}')
                      for pc in list(range(reset, root, 2)) + list(range(root, root + 28, 2))])
