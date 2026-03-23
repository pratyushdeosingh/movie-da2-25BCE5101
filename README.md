# DA2 - Movie Ticket Booking System (C++ OOP)

## Assignment Details

- **Course:** Structured and Object Oriented Programming  
- **Faculty:** Dr. Dinakaran M  
- **Student:** Pratyush Deo Singh  
- **Register No:** 25BCE5101  
- **Assignment:** DA2 (C++ OOP Mini Project)  
- **Assigned Question:** Q9 - Movie Ticket Booking - Seat Locking System

## Project Overview

This project implements a menu-driven theatre booking system with strict seat locking.
Once a seat is booked, it cannot be booked again. The system generates booking IDs,
stores booking history in a file, supports search by booking ID, and provides occupancy reports.

The implementation follows OOP design with three classes: `Show`, `Booking`, and `TheatreSystem`.

## Repository and Live Demo

- **GitHub Repository:**  
  `https://github.com/pratyushdeosingh/movie-da2-25BCE5101`
- **Live Demo (GitHub Pages):**  
  `https://pratyushdeosingh.github.io/movie-da2-25BCE5101/`

## Files in This Repository

- `movie_ticket_booking.cpp` - main C++ source code
- `da2_movie.html` - WebAssembly frontend page
- `da2_movie.js` - Emscripten-generated JS glue
- `da2_movie.wasm` - compiled WebAssembly binary
- `index.html` - redirect entry point for GitHub Pages
- `documentation.md` - detailed assignment documentation
- `bookings.txt` - runtime persistence file (created/updated during execution)

## OOP Design Summary

### `Show`
- Manages the 5x10 seat layout
- Validates seat codes
- Checks availability
- Locks/releases seats
- Generates occupancy report

### `Booking`
- Stores one booking record (ID, name, seats, amount, show time)
- Generates formatted receipt

### `TheatreSystem`
- Controls menu flow
- Orchestrates booking workflow
- Persists and restores bookings (`bookings.txt`)
- Handles booking search and reporting

## Features Implemented

- Fixed seat layout (5x10 = 50 seats)
- Seat map display (Available/Booked)
- Multi-seat booking
- Booking ID generation (`BK1001`, `BK1002`, ...)
- Seat lock enforcement (no double booking)
- Duplicate-seat prevention within same transaction
- Search booking by booking ID
- Occupancy report (booked/available/%)
- File persistence with load-on-startup
- Input normalization (`trim`, uppercase conversion)
- Robust validation and safe integer parsing

## C++ Concepts and STL Used

- Classes, encapsulation, composition
- Constructors with initializer lists
- `const` correctness
- `enum class` for seat state safety
- `constexpr` constants
- `std::vector`, `std::string`
- `std::ifstream`, `std::ofstream`
- `std::istringstream`, `std::ostringstream`
- Exception-safe parsing (`safeStoi`)

## How to Run Locally (C++)

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -o movie_ticket_booking movie_ticket_booking.cpp
./movie_ticket_booking
```

On Windows (MinGW):

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -o movie_ticket_booking.exe movie_ticket_booking.cpp
movie_ticket_booking.exe
```

## WebAssembly Build (Reference)

```bash
em++ movie_ticket_booking.cpp -o da2_movie.html -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1
```

## GitHub Pages Note

`index.html` redirects to `da2_movie.html` so the hosted root URL opens the app directly.

## Status

This project is complete, deployed, and ready for DA submission.
