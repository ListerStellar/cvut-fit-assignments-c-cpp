# Rectangle area comparison

**PA1 C.**

## Task

Read two rectangles (strictly positive `double` sides), compare areas with tolerant equality, print which is smaller or “equal” per spec.

## Algorithms / complexity

- **Arithmetic:** two products, one comparison with **epsilon** (same pattern as PA1 01).
- **Validation:** fail-fast on non-positive or malformed input.
- **Time / space:** `O(1)`.

## Stack

`scanf` with `%le`, `double`, `math.h` / `float.h` for tolerance helper.
