#!/usr/bin/env python3
"""Compile contextual C, verify a retained range, keep its unmodified bytes.

No bytes are sourced from the reference ROM: it is a comparison oracle only.
This intentionally supports a conservative subset of control flow. Unsupported
indirect jumps and PC-relative delay-slot loads fail closed by default.
An explicit opt-in can prove only bounded, single-entry literal tail calls
with original ELF relocation identity and a canonical external function map.
A separate guarded-switch opt-in proves the fixed 13-entry R0/R1 family
against original ELF text and the complete retained control-flow graph.
A separate local-loop opt-in authenticates same-function literal backedges.
An exclusive callback-tail opt-in proves a complete record dispatcher with
original data relocation, runtime callback provenance and balanced exits.
"""
import hashlib
import json
import re
import subprocess
import tempfile
from pathlib import Path


class PrefixError(ValueError):
    pass


def validate_prefix(compiled, rom, start, end, entries, *, context_start=None,
                    tail_provenance=None, canonical_functions=None, tail_proofs=None,
                    leaf_argument_tails=False, guarded_switches=False,
                    switch_original=None, switch_proofs=None, local_loop_jumps=False,
                    loop_original=None, loop_proofs=None, field_flag_tails=False,
                    startup_reset_tails=False, function_code_range=False,
                    function_code_proofs=None, callback_tails=False, callback_original=None,
                    callback_provenance=None, callback_proofs=None):
    """Verify linked bytes and reachable cross-boundary literal dependencies.

    Addresses are ROM offsets; compiled[0] corresponds to context_start (start
    by default). entries maps all contextual function starts to names. Both
    cuts must be at function boundaries. Returns the external literal
    dependency manifest. No instruction is rewritten.

    Optional tail_provenance maps ROM literal addresses to original-ELF facts;
    canonical_functions maps exact _f_hex symbols to canonical ROM entries.
    compile_prefix derives the former from the actual compiler object. A
    tail_proofs output list is extended only after complete validation succeeds.
    """
    size = end - start
    if start < 0 or start % 2 or end % 2 or size <= 0 or end > len(rom):
        raise PrefixError('invalid prefix bounds')
    context_start = start if context_start is None else context_start
    if context_start < 0 or context_start % 2 or context_start > start:
        raise PrefixError('invalid context origin')
    offset = start - context_start
    if end - context_start > len(compiled):
        raise PrefixError('compiled context shorter than prefix')
    if start not in entries:
        raise PrefixError('prefix must start/end at contextual function boundaries')
    if function_code_range:
        roots_in_range = sorted(a for a in entries if start <= a < end)
        if roots_in_range != [start]:
            raise PrefixError('function-code range must contain exactly one contextual function entry')
        contextual_end = min((a for a in entries if a > start),
                             default=context_start + len(compiled))
        if end > contextual_end:
            raise PrefixError('function-code range crosses its contextual function boundary')
    elif end not in entries and end - context_start != len(compiled):
        raise PrefixError('prefix must start/end at contextual function boundaries')
    for addr, name in entries.items():
        m = re.fullmatch(r'_?f_([0-9a-f]+)', name)
        if start <= addr < end and m and int(m[1], 16) != addr:
            raise PrefixError(f'kept function {name} is at the wrong address {addr:#x}')
    kept = compiled[offset:offset + size]
    if kept != rom[start:end]:
        first = next(i for i, (a, b) in enumerate(zip(kept, rom[start:end])) if a != b)
        raise PrefixError(f'compiled prefix differs from ROM at {start + first:#x}')

    def word(pc):
        off = pc - context_start
        if pc < start or pc + 2 > end:
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

    tail_enabled = tail_provenance is not None
    if callback_tails and (tail_enabled or local_loop_jumps or guarded_switches or
                           leaf_argument_tails or field_flag_tails or startup_reset_tails or function_code_range):
        raise PrefixError("callback tails require an exclusive proof mode")
    if local_loop_jumps and (guarded_switches or tail_enabled or leaf_argument_tails):
        raise PrefixError('mixed local-loop/switch/tail proof modes are unsupported')
    if guarded_switches and (tail_enabled or leaf_argument_tails):
        raise PrefixError('mixed switch/tail proof modes are unsupported')
    switches = None
    if guarded_switches:
        from switch_proof import GuardedSwitches
        switches = GuardedSwitches(switch_original, compiled, rom, start, end,
                                   context_start, entries, word, PrefixError)
    local_loops = None
    if local_loop_jumps:
        from local_loop_proof import LocalLoopJumps
        local_loops = LocalLoopJumps(loop_original, compiled, rom, start, end,
                                    context_start, entries, word, PrefixError)
    if leaf_argument_tails and (not tail_enabled or canonical_functions is None):
        raise PrefixError('whole-leaf argument tails require original provenance and canonical functions')
    if tail_enabled and canonical_functions is None:
        raise PrefixError('literal tail proof requires canonical functions')
    if field_flag_tails and (not tail_enabled or canonical_functions is None or leaf_argument_tails):
        raise PrefixError('field-flag tails require original provenance and canonical functions without leaf-argument mode')
    if startup_reset_tails and (not tail_enabled or canonical_functions is None or
                                field_flag_tails or leaf_argument_tails):
        raise PrefixError('startup-reset tails require original provenance and an exclusive proof mode')
    dependencies = {}
    visited = set()
    roots = [a for a in entries if start <= a < end]
    pending = list(roots)
    predecessors = {a: {None} for a in roots}
    delay_owners = {}
    candidates = []
    literal_reads = {}

    def follow(pc, source, *, fallthrough=False):
        if function_code_range and fallthrough and pc == end:
            raise PrefixError(f'function code falls through retained end at {pc:#x}')
        if start <= pc < end:
            predecessors.setdefault(pc, set()).add(source)
            pending.append(pc)
        else:
            # Direct calls/tail falls may reach retained assembly functions,
            # but never an arbitrary byte in discarded context.
            name = entries.get(pc, '')
            m = re.fullmatch(r'_?f_([0-9a-f]+)', name)
            if not m or int(m[1], 16) != pc:
                raise PrefixError(f'unverified control-flow exit to {pc:#x}')

    def check_instruction(pc, delay=False, owner=None):
        if delay:
            delay_owners.setdefault(pc, set()).add(owner)
        w = word(pc)
        pool = literal(w, pc)
        if delay and (pool or w & 0xff00 == 0xc700):
            raise PrefixError(f'PC-relative delay-slot instruction at {pc:#x} is unsupported')
        if pool:
            target, width = pool
            literal_reads[pc] = pool
            if target < start or target + width > end:
                off = target - context_start
                if off < 0 or off + width > len(compiled) or target + width > len(rom):
                    raise PrefixError(f'literal outside compiled context at {pc:#x}')
                value = compiled[off:off + width]
                if value != rom[target:target + width]:
                    raise PrefixError(f'discarded literal differs at {target:#x} (load {pc:#x})')
                dependencies[pc] = dict(pc=pc, target=target, size=width, value=value.hex())
        # MOVA can expose an arbitrary-length table. Do not guess its extent.
        if w & 0xff00 == 0xc700:
            if switches is None or delay:
                raise PrefixError(f'MOVA/table addressing at {pc:#x} is unsupported')
            switches.candidate(pc)
        if switches is not None and w == 0x0023 and not delay:
            switches.candidate(pc - 4)
            return w
        indirect = w & 0xf0ff == 0x402b
        if (w & 0xf0ff in (0x0023, 0x0003) or
                (indirect and not ((tail_enabled or local_loops is not None or callback_tails) and not delay)) or
                w in (0x002b, 0x001b) or w & 0xff00 == 0xc300):
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
            check_instruction(pc + 2, True, pc)
        elif w >> 12 in (10, 11):  # BRA / BSR
            check_instruction(pc + 2, True, pc)
            follow(pc + 4 + sext(w & 0xfff, 12) * 2, pc)
            if w >> 12 == 11:
                follow(pc + 4, pc, fallthrough=True)
        elif w & 0xff00 in (0x8900, 0x8b00, 0x8d00, 0x8f00):
            delayed = w & 0xff00 in (0x8d00, 0x8f00)
            if delayed:
                check_instruction(pc + 2, True, pc)
            follow(pc + 4 + sext(w & 255, 8) * 2, pc)
            follow(pc + (4 if delayed else 2), pc, fallthrough=True)
        elif w & 0xf0ff == 0x400b:  # JSR; all contextual callees are also roots
            check_instruction(pc + 2, True, pc)
            follow(pc + 4, pc, fallthrough=True)
        elif w & 0xf0ff == 0x402b:
            check_instruction(pc + 2, True, pc)
            if local_loops is not None:
                loop = local_loops.candidate(pc)
                follow(loop['target'], pc)
            else:
                # Tail candidates have no guessed target or fall-through edge.
                candidates.append(pc)
        elif switches is not None and w == 0x0023:
            switch = switches.candidate(pc - 4)
            check_instruction(pc + 2, True, pc)
            for target in switch['targets']:
                follow(target, pc)
        else:
            follow(pc + 2, pc, fallthrough=True)

    switch_results = (switches.finish(visited, predecessors, delay_owners, literal_reads)
                      if switches is not None else [])
    loop_results = (local_loops.finish(visited, predecessors, delay_owners, literal_reads)
                    if local_loops is not None else [])

    def safe_restore(w, register, *, delay=False):
        if delay and w == 0x0009:
            return True
        if w == 0x4f26:  # LDS.L @r15+,PR
            return True
        if w >> 8 == 0x7f:  # positive ADD #imm,r15
            return 0 < sext(w & 255, 8)
        if w & 0xf0ff == 0x60f6:  # MOV.L @r15+,Rm
            return ((w >> 8) & 15) not in (register, 15)
        return False

    def short_tail_load(jump, register):
        # Preserve the original four-restore proof, including its nearest-load rule.
        pc = jump - 2
        intervening = []
        while pc >= start:
            w = word(pc)
            if w >> 12 == 13 and (w >> 8) & 15 == register:
                return pc
            if len(intervening) == 4 or not safe_restore(w, register):
                return None
            intervening.append(pc)
            pc -= 2
        return None

    def function_code_tail_shape(jump, register):
        """Prove a short argument-setup spine without guessing its effects.

        This is available only when the retained interval is independently
        proven to be a complete single-entry function body. The whitelist is
        deliberately small: ordinary argument moves/immediates, SHLL, and two
        side-effect-safe delay-slot forms emitted by the currently reviewed
        leaf functions. None may overwrite the authenticated target register.
        """
        def effect(pc, *, delay=False):
            w = word(pc)
            n, m = (w >> 8) & 15, (w >> 4) & 15
            item = dict(pc=pc, word=f'{w:04x}', delay_slot=delay,
                        reads=[], writes=[], memory=None)
            if w & 0xf00f == 0x6003:  # MOV Rm,Rn
                if n == register:
                    raise PrefixError(f'function-code tail target overwritten at {pc:#x}')
                item.update(operation='move', reads=[m], writes=[n])
            elif w >> 12 == 14:  # MOV #imm,Rn
                if n == register:
                    raise PrefixError(f'function-code tail target overwritten at {pc:#x}')
                item.update(operation='argument_immediate', writes=[n],
                            immediate=sext(w & 255, 8))
            elif w & 0xf0ff == 0x4000:  # SHLL Rn
                if n == register:
                    raise PrefixError(f'function-code tail target overwritten at {pc:#x}')
                item.update(operation='shift_left_one', reads=[n], writes=[n])
            elif delay and w & 0xf00f == 0x0005:  # MOV.W Rm,@(R0,Rn)
                item.update(operation='store_indexed_word', reads=[0, m, n],
                            memory=dict(access='write', size=2, base=n, index=0,
                                        value_register=m))
            elif delay and w == 0x4f26 and register != 15:
                item.update(operation='restore_pr', reads=[15], writes=[15],
                            special_writes=['PR'],
                            memory=dict(access='read', size=4, base=15,
                                        destination_special='PR', postincrement=4))
            elif delay and w == 0x0009:
                item.update(operation='nop')
            else:
                raise PrefixError(f'unsupported function-code tail instruction at {pc:#x}')
            return item

        pc = jump - 2
        between = []
        while pc >= start and len(between) <= 4:
            w = word(pc)
            if w >> 12 == 13 and (w >> 8) & 15 == register:
                slot = jump + 2
                effects = [effect(a) for a in reversed(between)]
                effects.append(effect(slot, delay=True))
                return pc, effects
            between.append(pc)
            pc -= 2
        return None

    def paired_call_tail_shape(jump, register):
        """Complete non-leaf wrapper: one call then a tail to the same callee.

        The only frame is a saved PR. Both argument sets are immediates and
        the same literal data pointer; no caller memory or callee-saved GPR
        is touched. The shared callee literal gets the ordinary original-ELF
        provenance checks below, authenticating both transfers.
        """
        root = max(a for a in entries if a <= jump)
        function_end = min((a for a in entries if a > root),
                           default=context_start + len(compiled))
        if (register != 2 or root != start or jump != root + 20 or
                function_end != end or not 24 <= end - root <= 56):
            return None
        fixed = {0: 0x4f22, 8: 0x430b, 20: 0x422b, 22: 0x4f26}
        masked = {2: 0xe500, 4: 0xd300, 6: 0xd600, 10: 0xe400,
                  12: 0xd600, 14: 0xe500, 16: 0xd200, 18: 0xe400}
        if any(word(root + off) != value for off, value in fixed.items()):
            return None
        if any(word(root + off) & 0xff00 != value for off, value in masked.items()):
            return None
        ordinary = [root + off for off in (0, 2, 4, 6, 8, 12, 14, 16, 18, 20)]
        for pc in ordinary:
            predecessor = None if pc == root else pc - (4 if pc == root + 12 else 2)
            if pc not in visited or predecessors.get(pc) != {predecessor} or delay_owners.get(pc):
                raise PrefixError(f'alternate entry in paired-call wrapper at {pc:#x}')
        for off, owner in ((10, 8), (22, 20)):
            pc = root + off
            if pc in visited or predecessors.get(pc) or delay_owners.get(pc) != {root + owner}:
                raise PrefixError(f'unsafe paired-call delay ownership at {pc:#x}')
        first = literal(word(root + 4), root + 4)
        last = literal(word(root + 16), root + 16)
        data1 = literal(word(root + 6), root + 6)
        data2 = literal(word(root + 12), root + 12)
        if first != last or data1 != data2:
            raise PrefixError('paired-call wrapper literals do not agree')
        for target, width in (first, data1):
            if width != 4 or not root + 24 <= target <= end - 4:
                raise PrefixError('paired-call wrapper literal outside owned trailing pool')
        return root + 16, dict(
            proof_kind='paired_call_argument_tail',
            first_call_pc=root + 8, first_call_literal=first[0],
            shared_argument_literal=data1[0], complete_single_entry=True,
            instruction_pcs=ordinary, delay_slots=[root + 10, root + 22],
            stack_proof=dict(initial_displacement=0, final_displacement=0,
                             saved_pr=root, restored_pr=root + 22),
            unchanged_caller_registers=list(range(8, 15)),
            callee_abi_assumption='both transfers to the same reviewed canonical function')

    def long_tail_effect(pc, register, *, defining=False):
        w = word(pc)
        effect = dict(pc=pc, word=f'{w:04x}', reads=[], writes=[],
                      special_reads=[], special_writes=[], memory=None,
                      base_updates=[], stack_increment=0)
        if defining and w >> 12 == 13 and (w >> 8) & 15 == register:
            target, width = literal(w, pc)
            effect.update(operation='literal_load', writes=[register], special_reads=['PC'],
                          memory=dict(access='read', size=width, base=None, index=None,
                                      address=target, destination=register))
        elif w == 0x314c:
            effect.update(operation='add', reads=[1, 4], writes=[1])
        elif w == 0x2100:
            effect.update(operation='store_byte', reads=[0, 1],
                          memory=dict(access='write', size=1, base=1, index=None,
                                      value_register=0))
        elif w & 0xff00 in (0xe400, 0xe500):
            effect.update(operation='argument_immediate', writes=[(w >> 8) & 15],
                          immediate=sext(w & 255, 8))
        elif w >> 8 == 0xe1:
            effect.update(operation='immediate', writes=[1], immediate=sext(w & 255, 8))
        elif w >> 8 == 0xd1:
            address, width = literal(w, pc)
            effect.update(operation='literal_load', writes=[1], special_reads=['PC'],
                          memory=dict(access='read', size=width, base=None, index=None,
                                      address=address, destination=1))
        elif w in (0x2010, 0x2120):
            base, value = (0, 1) if w == 0x2010 else (1, 2)
            effect.update(operation='store_byte', reads=[base, value],
                          memory=dict(access='write', size=1, base=base, index=None,
                                      value_register=value))
        elif w == 0x02ed:
            effect.update(operation='load_indexed_word', reads=[0, 14], writes=[2],
                          memory=dict(access='read', size=2, base=14, index=0,
                                      destination=2, sign_extended=True))
        elif w == 0x64e3:
            effect.update(operation='move', reads=[14], writes=[4])
        elif w == 0x223b:
            effect.update(operation='or', reads=[2, 3], writes=[2])
        elif w == 0x0e25:
            effect.update(operation='store_indexed_word', reads=[0, 2, 14],
                          memory=dict(access='write', size=2, base=14, index=0,
                                      value_register=2))
        elif w in (0x4f16, 0x4f26):
            special = 'MACL' if w == 0x4f16 else 'PR'
            effect.update(operation='restore_' + special.lower(), reads=[15], writes=[15],
                          special_writes=[special], stack_increment=4,
                          base_updates=[dict(register=15, increment=4)],
                          memory=dict(access='read', size=4, base=15, index=None,
                                      destination_special=special, postincrement=4))
        elif w in (0x69f6, 0x6af6, 0x6bf6, 0x6cf6, 0x6df6, 0x6ef6):
            destination = (w >> 8) & 15
            effect.update(operation='restore_register', reads=[15], writes=[destination, 15],
                          stack_increment=4, base_updates=[dict(register=15, increment=4)],
                          memory=dict(access='read', size=4, base=15, index=None,
                                      destination=destination, postincrement=4))
        elif w == 0x402b | (register << 8):
            effect.update(operation='tail_jump', reads=[register], special_writes=['PC'])
        else:
            raise PrefixError(f'unsupported long-tail instruction at {pc:#x}')
        if not defining:
            memory = effect['memory']
            if (register in effect['writes'] or
                    memory and (register in (memory.get('base'), memory.get('index')) or
                                memory.get('access') == 'write' and
                                memory.get('value_register') == register)):
                raise PrefixError(f'long-tail target register altered at {pc:#x}')
        return effect

    def long_tail_shape(jump, register):
        # Three exact SHC setup/restore forms, never a generic enlarged opcode
        # window. Form C ends a three-register frame after two byte stores.
        forms = [
            (3, 'A', (0x314c, 0x2100, 'e4', 0x4f16, 0x4f26,
                      0x69f6, 0x6af6, 0x6bf6, 0x6cf6, 0x6df6)),
            (1, 'B', ('e5', 0x02ed, 0x64e3, 0x223b, 0x0e25, 0x4f16, 0x4f26)),
            (3, 'C', ('e1', 0x2010, 'd1', 0x2120, 0x4f26, 0x6cf6, 0x6df6)),
        ]
        slot = jump + 2

        def matches(between):
            load = jump - 2 * (len(between) + 1)
            if load < start or slot + 2 > end:
                return False
            defining = word(load)
            if defining >> 12 != 13 or (defining >> 8) & 15 != register:
                return False
            for index, expected in enumerate(between):
                actual = word(load + 2 * (index + 1))
                if (isinstance(expected, str) and actual >> 8 != int(expected, 16) or
                        isinstance(expected, int) and actual != expected):
                    return False
            return word(slot) == 0x6ef6

        found = [(form, between) for form_register, form, between in forms
                 if form_register == register and matches(between)]
        if len(found) != 1:
            return None
        form, between = found[0]
        load = jump - 2 * (len(between) + 1)
        # Even a complete opcode match must not span contextual function entries.
        function_root = max(a for a in entries if a <= load)
        function_end = min((a for a in entries if a > function_root),
                           default=context_start + len(compiled))
        if not function_root <= load < jump < slot + 2 <= function_end:
            raise PrefixError(f'long literal tail crosses function boundary at {jump:#x}')
        effects = [long_tail_effect(pc, register, defining=pc == load)
                   for pc in range(load, slot + 2, 2)]
        return load, form, effects

    def whole_leaf_shape(jump, register):
        """Separate opt-in class: prove every reachable instruction and machine ABI.

        No caller stack memory is admitted. The only stack addresses are fresh,
        four-byte push cells, consumed by exact matching pops or a bounded
        release of caller-clobbered spills. Reading/copying SP into a general
        register is forbidden, so ordinary loads cannot alias a tracked cell
        through an explicit SP-derived address. This is deliberately narrower
        than a general SH2 symbolic executor.
        """
        if register != 1:
            raise PrefixError(f'unsupported whole-leaf tail register at {jump:#x}')
        root = max(a for a in entries if a <= jump)
        function_end = min((a for a in entries if a > root),
                           default=context_start + len(compiled))
        slot = jump + 2
        if root not in roots or function_end > end or slot + 2 > function_end:
            raise PrefixError(f'whole-leaf tail must retain the complete function at {jump:#x}')
        pcs = list(range(root, slot + 2, 2))
        if len(pcs) > 22:
            raise PrefixError(f'whole-leaf tail exceeds 22 instructions at {jump:#x}')
        for pc in pcs[:-1]:
            expected = {None} if pc == root else {pc - 2}
            if pc not in visited or predecessors.get(pc) != expected or delay_owners.get(pc):
                raise PrefixError(f'alternate entry or control flow in whole-leaf tail at {pc:#x}')
        if slot in visited or predecessors.get(slot) or delay_owners.get(slot) != {jump}:
            raise PrefixError(f'unsafe whole-leaf delay ownership at {slot:#x}')

        defining = [pc for pc in pcs[:-2] if word(pc) >> 12 == 13 and (word(pc) >> 8) & 15 == 1]
        if len(defining) != 1:
            raise PrefixError(f'nonunique whole-leaf target definition at {jump:#x}')
        target_pool, _ = literal(word(defining[0]), defining[0])
        target_offset = target_pool - context_start
        if target_offset < 0 or target_offset + 4 > len(compiled):
            raise PrefixError(f'whole-leaf target pool outside context at {jump:#x}')
        target_value = int.from_bytes(compiled[target_offset:target_offset + 4], 'big')

        initial = {r: f'entry:r{r}' for r in range(15)}
        values = initial.copy()
        known = {r: None for r in range(15)}
        target_taint = {r: False for r in range(15)}
        sp = 0
        cells = {}
        effects = []
        load = None
        restored = []
        released = []
        for pc in pcs:
            w = word(pc)
            n, m = (w >> 8) & 15, (w >> 4) & 15
            before = dict(displacement=sp, cells=[dict(offset=k, **v) for k, v in sorted(cells.items())])
            effect = dict(pc=pc, word=f'{w:04x}', reads=[], writes=[], memory=None,
                          special_reads=[], special_writes=[], stack_increment=0)
            destination = None
            value = None
            known_value = None
            tainted = False
            if pc == jump and w == 0x412b:
                if load is None:
                    raise PrefixError(f'missing whole-leaf target load at {jump:#x}')
                effect.update(operation='tail_jump', reads=[1], special_writes=['PC'])
            elif w >> 12 in (9, 13):
                address, width = literal(w, pc)
                if address < slot + 2 and root < address + width:
                    raise PrefixError(f'whole-leaf literal overlaps executable instructions at {pc:#x}')
                if n == 15 or pc == slot:
                    raise PrefixError(f'unsafe whole-leaf literal destination at {pc:#x}')
                off = address - context_start
                if off < 0 or off + width > len(compiled) or address + width > len(rom):
                    raise PrefixError(f'whole-leaf literal outside context at {pc:#x}')
                raw = compiled[off:off + width]
                if raw != rom[address:address + width]:
                    raise PrefixError(f'whole-leaf literal differs at {address:#x}')
                if n == 1:
                    if width != 4 or load is not None:
                        raise PrefixError(f'nonunique whole-leaf target definition at {pc:#x}')
                    load = pc
                destination, value = n, f'literal:{address:x}:{raw.hex()}'
                known_value = int.from_bytes(raw, 'big')
                if width == 2:
                    known_value = sext(known_value, 16) & 0xffffffff
                effect.update(operation='literal_load', writes=[n], special_reads=['PC'],
                              memory=dict(access='read', size=width, address=address,
                                          base=None, index=None, destination=n,
                                          sign_extended=width == 2))
            elif w & 0xf00f == 0x2006 and n == 15:  # MOV.L Rm,@-SP
                if m in (1, 15) or pc == slot:
                    raise PrefixError(f'unsafe whole-leaf stack push at {pc:#x}')
                new_sp = sp - 4
                if any(new_sp < k + 4 and k < new_sp + 4 for k in cells):
                    raise PrefixError(f'overlapping whole-leaf stack cells at {pc:#x}')
                sp = new_sp
                cells[sp] = dict(value=values[m], source_register=m, push_pc=pc, size=4,
                                 known_value=known[m], target_tainted=target_taint[m])
                effect.update(operation='stack_push', reads=[m, 15], writes=[15],
                              stack_increment=-4,
                              memory=dict(access='write', size=4, base=15, index=None,
                                          offset=sp, value_register=m, predecrement=4))
            elif w & 0xf0ff == 0x60f6:  # MOV.L @SP+,Rn
                if n in (1, 15) or sp not in cells:
                    raise PrefixError(f'unmatched whole-leaf stack pop at {pc:#x}')
                cell = cells[sp]
                if cell['source_register'] != n or cell['size'] != 4:
                    raise PrefixError(f'whole-leaf stack restore register mismatch at {pc:#x}')
                destination, value = n, cell['value']
                known_value, tainted = cell['known_value'], cell['target_tainted']
                restored.append(dict(register=n, push_pc=cell['push_pc'], pop_pc=pc,
                                     value=value, offset=sp))
                del cells[sp]
                effect.update(operation='stack_pop', reads=[15], writes=[n, 15],
                              stack_increment=4,
                              memory=dict(access='read', size=4, base=15, index=None,
                                          offset=sp, destination=n, postincrement=4))
                sp += 4
            elif w >> 8 == 0x7f:  # private caller-clobbered spill release
                amount = sext(w & 255, 8)
                if amount <= 0 or amount % 4 or sp + amount > 0:
                    raise PrefixError(f'unsafe whole-leaf stack release at {pc:#x}')
                for offset in range(sp, sp + amount, 4):
                    cell = cells.get(offset)
                    if cell is None or cell['source_register'] > 7:
                        raise PrefixError(f'whole-leaf release discards saved register at {pc:#x}')
                    released.append(dict(**cell, offset=offset, release_pc=pc))
                    del cells[offset]
                effect.update(operation='stack_release', reads=[15], writes=[15],
                              stack_increment=amount, immediate=amount)
                sp += amount
            elif w == 0x0009 and pc == slot:
                effect.update(operation='nop')
            elif pc == slot:
                raise PrefixError(f'unsafe whole-leaf delay effect at {pc:#x}')
            elif w & 0xf00f == 0x6003:  # MOV Rm,Rn
                if n in (1, 15) or m in (1, 15):
                    raise PrefixError(f'whole-leaf target/SP alias at {pc:#x}')
                destination, value = n, values[m]
                known_value, tainted = known[m], target_taint[m]
                effect.update(operation='move', reads=[m], writes=[n])
            elif w >> 12 == 14:  # MOV #imm,Rn
                if n in (1, 15):
                    raise PrefixError(f'whole-leaf target/SP overwrite at {pc:#x}')
                destination, value = n, f'constant:{sext(w & 255, 8)}'
                known_value = sext(w & 255, 8) & 0xffffffff
                effect.update(operation='immediate', writes=[n], immediate=sext(w & 255, 8))
            elif w & 0xf00f == 0x300c:  # ADD Rm,Rn
                if n in (1, 15) or m in (1, 15):
                    raise PrefixError(f'whole-leaf target/SP arithmetic at {pc:#x}')
                destination, value = n, f'computed:{pc:x}'
                if known[n] is not None and known[m] is not None:
                    known_value = (known[n] + known[m]) & 0xffffffff
                tainted = target_taint[n] or target_taint[m]
                effect.update(operation='add', reads=[m, n], writes=[n])
            elif w >> 12 == 7:  # ADD #imm,Rn, except separately decoded SP
                if n in (1, 15):
                    raise PrefixError(f'whole-leaf target/SP arithmetic at {pc:#x}')
                destination, value = n, f'computed:{pc:x}'
                if known[n] is not None:
                    known_value = (known[n] + sext(w & 255, 8)) & 0xffffffff
                tainted = target_taint[n]
                effect.update(operation='add_immediate', reads=[n], writes=[n], immediate=sext(w & 255, 8))
            elif w & 0xf0ff == 0x4008:  # SHLL2 Rn; T is unchanged
                if n in (1, 15):
                    raise PrefixError(f'whole-leaf target/SP shift at {pc:#x}')
                destination, value = n, f'computed:{pc:x}'
                if known[n] is not None:
                    known_value = (known[n] << 2) & 0xffffffff
                tainted = target_taint[n]
                effect.update(operation='shift_left_two', reads=[n], writes=[n])
            elif w & 0xf00f in (0x6002, 0x000e):  # MOV.L @Rm,Rn / @(R0,Rm),Rn
                indexed = w & 0xf00f == 0x000e
                if n in (1, 15) or m in (1, 15):
                    raise PrefixError(f'whole-leaf memory through target/SP at {pc:#x}')
                if target_taint[m] or indexed and target_taint[0]:
                    raise PrefixError(f'whole-leaf memory through target-derived alias at {pc:#x}')
                if (indexed and known[m] is not None and known[0] is not None and
                        (known[m] + known[0]) & 0xffffffff == target_value):
                    raise PrefixError(f'whole-leaf memory through known target alias at {pc:#x}')
                destination, value = n, f'memory:{pc:x}'
                effect.update(operation='load_indexed_long' if indexed else 'load_long',
                              reads=[0, m] if indexed else [m], writes=[n],
                              memory=dict(access='read', size=4, base=m, index=0 if indexed else None,
                                          destination=n))
            else:
                raise PrefixError(f'unsupported whole-leaf instruction at {pc:#x}')
            if destination is not None:
                values[destination] = value
                known[destination] = known_value
                target_taint[destination] = tainted or known_value == target_value
                effect['value_written'] = value
            effect['stack_before'] = before
            effect['stack_after'] = dict(displacement=sp,
                                        cells=[dict(offset=k, **v) for k, v in sorted(cells.items())])
            effect['register_values_after'] = {f'r{r}': values[r] for r in range(15)}
            effect['known_register_values_after'] = {f'r{r}': known[r] for r in range(15) if known[r] is not None}
            effect['target_tainted_registers_after'] = [r for r in range(15) if target_taint[r]]
            effects.append(effect)
        if load is None or load not in visited or sp != 0 or cells:
            raise PrefixError(f'unbalanced whole-leaf stack at {jump:#x}')
        if any(values[r] != initial[r] for r in range(8, 15)):
            raise PrefixError(f'whole-leaf callee-saved register changed at {jump:#x}')
        # Unknown/special-register opcodes never enter the decoder; the only
        # admitted special effects are literal reads of PC and the final JMP.
        facts = dict(proof_kind='whole_leaf_argument_tail', effects=effects,
                     instruction_count=len(pcs), whole_function_single_entry=True,
                     function_end=function_end,
                     trailing_pool_interval=[slot + 2, function_end],
                     stack_proof=dict(initial_displacement=0, final_displacement=sp,
                                      restored_cells=restored, released_cells=released, remaining_cells=[]),
                     register_proof=dict(caller_clobbered=list(range(8)),
                                         preserved={f'r{r}': values[r] for r in range(8, 15)},
                                         unchanged_special=['PR', 'MACH', 'MACL', 'SR', 'GBR', 'VBR'],
                                         target_unique_definition=True))
        return load, facts

    proofs = []
    callback_results = []
    if callback_tails and len(candidates) != 1:
        raise PrefixError("callback mode requires exactly one dynamic tail")
    for jump in sorted(candidates):
        if callback_tails:
            from callback_tail_proof import prove_callback_tail
            callback_results.append(prove_callback_tail(
                jump, word=word, literal=literal, entries=entries, start=start, end=end,
                context_start=context_start, compiled=compiled, rom=rom,
                original=callback_original, provenance=callback_provenance, visited=visited,
                predecessors=predecessors, delay_owners=delay_owners,
                literal_reads=literal_reads, error=PrefixError))
            continue
        register = (word(jump) >> 8) & 15
        if register not in (1, 2, 3):
            raise PrefixError(f'unsupported literal tail register at {jump:#x}')
        flag_facts = None
        startup_facts = None
        if startup_reset_tails:
            from startup_reset_proof import prove_startup_reset_tail
            load, startup_facts = prove_startup_reset_tail(
                jump, word=word, literal=literal, entries=entries, start=start, end=end,
                context_start=context_start, compiled=compiled, rom=rom, visited=visited,
                predecessors=predecessors, delay_owners=delay_owners,
                literal_reads=literal_reads, tail_provenance=tail_provenance, error=PrefixError)
        elif field_flag_tails:
            from field_flag_proof import prove_field_flag_tail
            load, flag_facts = prove_field_flag_tail(
                jump, word=word, literal=literal, entries=entries, start=start, end=end,
                context_start=context_start, compiled=compiled, rom=rom, visited=visited,
                predecessors=predecessors, delay_owners=delay_owners,
                literal_reads=literal_reads, tail_provenance=tail_provenance, error=PrefixError)
        else:
            load = short_tail_load(jump, register)
        long_shape = None
        leaf_facts = None
        code_tail_facts = None
        paired_facts = None
        if load is None and not function_code_range and not leaf_argument_tails:
            paired = paired_call_tail_shape(jump, register)
            if paired is not None:
                load, paired_facts = paired
        if load is None:
            long_shape = long_tail_shape(jump, register)
            if long_shape is None:
                if function_code_range:
                    code_shape = function_code_tail_shape(jump, register)
                    if code_shape is None:
                        raise PrefixError(f'unproven function-code literal tail target at {jump:#x}')
                    load, effects = code_shape
                    code_tail_facts = dict(proof_kind='complete_function_code_tail',
                                           effects=effects)
                elif leaf_argument_tails:
                    load, leaf_facts = whole_leaf_shape(jump, register)
                else:
                    raise PrefixError(f'unproven literal tail target at {jump:#x}')
            else:
                load, form, effects = long_shape
        if load not in visited:
            raise PrefixError(f'unreachable literal tail defining load at {jump:#x}')
        # A unique normal predecessor chain proves dominance from every root,
        # including roots/branch edges found after this jump was visited.
        for pc in range(load + 2, jump + 2, 2):
            if predecessors.get(pc) != {pc - 2} or delay_owners.get(pc):
                raise PrefixError(f'alternate entry in literal tail spine at {pc:#x}')
        slot = jump + 2
        if (slot in visited or predecessors.get(slot) or
                delay_owners.get(slot) != {jump} or
                (flag_facts is None and code_tail_facts is None and
                 not safe_restore(word(slot), register, delay=True))):
            raise PrefixError(f'unsafe literal tail delay slot at {slot:#x}')
        target, width = literal(word(load), load)
        off = target - context_start
        if width != 4 or off < 0 or off + 4 > len(compiled) or target + 4 > len(rom):
            raise PrefixError(f'literal tail pool outside context at {load:#x}')
        literal_bytes = compiled[off:off + 4]
        if literal_bytes != rom[target:target + 4]:
            raise PrefixError(f'literal tail pool differs at {target:#x}')
        provenance = tail_provenance.get(target)
        if not isinstance(provenance, dict):
            raise PrefixError(f'missing literal tail provenance at {target:#x}')
        if (flag_facts is not None or startup_facts is not None or paired_facts is not None) and provenance.get('embedded_addend') != 0:
            raise PrefixError(f'specialized tail target has nonzero or missing original embedded addend at {target:#x}')
        symbol = provenance.get('symbol', '')
        match = re.fullmatch(r'_f_([0-9a-f]+)', symbol)
        canonical = canonical_functions.get(symbol)
        if (not match or not isinstance(canonical, int) or
                canonical != int(match[1], 16) or canonical % 2 or
                not 0x780 <= canonical < 0x313fc or start <= canonical < end):
            raise PrefixError(f'noncanonical literal tail function at {target:#x}')
        value = int.from_bytes(literal_bytes, 'big')
        if (provenance.get('relocation_type') != 'R_SH_DIR32' or
                provenance.get('addend') != 0 or
                provenance.get('binding') != 'STB_GLOBAL' or
                provenance.get('section') != 'SHN_UNDEF' or
                provenance.get('relocation_count', 1) != 1 or
                provenance.get('overlap', False) or
                provenance.get('value') != value or value != canonical + 0x5fff880):
            raise PrefixError(f'invalid literal tail relocation at {target:#x}')
        spine = list(range(load, jump + 2, 2))
        function_root = max(a for a in roots if a <= load)
        proofs.append(dict(function_root=function_root,
                           function_symbol=entries[function_root],
                           load_pc=load, jump_pc=jump, register=register,
                           spine=[dict(pc=a, word=f'{word(a):04x}') for a in spine],
                           delay_slot=dict(pc=slot, word=f'{word(slot):04x}', owner=jump),
                           literal=target, value=value, symbol=symbol,
                           relocation_type='R_SH_DIR32', addend=0,
                           provenance_binding=provenance['binding'],
                           provenance_section=provenance['section'],
                           provenance_relocation_count=provenance.get('relocation_count', 1),
                           target_rom=canonical, target_ram=value,
                           load_dominates_jump=True, single_entry_spine=True))
        if long_shape is not None:
            proofs[-1].update(form=form, effects=effects,
                              stack_increment=sum(effect['stack_increment'] for effect in effects))
        if leaf_facts is not None:
            proofs[-1].update(leaf_facts)
        if flag_facts is not None:
            proofs[-1].update(flag_facts)
        if startup_facts is not None:
            proofs[-1].update(startup_facts)
        if code_tail_facts is not None:
            proofs[-1].update(code_tail_facts)
        if paired_facts is not None:
            proofs[-1].update(paired_facts)
    code_results = []
    if function_code_range:
        covered = visited | set(delay_owners)
        expected = set(range(start, end, 2))
        if covered != expected:
            missing = sorted(expected - covered)
            extra = sorted(covered - expected)
            detail = (f'missing {missing[0]:#x}' if missing else
                      f'extra {extra[0]:#x}' if extra else 'coverage mismatch')
            raise PrefixError(f'function-code range is not completely reachable: {detail}')
        if not covered or max(covered) + 2 != end:
            raise PrefixError('function-code range does not end after its final instruction')
        code_results.append(dict(function_root=start, function_symbol=entries[start],
                                 start=start, end=end,
                                 instruction_pcs=sorted(visited),
                                 delay_slots=[dict(pc=pc, owners=sorted(owners))
                                              for pc, owners in sorted(delay_owners.items())],
                                 complete_single_entry=True,
                                 falls_through_end=False))
    if callback_proofs is not None:
        callback_proofs.extend(callback_results)
    if tail_proofs is not None:
        tail_proofs.extend(proofs)
    if switch_proofs is not None:
        switch_proofs.extend(switch_results)
    if loop_proofs is not None:
        loop_proofs.extend(loop_results)
    if function_code_proofs is not None:
        function_code_proofs.extend(code_results)
    return [dependencies[pc] for pc in sorted(dependencies)]


