# Animation sprite-ID conversion controls — continuation135

Fifteen genuine source controls distinguish explicit casts, wider values, local aliases and inline sprite lookup helpers. None matches the original routine. The register-correct continuation134 source remains the main candidate. An inline-reference alternative recovers the original body length and two important stack slots, but loses register assignments and does not reproduce the original R9 writebacks.

## Casts and aliases

The baseline is continuation134's `initial_separate.c`, SHA-256 `affe66bff8caee380faf62033e467a45ad3481a7c61de313f3e07f717baae2d4`. It preserves the original hold/mask15/constant2/total roles and a108-byte frame, but emits1716 bytes rather than1748.

Adding explicit `(short)` casts to either three or all four initial sprite-ID indices, or to the global sprite index, produces identical native sections outside comment metadata. Widening spr2 to int is value-preserving because its only assignment receives a signed-short return; the preparation script verifies the full signed-short range. Widening, with or without selected narrowing casts, changes code but does not match.

A named local pointer supplies either three or four actual field indices from the same initialized sprite ID. Moving that pointer's initialization into its declaration and making the pointer itself const are further controls. These pointers are consumed by the field address calculations, never passed to external functions, and add no fake read, padding or volatile qualifier. They produce real stack reads but not the original repeated R9 writebacks. Early plain and const-pointer controls have identical native sections.

## Inline lookup contrast

The helper experiment uses the compiler's supported inline pragma:

```c
#pragma inline(sprite_lookup)
static struct spr *sprite_lookup(short *id)
{
    return &g_606006c[*id];
}
```

It supplies the b2a, w18 and w2c field accesses. A by-value helper is the control. Extending the reference helper to the two visibility branches replaces their genuine staged address calculations with the same pointer lookup. All ID reads remain after the ID is initialized; the helper neither modifies the ID nor performs an external call. The helper is present as an intermediate record but emits no standalone function. Each native object still contains exactly36 functions.

The `inline_ref_all` alternative emits1748 bytes with a108-byte frame and places spr2 at slot0, selection at4, tbl at8 and spr1 at16, agreeing with these original positions. It does not reproduce the original writes from R9 to slot0 at A760/A77A/A7AC. Its total is in R9 rather than on the stack; mask15 is assigned a caller register rather than original R9. Equal length is therefore not evidence of exact matching or an overall improvement over the baseline. Using the legacy unspecified f_253bc declaration on this changed graph also fails, producing1764 bytes and a112-byte frame.

| Source | Routine bytes | Local frame |
| --- | ---: | ---: |
| baseline | 1716 | 108 |
| short_indices | 1716 | 108 |
| three_short_indices | 1716 | 108 |
| wide_sprite | 1714 | 108 |
| wide_three_indices | 1690 | 104 |
| global_short_index | 1716 | 108 |
| local_alias | 1702 | 104 |
| three_local_alias | 1706 | 104 |
| early_alias | 1706 | 104 |
| early_const_alias | 1706 | 104 |
| inline_ref | 1726 | 104 |
| inline_value | 1714 | 104 |
| inline_ref_all | 1748 | 108 |
| inline_value_all | 1690 | 104 |
| inline_ref_legacy | 1764 | 112 |

All candidates start at A604 instead of A608 and fail the unchanged strict contextual verifier. Relative byte diagnostics never earn coverage credit.

## Verification and next step

All15 native compiles succeed. Twelve independent captures and24 normal/debug middle replays reproduce native sections and exact `.reg`/`.ilb` output. The three native-identical cast controls do not require separate middle captures. Fifteen CFG audits resolve every stack access and check stack restoration; serialized spill groups agree with actual emitted frames. `SPRITE_HOME_WRITES.json` lists every store to each candidate's sprite-ID home, independent of which address register is used.

The first verification attempt incorrectly assumed every intermediate stream had36 records. Inline helpers add a37th diagnostic record while emitting no code. The corrected verifier binds the animation record to the compiler's actual function-name sequence, verifies every serialized record round-trip, and separately asserts36 emitted functions with no sprite_lookup export. The initial failure log is retained. No production gate or byte-matching rule was relaxed.

The next experiment should compare the baseline's ordinary spr2 web with the inline-reference variant's address-taken leaf before and after allocation. Trace how SHC decides whether to keep a scalar value in a register while its stack home remains visible to indirect reads. The original R9-to-slot0 synchronization is still unexplained. Do not infer a volatile qualifier, fabricated aggregate, dummy self-assignment or padding from these negative controls.

Both Nuada checkouts retain all44 checkpoint122 production pins, twenty previous frozen cases, original ROM hashes and all23 reference-ROM hashes. The latest full production gate remains checkpoint122's independent clean builds and243 enabled tests.

**New verified C bytes:0. Production remains128,316 /200,698 (63.93487%). The full goal is active.**
