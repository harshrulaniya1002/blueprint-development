#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <algorithm>
using namespace std;

// ============================================================
// ENUMS
// ============================================================
enum class SeatType { SILVER, GOLD, PLATINUM };
enum class BookingStatus { CONFIRMED, CANCELLED };

inline string seatTypeToString(SeatType t) {
    switch (t) {
        case SeatType::SILVER:   return "SILVER";
        case SeatType::GOLD:     return "GOLD";
        case SeatType::PLATINUM: return "PLATINUM";
        default: return "UNKNOWN";
    }
}

// ============================================================
// SEAT
// No mutex: seat state is protected by Show-level lock.
// ============================================================
class Seat {
    string seatId;
    SeatType type;
    bool booked;

public:
    Seat(const string& id, SeatType type)
        : seatId(id), type(type), booked(false) {}

    string getId() const { return seatId; }
    SeatType getType() const { return type; }
    bool isBooked() const { return booked; }
    void book() { booked = true; }
    void cancel() { booked = false; }
};

// ============================================================
// MOVIE
// Immutable after creation — no mutex needed.
// ============================================================
class Movie {
    string movieId;
    string title;
    string genre;
    int durationMins;

public:
    Movie(const string& id, const string& title,
          const string& genre, int duration)
        : movieId(id), title(title), genre(genre), durationMins(duration) {}

    string getId() const { return movieId; }
    string getTitle() const { return title; }
    string getGenre() const { return genre; }
    int getDuration() const { return durationMins; }
};

// ============================================================
// SHOW
// Has its own mutex for per-show seat locking.
// PESSIMISTIC: lock show before checking/modifying seats.
// Multiple shows can be booked in parallel (different locks).
// ============================================================
class Show {
    string showId;
    string movieId;
    string theatreId;
    int startTime;  // e.g., 14 = 2PM
    vector<Seat> seats;
    mutable mutex showMtx;

public:
    Show(const string& id, const string& movieId, const string& theatreId,
         int startTime, const vector<Seat>& seats)
        : showId(id), movieId(movieId), theatreId(theatreId),
          startTime(startTime), seats(seats) {}

    string getId() const { return showId; }
    string getMovieId() const { return movieId; }
    string getTheatreId() const { return theatreId; }
    int getStartTime() const { return startTime; }

    // Get available seats (thread-safe)
    vector<string> getAvailableSeats() {
        lock_guard<mutex> lock(showMtx);
        vector<string> avail;
        for (const auto& s : seats) {
            if (!s.isBooked()) avail.push_back(s.getId());
        }
        return avail;
    }

    // Get available seats by type
    vector<string> getAvailableSeatsByType(SeatType type) {
        lock_guard<mutex> lock(showMtx);
        vector<string> avail;
        for (const auto& s : seats) {
            if (!s.isBooked() && s.getType() == type)
                avail.push_back(s.getId());
        }
        return avail;
    }

    // Atomic check-and-book multiple seats
    // Returns true only if ALL requested seats are available
    bool tryBookSeats(const vector<string>& seatIds) {
        lock_guard<mutex> lock(showMtx);
        // Check all first
        for (const auto& sid : seatIds) {
            bool found = false;
            for (auto& s : seats) {
                if (s.getId() == sid) {
                    if (s.isBooked()) return false; // conflict
                    found = true;
                    break;
                }
            }
            if (!found) return false; // seat doesn't exist
        }
        // Book all
        for (const auto& sid : seatIds) {
            for (auto& s : seats) {
                if (s.getId() == sid) { s.book(); break; }
            }
        }
        return true;
    }

    // Cancel specific seats
    void cancelSeats(const vector<string>& seatIds) {
        lock_guard<mutex> lock(showMtx);
        for (const auto& sid : seatIds) {
            for (auto& s : seats) {
                if (s.getId() == sid) { s.cancel(); break; }
            }
        }
    }

    // Get seat type for pricing
    SeatType getSeatType(const string& seatId) {
        lock_guard<mutex> lock(showMtx);
        for (const auto& s : seats) {
            if (s.getId() == seatId) return s.getType();
        }
        return SeatType::SILVER;
    }

    int getTotalSeats() {
        lock_guard<mutex> lock(showMtx);
        return seats.size();
    }

    int getAvailableCount() {
        lock_guard<mutex> lock(showMtx);
        int count = 0;
        for (const auto& s : seats)
            if (!s.isBooked()) count++;
        return count;
    }

    void display() {
        lock_guard<mutex> lock(showMtx);
        int avail = 0;
        for (const auto& s : seats)
            if (!s.isBooked()) avail++;
        cout << "  Show[" << showId << "] Movie: " << movieId
             << " Theatre: " << theatreId << " Time: " << startTime
             << ":00 Seats: " << avail << "/" << seats.size() << endl;
    }
};

// ============================================================
// THEATRE
// Immutable structure. Shows are added once during setup.
// ============================================================
class Theatre {
    string theatreId;
    string name;
    string city;

public:
    Theatre(const string& id, const string& name, const string& city)
        : theatreId(id), name(name), city(city) {}

    string getId() const { return theatreId; }
    string getName() const { return name; }
    string getCity() const { return city; }
};

// ============================================================
// BOOKING
// Immutable after creation (status changes via setStatus).
// No mutex: protected by BookingService-level lock.
// shared_ptr<Booking>: service map + caller both hold ref.
// ============================================================
class Booking {
    string bookingId;
    string userId;
    string showId;
    vector<string> seatIds;
    double totalAmount;
    BookingStatus status;

public:
    Booking(const string& id, const string& userId, const string& showId,
            const vector<string>& seats, double amount)
        : bookingId(id), userId(userId), showId(showId),
          seatIds(seats), totalAmount(amount),
          status(BookingStatus::CONFIRMED) {}

    string getId() const { return bookingId; }
    string getUserId() const { return userId; }
    string getShowId() const { return showId; }
    const vector<string>& getSeatIds() const { return seatIds; }
    double getAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }
    void setStatus(BookingStatus s) { status = s; }

    void display() const {
        cout << "  Booking[" << bookingId << "] User: " << userId
             << " Show: " << showId << " Seats: ";
        for (const auto& s : seatIds) cout << s << " ";
        cout << "| $" << totalAmount
             << " | " << (status == BookingStatus::CONFIRMED ? "CONFIRMED" : "CANCELLED")
             << endl;
    }
};

#endif
