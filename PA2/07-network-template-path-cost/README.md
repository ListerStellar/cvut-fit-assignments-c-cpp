# Network path cost (`CNet<T>` template)

**PA2 C++.** Template parameter `T_` is node id: copyable, comparable with `==`. `add(x, y, cost)` adds an undirected weighted edge (stored twice in an adjacency-style `vector` of `(node, neighbors)`). Fluent: returns `*this`. `optimize()` is a no-op here (course allows empty; bonuses use preprocessing on a tree). `totalCost(x, y)` returns the sum of edge costs on the path, or -1 if a node is missing or unreachable.

Search: queue BFS-style expansion with accumulated cost; visited list is a `vector` checked by linear scan. Inputs are trees in tests, so there is a single simple path; this matches the sample asserts.

Bonuses: faster lookup with `std::hash` or `operator<`, and O(1) or near-O(1) queries after `optimize()` on a tree (e.g. LCA). Not implemented in this file.
