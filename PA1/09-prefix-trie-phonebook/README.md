# Prefix trie for digit strings (phonebook)

**PA1** (API file; C/C++ hybrid in template).

## Task

**Trie** keyed by digit strings: add prefix with name, delete prefix and **compress** empty chains, **longest-prefix** lookup, free whole book.

## Algorithms / complexity

- **Trie (prefix tree):** 10 children per node. Insert / find walk: **`O(L)`** for `L` digits. Delete walks then **prunes** upward **`O(L)`** in the best case.
- **Space:** **`O(total nodes)`** proportional to stored prefix set (sparse branches).
- **Comparison:** same asymptotics as a `std::map` char-by-char but with array children and explicit shape checks in Progtest.

## Stack

`malloc`/`free`, pointer array `m_Child[10]`, `__PROGTEST__` guard.
