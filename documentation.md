# DA2 - C++ OOP Mini Project Documentation

## Cover Details

| Field | Value |
|---|---|
| Project Title | Movie Ticket Booking - Seat Locking System |
| Question Number | 9 (as assigned in `G2_DA1_2_QUESTIONLIST.pdf`) |
| Student Name | Pratyush Deo Singh |
| Register Number | 25BCE5101 |
| Department | B.Tech CSE (BCE), VIT Chennai |
| Course | Structured and Object Oriented Programming |
| Faculty | Dr. Dinakaran M |
| Language Standard | C++17 |
| Source File | `movie_ticket_booking.cpp` |
| Data File | `bookings.txt` |
| GitHub Repository | `https://github.com/pratyushdeosingh/movie-da2-25BCE5101` |
| Live Hosted Link | `https://pratyushdeosingh.github.io/movie-da2-25BCE5101/` |

---

## Table of Contents

1. Problem Statement  
2. Requirement Mapping (DA2 Q9 + Common Rules)  
3. Features Implemented  
4. OOP Architecture and Class Design  
5. Class Interaction and Control Flow  
6. Constructor Design and Encapsulation  
7. STL Usage and Modern C++ Practices  
8. File Handling and Persistence Design  
9. Validation and Error Handling  
10. Test Cases (Detailed)  
11. Time/Space Complexity (Academic Note)  
12. WebAssembly Hosting Notes  
13. AI Usage Declaration  
14. Conclusion  

---

## 1. Problem Statement

This project addresses the seat-locking challenge in a single-screen theatre booking workflow. In a real theatre environment, once a seat is booked by one customer, that seat must become unavailable immediately to all subsequent users; otherwise, duplicate booking conflicts occur. The system must also generate a booking reference, preserve booking history, and provide occupancy-level reporting for management.

The implemented solution is a menu-driven Object-Oriented C++ application that models:

- The movie show and its seat map.
- Individual booking records.
- Overall theatre operations including persistence and search.

The program uses three classes (`Show`, `Booking`, `TheatreSystem`) and supports:

- Displaying a 5x10 seat map.
- Booking one or more seats with strict validation.
- Preventing duplicate selections and repeat booking of locked seats.
- Generating unique Booking IDs.
- Searching bookings by Booking ID.
- Reporting occupancy percentage and available seats.
- Saving/loading data from file (`bookings.txt`) using `fstream`.

This directly satisfies DA2 Question 9 requirements and the DA2 OOP expectations.

---

## 2. Requirement Mapping (DA2 Q9 + Common Rules)

### 2.1 DA2 Question 9 Requirement Mapping

**Q9 Requirement:** Fixed seat layout: 5x10 = 50 seats  
**Implemented in code:** `constexpr int ROWS = 5; constexpr int COLS = 10;` and `std::vector<std::vector<SeatStatus>> seatMap` (5 rows x 10 columns).

**Q9 Requirement:** Display seat map (Available/Booked)  
**Implemented in code:** `Show::displaySeatMap()` displays each seat as `A` or `B` with labels and legend.

**Q9 Requirement:** Book seats by seat numbers with range + availability validation  
**Implemented in code:** `TheatreSystem::bookTickets()` + `Show::isValidSeatCode()` + `Show::isSeatAvailable()`.

**Q9 Requirement:** Generate BookingID  
**Implemented in code:** `TheatreSystem::generateBookingID()` creates sequential IDs (`BK1001`, `BK1002`, ...).

**Q9 Requirement:** Store booking with customer name, seat list, amount  
**Implemented in code:** `Booking` class stores all fields; `bookings` vector stores all confirmed bookings.

**Q9 Requirement:** Search booking by BookingID  
**Implemented in code:** `TheatreSystem::searchBooking(const std::string&) const`.

**Q9 Requirement:** Occupancy % and available seats count reports  
**Implemented in code:** `Show::getOccupancyReport()` prints total, booked, available, and occupancy percent.

**Q9 Requirement:** Prevent duplicate seat selection in same booking  
**Implemented in code:** duplicate check loop on `selectedSeats` inside `bookTickets()`.

**Q9 Requirement:** Prevent booking already booked seat  
**Implemented in code:** `Show::isSeatAvailable()` + `Show::bookSeat()` checks.

### 2.2 OOP Expectation Mapping

**Expected:** `class Show` manages seat state  
**Status:** Implemented (`Show` owns and controls `seatMap`).

**Expected:** `class Booking`  
**Status:** Implemented (stores booking identity and receipt data).

**Expected:** `class TheatreSystem` manages persistence  
**Status:** Implemented (`saveToFile()`, `loadFromFile()`, menu orchestration).

### 2.3 Common DA2 Rules Compliance

