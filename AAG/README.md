# AAG: automata and grammars (CTU FIT, C++)

Theory track tied to **finite automata** and **formal languages**. This repo has a single programming hand-in.

## Assignment

| # | Folder | Core ideas | Typical complexity (this repo) |
|---|--------|------------|----------------------------------|
| 01 | [01-regex-position-automaton](01-regex-position-automaton/) | **Glushkov / follow-positions** automaton from regex AST; simulate on many words | Build **`O(m)`** positions with vector merges on the tree; per word **`O(|w| * m^2)`** worst-case style bound before filtering (active set + follow lists), often far less in practice |

## Stack

- C++17: `std::variant`, `std::visit`, `std::unique_ptr` for the regex AST (types live in the course harness / `sample.h`).
- Containers: `vector`, `set`, sorting + `unique` for deduplication of position sets.
- Interface: `std::set<size_t> wordsMatch(const regexp::RegExp&, const std::vector<Word>&)` with `Word = std::vector<uint8_t>`.

## Relation to lectures

- Regex syntax is **fully formal**: binary `+` and `.`, unary `*`, plus `Empty` (∅) and `Epsilon` (ε), `Symbol`.
- The implementation follows the **nullable / firstpos / lastpos / followpos** rules (McNaughton-Yamada / **Glushkov** construction), then runs a **subset simulation** on position sets instead of building an explicit DFA table.

Local compile expects `sample.h` and `main` under `#ifndef __PROGTEST__`; Progtest strips that block.
