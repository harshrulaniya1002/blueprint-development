#include <iostream>
#include <thread>
#include <vector>
#include "managers.hpp"
using namespace std;

int main() {
    cout << "=== Ride Booking Service ===" << endl << endl;

    auto* riderMgr = RiderMgr::getInstance();
    auto* driverMgr = DriverMgr::getInstance();
    auto* tripMgr = TripManager::getInstance();

    // Setup Riders
    // unique_ptr<Rider>: RiderMgr is sole owner
    riderMgr->addRider("R1", make_unique<Rider>("R1", "Alice", 5));
    riderMgr->addRider("R2", make_unique<Rider>("R2", "Bob", 3));
    riderMgr->addRider("R3", make_unique<Rider>("R3", "Charlie", 4));
    riderMgr->addRider("R4", make_unique<Rider>("R4", "Diana", 5));

    // Setup Drivers
    // unique_ptr<Driver>: DriverMgr is sole owner. Each Driver has
    // its own mutex for thread-safe availability management.
    driverMgr->addDriver(make_unique<Driver>("D1", "David", 5));
    driverMgr->addDriver(make_unique<Driver>("D2", "Emma", 4));
    driverMgr->addDriver(make_unique<Driver>("D3", "Frank", 3));

    cout << "Riders and Drivers registered." << endl;

    // Locations
    Location home(28.6139, 77.2090, "Home");
    Location airport(28.5562, 77.1000, "Airport");
    Location mall(28.5355, 77.3910, "Mall");
    Location office(28.4595, 77.0266, "Office");

    // ============================================================
    // DEMO 1: Sequential trips (shows strategy selection)
    // ============================================================
    cout << "\n--- Sequential Trips ---" << endl;
    tripMgr->createTrip("R1", home, airport);   // High rating + Airport -> Surge + HighRating
    tripMgr->createTrip("R2", mall, office);    // Low rating -> Default + LeastTime

    // ============================================================
    // DEMO 2: Concurrent trips from multiple threads
    // ----------------------------------------------------------
    // CONCURRENCY IN ACTION:
    // - TripManager uses lock_guard only for trip ID gen and storing trip
    // - Strategy execution (pricing + matching) happens WITHOUT manager lock
    // - Driver assignment uses per-driver lock (driver->tryAssign())
    //   so two threads targeting DIFFERENT drivers proceed in parallel
    //   but two threads targeting the SAME driver: one wins, one retries
    // ============================================================
    cout << "\n--- Concurrent Trips (3 threads, 1 driver left) ---" << endl;
    cout << "Only 1 driver available. First thread wins, others get cancelled." << endl << endl;

    // shared_ptr<Trip>: returned from createTrip. Trip is stored in
    // TripManager's vector AND here. Both owners -> shared_ptr.
    vector<shared_ptr<Trip>> results(3);

    vector<thread> threads;
    auto bookFn = [&](int idx, const string& riderId,
                      const Location& src, const Location& dest) {
        results[idx] = tripMgr->createTrip(riderId, src, dest);
    };

    threads.emplace_back(bookFn, 0, "R3", home, mall);
    threads.emplace_back(bookFn, 1, "R4", office, airport);
    threads.emplace_back(bookFn, 2, "R1", mall, airport);

    for (auto& t : threads) t.join();

    // ============================================================
    // RESULTS
    // ============================================================
    tripMgr->displayAllTrips();

    cout << "\nTotal trips: " << tripMgr->getTripCount() << endl;

    // ============================================================
    // CONCURRENCY SUMMARY:
    // ----------------------------------------------------------
    // Manager level:  lock_guard for simple ops (addRider, addDriver)
    //                 lock_guard for trip ID generation + trip storage
    //                 NO lock during strategy execution (pricing/matching)
    //
    // Driver level:   lock_guard inside tryAssign() and release()
    //                 Atomic check-and-set prevents double-assignment
    //
    // WHY PESSIMISTIC?
    //   Drivers are scarce resources with high contention.
    //   Optimistic (version-based CAS) would cause retry storms
    //   when many riders compete for few drivers.
    //
    // SMART POINTERS:
    //   unique_ptr<Rider/Driver> -> managers are sole owners
    //   shared_ptr<Trip>         -> manager + caller both hold refs
    //   unique_ptr<Strategy>     -> created per-trip, sole ownership
    //   raw ptr (Driver*/Rider*) -> non-owning refs in Trip
    // ============================================================

    cout << "\n=== Service Ended ===" << endl;
    return 0;
}