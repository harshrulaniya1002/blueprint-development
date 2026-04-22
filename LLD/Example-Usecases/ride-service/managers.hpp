#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include "strategies.hpp"
#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>
#include <thread>
using namespace std;

// ============================================================
// LOCKING STRATEGY: PESSIMISTIC (local locks, single server)
// ----------------------------------------------------------
// Level 1: Manager-level mutex (operationMtx)
//   -> protects riders/drivers collections, trips vector
// Level 2: Driver-level mutex (driverMtx inside Driver)
//   -> protects individual driver availability
//
// WHY 2 LEVELS?
//   When booking a trip, we only need the manager lock briefly
//   to look up rider/drivers. Then we release it and use the
//   per-driver lock for assignment. This way multiple trips
//   can be processed in parallel as long as they target
//   different drivers.
//
// LOCK CHOICES:
//   lock_guard  -> addRider, addDriver, getRider (simple scope)
//   unique_lock -> createTrip (release manager lock before
//                  strategy execution, re-acquire to store trip)
//
// SMART POINTERS:
//   unique_ptr<Driver>  -> DriverMgr is sole owner of drivers
//   unique_ptr<Rider>   -> RiderMgr is sole owner of riders
//   shared_ptr<Trip>    -> stored in vector AND returned to caller
//   raw ptr (Driver*)   -> non-owning ref in Trip and strategies
//   unique_ptr<Strategy>-> sole ownership, created per-trip
// ============================================================

class RiderMgr {
    static mutex singletonMtx;
    mutable mutex operationMtx;
    unordered_map<string, unique_ptr<Rider>> riders;

    RiderMgr() = default;

public:
    static RiderMgr* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static RiderMgr instance;
        return &instance;
    }

    void addRider(const string& id, unique_ptr<Rider> rider) {
        lock_guard<mutex> lock(operationMtx);
        riders[id] = move(rider);
    }

    // Returns non-owning raw ptr (caller does NOT own the rider)
    Rider* getRider(const string& id) {
        lock_guard<mutex> lock(operationMtx);
        auto it = riders.find(id);
        return (it != riders.end()) ? it->second.get() : nullptr;
    }

    RiderMgr(const RiderMgr&) = delete;
    RiderMgr& operator=(const RiderMgr&) = delete;
};

class DriverMgr {
    static mutex singletonMtx;
    mutable mutex operationMtx;
    // unique_ptr<Driver>: DriverMgr is sole owner. Strategies get
    // raw (non-owning) pointers via getDrivers().
    vector<unique_ptr<Driver>> drivers;

    DriverMgr() = default;

public:
    static DriverMgr* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static DriverMgr instance;
        return &instance;
    }

    void addDriver(unique_ptr<Driver> driver) {
        lock_guard<mutex> lock(operationMtx);
        drivers.push_back(move(driver));
    }

    // Returns const ref to the vector. Caller must NOT modify.
    // Individual driver thread-safety is handled by Driver's own mutex.
    const vector<unique_ptr<Driver>>& getDrivers() {
        lock_guard<mutex> lock(operationMtx);
        return drivers;
    }

    DriverMgr(const DriverMgr&) = delete;
    DriverMgr& operator=(const DriverMgr&) = delete;
};

class TripManager {
    static mutex singletonMtx;
    mutable mutex operationMtx;

    // shared_ptr<Trip>: stored here AND returned to caller (shared ownership)
    vector<shared_ptr<Trip>> trips;
    int tripCounter = 0;

    TripManager() = default;

    // Strategy factory methods (no lock needed, pure logic)
    unique_ptr<PricingStrategy> determinePricing(const TripMetaData* meta) {
        if (meta->getDestination().getName().find("Airport") != string::npos)
            return make_unique<SurgePricing>();
        if (meta->getRiderRating() >= 4)
            return make_unique<RatingBasedPricing>();
        return make_unique<DefaultPricing>();
    }

    unique_ptr<MatchingStrategy> determineMatching(const TripMetaData* meta) {
        if (meta->getRiderRating() >= 4)
            return make_unique<HighRatingMatching>();
        return make_unique<LeastTimeMatching>();
    }

public:
    static TripManager* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static TripManager instance;
        return &instance;
    }

    // createTrip: the core concurrent operation
    // unique_lock because we release the manager lock before
    // calling strategies (which lock individual drivers).
    shared_ptr<Trip> createTrip(const string& riderId,
                                const Location& src, const Location& dest) {
        // Step 1: Look up rider under manager lock
        Rider* rider = RiderMgr::getInstance()->getRider(riderId);
        if (!rider) {
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "Rider not found: " << riderId << endl;
            return nullptr;
        }

        // Step 2: Generate trip ID under lock
        string tripId;
        {
            lock_guard<mutex> lock(operationMtx);
            tripId = "TRIP_" + to_string(++tripCounter);
        }

        // Step 3: Create metadata and determine strategies (no lock needed)
        auto meta = make_unique<TripMetaData>(src, dest, rider->getRating());
        auto pricingStrategy = determinePricing(meta.get());
        auto matchingStrategy = determineMatching(meta.get());

        // Step 4: Calculate fare (pure computation, no lock)
        double fare = pricingStrategy->calculateFare(meta.get());

        // Step 5: Match driver (uses per-driver locks internally)
        // No manager lock held — other trips can proceed in parallel
        auto& drivers = DriverMgr::getInstance()->getDrivers();
        Driver* driver = matchingStrategy->matchDriver(drivers, meta.get());

        // Step 6: Create trip and store under lock
        auto trip = make_shared<Trip>(tripId, rider, move(meta));
        trip->setFare(fare);

        if (driver) {
            trip->setDriver(driver);
            trip->setStatus(TripStatus::ASSIGNED);
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "Trip " << tripId << " created: "
                 << rider->getName() << " -> " << driver->getName()
                 << " | Fare: " << fare
                 << " | Pricing: " << pricingStrategy->getName()
                 << " | Matching: " << matchingStrategy->getName() << endl;
        } else {
            trip->setStatus(TripStatus::CANCELLED);
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "Trip " << tripId << " cancelled: no driver for "
                 << rider->getName() << endl;
        }

        {
            lock_guard<mutex> lock(operationMtx);
            trips.push_back(trip);
        }
        return trip;
    }

    void displayAllTrips() {
        lock_guard<mutex> lock(operationMtx);
        cout << "\n=== All Trips ===" << endl;
        for (const auto& trip : trips) {
            trip->display();
        }
    }

    int getTripCount() {
        lock_guard<mutex> lock(operationMtx);
        return trips.size();
    }

    TripManager(const TripManager&) = delete;
    TripManager& operator=(const TripManager&) = delete;
};

// Static member definitions
mutex RiderMgr::singletonMtx;
mutex DriverMgr::singletonMtx;
mutex TripManager::singletonMtx;

#endif
