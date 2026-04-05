# HTML-like table (`CTable`, `CEmpty`, `CText`, `CImage`)

**PA2 C++.**

## Task

Fixed grid of polymorphic **cells**: empty, text (multiline, left/right), image (ASCII art, centered). `getCell` / `setCell`, equality of whole table, pretty `operator<<` with borders and column width from content.

## Algorithms / complexity

- **Polymorphism:** abstract **`CContent`**, concrete types, **`shared_ptr`** for value semantics / sharing.
- **Equality:** **`typeid` + `dynamic_cast`** per cell compare.
- **Layout:** compute each cell’s **intrinsic size**, take row/col maxima, then **`print`** pads each cell to unified height/width: roughly **`O(rows * cols * (h + w))`** output characters (depends on implementation loops).
- **setCell:** assigns new `shared_ptr` **`O(1)`**; deep compare walks grid **`O(rows*cols)`**.

Course requires **virtual** dispatch (not tagged union only).

## Stack

`memory` (`shared_ptr`), `vector`, `string`, `sstream`, `typeinfo`.
