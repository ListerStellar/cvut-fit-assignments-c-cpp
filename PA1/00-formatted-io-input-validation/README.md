# Formatted I/O and input validation (quotes)

**Course:** PA1 intro C. **Stack:** `stdio.h`, exact stdout for the grader.

Reads one integer, prints a fixed quote or a specific error line. Output must match the reference byte for byte (prompts, `\n`, messages).

Parsing: check `scanf` results. Out of range is one error string; invalid integer is another. This repo version also rejects junk after the number (e.g. `1.23`) for the stricter bonus tests.

Covers: prompts, formatted print, validation, and not "it runs locally" but "it matches the spec".
