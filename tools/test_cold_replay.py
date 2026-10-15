"""Negative controls for the new replay's independent acceptance checks."""
import hashlib
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
import cold_replay as replay


class ReplayControls(unittest.TestCase):
    def test_program_and_each_eprom_are_independent_gates(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            payloads = {"build/tgm2p.bin": b"original", "build/prog.bin": b"original",
                        "build/roms/tgm2p/2b.u21": b"high", "build/roms/tgm2p/1b.u22": b"low",
                        "build/report.json": b"{}", "splits.txt": b"0x0 c src/x.c\n0x30ffa bin tail\n"}
            for name, data in payloads.items():
                p = root / name
                p.parent.mkdir(parents=True, exist_ok=True)
                p.write_bytes(data)
            pins = {"2b.u21": hashlib.sha1(b"high").hexdigest(), "1b.u22": hashlib.sha1(b"low").hexdigest()}
            with patch.object(replay, "ROM_SHA1", pins), patch.object(replay, "PROGRAM_SHA256", hashlib.sha256(b"original").hexdigest()):
                self.assertEqual(replay.check_outputs(root)["coverage"]["c_bytes"], 200698)
                for name in ("build/tgm2p.bin", "build/prog.bin", "build/roms/tgm2p/2b.u21", "build/roms/tgm2p/1b.u22"):
                    with self.subTest(name=name):
                        (root / name).write_bytes(payloads[name] + b"mutation")
                        with self.assertRaises(ValueError):
                            replay.check_outputs(root)
                        (root / name).write_bytes(payloads[name])

    def test_same_size_same_timestamp_change_is_visible(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            p = root / "source.c"
            p.write_bytes(b"abcd")
            before = replay.manifest([p], root)
            p.write_bytes(b"abce")
            self.assertNotEqual(before, replay.manifest([p], root))

    def test_denominator_cannot_be_silently_reduced(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            (root / "splits.txt").write_text("0x0 c src/x.c\n0x10 bin tail\n")
            with self.assertRaisesRegex(ValueError, "denominator"):
                replay.coverage(root)


if __name__ == "__main__":
    unittest.main()
