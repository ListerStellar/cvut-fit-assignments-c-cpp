# Draughts: enumerate white moves

**Course:** PA1 C. **Stack:** recursion, `memcpy` board copies, `stdbool.h`.

Board size 3..26. Pieces `w`/`W` (white man/king), `b`/`B` (black). Only dark squares, one piece per square. List every legal white move: simple steps, jumps, multi-jumps for men (forward along diagonals toward higher row) and kings (long slides and jumps per spec). Each line is a path ending with `+k` captures if any.

Grader does not care about output order (it reorders). Code aims for complete enumeration, not micro-opts.
