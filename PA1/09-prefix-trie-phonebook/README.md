# Prefix trie for digit strings (phonebook)

**Course:** PA1 (API hand-in). **Stack:** `malloc`/`free`, 10 children per node, `__PROGTEST__` harness.

`addPhone(book, phone, name)`: digits only; insert or replace name at that prefix. `delPhone(book, phone)`: remove that prefix and prune empty branches. `findPhone(book, phone)`: longest stored prefix along the digit path, else null. `delBook(book)`: free everything.

Grader checks return values and trie shape (no dead chains after delete). Longest-prefix match is the usual routing-table idea.

Local test code may use `constexpr`/`nullptr`; under `__PROGTEST__` the school side supplies types and skips your `main`.
