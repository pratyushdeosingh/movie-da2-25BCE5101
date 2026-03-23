# Movie Ticket Booking System - Seat Locking

## DA2 - C++ OOP Mini Project

| Field | Details |
|---|---|
| **Project Title** | Movie Ticket Booking System - Seat Locking |
| **Student Name** | Pratyush Deo Singh |
| **Register No** | 25BCE5101 |
| **Department** | BCE |
| **Course** | Structured and Object Oriented Programming |
| **Faculty** | Dr. Dinakaran M |
| **Language** | C++17 |
| **Source File** | `movie_ticket_booking.cpp` |

---

## Table of Contents

1. [Problem Statement](#1-problem-statement)
2. [Features Implemented](#2-features-implemented)
3. [Class Definitions](#3-class-definitions)
4. [Class Interaction](#4-class-interaction)
5. [Constructor and Encapsulation](#5-constructor-and-encapsulation)
6. [STL Usage](#6-stl-usage)
7. [File Handling Design](#7-file-handling-design)
8. [OOP Concepts Application](#8-oop-concepts-application)
9. [Test Cases](#9-test-cases)
10. [Error Handling](#10-error-handling)
11. [Code Quality Improvements Applied](#11-code-quality-improvements-applied)
12. [AI Usage Declaration](#12-ai-usage-declaration)
13. [Conclusion](#13-conclusion)

---

## 1. Problem Statement

This project addresses the real-world problem of managing seat reservations in a movie theatre. In a single-screen theatre with 50 seats arranged in a 5x10 grid (rows A through E, seats 1 through 10), customers need to view available seats, select and book one or more seats, and receive a receipt with a unique booking ID. The system must enforce a seat-locking mechanism: once a seat is booked, it must not be bookable again, preventing double-booking conflicts. The system should persist all bookings to a file so that data is not lost between sessions. It must provide an occupancy report for management oversight and allow searching past bookings by their unique ID. The entire system is built using Object-Oriented Programming principles in C++ - encapsulation, constructors, STL containers, and file handling with fstream - to demonstrate practical application of OOP concepts in a menu-driven console application. Input validation ensures robustness against invalid seat codes, duplicate selections, and malformed inputs. The project uses three classes (Show, Booking, TheatreSystem) working together through composition to model the domain accurately. The code follows modern C++17 best practices including const-correctness, member initializer lists, `constexpr` compile-time constants, `enum class` type safety, and exception-safe integer parsing.

---

## 2. Features Implemented

- **Seat Map Display**: Visual 5x10 grid showing Available (A) and Booked (B) seats with row/column labels and a screen indicator at the top.
- **Multi-Seat Booking**: Book 1 or more seats in a single transaction with complete validation of every seat before confirming.
- **Seat Locking (Double-Booking Prevention)**: Once a seat is marked as Booked, it cannot be booked again by any customer. Enforced through `enum class SeatStatus` type safety.
- **Unique Booking ID Generation**: Auto-generated sequential IDs (BK1001, BK1002, ...) that persist across program restarts.
- **Formatted Receipt Printing**: Box-formatted receipt displaying booking ID, customer name, show time, seat list, count, and total amount.
- **Booking Search**: Look up any past booking by its ID and display full receipt details. Input is trimmed and case-normalized for user convenience.
- **Occupancy Report**: Real-time summary of total seats, booked seats, available seats, and occupancy percentage.
- **File Persistence (fstream)**: All bookings saved to `bookings.txt` using `std::ofstream`; data reloaded automatically on startup using `std::ifstream`.
- **Comprehensive Input Validation**: Validates seat code format, seat code range, duplicate seats within a booking, zero-seat bookings, empty customer names, and non-integer menu inputs. All string inputs are trimmed and normalized.
- **Seat Release Support**: A `releaseSeat()` method is available in the Show class for future cancellation functionality.
- **Exception-Safe Parsing**: All `std::stoi` calls are wrapped in `try-catch` via a `safeStoi()` utility to prevent crashes on malformed data.

---

## 3. Class Definitions

### 3.1 Class: Show

**Purpose**: Represents a single movie screening and manages the seat map. It is responsible for all seat-related operations - displaying the map, checking availability, booking, and releasing seats.

**Why it was created**: Separating seat management into its own class follows the Single Responsibility Principle. The Show class only concerns itself with seat state and movie information, making it independently testable and maintainable.

**Private data members**:
| Member | Type | Purpose |
|---|---|---|
| `seatMap[5][10]` | `SeatStatus[][]` | 2D array storing seat status using `enum class SeatStatus` (Available or Booked). Private to prevent direct manipulation - all modifications go through validated methods. The enum class provides compile-time type safety over raw `char` values. |
| `movieName` | `std::string` | Name of the movie being shown. Private to prevent unauthorized changes after initialization. |
| `showTime` | `std::string` | Scheduled time of the show. Private for the same reason. |

**Private methods**:
| Method | Description |
|---|---|
| `parseSeatCode(const std::string&, int&, int&) const` | Converts a seat code like "A1" or "B10" into array indices. Returns false for invalid codes. Private because it is an internal implementation detail not needed by external callers. Uses `static_cast<unsigned char>` for safe `toupper`/`isdigit` calls. |
| `initSeats()` | Helper that initializes all 50 seats to `SeatStatus::Available`. Extracted to avoid code duplication between the two constructors. |

**Public methods**:
| Method | Description |
|---|---|
| `Show()` | Default constructor using **member initializer list**. Initializes movie to "Interstellar", time to "07:00 PM", then calls `initSeats()`. |
| `Show(const std::string&, const std::string&)` | Parameterized constructor using **member initializer list**. Accepts custom movie name and time by **const reference**. |
| `displaySeatMap() const` | Renders the 5x10 seat grid to console. Marked `const` as it does not modify state. Uses `static_cast<char>` to convert `SeatStatus` to display character. |
| `isSeatAvailable(const std::string&) const` | Returns true if the seat exists and is not yet booked. Marked `const`. Parameter passed by const reference. |
| `bookSeat(const std::string&)` | Marks a seat as `SeatStatus::Booked`. Returns false if invalid or already booked. |
| `releaseSeat(const std::string&)` | Marks a seat back to `SeatStatus::Available`. Returns false if already available or invalid. |
| `getOccupancyReport() const` | Counts booked vs available seats and prints occupancy summary. Marked `const`. Uses `constexpr int total` for compile-time computation. |
| `isValidSeatCode(const std::string&) const` | Public wrapper that checks if a seat code is structurally valid. Marked `const`. |
| `getMovieName() const` | Returns movie name by **const reference**. Marked `const`. |
| `getShowTime() const` | Returns show time by **const reference**. Marked `const`. |

**Why constructor is needed**: The `seatMap` is a plain C-style array of `SeatStatus` values. Without a constructor, it would contain uninitialized garbage values. The constructor guarantees every seat starts as `SeatStatus::Available`. Member initializer lists are used for `movieName` and `showTime` to initialize them directly rather than default-constructing then assigning.

---

### 3.2 Class: Booking

**Purpose**: Stores a single confirmed booking as an immutable data record containing the booking ID, customer name, list of booked seats, total amount, and show time.

**Why it was created**: Separating booking data into its own class encapsulates all information about a reservation in one place. It can generate its own receipt and summary without depending on other classes.

**Private data members**:
| Member | Type | Purpose |
|---|---|---|
| `bookingID` | `std::string` | Unique identifier (e.g., "BK1001"). Private so it cannot be changed after creation. |
| `customerName` | `std::string` | Name of the customer. Private for data protection. |
| `bookedSeats` | `std::vector<std::string>` | List of seat codes booked in this transaction. Private to prevent modification. |
| `totalAmount` | `int` | Total cost in Rs. (seats x 150). Private to prevent tampering. |
| `showTime` | `std::string` | The show time at the time of booking. Private for consistency. |

**Public methods**:
| Method | Description |
|---|---|
| `Booking(const std::string&, const std::string&, const std::vector<std::string>&, int, const std::string&)` | Parameterized constructor using **member initializer list**. All parameters passed by **const reference**. No default constructor exists - prevents creation of invalid/empty bookings. |
| `generateReceipt() const` | Prints a box-formatted receipt. Marked `const`. |
| `display() const` | Prints a compact one-line summary. Marked `const`. |
| `getBookingID() const` | Returns booking ID by **const reference**. Marked `const`. |
| `getCustomerName() const` | Returns customer name by **const reference**. Marked `const`. |
| `getBookedSeats() const` | Returns seat vector by **const reference** (avoids copying the entire vector). Marked `const`. |
| `getTotalAmount() const` | Returns total amount. Marked `const`. |
| `getShowTime() const` | Returns show time by **const reference**. Marked `const`. |

**Why constructor is needed**: Ensures every Booking is fully initialized at creation time. The member initializer list directly constructs each member, avoiding the overhead of default-construction followed by assignment. There are no setter methods, making the object effectively immutable after construction.

---

### 3.3 Class: TheatreSystem

**Purpose**: The controller class that owns the Show and all Bookings. It drives the menu-based user interface, coordinates the booking workflow, and handles file persistence.

**Why it was created**: Following the MVC-like pattern, this class separates the control logic (menu, I/O, workflow coordination) from the data/model classes (Show, Booking). It is the single entry point for the application.

**Private data members**:
| Member | Type | Purpose |
|---|---|---|
| `theShow` | `Show` | The single show/screen managed by this system. Private because external code should not access the Show directly. |
| `bookings` | `std::vector<Booking>` | Dynamic list of all confirmed bookings. Private to protect booking records. |
| `nextBookingID` | `int` | Counter for generating unique sequential booking IDs. Private to prevent ID conflicts. |

**Private methods**:
| Method | Description |
|---|---|
| `generateBookingID()` | Creates sequential IDs like "BK1001". Uses `std::ostringstream` (output-only stream, more precise than `std::stringstream`). |

**Public methods**:
| Method | Description |
|---|---|
| `TheatreSystem()` | Constructor using **member initializer list**. Sets `nextBookingID = 1001` and calls `loadFromFile()`. |
| `run()` | The main menu loop with switch/case dispatch. |
| `bookTickets()` | Complete booking workflow. Trims and normalizes all user input. |
| `searchBooking(const std::string&) const` | Finds matching booking by ID. Marked `const`. Parameter by const reference. |
| `showOccupancyReport() const` | Delegates to Show. Marked `const`. |
| `saveToFile()` | Writes to file. Uses const references from getters to avoid copies. |
| `loadFromFile()` | Reads from file. Uses `std::istringstream`, simplified showTime parsing, `safeStoi()`, and `emplace_back()`. |

**Why constructor is needed**: Initializes the booking counter via initializer list and immediately loads saved data, ensuring continuity across sessions.

---

## 4. Class Interaction

The three classes interact through **composition** (not inheritance):

```
TheatreSystem
  |
  |--- contains ---> Show (1 instance)
  |                    - Manages seat map via SeatStatus enum
  |                    - Validates and locks seats
  |
  |--- contains ---> vector<Booking> (0..N instances)
                       - Stores confirmed booking records
                       - Generates receipts
```

### Interaction Flow (Booking a Ticket):

1. **TheatreSystem::bookTickets()** is called from the menu.
2. TheatreSystem asks **Show::displaySeatMap()** to show current availability.
3. User enters seat codes. TheatreSystem trims and uppercases input, then asks **Show::isValidSeatCode()** and **Show::isSeatAvailable()** (both `const` methods) to validate each one.
4. If all seats are valid and available, TheatreSystem calls **Show::bookSeat()** for each seat to lock them.
5. TheatreSystem creates a new **Booking** object (via initializer list constructor) with the confirmed details.
6. The Booking is added to `std::vector<Booking> bookings` using `push_back()`.
7. **Booking::generateReceipt()** (a `const` method) prints the receipt.
8. **TheatreSystem::saveToFile()** persists the data, reading Booking fields via const-reference getters to avoid unnecessary copies.

### Key Design Points:

- **Show is independent**: It knows nothing about Bookings or TheatreSystem. It only manages seat state. This makes it reusable.
- **Booking is independent**: It is a pure data record. It knows nothing about Show or TheatreSystem. It can generate its own receipt.
- **TheatreSystem is the glue**: It is the only class that knows about both Show and Booking, coordinating their interaction.
- This is **composition**, not aggregation: When TheatreSystem is destroyed, its Show and all its Bookings are destroyed with it.

---

## 5. Constructor and Encapsulation

### Constructors

All constructors use **member initializer lists** for efficient initialization:

| Class | Constructor | Initializer List | Body |
|---|---|---|---|
| **Show** | `Show()` | `: movieName("Interstellar"), showTime("07:00 PM")` | Calls `initSeats()` to fill the C-style array |
| **Show** | `Show(const std::string&, const std::string&)` | `: movieName(name), showTime(time)` | Calls `initSeats()` |
| **Booking** | `Booking(const std::string&, ...)` | `: bookingID(id), customerName(name), bookedSeats(seats), totalAmount(amount), showTime(time)` | Empty body `{}` — all initialization done in the list |
| **TheatreSystem** | `TheatreSystem()` | `: theShow(), bookings(), nextBookingID(1001)` | Calls `loadFromFile()` |

**Why initializer lists matter**: They construct members directly with their intended values, avoiding the cost of default-construction followed by assignment. For `std::string` and `std::vector`, this avoids an unnecessary empty-state creation step. For the Booking class, the constructor body is completely empty — every member is initialized in the list.

### Encapsulation

All data members across all three classes are declared `private`. Access is controlled through `public` methods:

- **Seat map protection**: You cannot write `show.seatMap[0][0] = SeatStatus::Booked` directly. You must call `show.bookSeat("A1")`, which validates the seat code before modifying state. The `SeatStatus` enum class adds an additional layer — you cannot accidentally assign a random `char` to a seat.
- **Booking immutability**: Booking has no setter methods. Once created through the constructor, the data cannot be changed. Getters return by `const` reference, so callers receive read-only access without copying the data.
- **System integrity**: The `nextBookingID` counter cannot be manipulated externally. The `bookings` vector cannot be directly modified. All operations go through validated methods.
- **Access restriction**: `parseSeatCode()` was moved from `public` to `private` since it is an internal implementation detail — external callers use `isValidSeatCode()` and `isSeatAvailable()` instead.

---

## 6. STL Usage

### Containers Used

| Container | Location | Purpose |
|---|---|---|
| `std::vector<Booking>` | `TheatreSystem::bookings` | Stores all confirmed bookings. Grows dynamically with `push_back()` and `emplace_back()`. |
| `std::vector<std::string>` | `Booking::bookedSeats` | Stores the list of seat codes per booking (variable length: 1 to 50). |
| `std::vector<std::string>` | `TheatreSystem::bookTickets()` | Temporarily holds selected seats during validation. |
| `std::string` | Throughout all classes | Used for all text data. Passed by `const std::string&` where possible to avoid copies. |
| `std::ostringstream` | `generateBookingID()` | Output-only string stream for building formatted booking IDs. |
| `std::istringstream` | `loadFromFile()` | Input-only string stream for parsing pipe-delimited file lines. |

### Why vector Was Chosen Over Arrays

1. **Dynamic sizing**: The number of bookings is unknown at compile time. `std::vector<Booking>` resizes automatically via `push_back()` and `emplace_back()`.
2. **Variable seat counts**: Each booking may have 1 to 50 seats. `std::vector<std::string>` handles this naturally.
3. **Automatic memory management**: Vectors handle allocation and deallocation internally. No manual `new`/`delete`.
4. **Built-in operations**: `push_back()`, `emplace_back()`, `size()`, iterators, and range-based for loops.
5. **Safety**: Vectors track their own size, preventing buffer overflows.

### Why string Was Chosen Over char Arrays

1. **Safe operations**: `std::string` supports `+`, `==`, `!=`, `substr()`, `find_first_not_of()` naturally.
2. **Dynamic length handling**: Customer names and seat codes have variable lengths.
3. **Integration with STL**: Works seamlessly with `std::vector`, `std::istringstream`, `std::getline()`, and file I/O.
4. **Const reference passing**: `const std::string&` parameters avoid copies entirely.

---

## 7. File Handling Design

### File Specification

| Property | Value |
|---|---|
| **File name** | `bookings.txt` (set via `constexpr const char* DATA_FILE`) |
| **I/O classes used** | `std::ofstream` (writing), `std::ifstream` (reading) |
| **Format** | Pipe-delimited (`|`) text, one booking per line |
| **Write mode** | Overwrite (not append) |
| **When saved** | After every booking and on program exit |
| **When loaded** | Automatically in TheatreSystem constructor |

### File Format

```
<nextBookingID>
<bookingID>|<customerName>|<seat1,seat2,...>|<totalAmount>|<showTime>
```

**Line 1**: The `nextBookingID` counter (integer) so ID generation resumes correctly.

**Subsequent lines**: One booking per line with 5 pipe-delimited fields.

### Example File Content

```
1003
BK1001|Pratyush|A1,A2,A3|450|07:00 PM
BK1002|Rahul|B5,B6|300|07:00 PM
```

### Design Decisions

- **Overwrite vs Append**: The entire file is rewritten on each save. This ensures the file always reflects the complete, consistent system state.
- **Pipe delimiter (`|`)**: Chosen because customer names may contain spaces or commas.
- **Comma separator for seats**: Seat codes never contain commas, so commas safely separate seats.
- **Last-field parsing**: The `showTime` field (which may contain spaces like "07:00 PM") is read with `std::getline(ss, showTimeStr)` without a delimiter, consuming the rest of the line cleanly. This replaced the original fragile multi-fallback parsing logic.
- **Seat state restoration**: On file load, each seat is re-marked via `bookSeat()`, which internally validates and prevents double-booking even on corrupted files.

### Error Handling in File I/O

- If `bookings.txt` does not exist on startup (first run), `loadFromFile()` returns silently.
- If the file cannot be opened for writing, a warning is printed but the program continues.
- All `std::stoi` calls are wrapped in `safeStoi()` with `try-catch` to prevent crashes on malformed data.
- All parsed strings are `trim()`-ed to handle trailing whitespace or newline differences across OS.
- Empty lines in the file are skipped during parsing.
- Malformed lines (missing delimiters) are skipped via `continue`.

---

## 8. OOP Concepts Application

### 8.1 Encapsulation

All data members in all three classes are `private`. Interactions with the data go through `public` methods that enforce rules:

- `Show::bookSeat()` validates the seat code format *and* checks `SeatStatus` before modifying the seat map.
- `Booking` has no setters - data is set once through the initializer-list constructor and is read-only afterward. Getters return `const` references.
- `TheatreSystem::bookTickets()` is a controlled workflow that validates every input before creating a Booking.

**Benefit**: Prevents invalid states. It is impossible to book an already-booked seat or create a Booking with missing data.

### 8.2 Data Hiding

The internal implementation of each class is hidden from the others:

- TheatreSystem calls `theShow.bookSeat("A1")` without knowing that seats are stored as `SeatStatus seatMap[5][10]`. If the internal storage changed to `std::vector<std::vector<SeatStatus>>`, only Show's code would change.
- `parseSeatCode()` is a private method — external code cannot access the raw array indexing logic.
- TheatreSystem calls `booking.generateReceipt()` without knowing how the receipt is formatted.

**Benefit**: Classes can be modified independently. Internal changes do not break the rest of the system.

### 8.3 Abstraction

Each class presents a simplified interface that hides complexity:

- **Show** abstracts seat management: callers say "book seat A1" without dealing with array indexing, `SeatStatus` comparisons, or boundary checks.
- **Booking** abstracts receipt generation: callers say "generate receipt" without knowing the formatting logic.
- **TheatreSystem** abstracts the entire system: `main()` simply calls `system.run()` and the entire application works.

**Benefit**: Complexity is managed in layers. Each layer only exposes what the next layer needs.

### 8.4 Modularity

Each class has a **single, clear responsibility**:

| Class | Responsibility |
|---|---|
| Show | Seat map management and movie information |
| Booking | Booking data storage and receipt generation |
| TheatreSystem | Application control, user interaction, file I/O |

**Benefit**: Classes can be understood, tested, and maintained independently.

### 8.5 Composition

TheatreSystem *contains* a Show and a vector of Bookings. This models the real-world relationship accurately:
- A theatre system *has* a show (composition - the show does not exist without the theatre system).
- A theatre system *has* a collection of bookings (composition - bookings are owned by the system).

**Benefit**: Natural modeling of real-world relationships. Lifetime management is automatic — when TheatreSystem is destroyed, all contained objects are destroyed with it.

---

## 9. Test Cases

| TC No | Input | Expected Output | Actual Output | Result |
|---|---|---|---|---|
| **TC01** - Normal Booking | Menu: 2, Name: "Pratyush", Seats: 2, A1, A2 | Booking confirmed, receipt with BK1001, seats A1 A2, Rs.300 | Receipt shows BK1001, A1, A2, Rs.300 | PASS |
| **TC02** - Display Seat Map | Menu: 1 (on fresh start) | 5x10 grid with all seats showing 'A' | Grid displayed correctly, all 'A' | PASS |
| **TC03** - Double Booking | Book A1 first, then try to book A1 again | "Seat A1 is already booked. Booking cancelled." | Error message displayed, booking cancelled | PASS |
| **TC04** - Invalid Seat Code | Menu: 2, Name: "Test", Seats: 1, Code: "Z9" | "'Z9' is not a valid seat code. Booking cancelled." | Error message displayed, rejected | PASS |
| **TC05** - Zero Seats | Menu: 2, Name: "Test", Seats: 0 | "Must book at least 1 seat." | Error displayed, returns to menu | PASS |
| **TC06** - Search Existing Booking | Menu: 3, ID: "BK1001" (after TC01) | Full receipt for BK1001 displayed | Receipt shown correctly | PASS |
| **TC07** - Search Non-existent Booking | Menu: 3, ID: "BK9999" | "No booking found with ID: BK9999" | Message displayed correctly | PASS |
| **TC08** - Occupancy Report | Menu: 4 (after TC01 with 2 seats booked) | 2 booked, 48 available, 4.0% | Report shows 2/50, 4.0% | PASS |
| **TC09** - File Persistence | Exit (5), restart program, Menu: 1 | Seats A1, A2 displayed as 'B', others as 'A' | Seat map restored from file | PASS |
| **TC10** - Non-integer Menu Input | Enter "abc" at menu prompt | "Invalid input. Please enter a number (1-5)." | Error handled, menu re-displayed | PASS |
| **TC11** - Boundary: Seat E10 | Menu: 2, Name: "Test", Seats: 1, Code: "E10" | Booking confirmed for E10 | Receipt with E10, Rs.150 | PASS |
| **TC12** - Duplicate Seat in Same Booking | Menu: 2, Name: "Test", Seats: 2, Codes: A3, A3 | "Seat A3 already selected. Booking cancelled." | Error displayed, cancelled | PASS |
| **TC13** - Lowercase Seat Input | Menu: 2, Name: "Test", Seats: 1, Code: "b5" | Normalized to "B5", booking confirmed | Seat B5 booked successfully | PASS |
| **TC14** - Whitespace in Seat Input | Menu: 2, Name: "Test", Seats: 1, Code: " A3 " | Trimmed to "A3", booking confirmed | Seat A3 booked successfully | PASS |

---

## 10. Error Handling

| Error Scenario | How It Is Handled |
|---|---|
| **Non-integer menu input** | `std::cin.fail()` detected, stream cleared with `std::cin.clear()` and `std::cin.ignore()`, user re-prompted |
| **Invalid seat code format** | `parseSeatCode()` rejects codes that are too short, too long, have invalid row letters, or non-numeric column parts. Uses `static_cast<unsigned char>` for safe `isdigit()` calls |
| **Seat code out of range** | Codes like "A0", "A11", "F1" are rejected by range checks in `parseSeatCode()` |
| **Double booking** | `isSeatAvailable()` checks if seat status is `SeatStatus::Booked` before allowing booking |
| **Duplicate seat in same transaction** | Selected seats are checked against the current selection vector before adding |
| **Zero or negative seat count** | Rejected with "Must book at least 1 seat" message |
| **Seat count exceeding 50** | Rejected with "Cannot book more than 50 seats" message |
| **Empty customer name** | Name is `trim()`-ed first; if empty after trimming, rejected |
| **Whitespace in seat codes** | Input is `trim()`-ed and `toUpperStr()`-ed before validation |
| **File not found on startup** | `loadFromFile()` returns silently - treated as first run |
| **File write failure** | Warning printed, program continues operating with in-memory data |
| **Corrupt/malformed file data** | `safeStoi()` wraps all `std::stoi` calls in `try-catch(std::exception&)` to prevent crashes. Malformed lines are skipped via `continue`. |
| **Empty lines in file** | `trim()`-ed, then skipped if empty |

---

## 11. Code Quality Improvements Applied

This section documents the refactoring improvements applied to the codebase for academic rigor and modern C++ compliance.

### 11.1 Syntax Fix
- Removed a stray `\''` token on line 52 of the original code that would cause a compilation error.
- Code now compiles cleanly with `g++ -std=c++17 -Wall -Wextra -Wpedantic` with zero warnings.

### 11.2 Removed `using namespace std`
- Replaced the global `using namespace std;` directive with explicit `std::` prefixes throughout the entire codebase (`std::cout`, `std::cin`, `std::string`, `std::vector`, `std::getline`, `std::setw`, `std::fixed`, `std::setprecision`, etc.).
- **Why**: `using namespace std;` pollutes the global namespace and can cause name collisions. Explicit prefixes make the code unambiguous and are considered best practice in professional and academic C++.

### 11.3 Const-Correctness
- All getter methods across all three classes are now marked `const` (e.g., `const std::string& getMovieName() const`).
- All read-only methods are marked `const`: `displaySeatMap()`, `isSeatAvailable()`, `isValidSeatCode()`, `getOccupancyReport()`, `parseSeatCode()`, `generateReceipt()`, `display()`, `searchBooking()`, `showOccupancyReport()`.
- Getters now return `const std::string&` and `const std::vector<std::string>&` instead of returning by value, eliminating unnecessary copies.
- Method parameters that are not modified are passed as `const std::string&` instead of by value.
- **Why**: Const-correctness is a core C++ principle that communicates intent (this method/parameter will not be modified), enables compiler optimizations, and prevents accidental mutations.

### 11.4 Member Initializer Lists
- All constructors now use member initializer lists instead of assignment in the body:
  - `Show()` : `movieName("Interstellar"), showTime("07:00 PM")`
  - `Show(const std::string&, const std::string&)` : `movieName(name), showTime(time)`
  - `Booking(...)` : `bookingID(id), customerName(name), bookedSeats(seats), totalAmount(amount), showTime(time)` — the constructor body is completely empty.
  - `TheatreSystem()` : `theShow(), bookings(), nextBookingID(1001)`
- **Why**: Initializer lists construct members directly with their final values, avoiding the extra cost of default-construction followed by assignment. For the Booking class, this is especially efficient.

### 11.5 Type Safety: constexpr and enum class
- `const int ROWS`, `const int COLS`, `const int TICKET_PRICE` replaced with `constexpr int` — these are now true compile-time constants.
- `const string DATA_FILE` replaced with `constexpr const char* DATA_FILE` — avoids static initialization order issues.
- `constexpr int total = ROWS * COLS` used inside `getOccupancyReport()` for compile-time computation.
- **`enum class SeatStatus : char { Available = 'A', Booked = 'B' }`** replaces raw `char` values:
  - Prevents accidental assignment of invalid status values (e.g., `seatMap[0][0] = 'X'` would be a compile error).
  - `SeatStatus::Available` and `SeatStatus::Booked` are self-documenting.
  - `static_cast<char>(seatMap[i][j])` is used only for display purposes.
- **Why**: `constexpr` evaluates at compile time, and `enum class` provides scoped, strongly-typed enumerations — both are modern C++ best practices.

### 11.6 Improved Type Safety for Indexing
- `std::size_t` used for all vector indexing loops and string length comparisons, eliminating signed/unsigned comparison warnings.
- Variables initialized at declaration (e.g., `int row = 0, col = 0;` and `int choice = 0;`) to prevent undefined behavior from uninitialized reads.
- **Why**: Prevents compiler warnings and potential bugs from signed/unsigned mismatches.

### 11.7 Input Validation: Trimming and Normalization
- Added `trim()` utility function that strips leading and trailing whitespace (`" \t\r\n"`) from strings.
- Added `toUpperStr()` utility that converts an entire string to uppercase (not just the first character as before).
- Customer name is trimmed before empty-check.
- Seat codes are trimmed *and* fully uppercased before validation.
- Search booking ID input is trimmed and uppercased — `"bk1001"` matches `"BK1001"`.
- File parsing trims lines and tokens to handle OS-specific line endings.
- **Why**: Real users type " A1 " or "a1" — robust software handles this gracefully.

### 11.8 Exception Safety: safeStoi()
- All `std::stoi()` calls are replaced with `safeStoi()`, a utility function that wraps the call in `try { ... } catch (const std::exception&) { return defaultVal; }`.
- This handles both `std::invalid_argument` (non-numeric string) and `std::out_of_range` (number too large).
- Used in: `parseSeatCode()` (default: -1), `loadFromFile()` for booking counter (default: -1), and amount parsing (default: 0).
- **Why**: The original code validated strings manually before calling `std::stoi`, but this was fragile. The `try-catch` approach is definitive and handles edge cases like empty strings.

### 11.9 File Loading Robustness
- **Simplified showTime parsing**: The original had a complex multi-fallback approach with `rfind('|')` and `substr()`. Replaced with a single `std::getline(ss, showTimeStr)` (no delimiter) that reads the remainder of the stringstream cleanly.
- Used `std::istringstream` (input-only) instead of `std::stringstream` (bidirectional) for parsing — more semantically precise.
- Used `std::ostringstream` for `generateBookingID()` similarly.
- `emplace_back()` used in `loadFromFile()` to construct Booking objects directly in the vector, avoiding a copy.
- `bookSeat()` already returns `false` for already-booked seats, so file loading cannot accidentally double-book.
- **Why**: Simpler parsing is more maintainable and less prone to off-by-one errors.

### 11.10 OOP Practice Improvements
- `parseSeatCode()` moved from `public` to `private` — it is an internal implementation detail. External callers use `isValidSeatCode()` and `isSeatAvailable()`.
- Extracted `initSeats()` private helper to avoid code duplication between the two Show constructors.
- `searchBooking()` and `showOccupancyReport()` marked `const` in TheatreSystem since they do not modify object state.
- `static_cast<unsigned char>` used in `toupper()` and `isdigit()` calls to avoid undefined behavior with negative `char` values on some platforms.
- **Why**: Better access control, reduced duplication, and portable type safety.

### 11.11 Code Style
- Consistent 4-space indentation throughout.
- Pre-increment `++i` used instead of post-increment `i++` in loops (avoids creating a temporary for non-primitive iterators — good habit).
- Range-based for loops (`for (const auto& seat : selectedSeats)`) used where appropriate.
- Added `<algorithm>`, `<cctype>`, and `<stdexcept>` headers for the functions they provide.
- Redundant comments trimmed; remaining comments focus on *why* rather than *what*.

---

## 12. AI Usage Declaration

This project was developed with assistance from **Claude** (Anthropic), an AI language model. Claude was used to:

- Generate the initial C++ source code based on detailed project specifications provided by the student.
- Perform a comprehensive refactoring pass applying modern C++17 best practices (const-correctness, initializer lists, constexpr, enum class, exception safety, input trimming).
- Structure the code following OOP principles (encapsulation, composition, constructors).
- Write documentation including problem statement, class explanations, test cases, and code quality improvement notes.
- Ensure WebAssembly compatibility by avoiding platform-specific code.

The student (Pratyush Deo Singh, 25BCE5101) provided the complete project requirements, design specifications, class structure, menu options, and all functional requirements. The AI assisted in translating these detailed requirements into working code and comprehensive documentation.

---

## 13. Conclusion

Through this project, I gained practical experience with several core C++ and OOP concepts:

- **Object-Oriented Design**: Designing three cooperating classes (Show, Booking, TheatreSystem) taught me how to decompose a real-world problem into modular, responsible components. Each class has a clear purpose and well-defined boundaries, following the Single Responsibility Principle.

- **Encapsulation and Data Hiding**: Making all data members private and exposing only necessary operations through public methods demonstrated how to protect data integrity. Moving `parseSeatCode()` to `private` showed that even methods can be hidden when they are implementation details. The `enum class SeatStatus` adds compile-time enforcement of valid seat states.

- **Constructors and Initializer Lists**: Using member initializer lists to directly construct members (rather than default-construct then assign) demonstrated efficient and idiomatic C++ object initialization. The Booking constructor with an empty body shows the power of initializer lists.

- **Const-Correctness**: Marking getters and read-only methods as `const`, returning `const` references, and passing parameters by `const` reference taught me how to communicate intent through the type system and prevent accidental mutations.

- **STL Containers**: Using `std::vector<Booking>` and `std::vector<std::string>` instead of fixed arrays taught me about dynamic memory management, `push_back()`, `emplace_back()`, and why STL containers are preferred in modern C++.

- **Modern C++ Features**: Using `constexpr` for compile-time constants, `enum class` for type-safe enumerations, `std::size_t` for indexing, and `static_cast` for safe type conversions showed how modern C++ prevents bugs at compile time.

- **File Handling with fstream**: Implementing save/load with `std::ofstream`/`std::ifstream` and a pipe-delimited format demonstrated persistent storage, serialization/deserialization, and the importance of defensive parsing with `safeStoi()`.

- **Input Validation and Exception Safety**: Handling edge cases (invalid seats, double bookings, non-numeric input, whitespace, case-insensitive search) reinforced the importance of defensive programming. Wrapping `std::stoi` in `try-catch` showed practical exception handling.

- **Composition over Inheritance**: This project naturally demonstrated composition — TheatreSystem *has-a* Show and *has* Bookings — which is often more appropriate than inheritance for "has-a" relationships.

Overall, this project provided a hands-on understanding of how OOP principles and modern C++ practices translate into real, working software. The refactoring pass demonstrated that code quality is iterative — even working code can be made significantly safer, more readable, and more maintainable through disciplined application of const-correctness, type safety, and clean design.

---

## Compilation

### Desktop (g++) — with strict warnings
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -o movie_ticket_booking movie_ticket_booking.cpp
```

### WebAssembly (em++)
```bash
em++ movie_ticket_booking.cpp -o movie_ticket_booking.html -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1
```

---

*Submitted by Pratyush Deo Singh (25BCE5101) for DA2 - Structured and Object Oriented Programming*
