# Building a C file that uses `#pragma inline_asm`

SHC only expands `#pragma inline_asm` bodies when it writes assembly
(`-code=asmcode`); with the default `-code=machinecode` it rejects them. So a
file like `wip/vm_ef04.c` has to go C -> `.src` (shc) -> SYSROF `.obj` (asmsh,
which ships in the same `shc-v5.0r32` tarball) -> ELF (rof2elf). Everything
here was tried on nuada; the patch, `elffix.py` and the `symoff_c.c` test case are in this directory.

## Status

This is built in now. `tools/shc_probe/shcc.sh` sends any source containing
`#pragma inline_asm` through shc `-code=asmcode` and asmsh, converts with
`rof2elf_expr.py` (the stock `rof2elf_fillff.py` with `rof2elf-expr.diff`
applied in memory, so neither nuada's toolchain nor the CI image changes),
and types the functions with `elffix.py` (plain struct parsing, no
pyelftools). Every other file keeps the direct shc path, so its output is
unchanged. `src/board_28334.c` is the first linked file that uses it.

The rest of this file is the original investigation.

## What blocks it today

Yes, rof2elf's "Relocation expression too big" blocks it, but only for pool
words of the form symbol+offset, and the fix is small.

shc's own objects carry `sym+0xc0` as a plain 4-byte reloc (`02 <idx16> ff`)
with the 0xc0 already in the data word. asmsh instead leaves 0 in the data and
writes an RPN expression, `03 04 000000c0 | 02 0000 | 20 | ff` (constant,
external ref, add, end; 11 bytes). rof2elf only accepts the 4-byte form. This
has nothing to do with inline asm: any asmsh object with `sym+const` fails
(`symoff_c.c`, no pragma, fails the same way). `wip/vm_ef04.c` only gets
through today because it spells `g_606006c[i].b2a` as a fake `g_6060096[]`.

`rof2elf-expr.diff` (about 40 lines, against `rof2elf_fillff.py`) folds
the expression's constants into the data word and emits the same plain
symbol reloc shc would. With it:

- `symoff_c.c` through the asm path gives a `.text` byte-identical to the
  plain shc path (`cmp` of the two `.text` sections), same `R_SH_DIR32 _g_606006c`
  reloc and same `0x00c0` addend word.
- `vm_ef04_off.c` (the real `g_606006c[i].b2a` spelling) scores 207/220,
  the same as the workaround; `f_ef04` is 61/61. Stock rof2elf fails on it.

Only operators seen so far are handled (constant 0x03, ext ref 0x02, section
0x00, add 0x20, end 0xff); anything else raises, so nothing is silently wrong.

## Minimal tool changes

1. `tools/shc_probe/shcc.sh`: take the asm path only when the source uses the
   pragma, so every existing file keeps the byte-identical direct path and
   build.py/probe.py/funcscore.py need no changes at all:

   ```sh
   if grep -q '#pragma[[:space:]]*inline_asm' "$SRC"; then
       SHC_LIB=. SHC_TMP=. "$COMPILERS/wibo" ./shc.exe src.c <same flags> "$@" \
           -code=asmcode -object=src.src > shc.log 2>&1 || { cat shc.log; exit 1; }
       SHC_LIB=. SHC_TMP=. "$COMPILERS/wibo" ./asmsh.exe src.src -cpu=sh2 -endian=big \
           -object=src.obj > asm.log 2>&1 || { cat asm.log; exit 1; }
   else
       <current shc.exe line>
   fi
   python3 "$COMPILERS/${ROF2ELF:-rof2elf_fillff.py}" src.obj "$OUT" --isa=sh2 > /dev/null
   ```

   plus, on the asm branch only, the symbol fix-up in `elffix.py`
   : asmsh's object gives global functions
   `STT_NOTYPE`, and funcscore/build.py's `c_functions` look for `STT_FUNC`.
   It sets type FUNC and a size running to the next global. A working copy of
   the whole asm path is `shcc_asm.sh`.

2. rof2elf: apply `rof2elf-expr.diff` to `/drive2/tgm2p/compilers/rof2elf_fillff.py`
   on nuada, and in `tools/ci/Dockerfile` apply the same patch after the sed
   that makes `rof2elf_fillff.py` (and update its pinned sha256). Keeping the
   patch in the repo (for example `tools/ci/rof2elf-expr.patch`) lets both
   places use one file. The patch only changes behavior for relocs longer than
   4 bytes, which previously raised, so existing builds can't change.

3. `tools/build.py`: nothing. It calls shcc.sh with the header flags and links
   the `.o`; the asm-path object has the same sections (`.text`, `.rela.text`,
   `.comment`) and reloc type.

Alternative considered: a flag in the header (`-code=asmcode`) instead of
grepping for the pragma. It makes the path explicit but means build.py,
probe.py and funcscore.py all have to strip or pass it; the grep keeps the
change to one script. I'd go with the grep.
