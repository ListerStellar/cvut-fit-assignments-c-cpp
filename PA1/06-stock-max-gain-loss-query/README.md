# Stock: max profit and max loss on an index range

**PA1 C.**

## Task

Online stream: `+ p` appends price; `? l r` on current array asks for **max profit** (max sell-buy with increasing indices) and **max loss** (largest drop), with deterministic tie-break on indices.

## Algorithms / complexity

- **Classic linear scan** in `[l,r]` (same family as “best time to buy and sell stock”): maintain **running minimum** for best profit and **running maximum** for best loss while scanning left to right.
- **Per query:** **`O(r - l + 1)`** time, **`O(1)`** extra space beyond the array.
- **Append:** dynamic array with doubling **`O(1)`** amortized per `+`.
- **Bonuses** in the course ask for structures that beat linear time in `r-l` (segment trees, etc.); not in this file.

## Stack

`malloc`/`realloc`, `limits.h` for sentinels.
