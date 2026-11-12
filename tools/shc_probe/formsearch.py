#!/usr/bin/env python3
"""Hill-climb over source forms of one function, scored by alignment with the ROM.

Starting from a draft, every single change from a fixed list of forms is
compiled and aligned against the ROM (fdiff.analyse). The best few results
form the next round's starting points (a beam search), until the score stops
improving. The forms are the ones that moved real functions:

  dog N      loop N (while or for) becomes a do-while entered at its test:
             goto L; do { body L:; } while (cond);
  addr V     local V is made memory-resident with (void)&V;
  swap N     declaration statements N and N+1 change places
  sw2if N    switch N (no fallthrough into code, no inner break) becomes an
             if/else chain
  cp MODE    hand-expanded centre prints f_2bf58(s, y, (320 - f_2c0d0(s)) >> 1)
             in this function become an inlined static helper (fn) or a block
             macro with its own locals (mys: y first, msy: s first); fnx, mxys
             and mxsy also keep x in a local. rfn is the form the ROM shows: a
             #pragma inline function that returns f_2bf58's result. SHC ends
             each expansion of a function with a return statement in a label,
             which stops loop rotation, hoisting and unrolling around it and
             keeps identical switch arms apart.
  lab N      loop N gets an unused label at the end of its body (the same
             effect without a helper)
  andk N     "x & K" number N becomes "K & x" (SHC loads the left operand into
             the destination register); andk_all does every one
  ty V T     scalar local V gets type T

This is a search aid. A higher score grants nothing; the result still has to
be read with fdiff.py and pass the ordinary proof.

usage: formsearch.py DRAFT.c FUNC [--out DIR] [--jobs N] [--rounds N] [--beam N] [--patience N] [--kinds dog,addr,...] [--apply "dog1 cp_fn"]
"""
import argparse
import hashlib
import json
import re
import sys
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "shc_probe"))

TYPE_WORD = r"(?:register|static|const|volatile|unsigned|signed|char|short|int|long|struct|union|enum|void|float|double)\b"
CP = re.compile(r"(^|[;{}:)]|\belse)(\s*)f_2bf58\(([\w\[\]\.>-]+), ([^,()]+), \(320 - f_2c0d0\(\3\)\) >> 1\)\s*;", re.M)
CP_DEFS = {
    "fn": "#pragma inline(cp_fn)\nstatic void cp_fn(char *s, short y)\n{\n\tf_2bf58(s, y, (320 - f_2c0d0(s)) >> 1);\n}\n",
    "mys": "#define CP_MYS(s_, y_) { short y = (y_); char *s = (s_); f_2bf58(s, y, (320 - f_2c0d0(s)) >> 1); }\n",
    "msy": "#define CP_MSY(s_, y_) { char *s = (s_); short y = (y_); f_2bf58(s, y, (320 - f_2c0d0(s)) >> 1); }\n",
}
CP_DEFS.update({
    "fnx": "#pragma inline(cp_fnx)\nstatic void cp_fnx(char *s, short y)\n{\n\tshort x;\n\n\tx = (320 - f_2c0d0(s)) >> 1;\n\tf_2bf58(s, y, x);\n}\n",
    "mxys": "#define CP_MXYS(s_, y_) { short y = (y_); char *s = (s_); short x; x = (320 - f_2c0d0(s)) >> 1; f_2bf58(s, y, x); }\n",
    "mxsy": "#define CP_MXSY(s_, y_) { char *s = (s_); short y = (y_); short x; x = (320 - f_2c0d0(s)) >> 1; f_2bf58(s, y, x); }\n",
})
CP_DEFS["rfn"] = "#pragma inline(cp_rfn)\nstatic short cp_rfn(char *s, short y)\n{\n\treturn f_2bf58(s, y, (320 - f_2c0d0(s)) >> 1);\n}\n"
CP_CALL = {"rfn": r"\1\2cp_rfn(\3, \4);", "fn": r"\1\2cp_fn(\3, \4);", "mys": r"\1\2CP_MYS(\3, \4)", "msy": r"\1\2CP_MSY(\3, \4)",
           "fnx": r"\1\2cp_fnx(\3, \4);", "mxys": r"\1\2CP_MXYS(\3, \4)", "mxsy": r"\1\2CP_MXSY(\3, \4)"}
