# Mail log (`CMailLog`)

**PA2 C++.**

## Task

Parse MTA-style lines; correlate `mailID` with `from=`, optional `subject=`, each `to=` as separate delivered message with its timestamp. **`parseLog`** cumulative; **`listMail`** by time window; **`activeUsers`** as set of addresses involved.

## Algorithms / complexity

- **Parsing:** line scan **`O(total input length)`**; month name -> int via **`std::map`**.
- **Staging:** `std::map<std::string, SMail>` by id while scanning; each `to=` may insert into inner **`std::multimap<CTimeStamp, std::string>`** ordered by time.
- **Final index:** flatten into **`std::multimap<CTimeStamp, CMail, CompareTime>`** for **`O(log M)`** insert per mail, `M` stored messages.
- **Queries:** **`lower_bound`** on timestamp then linear scan while in range: **`O(log M + k)`** for `k` results.
- **activeUsers:** same range walk, insert into **`std::set`**: **`O((log M + k) log U)`** naive on set ops.

Student code lives in a **namespace**; `CTimeStamp` / `CMail` mocked under `#ifndef __PROGTEST__`.

## Stack

`map`, `multimap`, `set`, `list`, `optional`, `sstream`.
