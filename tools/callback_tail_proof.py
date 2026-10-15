"""Complete SH2 record callback dispatcher, with a proven dynamic tail call.

This opt-in admits one constrained compiler pattern, not arbitrary register
jumps. Its target is read from a runtime data object, under the same valid C
function-pointer assumption as JSR. All control paths restore the caller's
stack, PR, MACL and R14. The original compiler ELF authenticates the code and
data-base relocation; the caller still requires exact linked ROM bytes.
"""
from switch_proof import OriginalText


# PC-relative immediates are decoded separately; every other effect is fixed.
PATTERN = (0x2fe6, 0x4f22, 0x4f12, '9e', 0x24ef, 'd3', 0x0e1a,
           0x6eef, 0x3e3c, 0x52ec, 0x2228, 0x8905, 0x52ec, 0x420b,
           0x64e3, 0xe300, 0xa009, 0x1e3c, 0x51ed, 0x2118, 0x8905,
           0x52ed, 0x64e3, 0x4f16, 0x4f26, 0x422b, 0x6ef6,
           0x4f16, 0x4f26, 0x000b, 0x6ef6)


def prove_callback_tail(jump, *, word, literal, entries, start, end,
                        context_start, compiled, rom, original, provenance,
                        visited, predecessors, delay_owners, literal_reads, error):
    root = start
    if (not isinstance(original, OriginalText) or original.origin != context_start or
            original.entries != entries or len(original.data) != len(compiled)):
        raise error('missing or inconsistent original callback ELF provenance')
    finish = min((a for a in entries if a > root), default=context_start + len(compiled))
    if (jump != root + 50 or end != finish or end != root + 74 or
            entries.get(root) not in (f'_f_{root:x}', f'f_{root:x}') or
            [a for a in entries if root <= a < end] != [root]):
        raise error('callback tail requires one complete dispatcher and its pool')
    for i, expected in enumerate(PATTERN):
        pc = root + 2 * i
        actual = word(pc)
        if (actual >> 8 != int(expected, 16) if isinstance(expected, str) else actual != expected):
            raise error(f'unsupported callback dispatcher effect at {pc:#x}')
    code_end = root + 62
    if any(a < code_end and root < a + width for a, width in original.relocations):
        raise error('callback instructions overlap an original ELF relocation')
    lo, hi = root - context_start, code_end - context_start
    if original.data[lo:hi] != compiled[lo:hi]:
        raise error('callback code differs from original compiler ELF')

    # Exact CFG, including call fallthrough and every delay-slot owner. The
    # nonnull cb48 path never reaches cb52; both null checks join the RTS exit.
    slots = {root + 28: root + 26, root + 34: root + 32,
             root + 52: root + 50, root + 60: root + 58}
    expected = {pc: {None} if pc == root else {pc - 2}
                for pc in range(root, code_end, 2) if pc not in slots}
    expected[root + 30] = {root + 26}
    expected[root + 36] = {root + 22}
    expected[root + 54] = {root + 32, root + 40}
    if visited != set(expected) or set(delay_owners) != set(slots):
        raise error('callback dispatcher has incomplete or extraneous reachable code')
    for pc, pred in expected.items():
        if predecessors.get(pc) != pred or delay_owners.get(pc):
            raise error(f'alternate callback entry at {pc:#x}')
    for pc, owner in slots.items():
        if pc in visited or predecessors.get(pc) or delay_owners.get(pc) != {owner}:
            raise error(f'unsafe callback delay ownership at {pc:#x}')

    stride_pool, stride_width = literal(word(root + 6), root + 6)
    base_pool, base_width = literal(word(root + 10), root + 10)
    if (stride_width != 2 or base_width != 4 or base_pool % 4 or
            not code_end <= stride_pool <= end - 2 or
            not code_end <= base_pool <= end - 4 or
            stride_pool < base_pool + 4 and base_pool < stride_pool + 2 or
            literal_reads != {root + 6: (stride_pool, 2), root + 10: (base_pool, 4)}):
        raise error('callback literal ownership or width is invalid')
    off = stride_pool - context_start
    raw = compiled[off:off + 2]
    stride = int.from_bytes(raw, 'big', signed=True)
    if (stride < 56 or stride % 4 or raw != original.data[off:off + 2] or
            raw != rom[stride_pool:stride_pool + 2] or
            any(a < stride_pool + 2 and stride_pool < a + width for a, width in original.relocations)):
        raise error('callback record stride lacks original constant provenance')
    off = base_pool - context_start
    base = int.from_bytes(compiled[off:off + 4], 'big')
    p = provenance.get(base_pool) if isinstance(provenance, dict) else None
    if (not isinstance(p, dict) or p.get('symbol') != f'_g_{base:x}' or
            p.get('relocation_type') != 'R_SH_DIR32' or p.get('addend') != 0 or
            p.get('embedded_addend') != 0 or p.get('binding') != 'STB_GLOBAL' or
            p.get('section') != 'SHN_UNDEF' or p.get('relocation_count') != 1 or
            p.get('overlap') or p.get('value') != base or
            not 0x06000000 <= base < 0x06100000 or base % 4 or
            compiled[off:off + 4] != rom[base_pool:base_pool + 4] or
            [(a, w) for a, w in original.relocations if a < base_pool + 4 and base_pool < a + w] != [(base_pool, 4)] or
            original.data[off:off + 4] != b'\0' * 4):
        raise error('callback base lacks unique zero-addend RAM data relocation')
    return dict(proof_kind='record_callback_tail', function_root=root,
                function_symbol=entries[root], function_end=end, code_end=code_end,
                complete_single_entry=True, jump_pc=jump, register=2,
                target=dict(kind='runtime_function_pointer', base_register=14,
                            offset=52, width=4, load_pc=root + 42,
                            interpretation='same valid callback assumption as indirect JSR'),
                record=dict(base=base, symbol=p['symbol'], stride=stride,
                            index='incoming signed short R4', offset='signed low 16 bits of index * stride'),
                argument_proof={'r4': 'selected_record'},
                first_callback=dict(test_load_pc=root + 18, test_pc=root + 20, reload_pc=root + 24,
                                    call_pc=root + 26, offset=48, clear_pc=root + 34),
                null_return=root + 54, delay_slot=dict(pc=jump + 2, owner=jump, word='6ef6'),
                stack_proof=dict(deepest_displacement=-12, final_displacement=0,
                                 pushes=['R14', 'PR', 'MACL'],
                                 callback_tail_pops=['MACL', 'PR', 'R14'],
                                 return_pops=['MACL', 'PR', 'R14']),
                preserved_registers=list(range(8, 16)), restored_special=['PR', 'MACL'],
                no_direct_writes_to=['MACH', 'SR except T', 'GBR', 'VBR'],
                callback_abi_assumption='valid function pointer; called function preserves callee-saved registers and returns normally',
                literal_pools=[dict(pc=root+6, target=stride_pool, size=2, value=stride),
                               dict(pc=root+10, target=base_pool, size=4, value=base)],
                original_object_sha256=original.object_sha256)
