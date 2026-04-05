# Freight: finish date and rental cost

**PA1 C.**

## Task

Parse many vehicles with active day intervals, daily capacity, daily rent. For each query `(start, cargo)`, ship as early as possible: each day sum capacities of all vehicles active that day. Cost sums rents of **all** active vehicles on every day from first shipping day through finish (course rule).

## Algorithms / complexity

- **Sorting:** **`qsort`** on vehicle `from` key: **`O(n log n)`**, `n <= 100000`.
- **Simulation:** outer loop over days until cargo done; inner loop over all vehicles to add capacity if `from <= day <= to`. Worst **`O(D * n)`** where `D` is days simulated; acceptable for mandatory tests; bonuses need **interval aggregation** (sweep line, prefix sums).
- **Space:** static array `O(n)`.

## Stack

`stdlib` (`qsort`), `long long` for cargo/cost, structured scanf parsing.
