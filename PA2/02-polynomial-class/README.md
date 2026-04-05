# Polynomial class (`CPolynomial`)

**PA2 C++.** Coefficients in `std::vector<double>`. Operators: `*`, `*=`, `==`, `!=`, `[]` (grow on write), `()` for evaluation, `bool` and `!` for zero test, `degree()`, stream `operator<<` with the assignment's formatting rules (skip zero terms, `x` and `x^n`, signs).

`poly_var` in this file is a stub manipulator: it compiles but does not change the variable name in output.

Multiply is the naive double loop over coefficients (O(deg^2)). Fast multiplication (FFT / NTT style) is for course bonuses only.

Copy control: implicit rule-of-zero is enough here (vector handles storage).
