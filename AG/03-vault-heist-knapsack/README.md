# Vault heist: graph components, subset sum, knapsack (`plan_heist`)

**Course:** AG (C++).

## Lock math as graph theory

Pebbles `0 .. pebbles-1`. `missing_connections` lists **non-edges** in the lock graph: if `(u,v)` is missing, those two pebbles **cannot** end up on **opposite sides** (one left, one right) after unlocking. Equivalently, in the graph **G** that has an edge for every missing pair, any **valid split** must assign each **connected component** of G entirely to the left or entirely to the right (all-or-nothing per component).

You must move **exactly `pebbles/2`** pebbles to the right. So you need a **subset of components** whose **total size** equals `pebbles/2`. Odd `pebbles` makes a half-split impossible; such vaults are skipped.

## Algorithms

1. **Connected components on G**  
   Build adjacency lists from `missing_connections`, run **BFS** (or DFS) in `O(pebbles + |missing_connections|)`.

2. **Subset sum on component sizes**  
   Let sizes be `s_1, ..., s_k`. Need subset with sum `pebbles/2`. Classic **DP** `reachable[j]` or back-pointer table in `O(k * pebbles)` time and `O(pebbles)` space (this implementation uses a small `memory` vector indexed by weight). Reconstruct one concrete subset of components, then mark which pebbles move (`vector<bool>`).

3. **Global optimization: 0/1 knapsack**  
   Each **openable** vault becomes an item: **weight** = time to open (`pebbles/2` moves) + **travel** `transition_time`, **value** = `vault.value`. Pick a subset of vaults maximizing value with total weight **`<= max_time + transition_time`** (budget as in the local tests). **1D DP** over capacity with bitset of “took item i” for reconstruction: **`O(m * W)`** time, **`O(W)`** space for the DP array plus backtracking storage, where `m` is the number of feasible vaults and `W` the time budget.

## Complexity summary

Per vault feasibility: **`O(P + M)`** for BFS + **`O(C * P)`** for subset-sum DP, `P = pebbles`, `M` = missing edges, `C` = component count ( `C <= P` ).

Knapsack: **`O(m * W)`**, **pseudopolynomial** in `W` (not polynomial in bit length of integers), which matches the assignment (“`max_time` is not very large”).

## Tech

`std::vector`, `std::queue`, unsigned arithmetic, hand-rolled knapsack tables (no external OR-Tools).
