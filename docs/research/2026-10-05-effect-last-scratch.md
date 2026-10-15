# Effect last-scratch writes — continuation 241

Production remains 135,594 / 200,698 bytes (67.56121%). Neither new source
control passes the unchanged admission gate; no C bytes were added.

Two hardware-watchpoint replays observe every write to GEN's last-scratch
byte at 0x46051c, plus function entry, emission count and result-planner entry.
Both preserve the ordinary SUA/ASA output. Each records 88 events: 84 writes
and four result-planner calls. All 59 scratch-selection writes per replay
agree with the independent frozen240 scratch trace. The first write is loader
initialization outside GEN; all later values form a continuous observed chain.

For f_20a9c, result planner 0x4234c0 is never called. At emission index20,
baseline writes [0,255,0,3,2]; product-first writes [3,0,255,0,2]. The reset255
comes from instruction42260e in operand planner422290. In baseline, there is
no intervening reset between first-product R3 and second-product R2 selection.
This rules out inspecting result-planner branches as the direct cause in these
two contexts. It does not establish that an equivalent source cannot match.

Two consumed inline arithmetic helpers test return-value boundaries while
preserving the correct pointer cast on the first drawing-call argument:

| Control | Context bytes | Full-context differences | Protected prefix differences |
| --- | ---: | ---: | ---: |
| Partial sum helper | 388 | 100 | 12 |
| Bottom product helper | 384 | 31 | 0 |

Both preserve all three original function roots. Partial sum introduces an
extra signed-word extension and uses R7 for accumulation. Bottom product uses
R7 for that product and changes ordering; neither recovers the desired R3
reuse. The height domain0..255 and signed-short y keep all helper arithmetic
within signed32 range. No artificial effects, register directives or padding
were introduced. Default proofs reject both, including the expanded context's
end-boundary failure. These two exact forms should not be repeated.

The Mac fallback is now operational: original SHC executables were authenticated
against all20 frozen Nuada executable hashes, then run with Wibo1.2.0 and Rosetta.
Four saved contexts calibrated equal before these experiments; both new outputs
also match fresh ordinary Nuada compiles byte-for-byte with identical roots.
GNU SH binutils2.45 built natively using system zlib after the bundled zlib
failed against the modern macOS SDK. The local toolchain has not yet completed
a full clean production build; these are six specific cross-host calibrations.
Provenance is preserved in LOCAL_TOOLCHAIN_PROVENANCE.json and local-toolchain/.

verify.py authenticates frozen240, all64 production pins, observed state chains,
118 scratch-write correspondences, ordinary/watched output equality, both
Mac/Nuada objects and default rejection. Checkpoint225 remains the last complete
production build and266-test gates; the full suite was not rerun for negatives.
Checkpoint240's previously pending sync is now complete and both mirrors verified.

Next inspect operand-planner inputs around42260e and the product allocation
requests. A useful new source hypothesis must account for both the retained R0
accumulator and unchanged R3 history, or pivot to another incomplete caller
with demonstrated239 array-argument skipping. Avoid another helper/order sweep.
The literal100% goal remains active, solo, without brainstorming.