- Menu-driven flow using switch/case: **Yes**
- Multiple classes with clear OOP design: **Yes (3 classes)**
- File handling using `fstream`: **Yes**
- STL usage: **Yes (`std::vector`, `std::string`, streams)**
- Input validation: **Yes (menu, seat code, seat count, duplicates, empty name)**

---

## 3. Features Implemented

1. **Fixed Seat Layout (5x10)**  
   Exactly 50 seats arranged by row labels A-E and seat numbers 1-10.

2. **Visual Seat Map**  
   Displays full seat matrix with screen indicator and A/B legend.

3. **Multi-seat Booking in One Transaction**  
   Allows booking any positive number of seats up to 50.

4. **Seat Locking / Double-booking Prevention**  
   Once booked, seat status changes from `Available` to `Booked` and is blocked.

5. **Duplicate Selection Prevention in Same Transaction**  
   Input like `A3, A3` is rejected before booking commit.

6. **Unique Booking ID Generation**  
   Sequential IDs (`BK1001+`) preserved across restarts by persisting next counter.

7. **Customer Receipt Generation**  
   Box-formatted receipt including ID, customer, show time, seat list, count, amount.

8. **Booking Search by ID**  
   User can retrieve full receipt by entering booking ID.

9. **Occupancy Analytics**  
   Reports total seats, booked seats, available seats, occupancy percentage.

10. **Persistent Storage (`bookings.txt`)**  
    All bookings and next ID saved to disk; auto-restored on startup.

11. **Input Normalization**  
    Trims whitespace and uppercases codes/IDs for practical usability (`trim`, `toUpperStr`).

12. **Exception-safe Integer Parsing**  
    `safeStoi` prevents crashes on malformed numeric values.

13. **Future-ready cancellation support**  
    `Show::releaseSeat()` exists for future extension.

---

## 4. OOP Architecture and Class Design

### 4.1 Class: `Show`

**Purpose:** Owns movie metadata and seat map; provides all seat operations.

**Private Members**

| Member | Type | Role |
|---|---|---|
| `seatMap` | `std::vector<std::vector<SeatStatus>>` | Stores current status of each seat in a fixed 5x10 logical layout |
| `movieName` | `std::string` | Name of the movie |
| `showTime` | `std::string` | Scheduled show time |

**Private Methods**

| Method | Purpose |
|---|---|
| `parseSeatCode(const std::string&, int&, int&) const` | Converts seat code like `B10` to row/col index with full format/range checks |
| `initSeats()` | Initializes all seats as available |

**Public Methods**

| Method | Purpose |
|---|---|
| `Show()` | Default constructor (`Interstellar`, `07:00 PM`) |
| `Show(const std::string&, const std::string&)` | Parameterized constructor |
| `displaySeatMap() const` | Prints complete seat layout |
| `isSeatAvailable(const std::string&) const` | Checks if valid seat is currently available |
| `bookSeat(const std::string&)` | Marks seat as booked |
| `releaseSeat(const std::string&)` | Marks seat as available again |
| `getOccupancyReport() const` | Prints occupancy metrics |
| `isValidSeatCode(const std::string&) const` | Public validator wrapper |
| `getMovieName() const` | Read-only movie name accessor |
| `getShowTime() const` | Read-only show time accessor |

---

### 4.2 Class: `Booking`

**Purpose:** Represents one immutable booking record.

**Private Members**

| Member | Type | Role |
|---|---|---|
| `bookingID` | `std::string` | Unique reference (`BKxxxx`) |
| `customerName` | `std::string` | Customer identity |
| `bookedSeats` | `std::vector<std::string>` | Seat list in this booking |
| `totalAmount` | `int` | Total ticket amount |
| `showTime` | `std::string` | Show time snapshot for receipt |

**Public Methods**

| Method | Purpose |
|---|---|
| `Booking(...)` | Full constructor with member initializer list |
| `generateReceipt() const` | Prints detailed booking receipt |
| `display() const` | Compact booking display |
| Getter methods (`getBookingID`, etc.) | Read-only access for persistence/search |

Design note:
- No setter methods are provided, preserving booking record integrity after creation.

---

### 4.3 Class: `TheatreSystem`

**Purpose:** Main controller class that coordinates all workflows.

**Private Members**

| Member | Type | Role |
|---|---|---|
| `theShow` | `Show` | Owned show object |
| `bookings` | `std::vector<Booking>` | All confirmed bookings |
| `nextBookingID` | `int` | Counter for next generated ID |

**Private Method**

| Method | Purpose |
|---|---|
| `generateBookingID()` | Returns next booking ID and increments counter |

**Public Methods**

| Method | Purpose |
|---|---|
| `TheatreSystem()` | Initializes system and loads persisted data |
| `run()` | Main menu loop |
| `bookTickets()` | End-to-end booking pipeline |
| `searchBooking(const std::string&) const` | Booking lookup |
| `showOccupancyReport() const` | Displays occupancy report |
| `saveToFile()` | Serializes bookings + counter |
| `loadFromFile()` | Reconstructs bookings + seat state |

