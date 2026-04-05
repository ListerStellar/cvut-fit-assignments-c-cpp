# Regular expressions: position automaton (`wordsMatch`)

**AAG, C++.** Implement `std::set<size_t> wordsMatch(const regexp::RegExp&, const std::vector<Word>&)` where `Word` is `std::vector<uint8_t>`. Return indices of words in the language of the regex. AST node types (`Empty`, `Epsilon`, `Symbol`, `Alternation`, `Concatenation`, `Iteration`) and `regexp::RegExp` come from the grader / local `sample.h`; you only implement matching.

## Formalism (course definition)

- **Empty** ∅: no string.
- **Epsilon** ε: only the empty word.
- **Symbol** `a`: one character.
- **Alternation** `+`: binary union of two sub-expressions.
- **Concatenation** `.`: binary catenation (not left-associative sugar; tree shape matters as in the lectures).
- **Iteration** `*`: Kleene star of one sub-expression.

Input regex is always valid; no parse errors to handle.

## Algorithm in this file: Glushkov-style follow positions

1. **Linearize** the regex into **positions**: one index per `Symbol` leaf (`nodeSymbols[i]` stores that byte). `Empty` / `Epsilon` add no symbol position.

2. **Recursive attributes** on the AST (post-order via `std::visit`):
   - **nullable**: can the sub-expression derive ε?
   - **firstpos** / **lastpos**: sets of symbol positions that can read the first / last symbol of a string derived from the sub-expression.

3. **followpos**: for each position `p`, a list of positions that may follow `p` in any derived string. Rules:
   - **Concatenation:** every `last` of the left child gets edges to every `first` of the right child.
   - **Star:** every `last` of the inner expression gets edges to every `first` of the same inner expression.

4. **Matching a word**
   - Empty word: accept iff root **nullable**.
   - Non-empty: **initial active positions** = root **firstpos** filtered by equality with the first character.
   - For each next character: from each active position `p`, follow `followTable[p]`, keep targets whose symbol equals the next character.
   - Deduplicate active set (`sort` + `unique`) each step.
   - After the whole word, accept if any active position lies in root **lastpos** (marked `isFinal`).

This is equivalent to simulating the **Glushkov / position NFA** without materializing a full DFA. You could alternatively build **Thompson NFA** + subset construction; the assignment suggests the lecture pipeline and **preprocessing once** when many words share the same regex.

## Complexity (informal)

Let **m** be the number of `Symbol` leaves (position count), **|w|** word length, **W** number of words.

- **Preprocessing (once per `wordsMatch` call):** one DFS over the AST **`O(|AST|)`** nodes; each step may **merge sorted vectors** for first/last (`O(m log m)` worst per merge if sets grow large; total often **`O(m^2 log m)`** in pessimistic merge accounting, **`O(m)`** space for `nodeSymbols` and `followTable`).

- **Per word:** up to **m** positions in the active set; each step scans all follows from each active position (**`O(m * outdeg)`**), then filter by symbol and dedupe **`O(m log m)`**. Rough worst **`O(|w| * m^2)`** if follow lists are dense; Progtest limits keep this feasible.

- **Total:** **`O(preprocess + W * cost_per_word)`**.

## Stack

`std::variant` dispatch, `std::vector`, `std::set` for result indices, `RegexAutomatonMaker` helper class.

## Local build

`#ifndef __PROGTEST__` pulls in `sample.h` and a `main` with asserts. On Progtest that block is removed so your file links against the official headers only.
