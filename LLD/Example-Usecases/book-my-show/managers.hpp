#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include "strategies.hpp"
#include <unordered_map>
#include <mutex>
#include <thread>
using namespace std;

// ============================================================
// LOCKING: 2-level pessimistic
// Level 1: serviceMtx (BookingService)
//   -> movies, theatres, shows, bookings maps, observers, strategies
//   -> lock_guard for simple reads/writes
//   -> Release before show-level locking in bookSeats
//
// Level 2: showMtx (per-Show)
//   -> seats vector (booked/available state)
//   -> lock_guard inside tryBookSeats, cancelSeats, getAvailable*
//   -> Multiple shows booked in parallel (no contention)
//
// LOCK ORDER: always L1 -> L2 (never reverse)
//
// SMART POINTERS:
//   shared_ptr<Movie>       -> service map + caller
//   shared_ptr<Theatre>     -> service map + caller
//   shared_ptr<Show>        -> service map + caller + strategies use raw ptr
//   shared_ptr<Booking>     -> service map + caller
//   unique_ptr<Strategy>    -> service sole owner
//   shared_ptr<Observer>    -> service vector + caller
// ============================================================

class BookingService {
    static mutex singletonMtx;
    mutable mutex serviceMtx;

    unordered_map<string, shared_ptr<Movie>> movies;
    unordered_map<string, shared_ptr<Theatre>> theatres;
    unordered_map<string, shared_ptr<Show>> shows;
    unordered_map<string, shared_ptr<Booking>> bookings;

    unique_ptr<PricingStrategy> pricingStrategy;
    unique_ptr<SeatSelectionStrategy> seatStrategy;
    vector<shared_ptr<BookingObserver>> observers;
    int bookingCounter = 0;

    BookingService() {
        pricingStrategy = make_unique<StandardPricing>();
        seatStrategy = make_unique<FirstAvailableSelection>();
    }

    void notifyConfirmed(const Booking& b) {
        vector<shared_ptr<BookingObserver>> snap;
        { lock_guard<mutex> lock(serviceMtx); snap = observers; }
        for (auto& obs : snap) obs->onBookingConfirmed(b);
    }

