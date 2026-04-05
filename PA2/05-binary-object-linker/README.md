# Binary object linker (`CLinker`)

**PA2 C++.** Reads simplified object files: export count, import count, code size, export table (name, offset in code), import table (name, list of patch offsets), then raw code bytes. Builds a global map of exported symbols across all added files; duplicate export names throw `std::runtime_error`.

`linkOutput(out, entryPoint)` starts from the entry symbol, follows imports reachable from each copied function body, appends each function's byte range once (no padding between functions). Patches 32-bit little-endian addresses with `memcpy` (unaligned-safe). Missing symbol throws.

`addFile` / `linkOutput` throw `std::runtime_error` on I/O or link errors. Copy ctor and assignment are `delete`.

Uses `std::vector`, `std::map`, `std::set`, streams, `uint8_t` / `uint32_t`.
