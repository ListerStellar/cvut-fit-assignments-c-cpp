# Word search: 8 directions, strike, secret

**Course:** PA1 C. **Stack:** `malloc`/`realloc` buffer, `ctype`, `string`.

Read a rectangle of `a-z` and `.`, blank line, then commands. `- word`: find all matches in 8 directions, count, mark hits (here: flip to uppercase). `# word`: count only. `?`: print leftover lowercase letters as the "secret", 60 chars per line.

Grid stored row-major in one buffer; from each cell try 8 step vectors. Input checks: rectangular grid, valid commands, words length >= 2, lowercase only.

Bonus tier wants faster search for many queries; this is a full grid scan each time.
