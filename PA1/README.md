# PA1: procedural C (CTU FIT)

Console programs and small library-style hand-ins. Focus: **I/O**, **numeric robustness**, **basic algorithms**, **memory**, **combinatorial / graph-flavored puzzles**. Each folder has `README.md` and sources.

## Assignments (algorithms at a glance)

| # | Folder | Main ideas | Typical complexity (this repo) |
|---|--------|------------|----------------------------------|
| 00 | [00-formatted-io-input-validation](00-formatted-io-input-validation/) | Parsing, spec output | `O(1)` time/space |
| 01 | [01-parallelograms-geometry-double](01-parallelograms-geometry-double/) | Vector geometry, FP tolerance | `O(1)` |
| 02 | [02-rectangle-areas-comparison](02-rectangle-areas-comparison/) | Area, epsilon compare | `O(1)` |
| 03 | [03-right-triangles-pythagorean](03-right-triangles-pythagorean/) | Euclid triples, GCD, enumeration vs counting | list `O((hi-lo)^3)` triple loop; count sub-quadratic in bound via generation |
| 04 | [04-date-weekmask-scheduling](04-date-weekmask-scheduling/) | Calendar, bitmasks, range sums | naive `O(days)`; chunked `O(weeks + boundary)` for long spans |
| 05 | [05-freight-capacity-simulation](05-freight-capacity-simulation/) | `qsort`, greedy simulation | sort `O(n log n)`; daily loop `O(D * n)` worst, `n` vehicles |
| 06 | [06-stock-max-gain-loss-query](06-stock-max-gain-loss-query/) | Prefix extrema in range (two pointers on min/max) | append `O(1)` amortized; query `O(r-l+1)` per `?` |
| 07 | [07-wordsearch-eight-directions](07-wordsearch-eight-directions/) | Grid scan, 8 directions | per query `O(R*C*L)` naive, `L` word length |
| 08 | [08-draughts-move-generation](08-draughts-move-generation/) | DFS / backtracking on moves | output- and branching-sensitive (exponential in captures depth) |
| 09 | [09-prefix-trie-phonebook](09-prefix-trie-phonebook/) | **Trie** (10-ary), prune on delete | `O(|phone|)` per op, memory `O(nodes)` |

## Stack

- **C** standard library: `stdio`, `stdlib`, `string`, `math`, `ctype`, `stdint` where used.
- **No STL** in several early tasks; later: `qsort`, manual `malloc`, or hand-in API only.
- **Harness:** `#ifndef __PROGTEST__` blocks for local `main` / extra includes on some tasks.

Complexity above is **asymptotic** for the implemented approach; Progtest time limits pick data sizes that fit.