SIMPLE_DECL = re.compile(r"\s*((?:unsigned |signed )?(?:char|short|int|long))\s+([A-Za-z_]\w*(?:\s*,\s*[A-Za-z_]\w*)*)\s*;", re.S)
ALT_TYPE = {"short": ["long", "char"], "long": ["short"], "int": ["short"], "char": ["short", "long"],
            "unsigned char": ["unsigned short", "unsigned long"], "unsigned short": ["unsigned long", "unsigned char"],
            "unsigned long": ["unsigned short"], "unsigned int": ["unsigned short"], "signed char": ["short", "long"]}
ANDK = re.compile(r"(\((?:[^()]|\([^()]*\))*\)|\b[A-Za-z_]\w*(?:(?:\.|->)\w+)*) & (0x[0-9a-fA-F]+|\d+)\b")
KINDS = ("sw2if", "andk", "addr", "swap", "dog", "lab", "cp", "ty")


def mask_of(s):
    """s with comments and string/char literals blanked, same length."""
    out, i, n = list(s), 0, len(s)
    while i < n:
        c = s[i]
        if s.startswith("/*", i):
            j = s.find("*/", i + 2); j = n if j < 0 else j + 2
        elif s.startswith("//", i):
            j = s.find("\n", i); j = n if j < 0 else j
        elif c in "\"'":
            j = i + 1
            while j < n and s[j] != c:
                j += 2 if s[j] == "\\" else 1
            j += 1
        else:
            i += 1
            continue
        for k in range(i, min(j, n)):
            if out[k] != "\n":
                out[k] = " "
        i = j
    return "".join(out)


def close_of(m, i):
    """Index of the bracket matching m[i]."""
    pair = {"(": ")", "{": "}", "[": "]"}[m[i]]
    depth = 0
    for j in range(i, len(m)):
        if m[j] == m[i]:
            depth += 1
        elif m[j] == pair:
            depth -= 1
            if depth == 0:
                return j
    raise ValueError("unbalanced")


def skip_ws(m, i):
    while i < len(m) and m[i].isspace():
        i += 1
    return i


def stmt_end(m, i):
    """Index of the ';' ending the simple statement that starts at i."""
    depth = 0
    for j in range(i, len(m)):
        if m[j] in "([{":
            depth += 1
        elif m[j] in ")]}":
            depth -= 1
        elif m[j] == ";" and depth == 0:
            return j
    raise ValueError("no statement end")


def find_function(text, func):
    """(start, end) of the definition of func: from its line start to after its closing brace."""
    m = mask_of(text)
    for hit in re.finditer(r"\b%s\s*\(" % re.escape(func), m):
        q = close_of(m, hit.end() - 1)
        j = skip_ws(m, q + 1)
        if j < len(m) and m[j] == "{":
            return text.rfind("\n", 0, hit.start()) + 1, close_of(m, j) + 1
    raise ValueError(f"{func} is not defined")


def strip_type(part):
    part = part.strip()
    while True:
        m = re.match(r"(?:struct|union|enum)\s+\w+\s*", part) or re.match(TYPE_WORD + r"\s*", part)
        if not m:
            return part
        part = part[m.end():]


def decls(ft):
    """[(start, end, text, names)] of the declaration statements at the top of the function body."""
    m = mask_of(ft)
    i = m.index("{") + 1
    out = []
    while True:
        j = skip_ws(m, i)
        if not re.match(TYPE_WORD, m[j:]):
            return out
        e = stmt_end(m, j)
        body = m[j:e]
        names = []
        if not re.match(r"(?:register|static)\b", body):
            for part in re.split(r",(?![^\[]*\])", re.sub(r"=\s*\{[^}]*\}", "", body)):
                mm = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*$", strip_type(part.split("=")[0]))
                if mm:
                    names.append(mm[1])
        out.append((j, e + 1, ft[j:e + 1], names))
        i = e + 1


