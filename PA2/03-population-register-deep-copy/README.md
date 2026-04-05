# Population register (`CRegister`, deep copy, no STL containers)

**PA2 C++.** The task blocks `std::string` and STL containers in the submitted code. This version uses a small `CString` (raw `char*`, ctor/dtor/assign) and `CPerson` with dynamic arrays for dated address history. `add` registers a person; `resettle` appends a move (same calendar day twice is rejected); `print` writes id, name, and addresses sorted by date.

`CRegister` keeps a dynamic array of `CPerson` with manual resize. Copy ctor and `operator=` do deep copies so independent registers (tests mutate one copy after the other).

Optional course parts ask for copy-on-write and string dedup; this code is the straight deep-copy approach.
