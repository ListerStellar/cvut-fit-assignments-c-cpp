# Word search: 8 directions, strike, secret

**PA1 C.**

## Task

Store grid; commands `- word` (find all, strike), `# word` (count), `?` (print remaining lowercase, wrap 60). Eight axial + diagonal directions, both orientations.

## Algorithms / complexity

- **Pattern matching on grid:** for each anchor cell matching first letter, walk **8 fixed direction vectors** for the full word length (**`O(L)`** check).
- **Per query:** **`O(R * C * L * 8)`** worst case, `R,C` grid size, `L` word length; same word may be searched many times (no Aho-Corasick / suffix structures here).
- **Secret output:** single pass over grid **`O(R*C)`**.
- **Space:** dynamic 1D buffer **`O(R*C)`**.

## Stack

`malloc`/`realloc`, `ctype` for case flip and validation.
