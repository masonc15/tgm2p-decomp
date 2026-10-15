# Save calling convention correction — continuation122

Production checkpoint122 keeps **128,316 / 200,698 bytes (63.934868%)** and all original ROM bytes. It corrects the reconstructed EEPROM call interface in six source files without claiming additional matched functions. Both independent clean builds pass. All243 enabled tests pass in the isolated checkout, zero skips. All44 production pins, reports, retained objects and original compiler object sections (apart from comment metadata) agree across nuada checkouts.

Evidence: `tools/shc_probe/cases/session_save_dataflow122/` and `session_checkpoint122_save_abi/`. The previous goal turn was progress: it completed the requested transfer and established that its TGM2/Plus payloads add no alternate build. This continuation uses the paired save contexts from120 instead.

## What was wrong with the earlier inference

The frozen save research correctly identified the EEPROM entry addresses and their byte-address machine behavior. It then treated the modern `unsigned char` prototype as the uniquely authenticated source interface and excluded promoted arguments. That uniqueness claim was too strong.

The real implementation stores its incoming address byte at stack offset15 and later reads that byte. An identifier-list C definition with an `unsigned char addr` parameter produces exactly the same entire2728-byte EEPROM unit. A no-prototype declaration makes short arguments undergo integer promotion at the caller. This preserves the implementation's byte addressing while allowing the call-site sign extensions found in the original save routines.

The C language rule is described in [WG14 N1570, sections6.5.2.2 and6.7.6.3](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): calls without prototypes apply default promotions; compatibility with identifier-list definitions accounts for promoted parameter types. The actual1998 SHC toolchain was tested independently. A mixed control placing an explicit int prototype before the byte-parameter identifier-list definition is rejected by SHC error2118. That failed control is preserved; it is not the adopted source form.

The adopted and compiled form uses identifier-list definitions and no-prototype declarations, with character-pointer arguments matching the implementation. This is evidence of a valid reconstructed source form, not proof of the original author's exact source spelling.

## Concrete compiler evidence

Three complete EEPROM controls match all2728 bytes under the unchanged flags: existing prototype definitions, identifier-list definitions, and identifier-list definitions preceded by empty-parameter declarations. All10 original entry offsets are preserved.

The corrected save context remains668 bytes with the same four roots. Compared with the previous reconstruction, only six halfwords change: at ROM offsets1330c,13330,13356,133d0,133f6,13436, `MOV Rn,R4` becomes `EXTS.W Rn,R4`. These recover the expected operation class. Some sites remain at the wrong location or use the wrong source register, so neither the writer nor reader is accepted.

All five retained save source contexts were independently recompiled with the consistent declarations. Their six retained ranges total880 bytes and still pass default validation, including external literal dependencies. No optional verifier modes were introduced.

## Source experiments and remaining mismatch

Twenty-four caller controls tested real shared offsets, signed/unsigned dirty-mask types, direct shifts, helper argument order, a consumed helper-local offset, a single-argument offset helper, and identifier-list helper syntax. Every writer and reader remains rejected. A shared offset changes function boundaries and is unsuitable. Reversing helper arguments reduces the writer's raw differences from88 to83 but does not change the disputed persistent register allocation. A consumed short helper-local gives85 differences. None is integrated.

Two fresh native captures match their compiler sections, excluding comment metadata. Four normal/debug middle-stage replays reproduce `.reg` and `.ilb` exactly; lossless register-file round trips pass and151 recovered priority/profit records validate. The original and Plus baseline writer allocation records agree. The service-function address has priority4/profit−2 and is allocated before the offset temporary at priority6/profit−3. The tested helper forms do not reverse that order. Original code still needs the service address inR9 and reused offsets inR10; the baseline has serviceR10 and different temporary assignments.

Production changes are confined to `src/eeprom.c`, `src/save_checksum_131e8.c`, `src/savechecksum_21170.c`, `src/savestats_211a2.c`, `src/saveio_21206.c`, and `src/savestatus_213da.c`. The lower-level byte behavior, compiler flags, split map, verifier and denominator are unchanged. The rejected source controls remain research artifacts.

Next work should target the offset temporary's lifetime/coalescing and the shift helper's seed placement using this corrected call model. Do not repeat declaration-only or mask helper controls without a predicted effect. All72,382 remaining bytes, including1,638 runtime bytes, still count as incomplete toward literal100%.
