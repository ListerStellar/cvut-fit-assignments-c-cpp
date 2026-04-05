# Binary object linker (`CLinker`)

**PA2 C++.**

## Task

Read toy **object files** (exports, imports, code blob). Build global symbol table. From **entry** symbol, **transitively** include every function whose code is needed (imports referenced from included bodies). **Relocate** 32-bit LE pointers with **`memcpy`** (unaligned-safe). Write concatenated code.

## Algorithms / complexity

- **Parsing:** sequential read **`O(file size)`** per object; total **`O(sum of file sizes)`**.
- **Symbol map:** `std::map<std::string, SFunction>` for exports **`O(F log F)`**, `F` functions across objects.
- **Closure:** queue / BFS over dependency graph extracted from import sites inside each selected function body: **`O(edges in closure)`**.
- **Patch pass:** for each included function, for each import slot in its byte range, write 4 bytes: **`O(output size)`**.
- **Space:** holds all object code in memory **`O(input)`**.

## Stack

`fstream`, `vector<uint8_t>`, `map`, `set`, `runtime_error`.
