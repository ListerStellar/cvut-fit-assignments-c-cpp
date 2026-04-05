# HTML-like table (`CTable`, `CEmpty`, `CText`, `CImage`)

**PA2 C++.** Polymorphism is required: abstract `CContent` with `operator==`, `print(cellSize)`, `share()` (returns `std::shared_ptr<CContent>`), `getSize()`. Derived: `CEmpty`, `CText` (multiline, left/right align), `CImage` (ASCII rows, centered). `CTable` has fixed rows/cols, `getCell` / `setCell`, `operator==`, `operator<<` for bordered layout with column widths from cell content.

Implementation uses `dynamic_cast` / `typeid` in equality for runtime types. Cells hold `shared_ptr` to content; table resizes display grid to fit text and images.

STL allowed (`string`, `vector`, etc.) per assignment.
