# Parallelograms from three points (2D, `double`)

**Course:** PA1 C. **Stack:** `double`, `math.h`, `float.h` (epsilon compares).

Three points `A`, `B`, `C`. Compute the three possible fourth vertices for a parallelogram, label the shape (parallelogram / rectangle / rhombus / square), detect collinear input.

Uses midpoint idea for the fourth point. Compares lengths with a scaled `DBL_EPSILON` tolerance (~1 per mille in the checker), not raw `==`.

Parsing: `[x, y]` with flexible spaces, strict brackets and comma.
