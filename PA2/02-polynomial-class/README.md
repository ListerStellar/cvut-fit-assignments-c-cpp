# Polynomial class (`CPolynomial`)

**PA2 C++.**

## Task

Dense polynomial in `x`: coeff per power, `operator[]`, `*`, `*=`, `==`, stream output, evaluation `()`, `degree`, `bool` / `!`. Optional `poly_var` manipulator (stub here).

## Algorithms / complexity

- **Representation:** `std::vector<double>` as coefficient array (trim implicit trailing zeros in `==` logic).
- **Multiplication:** **naive convolution** double loop **`O(deg(a) * deg(b))`**.
- **Evaluation:** Horner would be **`O(deg)`**; this code uses direct powers **`O(deg)`** as well.
- **Compare `==`:** linear over min size with tail zero check **`O(deg)`**.
- **Space:** **`O(degree)`** coefficients.

Bonuses: **FFT / NTT**-style multiply for huge degree (not implemented).

## Stack

`vector`, `iostream`, rule-of-zero copy (vector owns heap).
