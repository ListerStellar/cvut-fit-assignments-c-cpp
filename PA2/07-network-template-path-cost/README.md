# Network path cost (`CNet<T>` template)

**PA2 C++.**

## Task

Undirected **weighted** graph (tests are **trees**). `add(x,y,cost)`, `optimize()` (no-op here), `totalCost(x,y)` sum of edge costs on the connecting path, or `-1` if missing.

## Algorithms / complexity

- **Storage:** `vector` of `(node, vector<(neighbor,cost)>)`; finding adjacency list for a node scans **`O(V)`** in this file (no hash map from `T_`).
- **Query:** **queue BFS** style expansion with **accumulated cost**; **visited** via linear scan over **`O(V)`** vector per step. On a **tree**, unique simple path so first arrival at target is correct total.
- **Time:** worst **`O(V * (V+E))`** with this linear lookup; with `unordered_map` on nodes would be **`O(V+E)`** per query.
- **`optimize`:** empty; bonuses use **LCA + prefix sums** on a tree for **`O(log V)`** or **`O(1)`** queries after **`O(V)`** prep.

## Stack

`queue`, `vector`, templates; tests use `std::string`, `int`, `CDumbString`.
