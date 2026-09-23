"""Create a scratch on the private decomp.me on nuada for a ROM range, then compile it.

The target is rendered from the exact ROM bytes (tools/build.py's splitter), with
literal-pool words that hold a symbols.txt address written as that symbol, so a
matching C source scores 0.

usage: scratch.py <src.c> <start> <end> <function> "<flags>"
e.g.   scratch.py src/field_clear.c 0x188ac 0x18950 field_clear_flag "-optimize=1 -speed"
"""
import json, sys, urllib.error, urllib.request
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn, build

BASE = "http://127.0.0.1:28080/api"  # nginx of /drive2/tgm2p/decomp.me-local
src_path, start, end, name, flags = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16), sys.argv[4], sys.argv[5]
data = build.pool_targets(start, end)
text, _ = build.render(start, end, set(), set(), data)
asm = text.replace("\t.text\n", f"\t.text\n\t.global\t_{name}\n_{name}:\n", 1)
# Name pool words that hold known symbol addresses, as a real target would.
import re, struct
syms = {v: k for k, v in build.read_symbols().items()}
lines = asm.splitlines()
out, i = [], 0
while i < len(lines):
    m = re.search(r"/\* ([0-9a-f]{6}) \*/", lines[i])
    if m and lines[i].lstrip().startswith(".short") and i + 1 < len(lines) and lines[i + 1].lstrip().startswith(".short"):
        a = int(m[1], 16)
        v = struct.unpack(">I", fn.ROM[a:a + 4])[0]
        if a % 4 == 0 and v in syms:
            out.append(f"\t.long\t_{syms[v]}\t/* {a:06x} */")
            i += 2
            continue
    out.append(lines[i]); i += 1
asm = "\n".join(out) + "\n"
src = open(src_path).read()

def call(method, url, body=None):
    req = urllib.request.Request(BASE + url, method=method, data=json.dumps(body).encode() if body else None,
                                 headers={"Content-Type": "application/json"})
    try:
        with urllib.request.urlopen(req) as r:
            return json.load(r)
    except urllib.error.HTTPError as e:
        sys.exit(f"{method} {url}: {e.code} {e.read().decode()[-1500:]}")

s = call("POST", "/scratch", {"compiler": "shc-v5.0r32-sh2", "platform": "saturn", "compiler_flags": flags,
                              "diff_label": f"_{name}", "target_asm": asm, "source_code": src, "context": ""})
print("scratch", s["slug"], "score", s.get("score"), "max", s.get("max_score"))
c = call("POST", f"/scratch/{s['slug']}/compile", {"compiler_flags": flags, "source_code": src})
print("compile success", c.get("success"), "| errors:", (c.get("compiler_output") or "").strip()[:300])
d = c.get("diff_output") or {}
print("current score", d.get("current_score"), "max", d.get("max_score"))
rows = d.get("rows", [])
bad = [r for r in rows if any(k in json.dumps(r) for k in ('"diff_change"', '"diff_remove"', '"diff_add"'))]
print("rows", len(rows), "rows with diffs", len(bad))
for r in bad[:8]:
    print(" ", json.dumps(r)[:260])

def cell_text(cell):
    return "".join(t.get("text", "") for t in (cell or {}).get("text", []))
for r in rows:
    b, c2 = cell_text(r.get("base")), cell_text(r.get("current"))
    if b.split() != c2.split() or any(t.get("format") not in (None, "", "diff_none") for side in ("base", "current") for t in (r.get(side) or {}).get("text", [])):
        print("DIFF |", b, "||", c2)
print("url: http://100.79.208.43:28080/scratch/" + s["slug"])
