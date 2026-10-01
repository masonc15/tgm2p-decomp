# Compiler runtime exceptions

Colin approved explicit compiler-runtime assembly exceptions during the coverage work. The completion criterion is matching C for every non-exempt code byte, with the original ROMs still byte-identical. This replaces the literal 100% C requirement; it does not change how coverage is measured.

The existing report denominator stays **200,698 bytes**. Exempt assembly stays assembly and remains incomplete in the report. Do not wrap it in C, alter completeness flags, reclassify it as data, or remove it from the denominator.

## Boundaries

ROM ranges are half-open:

```text
0x030d40–0x03127c  1,340 bytes  private-ABI division, remainder, shifts and their tables
0x0312ba–0x0313e4    298 bytes  alignment, 16-bit remainder, logical right shift and its table
```

These exceptions total **1,638 bytes**. With only these exceptions remaining, linked C would cover **199,060 / 200,698 bytes (99.183848%)**, not 100%. Completion still requires `make split check` to pass both original ROM SHA-1 checks and `make report` to verify the actual linked C coverage. Any other remaining assembly is unresolved work, not an implicit exception.

The ordinary-C string comparison routine at `0x3127c–0x312ba` and memory-fill routine at `0x313e4–0x313fc` are deliberately excluded from the exceptions. Their C candidates match the respective instruction spans. The original `0xffff` alignment halfword at `0x312ba` stays within the assembly exception rather than being counted as C.

## Evidence

`symbols.txt` identifies the SHC division, remainder and shift helper addresses. `README.md` identifies the runtime block beginning at ROM `0x30d40`. Disassembly of that routine starts with `tst r0,r0` and uses the dividend in `r1`; the arithmetic helpers use a private register convention, not ordinary C parameters.

The local SHC manual, `refs/shc_manual_1997.txt`, documents ordinary parameter allocation to `R4` through `R7` around lines 2354–2389. Its global-register pragma restricts integer register bindings to R8–R14 at line 3322. The intrinsic list at lines 2631–2840 provides no direct arbitrary-register parameter binding.

The diagnostic `tools/shc_probe/cases/inline_goal/runtime_30d40_probe.c` was compiled with SHC v5.0r32, `-macsave=1 -optimize=1 -speed`. It uses `R4/R5` parameters and emits a relocation to `__divls` at object offset `0x28`: ordinary C division calls the original helper rather than implementing its private ABI. This is evidence of the supported-toolchain obstacle, not a proof about every hypothetical compiler.

No new assembly implementation is authorized by these exceptions. Preserve the original runtime bytes.
