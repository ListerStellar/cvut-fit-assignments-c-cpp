# Pythagorean triples on an integer interval

**PA1 C.**

## Task

In `[lo; hi]` list or count **distinct** integer right triangles (order of sides irrelevant). Commands `?` (print) and `#` (count only) until EOF.

## Algorithms / complexity

- **Number theory:** **primitive Pythagorean triples** via coprime `(m, i)` with parity constraints; **Euclid GCD**; scale by `k` so all sides land in `[lo,hi]` for **`#`** (fewer iterations than triple nested loop on the full range when `hi` is large).
- **Listing (`?`):** naive **triple loop** `a<=b<=c`, check `a^2+b^2=c^2` (grader accepts any order of lines / sides). Worst **`O((hi-lo)^3)`** in the naive radius (here bounded by problem tests).
- **Counting (`#`):** structured generation loop over `m`, inner `i`, inner `k`; roughly **`O(\sqrt{hi} \cdot \sqrt{hi} \cdot (hi / c_{min}))`** style bound, much better than pure triple loop for big intervals (course speed tests target this path).
- **Space:** `O(1)` besides input buffer.

## Stack

Loops, integer arithmetic, `scanf` mini-language.
