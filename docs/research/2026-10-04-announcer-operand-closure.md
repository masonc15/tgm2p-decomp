# Announcement: operand views do not solve prologue scheduling

Continuation219; production stays135502/200698 (67.51537%). No C gain or
production/proof change. The previous goal turn was progress: checkpoint218
integrated the ten-byte lookup and passed both complete native build/test gates.

The self-contained816-byte f_7c08 remains a24-byte near-match. Its current
differences are the global-pointer prologue load scheduling, the call register
at7C74, and the final input-index/table/remainder register choices. The initial
mode-test sequence otherwise matches; older descriptions of that area should
not be interpreted as additional current mismatches.

Checkpoint218's array-operand result motivated fresh views of the menu base,
menu members, global flag word, field mode and ID. All reproduce the baseline
linked context, except removing the global pointer alias, which produces55
differences. Native tracing confirms baseline, direct-array and constant-key
variants make zero calls to218's extra symbolic-memory address selector. Normal
compiler completion, the actual function entry and separate parent/scratch
traces authenticate this absence. Parent source kinds are already1/7/8/9.

Four complete MDL models reconstruct priority, interference costs, sort order
and homes. Baseline has24 candidates. Its named global pointer(symbol65) has
priority5/profit0, index8(R11). Removing it creates a symbol-address candidate
with priority4/profit-1, index10(R9), and changes the zero/110 and10/63 homes.
A consumed constant key base plus separate cursor adds a25th candidate, raises
the10 constant's profit from0 to1 and moves the global pointer toR10. The
explicit-first-write control also has25 candidates; its address priority stays4.
The proposed explicit-write frequency fix is therefore rejected by native data.

Eighteen controls cover seven initial forms, five ordinary const/register-hint
forms, three explicit flag writes and three equivalent numerical-address forms.
The best remains24 differences. Explicit-second/both grow to820 bytes and fail
the original boundary. All original objects, linked contexts and rejections are
preserved. No forced register, inert use, padding, compiler/IR/object/ROM patch
or partial credit is used.

Baseline loads global0x6064880 at7C20 intoR11; the ROM loads it at7C18 intoR11.
Direct-array, constant-key, explicit-first and numeric-pointer variants still
load at7C20, intoR9/R10/R10/R8 respectively. This separates the scheduling issue
from the home changes. Final object evidence alone cannot establish whether
GEN initialization or PEP scheduling causes the late load.

Verification authenticates18 rejected contexts, four independently assembled
native captures, four MDL graph replays/models and nine GEN replays. All13
read-only replays preserve fresh native control outputs over1307 events. All459
scratch selections and258 parent masks are reconstructed. REG/ILB match native
captures; SUA/ASA match fresh untraced controls, with cleanup-snapshot ASA
metadata differences explicit. model_graph.py and verify.py pass.

Next trace SHCGEN prologue emission and SHCPEP scheduling around the global
address materialization, R9/R8/PR saves and stack adjustment. Find where the late
position is established before more C controls. Preserve baseline homes and
separately investigate the final index/table tie. Do not repeat the closed views,
hints, explicit writes or numerical-address forms. All62 production pins and
original ROM identity remain required. Checkpoint218's two263-test gates remain
the latest full production validation. The literal100% goal stays active, solo.
