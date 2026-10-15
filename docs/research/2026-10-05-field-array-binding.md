# Field array call binding — checkpoint243

No production gain: 135,594 / 200,698 bytes (67.56121%). All five controls preserve
800 previously matched bytes and original function roots, but none passes the
unchanged prefix admission gate.

| Source | Context bytes | Target byte differences |
|---|---:|---:|
| baseline |1624|644|
| named coordinate |1620|380|
| named coordinate plus frame pointer cast |1616|602|
| named coordinate plus cached state |1616|598|
| pointer cast plus cached state |1616|609|

Missing target bytes count as differences. Baseline has another four extra bytes.
The 380-difference source is the retained research draft, not production C.
Independent relocation confirms all five original Mac compiler outputs equal
Nuada outputs (three native captures and two ordinary compiles).

The frame array has internal type0x88. Original4054D0 skips this argument when
counting integer parameter positions; explicitly casting it to a pointer makes
it count. Original405220 consequently prebinds the named coordinate toR5 without
the cast andR6 with it. Baseline changes no WEB assignment. The generalized
no-FPU eligibility model matches all417 events in103 requests. All six replay
outputs remain byte-identical to their ordinary reg/ilb captures.

Verification: session_field_array243/verify.py authenticates64 production pins,
source provenance, original roots, protected bytes, compiler outputs, default
rejection, ordinal predictions and binding changes. Frozen case contains raw
observations and validation receipt. Full production suite was not rerun for
these rejected research-only changes; checkpoint225 retains its two266-test gates.

Stop this target. Neither the explicit cast nor cached-state followups improve
the best draft. Do not repeat these sweeps without a new specific mechanism.
