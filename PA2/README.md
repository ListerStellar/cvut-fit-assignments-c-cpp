# PA2: C++ and OOP (CTU FIT)

Classes, **operator overloading**, **STL**, **polymorphism**, and a few **systems-style** bits (binary I/O, linking toy model). Each folder is one `main.cpp` hand-in (local tests under `#ifndef __PROGTEST__`).

## Assignments (structures and complexity)

| # | Folder | Main DS / algorithms | Typical complexity (this repo) |
|---|--------|----------------------|--------------------------------|
| 01 | [01-tax-register](01-tax-register/) | Sorted `vector`, `upper_bound`, linear scan by account | insert/remove **`O(n)`** shift; find by name **`O(log n)`** + **`O(n)`** insert; income by account **`O(n)`** |
| 02 | [02-polynomial-class](02-polynomial-class/) | Dense coeff vector, naive multiply | `*`: **`O(deg(a)*deg(b))`**; eval **`O(deg)`**; `==` **`O(deg)`** |
| 03 | [03-population-register-deep-copy](03-population-register-deep-copy/) | Manual strings, deep copy, per-person history arrays | copy register **`O(total data)`**; `add` amortized **`O(1)`** resize; `resettle` may shift **`O(k)`** per person |
| 04 | [04-mail-log-parser](04-mail-log-parser/) | `map` by mail id, `multimap` by timestamp | parse **`O(lines)`**; `listMail` **`O(log M + k)`** range on `k` hits |
| 05 | [05-binary-object-linker](05-binary-object-linker/) | Symbol map, BFS/queue closure over imports | **`O(total input bytes + output bytes)`** |
| 06 | [06-polymorphic-table](06-polymorphic-table/) | `shared_ptr`, RTTI equality, layout pass | render **`O(rows*cols*h)`** on cell heights/widths |
| 07 | [07-network-template-path-cost](07-network-template-path-cost/) | Adjacency list, BFS with path sum | **`O(V+E)`** expansion; node lookup **`O(V)`** linear scan by `T_` |

Constraints: task **01** forbids `std::map`. Task **03** forbids STL string/containers in the submission (custom `CString` + raw arrays here).

## Stack

C++17/20 features as used (`<=>`, structured bindings, `optional`, `span` in local tests for 02, etc.), RTTI where needed (task 06), `std::` containers per task rules.
