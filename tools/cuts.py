#!/usr/bin/env python3
"""List the smallest ROM ranges that can move into C on their own.

A C unit can replace a stretch of the asm only when nothing crosses its
edges: no pc-relative load, branch or bsr may reach from inside it to outside
or the other way, because SHC places literal pools after a run of functions
and only uses bsr within a file. This walks the functions in the RAM-copied
block, spans each one over everything it references, and merges overlapping
spans. Each merged span is a candidate unit; the columns are its range,
length, function count, the codegen shape of each function (o optimized,
0 unoptimized) and the calls it makes.

usage: cuts.py [--max LEN] [--opt-only] [--c]
"""
import argparse

import fn


def refs():
    """(lo, hi) byte span of every pc-relative load, branch and bsr in the
    copied block, skipping halfwords that are themselves literal-pool data."""
    lo_end, hi_end = fn.RAM_ROM, fn.TEXT_END

    def decode(a):
        w = fn.hw(a)
        op = w >> 12
        if op == 0xD:
            t = ((a + 4) & ~3) + (w & 0xFF) * 4
            return t, 4, "pool"
        if op == 0x9:
            return a + 4 + (w & 0xFF) * 2, 2, "pool"
        if w & 0xFF00 == 0xC700:
            return ((a + 4) & ~3) + (w & 0xFF) * 4, 4, "mova"
        if op == 0x8 and (w >> 8) & 0xF in (0x9, 0xB, 0xD, 0xF):
            return a + 4 + fn.sdisp(w & 0xFF, 8) * 2, 2, "br"
        if op in (0xA, 0xB):
            return a + 4 + fn.sdisp(w & 0xFFF, 12) * 2, 2, "br"
        return None

    data = set()
    for a in range(lo_end, hi_end, 2):
        d = decode(a)
        if d and d[2] == "pool":
            data.update(range(d[0], d[0] + d[1], 2))
    out = []
    for a in range(lo_end, hi_end, 2):
        if a in data:
            continue
        d = decode(a)
        if d and lo_end <= d[0] < hi_end:
            out.append((min(a, d[0]), max(a + 2, d[0] + d[1])))
    return out


def spans():
    """Maximal runs between valid cuts. A cut may go at a function start that
    no reference spans; the runs between cuts are the candidate units."""
    cover = [0] * ((fn.TEXT_END - fn.RAM_ROM) // 2 + 2)
    for lo, hi in refs():
        cover[(lo - fn.RAM_ROM) // 2 + 1] += 1  # halfword boundaries strictly inside
        cover[(hi - fn.RAM_ROM) // 2] -= 1
    starts = set(fn.call_targets())
    cuts, depth = [fn.RAM_ROM], 0
    for i in range(len(cover) - 1):
        depth += cover[i]
        a = fn.RAM_ROM + i * 2
        if depth == 0 and a in starts and a % 4 == 0 and a > cuts[-1]:
            cuts.append(a)
    cuts.append(0x313fc)
    return [(lo, hi, sorted(s for s in starts if lo <= s < hi)) for lo, hi in zip(cuts, cuts[1:])]


def c_ranges():
    import build
    return [(a, b) for a, b, kind, _ in build.read_splits() if kind == "c"]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--max", type=lambda s: int(s, 0), default=0x200, help="longest unit to list")
    ap.add_argument("--opt-only", action="store_true", help="skip units with unoptimized functions")
    ap.add_argument("--c", action="store_true", help="also list units already in C")
    args = ap.parse_args()
    done = c_ranges()
    for lo, hi, members in spans():
        if hi - lo > args.max or lo % 4:
            continue
        in_c = any(a <= lo and hi <= b for a, b in done)
        if in_c and not args.c:
            continue
        shapes = "".join("0" if fn.shape(m, fn.extent(m)[0]) == "O0" else "o" for m in members)
        if args.opt_only and "0" in shapes:
            continue
        calls = sum(i.mnemonic in ("jsr", "bsr") for m in members
                    for i in fn.MD.disasm(fn.ROM[m:fn.extent(m)[0]], m))
        tag = " (C)" if in_c else ""
        print(f"{lo:06x}-{hi:06x} {hi - lo:5x} funcs {len(members):2d} {shapes:6s} calls {calls}{tag}")


if __name__ == "__main__":
    main()
