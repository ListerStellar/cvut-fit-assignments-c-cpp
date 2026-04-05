# PA2: C++ and OOP (CTU FIT)

Classes, operators, STL where allowed, and a few "systems flavored" exercises. Each folder is one hand-in with `main.cpp` (local tests under `#ifndef __PROGTEST__`).

## Assignments

| # | Folder | Summary |
|---|--------|---------|
| 01 | [01-tax-register](01-tax-register/) | `CTaxRegister`, sorted `vector`, iterator, uniqueness rules |
| 02 | [02-polynomial-class](02-polynomial-class/) | `CPolynomial`, operators, stream output, `vector<double>` coeffs |
| 03 | [03-population-register-deep-copy](03-population-register-deep-copy/) | `CRegister`, own string class, deep copy, address history |
| 04 | [04-mail-log-parser](04-mail-log-parser/) | `CMailLog`, parse server log, `multimap`, time range queries |
| 05 | [05-binary-object-linker](05-binary-object-linker/) | `CLinker`, read `.o` blobs, resolve imports, patch LE 32-bit |
| 06 | [06-polymorphic-table](06-polymorphic-table/) | `CTable`, `CContent` hierarchy, `shared_ptr`, `operator<<` |
| 07 | [07-network-template-path-cost](07-network-template-path-cost/) | `CNet<T>` template, adjacency list, path cost (`totalCost`) |

Course forbids `std::map` in task 01 only; task 03 forbids STL strings/containers in the hand-in (this code uses a custom `CString` and raw arrays).
