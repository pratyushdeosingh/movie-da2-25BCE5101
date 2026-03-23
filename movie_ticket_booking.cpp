/*
  Project Title: Movie Ticket Booking System - Seat Locking
  Student Name: Pratyush Deo Singh
  Register No: 25BCE5101
  DA2 - C++ OOP Mini Project
  Department: BCE
  Course: Structured and Object Oriented Programming
  Faculty: Dr. Dinakaran M
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <stdexcept>

// ============================================================================
// Compile-time constants for theatre configuration
// ============================================================================
constexpr int ROWS = 5;
constexpr int COLS = 10;
constexpr int TICKET_PRICE = 150;
constexpr const char* DATA_FILE = "bookings.txt";

// ============================================================================
// Type-safe seat status using enum class instead of raw char literals
// ============================================================================
enum class SeatStatus : char {
    Available = 'A',
    Booked    = 'B'
};

// ============================================================================
// Utility: trims leading and trailing whitespace from a string
// ============================================================================
std::string trim(const std::string& str) {
    std::size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    std::size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

// ============================================================================
// Utility: converts an entire string to uppercase
// ============================================================================
std::string toUpperStr(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return result;
}

// ============================================================================
// Utility: safely converts string to int with try-catch; returns defaultVal
// on any conversion failure (invalid_argument, out_of_range)
// ============================================================================
int safeStoi(const std::string& str, int defaultVal = 0) {
    try {
        return std::stoi(str);
    } catch (const std::exception&) {
        return defaultVal;
    }
}

// ============================================================================
// Class 1: Show
// Represents a single movie show with its seat map and scheduling info.
// Private members ensure seat state cannot be tampered with from outside;
// all access goes through validated public methods.
// ============================================================================
class Show {
private:
    std::vector<std::vector<SeatStatus>> seatMap;
    std::string movieName;
    std::string showTime;

    // Parses a seat code like "A1" or "B10" into row and column indices.
    // Private: only called internally by other Show methods.
    bool parseSeatCode(const std::string& code, int& row, int& col) const {
        if (code.length() < 2 || code.length() > 3) {
            return false;
        }
        char rowChar = static_cast<char>(
            std::toupper(static_cast<unsigned char>(code[0])));
        if (rowChar < 'A' || rowChar >= 'A' + ROWS) {
            return false;
        }
        std::string colStr = code.substr(1);
        for (std::size_t i = 0; i < colStr.length(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(colStr[i]))) {
                return false;
            }
        }
        int colNum = safeStoi(colStr, -1);
        if (colNum < 1 || colNum > COLS) {
            return false;
        }
        row = rowChar - 'A';
        col = colNum - 1;
        return true;
    }

    // Helper: initializes all seats to Available
    void initSeats() {
        seatMap.assign(ROWS, std::vector<SeatStatus>(COLS, SeatStatus::Available));
    }

public:
    // Default constructor using initializer list
    Show() : movieName("Interstellar"), showTime("07:00 PM") {
        initSeats();
    }

    // Parameterized constructor using initializer list with const ref params
    Show(const std::string& name, const std::string& time)
        : movieName(name), showTime(time) {
        initSeats();
    }

    // Displays the full seat grid with row/column labels (const: read-only)
    void displaySeatMap() const {
        std::cout << "\n========================================\n";
        std::cout << "  Movie : " << movieName << "\n";
        std::cout << "  Show  : " << showTime << "\n";
        std::cout << "========================================\n";
        std::cout << "        [ S C R E E N ]\n";
        std::cout << "  ";
        for (int j = 1; j <= COLS; ++j) {
            std::cout << std::setw(3) << j;
        }
        std::cout << "\n  ";
        for (int j = 0; j < COLS * 3; ++j) {
            std::cout << "-";
        }
        std::cout << "\n";
        for (int i = 0; i < ROWS; ++i) {
            char rowLabel = static_cast<char>('A' + i);
            std::cout << rowLabel << " |";
            for (int j = 0; j < COLS; ++j) {
                std::cout << std::setw(3) << static_cast<char>(seatMap[i][j]);
            }
            std::cout << "\n";
        }
        std::cout << "\n  Legend: A = Available, B = Booked\n";
        std::cout << "========================================\n";
    }

    // Checks whether a given seat code refers to an available seat (const)
    bool isSeatAvailable(const std::string& seatCode) const {
        int row = 0, col = 0;
        if (!parseSeatCode(seatCode, row, col)) {
            return false;
        }
        return (seatMap[row][col] == SeatStatus::Available);
    }

    // Marks the specified seat as Booked; returns false if invalid or already booked
    bool bookSeat(const std::string& seatCode) {
        int row = 0, col = 0;
        if (!parseSeatCode(seatCode, row, col)) {
            return false;
        }
        if (seatMap[row][col] == SeatStatus::Booked) {
            return false;
        }
        seatMap[row][col] = SeatStatus::Booked;
        return true;
    }

    // Marks the specified seat as Available again (cancellation support)
    bool releaseSeat(const std::string& seatCode) {
        int row = 0, col = 0;
        if (!parseSeatCode(seatCode, row, col)) {
            return false;
        }
        if (seatMap[row][col] == SeatStatus::Available) {
            return false;
        }
        seatMap[row][col] = SeatStatus::Available;
        return true;
    }

    // Prints occupancy summary: total, booked, available, percentage (const)
    void getOccupancyReport() const {
        constexpr int total = ROWS * COLS;
        int booked = 0;
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (seatMap[i][j] == SeatStatus::Booked) {
                    ++booked;
                }
            }
        }
        int available = total - booked;
        double occupancy = (static_cast<double>(booked) / total) * 100.0;

        std::cout << "\n========================================\n";
        std::cout << "         OCCUPANCY REPORT\n";
        std::cout << "========================================\n";
        std::cout << "  Movie       : " << movieName << "\n";
        std::cout << "  Show Time   : " << showTime << "\n";
        std::cout << "----------------------------------------\n";
        std::cout << "  Total Seats    : " << total << "\n";
        std::cout << "  Booked Seats   : " << booked << "\n";
        std::cout << "  Available Seats: " << available << "\n";
        std::cout << "  Occupancy      : " << std::fixed << std::setprecision(1)
                  << occupancy << "%\n";
        std::cout << "========================================\n";
    }

    // Returns whether a seat code string is structurally valid (const)
    bool isValidSeatCode(const std::string& seatCode) const {
        int row = 0, col = 0;
        return parseSeatCode(seatCode, row, col);
    }

    // Const getters returning by const reference (no unnecessary copies)
    const std::string& getMovieName() const { return movieName; }
    const std::string& getShowTime() const { return showTime; }
};

// ============================================================================
// Class 2: Booking
// Stores a single confirmed booking: customer name, selected seats, total cost.
// Private members protect booking data from accidental modification after
// creation. The constructor enforces that every Booking is fully initialized.
// ============================================================================
class Booking {
private:
    std::string bookingID;
    std::string customerName;
    std::vector<std::string> bookedSeats;
    int totalAmount;
    std::string showTime;

public:
    // Parameterized constructor using member initializer list.
    // All parameters passed by const reference to avoid unnecessary copies.
    Booking(const std::string& id,
            const std::string& name,
            const std::vector<std::string>& seats,
            int amount,
            const std::string& time)
        : bookingID(id),
          customerName(name),
          bookedSeats(seats),
          totalAmount(amount),
          showTime(time) {}

    // Prints a formatted receipt suitable for the customer (const)
    void generateReceipt() const {
        std::cout << "\n+======================================+\n";
        std::cout << "|        MOVIE TICKET RECEIPT          |\n";
        std::cout << "+======================================+\n";
        std::cout << "| Booking ID  : " << std::setw(22) << std::left
                  << bookingID << "|\n";
        std::cout << "| Customer    : " << std::setw(22) << std::left
                  << customerName << "|\n";
        std::cout << "| Show Time   : " << std::setw(22) << std::left
                  << showTime << "|\n";
        std::cout << "| Seats       : ";
        std::string seatStr;
        for (std::size_t i = 0; i < bookedSeats.size(); ++i) {
            if (i > 0) seatStr += ", ";
            seatStr += bookedSeats[i];
        }
        std::cout << std::setw(22) << std::left << seatStr << "|\n";
        std::cout << "| No. of Seats: " << std::setw(22) << std::left
                  << bookedSeats.size() << "|\n";
        std::cout << "| Total Amount: Rs. " << std::setw(18) << std::left
                  << totalAmount << "|\n";
        std::cout << "+======================================+\n";
    }

    // Displays a compact one-line summary of this booking (const)
    void display() const {
        std::cout << "  " << bookingID << " | " << customerName << " | ";
        for (std::size_t i = 0; i < bookedSeats.size(); ++i) {
            if (i > 0) std::cout << ",";
            std::cout << bookedSeats[i];
        }
        std::cout << " | Rs." << totalAmount << "\n";
    }

    // Const getters returning by const reference to avoid unnecessary copies
    const std::string& getBookingID() const { return bookingID; }
    const std::string& getCustomerName() const { return customerName; }
    const std::vector<std::string>& getBookedSeats() const { return bookedSeats; }
    int getTotalAmount() const { return totalAmount; }
    const std::string& getShowTime() const { return showTime; }
};

// ============================================================================
// Class 3: TheatreSystem
// The controller class that owns the Show and all Bookings.
// It drives the menu loop, coordinates booking creation, and handles
// file-based persistence. Composition is used: TheatreSystem *contains*
// a Show object and a vector of Booking objects.
// ============================================================================
class TheatreSystem {
private:
    Show theShow;
    std::vector<Booking> bookings;
    int nextBookingID;

    // Generates a booking ID string like "BK1001", "BK1002", etc.
    std::string generateBookingID() {
        std::ostringstream oss;
        oss << "BK" << nextBookingID;
        ++nextBookingID;
        return oss.str();
    }

public:
    // Constructor using initializer list; loads saved data from file
    TheatreSystem() : theShow(), bookings(), nextBookingID(1001) {
        loadFromFile();
    }

    // Main menu loop — runs until the user chooses to exit
    void run() {
        int choice = 0;
        std::cout << "\n****************************************************\n";
        std::cout << "*    MOVIE TICKET BOOKING SYSTEM - SEAT LOCKING    *\n";
        std::cout << "*    Movie: " << theShow.getMovieName()
                  << " | Show: " << theShow.getShowTime() << "    *\n";
        std::cout << "****************************************************\n";

        do {
            std::cout << "\n----------------------------------------------------\n";
            std::cout << "                   MAIN MENU\n";
            std::cout << "----------------------------------------------------\n";
            std::cout << "  1. Display Seat Map\n";
            std::cout << "  2. Book Tickets\n";
            std::cout << "  3. Search Booking by ID\n";
            std::cout << "  4. Occupancy Report\n";
            std::cout << "  5. Exit\n";
            std::cout << "----------------------------------------------------\n";
            std::cout << "  Enter your choice (1-5): ";

            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "  Invalid input. Please enter a number (1-5).\n";
                continue;
            }
            std::cin.ignore(10000, '\n');

            switch (choice) {
                case 1:
                    theShow.displaySeatMap();
                    break;
                case 2:
                    bookTickets();
                    break;
                case 3: {
                    std::string searchID;
                    std::cout << "  Enter Booking ID (e.g., BK1001): ";
                    std::getline(std::cin, searchID);
                    searchBooking(trim(toUpperStr(searchID)));
                    break;
                }
                case 4:
                    showOccupancyReport();
                    break;
                case 5:
                    saveToFile();
                    std::cout << "\n  Bookings saved. Thank you for using the system!\n";
                    std::cout << "  Goodbye!\n\n";
                    break;
                default:
                    std::cout << "  Invalid choice. Please select 1-5.\n";
            }
        } while (choice != 5);
    }

    // Handles the full ticket booking workflow
    void bookTickets() {
        std::cout << "\n========================================\n";
        std::cout << "           BOOK TICKETS\n";
        std::cout << "========================================\n";

        theShow.displaySeatMap();

        // Get and validate customer name (trimmed)
        std::string customerName;
        std::cout << "  Enter customer name: ";
        std::getline(std::cin, customerName);
        customerName = trim(customerName);
        if (customerName.empty()) {
            std::cout << "  Error: Customer name cannot be empty.\n";
            return;
        }

        // Get and validate number of seats
        int numSeats = 0;
        std::cout << "  How many seats to book? ";
        if (!(std::cin >> numSeats)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Error: Invalid number.\n";
            return;
        }
        std::cin.ignore(10000, '\n');

        if (numSeats <= 0) {
            std::cout << "  Error: Must book at least 1 seat.\n";
            return;
        }
        if (numSeats > ROWS * COLS) {
            std::cout << "  Error: Cannot book more than "
                      << ROWS * COLS << " seats.\n";
            return;
        }

        // Collect and validate each seat code
        std::vector<std::string> selectedSeats;
        for (int i = 0; i < numSeats; ++i) {
            std::string seatCode;
            std::cout << "  Enter seat " << (i + 1) << " (e.g., A1, B10): ";
            std::getline(std::cin, seatCode);

            // Trim whitespace and normalize full code to uppercase
            seatCode = trim(toUpperStr(seatCode));

            if (!theShow.isValidSeatCode(seatCode)) {
                std::cout << "  Error: '" << seatCode
                          << "' is not a valid seat code.\n";
                std::cout << "  Valid range: A1-E10. Booking cancelled.\n";
                return;
            }

            if (!theShow.isSeatAvailable(seatCode)) {
                std::cout << "  Error: Seat " << seatCode
                          << " is already booked.\n";
                std::cout << "  Booking cancelled.\n";
                return;
            }

            // Check for duplicate seat in current selection
            bool duplicate = false;
            for (std::size_t j = 0; j < selectedSeats.size(); ++j) {
                if (selectedSeats[j] == seatCode) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                std::cout << "  Error: Seat " << seatCode
                          << " already selected in this booking.\n";
                std::cout << "  Booking cancelled.\n";
                return;
            }

            selectedSeats.push_back(seatCode);
        }

        // All seats validated — lock them in the Show
        for (const auto& seat : selectedSeats) {
            theShow.bookSeat(seat);
        }

        // Create booking and store
        int totalAmount = numSeats * TICKET_PRICE;
        std::string bookingID = generateBookingID();
        Booking newBooking(bookingID, customerName, selectedSeats,
                           totalAmount, theShow.getShowTime());
        bookings.push_back(newBooking);

        std::cout << "\n  Booking successful!\n";
        newBooking.generateReceipt();

        saveToFile();
    }

    // Searches for a booking by its ID and displays it if found (const)
    void searchBooking(const std::string& bookingID) const {
        for (std::size_t i = 0; i < bookings.size(); ++i) {
            if (bookings[i].getBookingID() == bookingID) {
                std::cout << "\n  Booking found:\n";
                bookings[i].generateReceipt();
                return;
            }
        }
        std::cout << "\n  No booking found with ID: " << bookingID << "\n";
    }

    // Delegates to Show's occupancy report (const)
    void showOccupancyReport() const {
        theShow.getOccupancyReport();
    }

    // Saves all bookings to file using pipe-delimited format (overwrite mode)
    void saveToFile() {
        std::ofstream outFile(DATA_FILE);
        if (!outFile.is_open()) {
            std::cout << "  Warning: Could not open file for saving.\n";
            return;
        }

        outFile << nextBookingID << "\n";

        for (std::size_t i = 0; i < bookings.size(); ++i) {
            outFile << bookings[i].getBookingID() << "|"
                    << bookings[i].getCustomerName() << "|";

            const std::vector<std::string>& seats =
                bookings[i].getBookedSeats();
            for (std::size_t j = 0; j < seats.size(); ++j) {
                if (j > 0) outFile << ",";
                outFile << seats[j];
            }

            outFile << "|" << bookings[i].getTotalAmount()
                    << "|" << bookings[i].getShowTime() << "\n";
        }

        outFile.close();
    }

    // Loads bookings from file on startup, restoring seat state.
    // If the file does not exist (first run), this simply returns.
    void loadFromFile() {
        std::ifstream inFile(DATA_FILE);
        if (!inFile.is_open()) {
            return;
        }

        // Read and validate the booking ID counter from the first line
        std::string firstLine;
        if (!std::getline(inFile, firstLine)) {
            inFile.close();
            return;
        }
        firstLine = trim(firstLine);
        if (!firstLine.empty()) {
            int parsedID = safeStoi(firstLine, -1);
            if (parsedID >= 1001) {
                nextBookingID = parsedID;
            } else {
                std::cout << "  [WARNING] Invalid booking counter in file. Using default BK1001.\n";
            }
        }

        // Read each booking line
        std::string line;
        while (std::getline(inFile, line)) {
            line = trim(line);
            if (line.empty()) continue;

            // Parse: bookingID|customerName|seats|totalAmount|showTime
            std::istringstream ss(line);
            std::string bookingID, customerName, seatsStr, amountStr,
                        showTimeStr;

            if (!std::getline(ss, bookingID, '|')) continue;
            if (!std::getline(ss, customerName, '|')) continue;
            if (!std::getline(ss, seatsStr, '|')) continue;
            if (!std::getline(ss, amountStr, '|')) continue;
            // Last field: read rest of line without delimiter
            if (!std::getline(ss, showTimeStr)) continue;

            // Parse comma-separated seat list
            std::vector<std::string> seats;
            std::istringstream seatStream(seatsStr);
            std::string seatToken;
            while (std::getline(seatStream, seatToken, ',')) {
                seatToken = trim(seatToken);
                if (!seatToken.empty()) {
                    seats.push_back(seatToken);
                }
            }

            int amount = safeStoi(amountStr, 0);

            // Validate and restore all seats atomically so corrupted lines do not
            // produce booking/seat-map mismatches.
            bool restoreOk = true;
            std::vector<std::string> lockedSeats;
            for (const auto& seat : seats) {
                if (!theShow.isValidSeatCode(seat) || !theShow.bookSeat(seat)) {
                    restoreOk = false;
                    break;
                }
                lockedSeats.push_back(seat);
            }

            if (!restoreOk) {
                for (const auto& seat : lockedSeats) {
                    theShow.releaseSeat(seat);
                }
                std::cout << "  [WARNING] Skipping corrupted booking record: "
                          << bookingID << "\n";
                continue;
            }

            // Reconstruct the Booking object and store only after successful seat
            // restoration.
            bookings.emplace_back(bookingID, customerName, seats,
                                  amount, showTimeStr);
        }

        inFile.close();
    }
};

// ============================================================================
// Main function: creates the TheatreSystem and starts the menu loop
// ============================================================================
int main() {
    TheatreSystem system;
    system.run();
    return 0;
}
