# Formatted I/O and input validation (quotes)

**PA1 intro C.**

## Task

Read one integer (strictly: integer token and optional bonus rejection of trailing garbage), print one of several fixed Klingon-flavored lines or a fixed error string. Grader compares stdout **exactly**.

## Algorithms / complexity

- **No algorithmic depth:** constant branching on parsed value.
- **Parsing:** `scanf` return value inspection; distinguish EOF, no conversion, out-of-range, and (bonus) extra characters after the number.
- **Time / space:** `O(1)` each.

## Stack

`stdio.h` only in the hand-in path; deterministic string literals.