def loops(ft):
    """[(kind, start, end, parts)] for while and for loops that can be rewritten."""
    m = mask_of(ft)
    out = []
    for hit in re.finditer(r"\b(while|for)\s*\(", m):
        kind = hit[1]
        q = close_of(m, hit.end() - 1)
        head = ft[hit.end():q]
        j = skip_ws(m, q + 1)
        if m[j] == ";":
            continue  # do-while tail or empty loop
        if m[j] == "{":
            k = close_of(m, j)
            body, end = ft[j + 1:k], k + 1
        else:
            if re.match(r"(?:if|for|while|do|switch)\b", m[j:]):
                continue
            k = stmt_end(m, j)
            body, end = " " + ft[j:k + 1] + " ", k + 1
        if kind == "for":
            parts = re.split(r";(?![^(]*\))", head)
            if len(parts) != 3 or not parts[1].strip() or re.search(r"\bcontinue\b", mask_of(body)):
                continue
            parts = [p.strip() for p in parts] + [body]
        else:
            parts = [head.strip(), body]
        prev = m[:hit.start()].rstrip()
        bare = prev.endswith(")") or prev.endswith("else")
        out.append((kind, hit.start(), end, parts, bare))
    return out


def loop_body_ends(ft):
    """Index of the closing brace of every braced loop body (while, for, do), in source order."""
    m = mask_of(ft)
    ends = []
    for hit in re.finditer(r"\b(while|for)\s*\(|\bdo\s*\{", m):
        if hit[1]:
            j = skip_ws(m, close_of(m, hit.end() - 1) + 1)
            if j >= len(m) or m[j] != "{":
                continue
        else:
            j = hit.end() - 1
        ends.append(close_of(m, j))
    return ends


def switches(ft):
    """[(start, end, replacement)] for switches that convert cleanly to if/else chains."""
    m = mask_of(ft)
    out = []
    for hit in re.finditer(r"\bswitch\s*\(", m):
        q = close_of(m, hit.end() - 1)
        expr = ft[hit.end():q].strip()
        j = skip_ws(m, q + 1)
        if m[j] != "{" or not re.fullmatch(r"[\w\.\[\]>-]+", expr):
            continue
        k = close_of(m, j)
        inner, im = ft[j + 1:k], m[j + 1:k]
        labels, depth = [], 0
        for lab in re.finditer(r"\b(case\s+([^:]+)|default)\s*:", im):
            depth = im[:lab.start()].count("{") - im[:lab.start()].count("}")
            if depth == 0:
                labels.append(lab)
        if not labels or im[:labels[0].start()].strip():
            continue
        groups, conds, ok = [], [], True
        for n, lab in enumerate(labels):
            stop = labels[n + 1].start() if n + 1 < len(labels) else len(inner)
            conds.append(lab[2].strip() if lab[2] else None)
            body, bm = inner[lab.end():stop], im[lab.end():stop]
            if not bm.strip():
                continue  # shares the next label's body
            last = n + 1 == len(labels)
            tail = re.search(r"\bbreak\s*;\s*$", bm)
            if tail:
                body, bm = body[:tail.start()], bm[:tail.start()]
            elif not last:
                ok = False
            if re.search(r"\bbreak\b", bm):
                ok = False
            groups.append((conds, body))
            conds = []
        if not ok or conds or len(groups) < 2:
            continue
        chain, default = [], None
        for cs, body in groups:
            if None in cs:
                default = body
            else:
                cond = " || ".join(f"{expr} == {c}" for c in cs)
                chain.append(f"if ({cond}) {{{body.rstrip()}\n}}")
        text = " else ".join(chain) + (f" else {{{default.rstrip()}\n}}" if default is not None else "")
        out.append((hit.start(), k + 1, text))
    return out


def label_for(ft):
    n = 0
    while f"fsl{n}:" in ft:
        n += 1
    return f"fsl{n}"


