# AG: algorithms and graphs (CTU FIT, C++)

Course track on **graph algorithms**, **shortest paths**, **balanced search trees**, **range updates**, and **dynamic programming**. Each folder is one hand-in; local tests live under `#ifndef __PROGTEST__`.

## Assignments

| # | Folder | Core algorithms | Notes |
|---|--------|-----------------|--------|
| 01 | [01-dungeon-state-graph-bfs](01-dungeon-state-graph-bfs/) | **0-1 BFS** (deque) on implicit state graph; unweighted shortest path in action count | Not Dijkstra: only weights 0 and 1 on edges. Plain BFS would be wrong if pickup/drop were free moves. |
| 02 | [02-hobbit-army-avl](02-hobbit-army-avl/) | **AVL tree** (rotations), **lazy propagation**, augmented min in subtree | Ordered map by `std::string`; range add with rollback if HP would hit 0. |
| 03 | [03-vault-heist-knapsack](03-vault-heist-knapsack/) | **BFS/DFS** for connected components, **subset-sum DP**, **0/1 knapsack DP** | Lock feasibility reduces to partitioning pebble components; global plan is knapsack on time budget. |

## Tech stack

- **Language:** C++17/20 (`<=>`, `std::ranges`, `std::optional`, `std::variant`, concepts-style `auto` parameters where used).
- **STL:** `std::vector`, `std::deque`, `std::queue`, `std::unordered_map` / `unordered_set`, `std::string`, iterators; no exotic third-party libs.
- **Complexity mindset:** state-space size (01), tree height and lazy tags (02), pseudopolynomial knapsack in budget (03).

## How this relates to classic lectures

- **BFS:** layer-by-layer search; here generalized to **0-1 BFS** when edges have weight 0 or 1 (same idea as a tiny **Dial’s algorithm** / bucket Dijkstra).
- **Dijkstra:** shortest paths with nonnegative weights; would subsume 0-1 BFS but with a heap; not required when only two weights exist.
- **Balanced BST:** guarantees `O(log n)` height vs skewed BST `O(n)`; AVL keeps rotations bounded.
- **DP:** knapsack and subset-sum are textbook **pseudopolynomial** time in the numeric budget or target sum.
