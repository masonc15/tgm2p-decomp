# Text branch scheduling — checkpoint245

No new matched bytes. Fresh Nuada verification confirmed all64 production pins
and135594/200698 before research. No production/compiler/admission changes.

The original compiler help documents speed/nospeed/size, optimize0/1 and
loop/noloop. Five bounded controls use the frozen genuine cached-callee source
from244, except the final explicit guarded do-loop already retained in244.

| Option/source | Context bytes | Nominal E39C differences | Next root |
|---|---:|---:|---|
| nospeed |64|26|E3B4|
| size |64|26|E3B4|
| speed,noloop |68|6|E3B8|
| optimize0,speed |108|28|E3D4|
| nospeed, guarded do-loop |68|16|E3B8|

All five outputs independently reproduce on Nuada. Nospeed equals size; noloop
equals244 baseline; guarded do-loop nospeed equals its244 speed output. No match.

Two read-only original SHCASM replays use untouched stage-entry snapshots.
Baseline42 events and nospeed39 events cover input/dependency/selected phases.
All selected nodes retain input order, operand identity and graph edges. Fresh
untraced control, traced output and original native assembly are identical.
Independent native assembly/relocation also equals ordinary machine-code output.

The baseline loop buffer already contains LABEL/MOV/SHLL/MOV/MOV/TST/BF_S.
ADD is not a schedulable node in that buffer; it is already reserved as the
branch delay operation. The final output remains TST/BF-S/delay ADD.
Nospeed instead has an ADD-ending body buffer followed by a labeled test buffer
ending BF. This explains why changing final scheduler queue order cannot solve
this candidate. It does not yet identify which earlier pass chooses the delay.

Static original SHCASM decompilation of414990/41BFC0 agrees with these stream
boundaries; inferred source is retained as supporting evidence, not credited as
a traced earlier-stage decision. The raw events, snapshots, compiler help,
objects, provenance and verifier are frozen together.

Next inspect earlier delay-slot formation, or pivot. Do not repeat the documented
flags, final scheduler queue experiments or broad loop-spelling sweeps. Full
production suite was not rerun for research-only negatives. Last full gates remain
two266-test runs at checkpoint225; literal100% remains unachieved.
