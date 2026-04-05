# Hobbit army: AVL BST, lazy range updates (`HobbitArmy`)

**Course:** AG (C++).

## ADT

Dictionary keyed by hobbit **name** (`std::string`), in-order = **lexicographic** order. Operations: insert, delete, point query, in-order walk, **range add** of `(hp, off, def)` on `[first, last]`.

## Tree algorithm: balanced BST (AVL)

Underlying structure is a **binary search tree** with **height balancing** after `add` and `erase` (AVL-style **rotations**: single and double). Balance factor from subtree heights; rotations restore `|balance| <= 1` along the search path.

**Why not `std::map` only?** The hard part is **`enchant` on a name interval** with **lazy tags** and optional **validation** (no hobbit may drop to HP <= 0 when `hp_diff < 0`). A plain map would force `O(k log n)` or worse if you touch every element; the hand-in expects **subtree-level** shortcuts.

## Range updates: lazy propagation + augmentation

- Each node stores **pending** `(dhp, doff, ddef)` for its whole subtree.
- **`lazyPush`** applies pending deltas to this node and forwards them to children before descending.
- **`BVSUpdate`**: if `[first, last]` fully covers a node’s subtree (using stored **min/max name** in subtree), only adjust lazy tags and **`O(1)`** work at that node.
- If the range splits the subtree, push, recurse left/right, recompute aggregates (**`updateData`**).

For **`CHECK_NEGATIVE_HP`**: before applying a negative HP delta to a range, query **minimum HP in range** (with pending HP included). Augmentation: **`minSubTreeHobbitHP`** maintained from children plus lazy HP. If `min + hp_diff <= 0`, abort and return `false` (transaction rolled back).

## Complexity (typical)

- **`add` / `erase` / `stats`:** `O(log n)` **height** of AVL (`n` hobbits), each step `O(1)` rotations amortized per insert/delete.
- **`for_each`:** `O(n)` in-order traversal; lazy push along path adds `O(log n)` overhead per visited node in the worst skew, still linear overall for full scan.
- **`enchant`:** `O(log n)` when updates **fully cover** disjoint subtrees; **partial overlap** can descend to many nodes, worst **`O(n)`** in pathological ranges (same order as segment-tree range updates touching many leaves).

Space: **`O(n)`** nodes, `O(1)` extra per node for lazy and aggregates.

## Tech

Raw pointers, manual `new`/`delete`, `std::optional` for `erase` result, C++20 `operator<=>` for string compare in search.

`CHECK_NEGATIVE_HP` is **`true`** in this repo, so `enchant` must reject illegal HP drops.
