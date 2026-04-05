# Population register (`CRegister`, deep copy, no STL containers)

**PA2 C++.**

## Task

Track people and **full history** of addresses by date. `add`, `resettle` (no two moves same day), `print`. **Deep copy** ctor and `operator=`. Hand-in must avoid `std::string` / STL containers (this code uses `CString` and raw dynamic arrays).

## Algorithms / complexity

- **`CString`:** classic **`O(len)`** copy on C string operations.
- **`CPerson`:** dynamic parallel arrays of dates and addresses; **`resettle`** inserts in sorted date order with possible **`O(k)`** shift, **`k`** history length; resize geometric.
- **`CRegister`:** dynamic array of people; **`add`** **`O(n)`** duplicate scan by id; **`print`** **`O(k)`** for that person.
- **Copy whole register:** **`O(sum of all strings + all history entries)`** deep copy.

Optional course improvements: **copy-on-write**, **string interning** (not here).

## Stack

Raw `new[]`/`delete[]`, `strcmp`, `iostream` only for tests.