def compile_prefix(src, out, start, end, *, rom, shcc, version, flags, symbols, assembler,
                   context_start=None, literal_tail_calls=False, canonical_functions=None,
                   leaf_argument_tails=False, guarded_switches=False, local_loop_jumps=False,
                   field_flag_tails=False, startup_reset_tails=False,
                   function_code_range=False, callback_tails=False):
    """Produce a relocation-free object with only verified compiler bytes.

    Full-context compilation/linking is repeated each build: a stale manifest
    or changed literal dependency must never bypass validation.
    """
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    src, out = Path(src), Path(out)
    # A rejected new attempt must not leave a stale success artifact behind.
    manifest_path = out.with_suffix('.prefix.json')
    out.unlink(missing_ok=True)
    manifest_path.unlink(missing_ok=True)
    if callback_tails and (literal_tail_calls or local_loop_jumps or guarded_switches or
                           leaf_argument_tails or field_flag_tails or startup_reset_tails or function_code_range):
        raise PrefixError("callback tails require an exclusive proof mode")
    if local_loop_jumps and (guarded_switches or literal_tail_calls or leaf_argument_tails):
        raise PrefixError('mixed local-loop/switch/tail proof modes are unsupported')
    if guarded_switches and (literal_tail_calls or leaf_argument_tails):
        raise PrefixError('mixed switch/tail proof modes are unsupported')
    if leaf_argument_tails and (not literal_tail_calls or canonical_functions is None):
        raise PrefixError('whole-leaf argument tails require explicit literal tail calls and canonical functions')
    if field_flag_tails and (not literal_tail_calls or canonical_functions is None or leaf_argument_tails):
        raise PrefixError('field-flag tails require explicit literal tail calls without leaf-argument mode')
    if startup_reset_tails and (not literal_tail_calls or canonical_functions is None or
                                field_flag_tails or leaf_argument_tails):
        raise PrefixError('startup-reset tails require explicit literal tail calls and an exclusive proof mode')
    context = out.with_suffix('.context.o')
    linked = out.with_suffix('.context.elf')
    script = out.with_suffix('.context.ld')
    binary = out.with_suffix('.context.bin')
    prefix = out.with_suffix('.prefix.bin')
    wrapper = out.with_suffix('.prefix.s')
    ram_delta = 0x5fff880
    mode = 'c-prefix' if context_start is None else 'c-range'
    context_start = start if context_start is None else context_start
    if context_start % 2 or not 0x780 <= context_start <= start < end <= 0x313fc:
        raise PrefixError('c-prefix currently supports only the copied RAM code block')
    source_bytes = src.read_bytes()
    source_sha = hashlib.sha256(source_bytes).hexdigest()
    with tempfile.TemporaryDirectory(prefix='prefix-source-') as td:
        snapshot = Path(td) / src.name
        snapshot.write_bytes(source_bytes)
        subprocess.run([str(shcc), version, str(context), str(snapshot), *flags], check=True)
    if src.read_bytes() != source_bytes:
        raise PrefixError('source changed during contextual compilation')
    if literal_tail_calls and canonical_functions is None:
        raise PrefixError('literal tail calls require canonical functions')
    definitions = {}
    exports = []
    unresolved = []
    provenance = {}
    readonly_context_sections = []
    with context.open('rb') as f:
        elf = ELFFile(f)
        text_index = next(i for i, sec in enumerate(elf.iter_sections()) if sec.name == '.text')
        for sec in elf.iter_sections():
            if sec.name != '.text' and sec['sh_flags'] & 2 and sec['sh_size']:
                # SHC emits automatic-array templates and string constants in
                # read-only data even when only a later .text range is kept.
                # The package still extracts .text alone. A retained PC-relative
                # reference to this discarded section cannot pass validate_prefix:
                # its target is outside the compiled .text context.
                if sec.name != '.rodata' or sec['sh_flags'] & (1 | 4):
                    raise PrefixError(f'context contains unsupported allocated section {sec.name}')
                readonly_context_sections.append(dict(name=sec.name, size=sec['sh_size']))
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            if symbol['st_shndx'] == 'SHN_UNDEF' and symbol.name:
                unresolved.append(symbol.name)
            if symbol['st_shndx'] == text_index and symbol['st_info']['type'] == 'STT_FUNC':
                addr = context_start + symbol['st_value']
                definitions[addr] = symbol.name
                if start <= addr < end:
                    exports.append((symbol.name, addr - start, symbol['st_info']['bind']))
        if literal_tail_calls or callback_tails:
            for section in elf.iter_sections():
                if not isinstance(section, RelocationSection) or section['sh_info'] != text_index:
                    continue
                table = elf.get_section(section['sh_link'])
                text = elf.get_section(text_index).data()
                for relocation in section.iter_relocations():
                    off = relocation['r_offset']
                    if off < 0 or off + 4 > len(text):
                        raise PrefixError('tail provenance relocation outside .text')
                    symbol = table.get_symbol(relocation['r_info_sym'])
                    addend = (relocation['r_addend'] if section.is_RELA() else
                              int.from_bytes(text[off:off + 4], 'big', signed=True))
                    addr = context_start + off
                    count = provenance.get(addr, {}).get('relocation_count', 0) + 1
                    overlap = False
                    for previous, item in provenance.items():
                        if previous != addr and abs(previous - addr) < 4:
                            item['overlap'] = True
                            overlap = True
                    overlap = overlap or provenance.get(addr, {}).get('overlap', False)
                    provenance[addr] = dict(symbol=symbol.name,
                        relocation_type='R_SH_DIR32' if relocation['r_info_type'] == 1 else str(relocation['r_info_type']),
                        addend=addend, embedded_addend=int.from_bytes(text[off:off + 4], 'big'),
                        binding=symbol['st_info']['bind'],
                        section=symbol['st_shndx'], relocation_count=count, overlap=overlap)
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
        f'SECTIONS {{ .text 0x{context_start + ram_delta:x} : {{ *(.text) }} /DISCARD/ : {{ *(.comment) }} }}\n' +
        '\n'.join(f'{name} = 0x{addr:x};' for name, addr in sorted(resolved.items())) + '\n')
    subprocess.run(['sh-elf-ld', '-T', str(script), '-o', str(linked), str(context)], check=True)
    subprocess.run(['sh-elf-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(binary)], check=True)
    compiled = binary.read_bytes()
    proofs = []
    switch_results = []
    loop_results = []
    code_results = []
    loop_original = None
    original = None
    if guarded_switches or callback_tails:
        from switch_proof import OriginalText
        original = OriginalText.read(context, context_start, PrefixError)
    if local_loop_jumps:
        from local_loop_proof import LocalLoopOriginal
        loop_original = LocalLoopOriginal.read(context, context_start, PrefixError)
        original = loop_original.text
    if literal_tail_calls or callback_tails:
        for item in provenance.values():
            item['value'] = resolved.get(item['symbol'])
    callback_results = []
    deps = validate_prefix(compiled, rom, start, end, definitions, context_start=context_start,
                           tail_provenance=provenance if literal_tail_calls else None,
                           canonical_functions=canonical_functions, tail_proofs=proofs,
                           leaf_argument_tails=leaf_argument_tails,
                           guarded_switches=guarded_switches, switch_original=original,
                           switch_proofs=switch_results, local_loop_jumps=local_loop_jumps,
                           loop_original=loop_original, loop_proofs=loop_results,
                           field_flag_tails=field_flag_tails, startup_reset_tails=startup_reset_tails,
                           function_code_range=function_code_range,
                           function_code_proofs=code_results, callback_tails=callback_tails,
                           callback_original=original if callback_tails else None,
                           callback_provenance=provenance if callback_tails else None,
                           callback_proofs=callback_results)
    offset = start - context_start
    kept = compiled[offset:offset + end - start]
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
    with tempfile.TemporaryDirectory(prefix='prefix-package-', dir=out.parent) as td:
        package = Path(td) / 'kept.o'
        subprocess.run([*assembler, '-no-pad-sections', '-o', str(package), str(wrapper)], check=True)
        if src.read_bytes() != source_bytes:
            raise PrefixError('source changed before proof publication')
        package.replace(out)
    manifest = dict(source=str(src), start=start, end=end, context_start=context_start,
                    mode=mode, kept_bytes=len(kept),
                    context_bytes=len(compiled), source_sha256=source_sha,
                    context_sha256=hashlib.sha256(compiled).hexdigest(),
                    prefix_sha256=hashlib.sha256(kept).hexdigest(),
                    functions=[name for name, _, _ in exports], literal_dependencies=deps,
                    discarded_readonly_sections=readonly_context_sections)
    if callback_tails:
        manifest["callback_tails"] = True
        manifest["callback_proofs"] = callback_results
    if literal_tail_calls:
        manifest['tail_proofs'] = proofs
    if field_flag_tails:
        manifest['field_flag_tails'] = True
    if startup_reset_tails:
        manifest['startup_reset_tails'] = True
    if leaf_argument_tails:
        manifest['leaf_argument_tails'] = True
    if guarded_switches:
        manifest['guarded_switches'] = True
        manifest['switch_proofs'] = switch_results
    if local_loop_jumps:
        manifest['local_loop_jumps'] = True
        manifest['loop_proofs'] = loop_results
    if function_code_range:
        manifest['function_code_range'] = True
        manifest['function_code_proofs'] = code_results
    if guarded_switches or local_loop_jumps or callback_tails:
        manifest['original_object_sha256'] = original.object_sha256
        manifest['rom_range_sha256'] = hashlib.sha256(rom[start:end]).hexdigest()
    with tempfile.NamedTemporaryFile(mode='w', dir=out.parent, delete=False) as f:
        temporary_manifest = Path(f.name)
        try:
            f.write(json.dumps(manifest, indent=2) + '\n')
            f.close()
            temporary_manifest.replace(manifest_path)
        finally:
            temporary_manifest.unlink(missing_ok=True)
    print(f'{mode}: {src.name} {start:#x}-{end:#x}: {len(kept)} verified C bytes; '
          f'{len(compiled) - len(kept)} context-only bytes excluded; {len(deps)} external literal loads')
