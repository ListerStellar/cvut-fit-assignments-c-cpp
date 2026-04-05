# Pythagorean triples on an integer interval

**Course:** PA1 C. **Stack:** loops, GCD (Euclid), stdin until EOF.

Closed interval `[lo; hi]`, positive integers. Count/list right triangles with integer sides, all three sides in range. Permutations of the same triple count once.

Commands: `? <lo;hi>` prints triples; `# <lo;hi>` prints only the count. Strict syntax on `<`, `;`, `>`.

Implementation: Euclid-style generation + scaling for `#`; nested loops for `?` (order of lines can vary; grader normalizes). Faster bonuses in the course are optional; this split matches the usual mandatory layout.
