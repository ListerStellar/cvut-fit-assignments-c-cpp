# Mail log (`CMailLog`)

**PA2 C++.** STL: `map`, `multimap`, `set`, `list`, `optional`, iostreams. Your code lives in a namespace (here `MysteriousNamespace` in the template); `CTimeStamp` and `CMail` are provided locally under `#ifndef __PROGTEST__`.

`parseLog` reads lines, parses month/day/year time, mail id, and message. Keeps `from=`, `subject=`, `to=`; skips other lines. Each `to=` counts as one delivered mail (possibly multiple `to=` per id). Late `to=` lines are allowed by the spec. After a pass, builds `CMail` objects and stores them in `std::multimap<CTimeStamp, CMail, CompareTime>` keyed by delivery time.

`listMail(from, to)` returns mails in that inclusive time window in ascending time order. `activeUsers` returns distinct from/to addresses in the window.

`CMailLog` and helpers sit inside the student namespace; closing brace before local `main` matches the harness layout.