def mutations(ft, kinds):
    """{name: new function text, or (definitions to put before it, new text)} for every single change."""
    out = {}
    if "dog" in kinds:
        for n, (kind, a, b, parts, bare) in enumerate(loops(ft)):
            lab = label_for(ft)
            if kind == "while":
                cond, body = parts
                new = f"goto {lab};\n\tdo {{{body.rstrip()}\n\t{lab}:;\n\t}} while ({cond});"
            else:
                init, cond, inc, body = parts
                new = (f"{init};\n\t" if init else "") + f"goto {lab};\n\tdo {{{body.rstrip()}\n\t{inc + ';' if inc else ''}\n\t{lab}:;\n\t}} while ({cond});"
            out[f"dog{n}"] = ft[:a] + ("{ " + new + " }" if bare else new) + ft[b:]
    if "lab" in kinds:
        lab = label_for(ft)
        for n, close in enumerate(loop_body_ends(ft)):
            out[f"lab{n}"] = ft[:close] + f"{lab}:;\n\t" + ft[close:]
    if "andk" in kinds:
        hits = list(ANDK.finditer(ft))
        for n, h in enumerate(hits):
            out[f"andk{n}"] = ft[:h.start()] + f"{h[2]} & {h[1]}" + ft[h.end():]
        if len(hits) > 1:
            out["andk_all"] = ANDK.sub(r"\2 & \1", ft)
    ds = decls(ft)
    if "addr" in kinds and ds:
        at = ds[-1][1]
        for _, _, _, names in ds:
            for v in names:
                if f"(void)&{v};" not in ft:
                    out[f"addr_{v}"] = ft[:at] + f"\n\t(void)&{v};" + ft[at:]
    if "swap" in kinds:
        for n in range(len(ds) - 1):
            (a0, a1, t0, _), (b0, b1, t1, _) = ds[n], ds[n + 1]
            out[f"swap{n}"] = ft[:a0] + t1 + ft[a1:b0] + t0 + ft[b1:]
    if "ty" in kinds:
        for a0, a1, t, _ in ds:
            m = SIMPLE_DECL.fullmatch(t)
            if not m or m[1] not in ALT_TYPE:
                continue
            base, vs = m[1], [v.strip() for v in m[2].split(",")]
            for i, v in enumerate(vs):
                for alt in ALT_TYPE[base]:
                    parts = ([f"{base} {', '.join(vs[:i])};"] if vs[:i] else []) + [f"{alt} {v};"]
                    parts += [f"{base} {', '.join(vs[i + 1:])};"] if vs[i + 1:] else []
                    out[f"ty_{v}_{alt.replace(' ', '')}"] = ft[:a0] + "\n\t".join(parts) + ft[a1:]
    if "sw2if" in kinds:
        for n, (a, b, new) in enumerate(switches(ft)):
            out[f"sw2if{n}"] = ft[:a] + new + ft[b:]
    if "cp" in kinds and CP.search(ft):
        for mode in CP_DEFS:
            out[f"cp_{mode}"] = (CP_DEFS[mode], CP.sub(CP_CALL[mode], ft))
    return out


