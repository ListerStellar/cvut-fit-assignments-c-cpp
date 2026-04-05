# Calendar + weekday bitmask scheduling

**PA1** (library functions, often compiled as C++ in the template).

## Task

`countConnections(from,to,perWorkDay,dowMask)` sums daily “connections” with weekend scaling (ceil halves/thirds). `endDate` inverts the budget: last day still covered. Gregorian **leap** rules including **4000** exception. No `time.h`.

## Algorithms / complexity

- **Date arithmetic:** month lengths, compare, add/sub days, optional **Julian day number** for difference of dates.
- **Weekday:** **Zeller-type** congruence (or similar) to map `TDATE` to `DOW_*` bit.
- **Bitmask scheduling:** bitwise `&` of day bit with `dowMask`.
- **Long intervals:** naive loop **`O(number of days)`** (too slow for huge spans); this file adds **week bucketing** (count one full week’s contribution, multiply) plus boundary segments **`O(weeks + boundary days)`** for `countConnections`, and a hybrid path for `endDate` when the budget is large.
- **Space:** `O(1)`.

## Stack

`TDATE`, `long long` for sums, `ceil` for weekend rules, conditional `__PROGTEST__`.
