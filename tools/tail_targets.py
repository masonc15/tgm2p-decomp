"""Read explicitly reviewed ROM function entries for static tail proofs.

Discovery scans and generated assembly labels are not entry evidence. This
registry authenticates reviewed inputs; it does not infer new functions or
supply output bytes. Adding an entry requires independent function review.
"""
import hashlib
import json
import re


class TailTargetError(ValueError):
    pass


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise TailTargetError(f'duplicate registry key: {key}')
        result[key] = value
    return result


def load_tail_targets(path, rom):
    try:
        registry = json.loads(path.read_text(), object_pairs_hook=unique_object)
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise TailTargetError(f'cannot read reviewed tail-target registry: {path}') from exc
    if not isinstance(registry, dict) or set(registry) != {'rom_sha256', 'entries'}:
        raise TailTargetError('invalid tail-target registry schema')
    if registry['rom_sha256'] != hashlib.sha256(rom).hexdigest():
        raise TailTargetError('reviewed tail targets belong to a different ROM')
    entries = registry['entries']
    if not isinstance(entries, list) or not entries:
        raise TailTargetError('tail-target registry must contain reviewed entries')
    result = {}
    for entry in entries:
        if not isinstance(entry, dict) or set(entry) != {'symbol', 'start', 'end', 'sha256', 'evidence'}:
            raise TailTargetError('invalid reviewed entry schema')
        start, end = entry['start'], entry['end']
        if (type(start) is not int or type(end) is not int or
                not 0x780 <= start < end <= min(0x313fc, len(rom)) or start % 2 or end % 2):
            raise TailTargetError('invalid reviewed function span')
        symbol = entry['symbol']
        if symbol != f'_f_{start:x}':
            raise TailTargetError('reviewed entry does not have its exact function identity')
        if symbol in result:
            raise TailTargetError(f'duplicate reviewed function: {symbol}')
        digest = entry['sha256']
        if (not isinstance(digest, str) or not re.fullmatch('[0-9a-f]{64}', digest) or
                digest != hashlib.sha256(rom[start:end]).hexdigest()):
            raise TailTargetError(f'reviewed function bytes changed: {symbol}')
        if not isinstance(entry['evidence'], str) or not entry['evidence'].strip():
            raise TailTargetError(f'missing function-entry evidence: {symbol}')
        result[symbol] = start
    return result
