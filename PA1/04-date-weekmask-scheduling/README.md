# Calendar + weekday bitmask scheduling

**Course:** PA1 (library functions). **Stack:** `TDATE`, bitmasks, no `time.h`.

Implement:

- `countConnections(from, to, perWorkDay, dowMask)` - sum "connections" on each day in the inclusive range; Sat/Sun use ceil(perWorkDay/2) and ceil(perWorkDay/3); invalid dates or `from > to` returns -1.
- `endDate(from, connections, perWorkDay, dowMask)` - last day covered by that many connections, or `0000-00-00` if invalid / not enough for the first day.

Gregorian leap years with the course rule (including years divisible by 4000 non-leap). Weekday from a formula (Zeller-type). Long spans: day loop for short ranges; chunking by weeks / hybrid paths in this file for speed.

Source uses `#ifdef __PROGTEST__` so local `main` is stripped when linked to the school harness.
