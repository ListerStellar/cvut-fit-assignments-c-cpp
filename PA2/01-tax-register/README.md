# Tax register (`CTaxRegister`, `CIterator`)

**PA2 C++.** `std::vector`, `std::string`, `algorithm` (`upper_bound`, `insert`, `erase`). No `std::map`.

People are unique by `(name, address)` and by `account`. `birth` inserts in sort order (name, then address). `death` removes by name+address. `income` / `expense` have overloads for account string or name+address; lookup by account scans the vector linearly. `audit` fills account and running income/expense totals. `listByName` returns an iterator over the sorted vector (const reference to the internal list inside the iterator).

Birth/death do binary search for the slot, then vector insert/erase (shift cost). Hot path is many income/expense calls; account lookup is O(n) in this implementation.