---

## 5. Class Interaction and Control Flow

### 5.1 Composition Graph

`TheatreSystem` contains:
- One `Show`
- Many `Booking` objects (`std::vector<Booking>`)

This is composition (has-a relationship), not inheritance.

### 5.2 Booking Workflow

1. User selects **Book Tickets**.
2. `TheatreSystem::bookTickets()` asks `Show` to display current seat map.
3. Name, seat count, and seat codes are collected and normalized.
4. `Show::isValidSeatCode()` validates format/range.
5. `Show::isSeatAvailable()` ensures lock rule compliance.
6. Duplicate check ensures a seat is not repeated in same request.
7. All valid seats are locked via `Show::bookSeat()`.
8. `generateBookingID()` creates ID.
9. New `Booking` object is created and pushed to vector.
10. Receipt printed (`Booking::generateReceipt()`).
11. Persisted to disk using `saveToFile()`.

### 5.3 Startup Workflow

1. `main()` creates `TheatreSystem`.
2. Constructor calls `loadFromFile()`.
3. Existing booking lines are parsed.
4. `bookings` vector rebuilt and each seat re-locked in `Show`.
5. `nextBookingID` restored from file first line.

---

## 6. Constructor Design and Encapsulation

### 6.1 Constructor Usage

- `Show` constructors initialize movie/time via initializer lists, then fill seat map using `initSeats()`.
- `Booking` constructor initializes all members in initializer list.
- `TheatreSystem` constructor initializes members and then loads persisted records.

Initializer-list based construction ensures:
- Clear initialization order.
- Avoidance of unnecessary default-construction then assignment.

### 6.2 Encapsulation Decisions

- All state variables are `private`.
- External code cannot modify seat matrix directly.
- Seat updates must pass through validated member functions.
- Booking records are effectively immutable after construction.
- Internal parsing details (`parseSeatCode`) are hidden from public API.

---

## 7. STL Usage and Modern C++ Practices

### 7.1 STL Components Used

| Component | Usage |
|---|---|
| `std::vector<Booking>` | Dynamic storage of bookings |
| `std::vector<std::string>` | Variable-length seat list per booking |
| `std::string` | Safe string operations and manipulation |
| `std::ifstream` / `std::ofstream` | File persistence |
| `std::istringstream` | Line-level parsing of persisted booking records |
| `std::ostringstream` | Booking ID formatting |
| `<iomanip>` | Formatted output (`setw`, `setprecision`) |

### 7.2 Modern C++ Safety/Quality Practices

- `constexpr` for compile-time constants.
- `enum class SeatStatus` for strong seat-state typing.
- `const` correctness on read-only methods.
- `const std::string&` and `const std::vector<std::string>&` returns to reduce copy overhead.
- `static_cast<unsigned char>` before `toupper`/`isdigit` to avoid undefined behavior.
- `safeStoi` utility with exception handling for robust numeric conversion.
- Input normalization (`trim` + uppercase conversion) for better UX correctness.

---

## 8. File Handling and Persistence Design

### 8.1 File Specification

| Property | Value |
|---|---|
| File name | `bookings.txt` |
| Format | Line-based, pipe-delimited text |
| Save operation | Full overwrite |
| Load operation | Full parse and reconstruction |

### 8.2 Storage Format

Line 1:
`<nextBookingID>`

Each next line:
`<bookingID>|<customerName>|<seat1,seat2,...>|<totalAmount>|<showTime>`

### 8.3 Example

```text
1003
BK1001|Pratyush|A1,A2|300|07:00 PM
BK1002|Rahul|B5,B6,B7|450|07:00 PM
```

### 8.4 Save Logic

- Open file in output mode.
- Write `nextBookingID`.
- For every booking:
  - Write ID, name, comma-joined seat list, amount, and show time.
- Close stream.

### 8.5 Load Logic

- If file missing: treat as first run (do nothing).
- Parse first line as next ID.
- Parse each booking line using `std::getline` with `|` delimiter.
- Parse seat list by comma.
- Validate and re-lock all parsed seats first.
- Construct booking object via `emplace_back` only after successful seat restoration.
- If a booking record is corrupted or has conflicting seats, skip that record safely and continue loading.

### 8.6 Why this design is reliable

- Counter persistence avoids booking ID reset.
- Rebuilding seat map from booking records ensures consistency.
- Malformed lines are skipped instead of crashing.
- Parsing is robust to whitespace due to `trim`.
- Corrupted/conflicting booking lines are skipped to avoid booking list and seat-map mismatch.

---

## 9. Validation and Error Handling

