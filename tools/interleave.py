#!/usr/bin/env python3
"""Join or split the TGM2+ SH-2 program ROMs.

The PS5V2 board has two 16-bit EPROMs on a 32-bit bus. MAME loads them with
ROM_LOAD32_WORD_SWAP: u21 supplies the high word of each longword, u22 the low
word, and each ROM stores its words little-endian.
"""
import argparse
import hashlib
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HI_NAME, LO_NAME = "2b.u21", "1b.u22"
EXPECTED_SHA1 = {
    HI_NAME: "783e8413b11f1fa08d331b09ef4ed63f62b87ead",
    LO_NAME: "3f7e81756470c173cc17a7e7dee91437571fd0c3",
}


def join(hi: bytes, lo: bytes) -> bytes:
    if len(hi) != len(lo):
        sys.exit(f"size mismatch: {len(hi)} vs {len(lo)}")
    out = bytearray(len(hi) * 2)
    out[0::4] = hi[1::2]
    out[1::4] = hi[0::2]
    out[2::4] = lo[1::2]
    out[3::4] = lo[0::2]
    return bytes(out)


def split(prog: bytes) -> tuple[bytes, bytes]:
    if len(prog) % 4:
        sys.exit("program image length must be a multiple of 4")
    hi = bytearray(len(prog) // 2)
    lo = bytearray(len(prog) // 2)
    hi[1::2] = prog[0::4]
    hi[0::2] = prog[1::4]
    lo[1::2] = prog[2::4]
    lo[0::2] = prog[3::4]
    return bytes(hi), bytes(lo)


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    sub = ap.add_subparsers(dest="cmd", required=True)
    j = sub.add_parser("join", help="u21 + u22 -> big-endian program image")
    j.add_argument("--roms", type=Path, default=ROOT / "roms")
    j.add_argument("-o", "--out", type=Path, default=ROOT / "build" / "prog.bin")
    s = sub.add_parser("split", help="program image -> u21 + u22")
    s.add_argument("image", type=Path)
    s.add_argument("-o", "--outdir", type=Path, default=ROOT / "build" / "roms" / "tgm2p")
    s.add_argument("--check", action="store_true", help="compare with the original sha1s")
    args = ap.parse_args()

    if args.cmd == "join":
        hi = (args.roms / HI_NAME).read_bytes()
        lo = (args.roms / LO_NAME).read_bytes()
        prog = join(hi, lo)
        if split(prog) != (hi, lo):
            sys.exit("round trip failed")
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_bytes(prog)
        print(f"{args.out}  {len(prog):#x} bytes  sha1 {sha1(prog)}")
    else:
        hi, lo = split(args.image.read_bytes())
        args.outdir.mkdir(parents=True, exist_ok=True)
        ok = True
        for name, data in ((HI_NAME, hi), (LO_NAME, lo)):
            (args.outdir / name).write_bytes(data)
            digest = sha1(data)
            match = digest == EXPECTED_SHA1[name]
            ok &= match
            print(f"{args.outdir / name}  sha1 {digest}  {'OK' if match else 'DIFFERS'}")
        if args.check and not ok:
            sys.exit(1)


if __name__ == "__main__":
    main()
