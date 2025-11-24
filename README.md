# Simple Library Management System (C)

A clean, modular, and production‑quality **Library Management System** written in C.  
This project demonstrates structured programming, file handling, dynamic memory management, and separation of concerns using a multi‑file architecture.

---

## Features

- Add new books to the library
- Remove existing books
- Borrow and return books with borrower name and due date
- Search books by title substring
- List all books with formatted output
- Persistent storage using a binary `books.dat` file
- Clean separation into modules:
  - `library.c` / `library.h` — core logic & persistence
  - `utils.c` / `utils.h` — input utilities
  - `main.c` — menu‑driven UI
- Build automation using `Makefile`

---

## Project Structure

```
simple-library-management/
├── Makefile
└── src
    ├── main.c
    ├── library.c
    ├── library.h
    ├── utils.c
    └── utils.h
```

---

## How to Build

### Linux / macOS / WSL

```bash
make
```

### Windows (MinGW / MSYS2)

```bash
mingw32-make
```

This produces an executable named:

```
library_management
```

---

## How to Run

```bash
./library_management
```

A `books.dat` file is created automatically to store library data persistently.

---

## Data Persistence

The project uses a binary file (`books.dat`) to store all `Book` structures.  
This ensures:

- Fast I/O  
- No parsing overhead  
- Perfect structural consistency between runs  

---

## Code Quality Highlights

- Fully modular design
- Defensive file I/O and memory checks
- Consistent naming convention and header documentation
- `utils_read_line()` and `utils_read_int()` prevent input corruption
- Avoids undefined behavior by checking all pointers and file handles
- Clear, maintainable control flow for each library operation

---

## Requirements

- C11 compatible compiler (GCC / Clang / MSVC)
- Standard C library

---

## License

This project is released under the **MIT License**.  
See the `LICENSE` file for full details.

---

## Author

**Mobin Yousefi**  
GitHub: https://github.com/mobinyousefi-cs
