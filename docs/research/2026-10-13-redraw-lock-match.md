# Verified percentage increase — 2026-10-13

Verified linked C increased from **137332 to137402 /200698 bytes**:
**68.42718911000608% →68.46206738482695%**, a gain of **70 bytes** and
**0.034878274820875146 percentage points**. The denominator and existing1638-byte
runtime exception are unchanged;61658 non-exempt assembly bytes remain.

## What made the percentage move

The redraw-lock wrapper at26faa receives three full-width arguments, then passes
them to a redraw function with short parameters. Correcting those incoming types
recovered the ROM's12-byte parameter spill frame, all branch/call displacements,
and the shared literal pool. The previous short-parameter draft omitted that
frame, moving the next function20 bytes early. The exact retained interval is
[26faa,26ff0):56 code bytes plus14 shared-pool bytes.

Genuine26f64 predecessor and26ff0/2722c following bodies preserve compiler
context. They are excluded from coverage. The new context-only `_modws` symbol
maps the original signed-short remainder helper at06030b3c; no runtime code
changed. Source: `src/board_redraw_lock_26faa.c`.

## Verification

- Unmodified pinned SHC v5.0r32 and unchanged strict c-range verifier.
- Independent raw-byte, original relocation, ABI, pool and GNU-link reviews.
- Isolated and canonical `make split check report`; both original ROM chip
  SHA1s and complete program SHA256 unchanged.
- All289 native-enabled tests pass, zero skips. New positive and negative
  regression tests verify the retained code/pool/callee identity and reject the
  old narrow incoming argument declarations.
- Two independent cold whole-ROM builds pass from233 frozen inputs. Receipt:
  `research/2026-10-13-wrapper-parameters/cold/receipt.json`.
- Existing verified publication service publishes the actual report; live
  evidence is saved in the adjacent `PUBLICATION.json`.

The first staged suite attempt lacked copied test-only Python dependencies and
reported two import errors. Copying the unmodified dependencies resolved those;
the complete rerun above passed. No proof rules were weakened.

## The requested94-byte startup helper

Native compiler traces show true-arm TEMP1 is assignedR7 and copied to selected
pointerR5. The PEP copy-chain pass stops at ADD#8,R7, so the copy survives in the
BRA delay slot. A genuine nested false-arm assignment reduces9 raw differences
to5 while preserving the106-byte predecessor and root1340. Early coallocation
or early selected-pointer definitions merge the stores and shorten the function;
an inline consumed store preserves the separate scratch registers but introduces
formal-parameter copies. These controls are rejected and grant no coverage.

Evidence: `research/2026-10-13-startup-native/round4/REPORT.md` and
`research/2026-10-13-startup-match/nested_false_consumer.result.json`.
Next targeted question: find an existing positive compiler pattern whose genuine
inline argument binding preserves separate stores without occupying selected
pointerR5 with the field formal. Do not repeat the failed cast/register/early-p
families. Full decompilation remains incomplete.
