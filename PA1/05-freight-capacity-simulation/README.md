# Freight: finish date and rental cost

**Course:** PA1 C. **Stack:** `qsort`, static array up to 100k vehicles, `long long`.

Input: list of vehicles `{ [from-to, capacity, price], ... }` then lines `start_day cargo`. Each day you add the capacity of every vehicle active that day until cargo is 0. Goal is to finish as early as possible. Cost: every active vehicle's daily price is charged for each day from start through the last shipping day (course rule).

Parse braces/brackets/commas strictly. Sort by `from` to drive the simulation. If cargo never finishes after the last vehicle ends, print the "too large load" message.

Bonuses in the original task want faster range tricks; here it is day-by-day simulation after sort.
