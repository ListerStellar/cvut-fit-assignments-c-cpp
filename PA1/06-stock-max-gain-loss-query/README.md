# Stock: max profit and max loss on an index range

**Course:** PA1 C. **Stack:** `malloc`/`realloc` price array, one linear scan per query.

Stream: `+ price` appends a nonnegative price; `? l r` queries a closed index range. Max profit: best sell minus buy with buy before sell. Max loss: worst drop (output format is the assignment's Czech lines). Tie-break: smallest buy index, then smallest sell index.

Validate query indices against how many prices exist so far.

Hardest course bonus wants better than O(range) per query; this code scans the range each time, enough for the usual tests.