def evaluate(job):
    workdir, draft_text, a, b, pre, ft, func = job
    import fdiff
    name = hashlib.sha1((pre + ft).encode()).hexdigest()[:12]
    path = Path(workdir) / f"{name}.c"
    path.write_text(draft_text[:a] + pre + ft + draft_text[b:])
    try:
        r = fdiff.analyse(path, func)
    except Exception as e:  # a form that does not compile is simply not a candidate
        return name, None, str(e)[-160:]
    rom_n, our_n = len(r["theirs"]), len(r["ours"])
    sig = hashlib.sha1("\n".join(t[2] for t in r["ours"]).encode()).hexdigest()[:12]
    return name, {"rom": rom_n, "ours": our_n, "eq": r["eq"], "eq_noreg": r["eq_noreg"], "sig": sig,
                  "score": r["eq"] + r["eq_noreg"] - abs(our_n - rom_n)}, ""


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("draft", type=Path)
    ap.add_argument("func")
    ap.add_argument("--out", type=Path)
    ap.add_argument("--jobs", type=int, default=9)
    ap.add_argument("--rounds", type=int, default=12)
    ap.add_argument("--beam", type=int, default=8)
    ap.add_argument("--patience", type=int, default=3, help="rounds without a better score before stopping")
    ap.add_argument("--kinds", default="lab,addr,swap,sw2if,cp,ty")
    ap.add_argument("--apply", help="apply this space-separated list of changes in order, score it and stop")
    args = ap.parse_args()
    kinds = set(args.kinds.split(","))
    text = args.draft.read_text()
    a, b = find_function(text, args.func)
    ft = text[a:b]
    out = args.out or ROOT / "build" / "formsearch" / args.func
    out.mkdir(parents=True, exist_ok=True)
    log = {"draft": str(args.draft), "func": args.func, "rounds": []}
    key = lambda pre, ft: hashlib.sha1((pre + ft).encode()).hexdigest()[:12]
    if args.apply:
        pre = ""
        for name in args.apply.split():
            v = mutations(ft, {next(k for k in KINDS if name.startswith(k))}).get(name)
            if v is None:
                sys.exit(f"{name} is not available here; have: {' '.join(mutations(ft, kinds))}")
            p, ft = v if isinstance(v, tuple) else ("", v)
            pre += p
        _, r, err = evaluate((out, text, a, b, pre, ft, args.func))
        if r is None:
            sys.exit("does not compile: " + err)
        final = out / f"{args.func}.{'_'.join(args.apply.split())}.c"
        final.write_text(text[:a] + pre + ft + text[b:])
        print(f"{args.apply}: ours {r['ours']}/{r['rom']} eq {r['eq']} noreg {r['eq_noreg']} score {r['score']} -> {final}")
        return
    with ProcessPoolExecutor(args.jobs) as ex:
        _, base, err = evaluate((out, text, a, b, "", ft, args.func))
        if base is None:
            sys.exit("the draft does not compile: " + err)
        show = lambda r: f"ours {r['ours']:4d}/{r['rom']} eq {r['eq']:4d} noreg {r['eq_noreg']:4d} score {r['score']}"
        print(f"base: {show(base)}", flush=True)
        # beam search: single changes of every state in the beam, best few kept
        beam = [("", ft, [], base)]
        best = beam[0]
        seen = {key("", ft)}
        sigs = {base["sig"]}
        stale = 0
        for rnd in range(args.rounds):
            cands = []
            for pre, cur, applied, _ in beam:
                for name, v in mutations(cur, kinds).items():
                    p, t = v if isinstance(v, tuple) else ("", v)
                    k = key(pre + p, t)
                    if k not in seen:
                        seen.add(k)
                        cands.append((pre + p, t, applied + [name]))
            if not cands:
                break
            results = list(ex.map(evaluate, [(out, text, a, b, p, t, args.func) for p, t, _ in cands]))
            # a change that leaves the emitted code as some earlier state had it is dropped
            scored = []
            for n, r in sorted(enumerate(results), key=lambda x: -(x[1][1] or {"score": -1})["score"]):
                if r[1] and r[1]["sig"] not in sigs:
                    sigs.add(r[1]["sig"])
                    scored.append((r[1]["score"], n))
            log["rounds"].append([{"applied": cands[n][2], **results[n][1]} for _, n in scored[:40]])
            beam = [(cands[n][0], cands[n][1], cands[n][2], results[n][1]) for _, n in scored[:args.beam]]
            if not beam:
                break
            if beam[0][3]["score"] > best[3]["score"]:
                best, stale = beam[0], 0
            else:
                stale += 1
            print(f"round {rnd + 1}: {len(cands):3d} tried; best {' '.join(beam[0][2])}: {show(beam[0][3])}", flush=True)
            if stale >= args.patience:
                break
    pre, ft, applied, best = best
    final = out / f"{args.func}.best.c"
    final.write_text(text[:a] + pre + ft + text[b:])
    log["applied"], log["best"] = applied, best
    (out / "log.json").write_text(json.dumps(log, indent=1) + "\n")
    print(f"best: {' '.join(applied) or '(draft)'} -> {final}")


if __name__ == "__main__":
    main()
