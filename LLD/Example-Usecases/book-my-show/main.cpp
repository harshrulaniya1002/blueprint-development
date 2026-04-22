#include <iostream>
#include <thread>
#include <vector>
#include "managers.hpp"
using namespace std;

int main() {
    auto* svc = BookingService::getInstance();

    cout << "=== Book My Show ===" << endl << endl;

    // Observers
    svc->addObserver(make_shared<EmailNotifier>());
    svc->addObserver(make_shared<SMSNotifier>());

    // ========== Setup ==========
    cout << "--- Setup ---" << endl;

    // Movies
    auto movie1 = make_shared<Movie>("M1", "Interstellar", "Sci-Fi", 169);
    auto movie2 = make_shared<Movie>("M2", "Inception", "Thriller", 148);
    svc->addMovie(movie1);
    svc->addMovie(movie2);

    // Theatres
    auto theatre1 = make_shared<Theatre>("T1", "PVR Saket", "Delhi");
    auto theatre2 = make_shared<Theatre>("T2", "INOX CP", "Delhi");
    svc->addTheatre(theatre1);
    svc->addTheatre(theatre2);

    // Shows with seats
    // Show 1: Interstellar at PVR Saket, 2PM
    vector<Seat> seats1;
    seats1.push_back(Seat("S1", SeatType::SILVER));
    seats1.push_back(Seat("S2", SeatType::SILVER));
    seats1.push_back(Seat("S3", SeatType::SILVER));
    seats1.push_back(Seat("G1", SeatType::GOLD));
    seats1.push_back(Seat("G2", SeatType::GOLD));
    seats1.push_back(Seat("P1", SeatType::PLATINUM));
    svc->addShow(make_shared<Show>("SH1", "M1", "T1", 14, seats1));

    // Show 2: Interstellar at INOX CP, 6PM
    vector<Seat> seats2;
    seats2.push_back(Seat("S1", SeatType::SILVER));
    seats2.push_back(Seat("S2", SeatType::SILVER));
    seats2.push_back(Seat("G1", SeatType::GOLD));
    seats2.push_back(Seat("G2", SeatType::GOLD));
    seats2.push_back(Seat("P1", SeatType::PLATINUM));
    seats2.push_back(Seat("P2", SeatType::PLATINUM));
    svc->addShow(make_shared<Show>("SH2", "M1", "T2", 18, seats2));

    // Show 3: Inception at PVR Saket, 8PM
    vector<Seat> seats3;
    seats3.push_back(Seat("S1", SeatType::SILVER));
    seats3.push_back(Seat("S2", SeatType::SILVER));
    seats3.push_back(Seat("G1", SeatType::GOLD));
    seats3.push_back(Seat("P1", SeatType::PLATINUM));
    svc->addShow(make_shared<Show>("SH3", "M2", "T1", 20, seats3));

    // ========== Search ==========
    cout << "\n--- Search: Interstellar in Delhi ---" << endl;
    auto results = svc->searchShows("M1", "Delhi");
    for (auto& show : results) show->display();

    // ========== Sequential Booking ==========
    cout << "\n--- Sequential Bookings (Standard Pricing) ---" << endl;
    svc->bookSeats("Alice", "SH1", {"G1", "G2"});     // 2 Gold
    svc->bookSeats("Bob", "SH1", {"P1"});              // 1 Platinum
    svc->bookSeats("Charlie", "SH1", {"G1"});          // FAIL: G1 already taken

    // ========== Auto-select by type ==========
    cout << "\n--- Auto-select 2 Silver seats ---" << endl;
    svc->bookByType("Diana", "SH1", 2, SeatType::SILVER);

    // ========== Strategy switch: Peak Hour Pricing ==========
    cout << "\n--- Switch to Peak Hour Pricing (1.5x) ---" << endl;
    svc->setPricingStrategy(make_unique<PeakHourPricing>(1.5));
    svc->bookSeats("Eve", "SH2", {"P1", "P2"});       // 2 Platinum at peak price

    // ========== Concurrent Bookings ==========
    // Two users race for the same seats on SH2
    // Per-show lock ensures only one wins
    cout << "\n--- Concurrent Bookings (2 users race for same seats) ---" << endl;
    vector<shared_ptr<Booking>> concResults(3);
    vector<thread> threads;

    auto bookFn = [&](int idx, const string& user, const string& show,
                      vector<string> seats) {
        concResults[idx] = svc->bookSeats(user, show, seats);
    };

    threads.emplace_back(bookFn, 0, "Frank", "SH2", vector<string>{"G1", "G2"});
    threads.emplace_back(bookFn, 1, "Grace", "SH2", vector<string>{"G1"});  // Conflict with Frank
    threads.emplace_back(bookFn, 2, "Hank", "SH3", vector<string>{"G1", "P1"}); // Different show, parallel

    for (auto& t : threads) t.join();

    // ========== Cancellation ==========
    cout << "\n--- Cancellation ---" << endl;
    svc->cancelBooking("BK_1");  // Cancel Alice's Gold seats
    cout << "\n--- Rebook cancelled seats ---" << endl;
    svc->bookSeats("Ivan", "SH1", {"G1", "G2"});  // Should succeed now

    // ========== Discount Pricing ==========
    cout << "\n--- Switch to Discount Pricing (20% off) ---" << endl;
    svc->setPricingStrategy(make_unique<DiscountPricing>(0.8));
    svc->bookSeats("Jane", "SH3", {"S1", "S2"});

    // ========== Display ==========
    svc->displayShows();
    svc->displayBookings();

    cout << "\n=== Done ===" << endl;
    return 0;
}
