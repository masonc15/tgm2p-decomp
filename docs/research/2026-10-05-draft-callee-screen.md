# Draft callee screen (254)

Current production remains 135,910 / 200,698 bytes. The immutable old ABI
selection was filtered through the current split map, leaving 33 unmatched
bodies and 157 distinct source callee names counted per body. Source hashes
were checked before use; no stale file was silently treated as current.

A bounded ROM control-flow traversal collected direct calls/tails and literal
code-address candidates. It does not prove every candidate is a call or that
all calls are resolved. Eight bodies contain unresolved indirect transfers.

Only three source names were absent from these candidates:

- B36E's 2F36E is the already confirmed wrong EEPROM callee from253 (3036E).
- 8C96's two 8B7C calls correspond to inline reads of the same global index
  and ROM table. The actual MOV.B operations are present at8F04 and8F1C.
- 13380's 13378 call corresponds to the same inline global assignment followed
  by the1326C writer call. Both forms store7FFFF to60794BC.

The latter two are not wrong callees. The verifier checks original words,
literal values and direct branch targets rather than interpreting an absent
call name as an error. This screen found no additional confirmed wrong callee
in the selected inventory. It is not a full call-graph or whole-game audit.

Do not repeat this name screen on the same frozen inputs. Proceed from fresh
source/ROM discrepancies, including genuine bodies excluded by old context
selection, rather than assuming another register mismatch is a wrong callee.