    void notifyCancelled(const Booking& b) {
        vector<shared_ptr<BookingObserver>> snap;
        { lock_guard<mutex> lock(serviceMtx); snap = observers; }
        for (auto& obs : snap) obs->onBookingCancelled(b);
    }

public:
    static BookingService* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static BookingService instance;
        return &instance;
    }

    // --- Setup ---
    void setPricingStrategy(unique_ptr<PricingStrategy> s) {
        lock_guard<mutex> lock(serviceMtx);
        pricingStrategy = move(s);
        cout << "[Service] Pricing: " << pricingStrategy->getName() << endl;
    }

    void setSeatStrategy(unique_ptr<SeatSelectionStrategy> s) {
        lock_guard<mutex> lock(serviceMtx);
        seatStrategy = move(s);
    }

    void addObserver(shared_ptr<BookingObserver> obs) {
        lock_guard<mutex> lock(serviceMtx);
        observers.push_back(obs);
    }

    void addMovie(shared_ptr<Movie> movie) {
        lock_guard<mutex> lock(serviceMtx);
        movies[movie->getId()] = movie;
        cout << "[Service] Movie added: " << movie->getTitle() << endl;
    }

    void addTheatre(shared_ptr<Theatre> theatre) {
        lock_guard<mutex> lock(serviceMtx);
        theatres[theatre->getId()] = theatre;
        cout << "[Service] Theatre added: " << theatre->getName()
             << " (" << theatre->getCity() << ")" << endl;
    }

    void addShow(shared_ptr<Show> show) {
        lock_guard<mutex> lock(serviceMtx);
        shows[show->getId()] = show;
        cout << "[Service] Show added: " << show->getId()
             << " Movie: " << show->getMovieId()
             << " at " << show->getStartTime() << ":00" << endl;
    }

    // --- Search ---
    vector<shared_ptr<Show>> searchShows(const string& movieId, const string& city) {
        lock_guard<mutex> lock(serviceMtx);
        vector<shared_ptr<Show>> result;
        for (auto& [id, show] : shows) {
            if (show->getMovieId() == movieId) {
                auto it = theatres.find(show->getTheatreId());
                if (it != theatres.end() && it->second->getCity() == city) {
                    result.push_back(show);
                }
            }
        }
        return result;
    }

    // --- Core: Book Seats ---
    // 1. Lock service to generate booking ID, look up show, calculate price
    // 2. Release service lock
    // 3. Show::tryBookSeats uses per-show lock (Level 2)
    // 4. Lock service again to store booking
    shared_ptr<Booking> bookSeats(const string& userId, const string& showId,
                                   const vector<string>& seatIds) {
        shared_ptr<Show> show;
        string bookingId;
        double totalAmount = 0;

        // Step 1: Lookup + pricing under service lock
        {
            lock_guard<mutex> lock(serviceMtx);
            auto it = shows.find(showId);
            if (it == shows.end()) {
                cout << "[Thread " << this_thread::get_id() << "] "
                     << "Show not found: " << showId << endl;
                return nullptr;
            }
            show = it->second;
            bookingId = "BK_" + to_string(++bookingCounter);

            // Calculate total price
            for (const auto& sid : seatIds) {
                SeatType type = show->getSeatType(sid);
                totalAmount += pricingStrategy->calculatePrice(type);
            }
        }
        // Service lock released

        // Step 2: Try to book seats (per-show lock)
        if (!show->tryBookSeats(seatIds)) {
            cout << "[Thread " << this_thread::get_id() << "] "
                 << bookingId << " FAILED: seats unavailable in " << showId << endl;
            return nullptr;
        }

        // Step 3: Store booking
        auto booking = make_shared<Booking>(bookingId, userId, showId,
                                             seatIds, totalAmount);
        {
            lock_guard<mutex> lock(serviceMtx);
            bookings[bookingId] = booking;
        }

        cout << "[Thread " << this_thread::get_id() << "] "
             << bookingId << " confirmed: " << userId << " "
             << seatIds.size() << " seats $" << totalAmount << endl;

        notifyConfirmed(*booking);
        return booking;
    }

    // Auto-select seats by type
    shared_ptr<Booking> bookByType(const string& userId, const string& showId,
                                    int count, SeatType preferred) {
        shared_ptr<Show> show;
        vector<string> selectedSeats;

        {
            lock_guard<mutex> lock(serviceMtx);
            auto it = shows.find(showId);
            if (it == shows.end()) return nullptr;
            show = it->second;
            selectedSeats = seatStrategy->selectSeats(show.get(), count, preferred);
        }

        if ((int)selectedSeats.size() < count) {
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "Not enough seats in " << showId << endl;
            return nullptr;
        }

        return bookSeats(userId, showId, selectedSeats);
    }

    // --- Cancel Booking ---
    bool cancelBooking(const string& bookingId) {
        shared_ptr<Booking> booking;
        shared_ptr<Show> show;

        {
            lock_guard<mutex> lock(serviceMtx);
            auto it = bookings.find(bookingId);
            if (it == bookings.end()) {
                cout << "Booking not found: " << bookingId << endl;
                return false;
            }
            booking = it->second;
            auto sit = shows.find(booking->getShowId());
            if (sit != shows.end()) show = sit->second;
        }

        if (booking->getStatus() == BookingStatus::CANCELLED) {
            cout << "Already cancelled: " << bookingId << endl;
            return false;
        }

        // Release seats (per-show lock)
        if (show) show->cancelSeats(booking->getSeatIds());

        booking->setStatus(BookingStatus::CANCELLED);
        cout << "[Thread " << this_thread::get_id() << "] "
             << bookingId << " cancelled" << endl;

        notifyCancelled(*booking);
        return true;
    }

    // --- Display ---
    void displayShows() {
        lock_guard<mutex> lock(serviceMtx);
        cout << "\n=== Shows ===" << endl;
        for (auto& [id, show] : shows) show->display();
    }

    void displayBookings() {
        lock_guard<mutex> lock(serviceMtx);
        cout << "\n=== Bookings ===" << endl;
        for (auto& [id, b] : bookings) b->display();
    }

    BookingService(const BookingService&) = delete;
    BookingService& operator=(const BookingService&) = delete;
};

mutex BookingService::singletonMtx;

#endif
