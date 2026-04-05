# Draughts: enumerate white moves

**PA1 C.**

## Task

Generalized draughts on `n x n` (small `n`): enumerate all legal **white** moves including repeated captures; kings with long slides and jumps per PDF.

## Algorithms / complexity

- **Search:** **depth-first expansion** of capture sequences; board copied with **`memcpy`** for branching (backtracking without global undo in places).
- **Complexity:** highly **output-sensitive**; branching grows with capture graph. Not a single clean `O(...)` bound (exponential in worst capture depth). Grader reorders output.
- **Rules engine:** constant-time neighbor checks, validity of dark squares, piece types.

## Stack

Recursion, fixed max board arrays, `stdbool.h`.
