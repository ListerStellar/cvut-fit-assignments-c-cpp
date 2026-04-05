# Parallelograms from three points (2D, `double`)

**PA1 C.**

## Task

From `A,B,C` compute three candidate fourth vertices (parallelogram completions), classify quadrilateral type (general / rectangle / rhombus / square), detect collinear triples.

## Algorithms / complexity

- **Geometry:** fourth point via **midpoint of diagonals** (vector identity), constant number of length checks.
- **Classification:** compare six side / diagonal squared lengths with **scaled epsilon** (`DBL_EPSILON` scaling) to survive `~1e-3` relative tolerance in the grader. Not graph algorithms; pure numeric robustness.
- **Degeneracy:** **collinearity** via coordinate ratios or equivalent (aligned points).
- **Time / space:** `O(1)` per test case.

## Stack

`double`, `math.h` (`fabs`, `fmax`), `float.h`.
