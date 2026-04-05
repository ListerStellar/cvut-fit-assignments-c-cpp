# Tax register (`CTaxRegister`, `CIterator`)

**PA2 C++.**

## Task

Database of people: unique `(name,addr)` and unique `account`. `birth`/`death`, `income`/`expense` by name or account, `audit`, sorted iteration via `CIterator`. **`std::map` forbidden**; use sorted sequential container + binary search.

## Algorithms / complexity

- **Ordering:** vector kept sorted by `(name, address)` using **`std::upper_bound`** + **`vector::insert`/`erase`**.
- **birth / death:** locate slot **`O(log n)`** binary search, then **`O(n)`** array shift in worst case.
- **income / expense by account:** **linear scan** **`O(n)`** over the vector (hot path in the assignment; map would be `O(log n)` but disallowed).
- **income / expense / audit by name+addr:** **`O(log n)`** search + `O(1)` update at iterator.
- **listByName:** iterator walks **`O(n)`** total.
- **Space:** **`O(n)`** records.

## Stack

`std::vector`, `std::string`, `<algorithm>`.
