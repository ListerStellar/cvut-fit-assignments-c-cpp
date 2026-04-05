# CTU FIT coursework: C and C++

A portfolio of **graded programming assignments** from Czech Technical University, Faculty of Information Technology ([FIT CTU](https://fit.cvut.cz/)): **procedural C**, **C++ / OOP**, **algorithms and graphs**, and **automata theory**. Each block mirrors a semester course; folders are renamed for readability; per-task `README.md` files summarize algorithms and complexity.

---

## Repository layout

| Directory | Course theme | Contents |
|-----------|----------------|----------|
| [**PA1/**](PA1/README.md) | Programming and algorithmic basics (C) | 10 console / library tasks: I/O, geometry, simulation, dynamic memory, grids, game rules, **trie** |
| [**PA2/**](PA2/README.md) | C++ and object-oriented design | 7 tasks: STL, **operator overloading**, **polymorphism**, templates, parsers, **binary linking** toy model |
| [**AG/**](AG/README.md) | Algorithms and graphs (C++) | 3 tasks: **0-1 BFS** on a state graph, **AVL** with lazy range updates, **knapsack** + graph components |
| [**AAG/**](AAG/README.md) | Automata and grammars (C++) | 1 task: **regular expressions** via Glushkov-style **position automaton** (`nullable` / `firstpos` / `lastpos` / `followpos`) |

Sources are the solutions as submitted or refined for the university **Progtest**-style autograder (exact I/O, memory/time limits, conditional `__PROGTEST__` harness where the course requires it).

---

## Tech stack (what the code actually uses)

**Languages:** **C** (C99-style), **C++17/20** (ranges, `variant`, `optional`, spaceship operator, templates).

**C standard library:** `stdio`, string handling, `math` / `float.h` for numeric tolerance, `qsort`, `malloc` / `realloc`, `stdint`.

**C++ standard library:** `vector`, `string`, `map` / `multimap` / `set` / `unordered_*`, `deque`, `queue`, `stack`, `memory` (`unique_ptr`, `shared_ptr`), `variant` + `visit`, `sstream`, `fstream`, binary I/O, `exception` / `stdexcept`.

**Practices:** specification-driven I/O, **algorithmic complexity** awareness, **manual memory management** where required, **deep copy** semantics, **RAII** where STL owns resources, **bitmasks**, **recursion** and **backtracking**, **hashing** for state spaces.

---

## Concepts

- **Low-level and systems:** exact formatted I/O, endian-safe patching (`memcpy` for unaligned 32-bit LE), object-file style **linking** (symbol resolution, transitive closure).
- **Data structures:** dynamic arrays, **prefix trie** (10-ary), sorted vector + binary search, **AVL**-style balanced BST with **lazy propagation**, **Glushkov** position sets for regex, STL containers as problem-appropriate tools.
- **Algorithms:** **0-1 BFS**, BFS/DFS on graphs, **Euclid** / Pythagorean generation, **greedy simulation**, **0/1 knapsack** and **subset sum**, calendar arithmetic and **range aggregation**, single-pass **range extrema** for stock-style queries, grid search in 8 directions, combinatorial **move generation**.
- **Software design:** **OOP** (virtual polymorphism, CRTP-style patterns avoided; classic inheritance for table cells), **operator overloading**, **functors** / templates, **namespaces** for grader integration, separation of **API hand-ins** vs local `main`.
- **Theory:** **formal regular expressions** (∅, ε, `+`, `.`, `*`) and their connection to **finite automata** (follow-pos construction).