| Scenario | Handling |
|---|---|
| Non-integer menu input | `cin` fail-state clear + ignore + re-prompt |
| Empty customer name | Rejected after trimming |
| Seat count <= 0 | Rejected |
| Seat count > 50 | Rejected |
| Invalid seat format (`Z9`, `A0`, `A11`) | Rejected by `parseSeatCode` |
| Already booked seat | Booking cancelled |
| Duplicate seat within same request | Booking cancelled |
| File not found | Silent first-run behavior |
| File open failure on save | Warning printed |
| Corrupt numeric field | `safeStoi` default fallback |
| Lowercase / spaced input | Normalized before validation |

Error-handling style:
- Fail-fast validation before commit.
- No partial booking commit.
- Seats are only locked after full input validation succeeds.

---

## 10. Test Cases (Detailed)

| TC | Input / Scenario | Expected Output | Result |
|---|---|---|---|
| 01 | Book 2 seats (`A1`, `A2`) | Success receipt, total Rs.300 | PASS |
| 02 | Book already booked seat (`A1` again) | Rejected during validation with "already booked" message | PASS |
| 03 | Invalid seat code (`F1`) | Rejected as invalid range | PASS |
| 04 | Invalid format (`AA1` or `A`) | Rejected | PASS |
| 05 | Duplicate seat in same booking (`A3`, `A3`) | Rejected | PASS |
| 06 | Lowercase seat input (`b5`) | Accepted as `B5` after normalization | PASS |
| 07 | Spaced seat input (` A4 `) | Trimmed and accepted | PASS |
| 08 | Search existing BookingID (`BK1001`) | Full receipt displayed | PASS |
| 09 | Search unknown BookingID (`BK9999`) | Not found message | PASS |
| 10 | Occupancy report after 2 bookings | Correct booked/available/% values | PASS |
| 11 | Restart app after save | Bookings restored, seat locks retained | PASS |
| 12 | Non-numeric menu value (`abc`) | Error + safe continuation | PASS |
| 13 | Zero seats requested | Rejected with clear message | PASS |
| 14 | Seat count > 50 | Rejected with cap message | PASS |

---

## 11. Time/Space Complexity (Academic Note)

Let `n` = number of bookings, `k` = seats in current booking.

- Seat validation and duplicate check in booking flow: O(k^2) worst-case (duplicate scan inside loop).
- Seat availability checks and locking: O(k).
- Search booking by ID: O(n) linear scan.
- Occupancy report: O(ROWS * COLS) = O(50), effectively constant.
- Save to file: O(n + totalSeatsStored).
- Load from file: O(n + totalSeatsStored).

Given assignment scale (single screen, 50 seats), this is fully acceptable.

---

## 12. WebAssembly Hosting Notes

Generated deployment files:
- `da2_movie.html`
- `da2_movie.js`
- `da2_movie.wasm`
- `index.html` (meta refresh redirect to `da2_movie.html`)

Typical compile command:

```bash
em++ movie_ticket_booking.cpp -o da2_movie.html -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1
```

GitHub Pages:
- Repo: `movie-da2-25BCE5101`
- Source: `main` branch, root (`/`)
- Live URL: `https://pratyushdeosingh.github.io/movie-da2-25BCE5101/`

---

## 13. AI Usage Declaration

AI tools were used for:
- Drafting and refining code structure.
- Improving robustness (validation, parsing, formatting).
- Documentation expansion and quality enhancement.

Student work performed manually:
- Requirement interpretation and scope control.
- Logic verification against DA2 Q9 requirements.
- Testing of menu operations and validation paths.
- Final review and submission preparation.

I confirm that I understand the implementation and can explain each class,
function, validation rule, and persistence behavior.

---

## 14. Conclusion

This DA2 implementation is a complete OOP mini-project that models real seat
reservation constraints with strong validation and persistence. The design
separates concerns cleanly across `Show`, `Booking`, and `TheatreSystem`, making
the program easy to understand, maintain, and demonstrate. It satisfies all
mandatory requirements for DA2 Question 9, including seat locking, booking ID
generation, search, occupancy reporting, file storage, and robust input checks.
The final deployment is hosted publicly through GitHub Pages and is submission-ready.

---

## 15. Final PDF Submission Checklist (To Avoid Mark Cuts)

Before final DA2 PDF submission, ensure all required items are included:

- Cover page with:
  - Project title
  - Student name and register number
  - Course and faculty details
  - GitHub repository link
  - Live hosted link
- Problem statement
- Features implemented (minimum 6; this project includes 10+)
- Class definitions and class interaction explanation
- Constructor and encapsulation explanation
- STL usage explanation
- File handling design (`bookings.txt` format + load/save logic)
- Testing table (minimum 6 test cases; this document includes 14)
- At least 3 screenshots (seat map, successful booking receipt, occupancy/search)
- AI usage declaration
- Conclusion

Submission format reminder from instructions:
- Submit/attach only one documentation PDF as required by faculty instructions.
