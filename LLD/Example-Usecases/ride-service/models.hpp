#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <iostream>
#include <cmath>
#include <mutex>
#include <memory>
using namespace std;

enum class TripStatus {
    REQUESTED,
    ASSIGNED,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};

class Location {
    double latitude;
    double longitude;
    string name;

public:
    Location(double lat = 0, double lon = 0, const string& name = "")
        : latitude(lat), longitude(lon), name(name) {}

    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    string getName() const { return name; }

    double distanceTo(const Location& other) const {
        double dlat = other.latitude - latitude;
        double dlon = other.longitude - longitude;
        return sqrt(dlat * dlat + dlon * dlon);
    }
};

class Rider {
    string id;
    string name;
    int rating;

public:
    Rider() : rating(0) {}
    Rider(const string& id, const string& name, int rating = 5)
        : id(id), name(name), rating(rating) {}

    string getId() const { return id; }
    string getName() const { return name; }
    int getRating() const { return rating; }
};

// Driver has mutable state (isAvailable) that needs thread-safe access.
// The mutex lives here so each driver can be locked independently.
class Driver {
    string id;
    string name;
    int rating;
    bool available;
    mutable mutex driverMtx;

public:
    Driver(const string& id, const string& name, int rating = 5)
        : id(id), name(name), rating(rating), available(true) {}

    string getId() const { return id; }
    string getName() const { return name; }
    int getRating() const { return rating; }

    // Thread-safe availability check + set
    bool isAvailable() const {
        lock_guard<mutex> lock(driverMtx);
        return available;
    }

    // tryAssign: atomic check-and-set. Returns true if successfully claimed.
    // This is PESSIMISTIC locking at the driver level:
    // lock first, then check, then modify.
    bool tryAssign() {
        lock_guard<mutex> lock(driverMtx);
        if (!available) return false;
        available = false;
        return true;
    }

    void release() {
        lock_guard<mutex> lock(driverMtx);
        available = true;
    }
};

class TripMetaData {
    Location source;
    Location destination;
    int riderRating;
    double distance;

public:
    TripMetaData(const Location& src, const Location& dest, int rating)
        : source(src), destination(dest), riderRating(rating) {
        distance = source.distanceTo(dest);
    }

    const Location& getSource() const { return source; }
    const Location& getDestination() const { return destination; }
    int getRiderRating() const { return riderRating; }
    double getDistance() const { return distance; }
};

// Trip: created by the service, stored as shared_ptr
// (service owns it in the vector, but caller may also hold a ref)
class Trip {
    string tripId;
    Rider* rider;       // non-owning: RiderMgr owns riders
    Driver* driver;     // non-owning: DriverMgr owns drivers
    unique_ptr<TripMetaData> metaData;  // sole ownership
    double fare;
    TripStatus status;

public:
    Trip(const string& id, Rider* r, unique_ptr<TripMetaData> meta)
        : tripId(id), rider(r), metaData(move(meta)),
          driver(nullptr), fare(0.0), status(TripStatus::REQUESTED) {}

    ~Trip() {
        if (driver) driver->release();
    }

    string getTripId() const { return tripId; }
    Rider* getRider() const { return rider; }
    Driver* getDriver() const { return driver; }
    const TripMetaData* getMetaData() const { return metaData.get(); }
    double getFare() const { return fare; }
    TripStatus getStatus() const { return status; }

    void setDriver(Driver* d) { driver = d; }
    void setFare(double f) { fare = f; }
    void setStatus(TripStatus s) { status = s; }

    void display() const {
        cout << "  Trip[" << tripId << "] "
             << rider->getName() << " -> "
             << (driver ? driver->getName() : "None")
             << " | " << metaData->getSource().getName()
             << " to " << metaData->getDestination().getName()
             << " | " << metaData->getDistance() << " km"
             << " | Fare: " << fare
             << " | " << (status == TripStatus::ASSIGNED ? "Assigned" :
                          status == TripStatus::COMPLETED ? "Completed" :
                          status == TripStatus::CANCELLED ? "Cancelled" : "Requested")
             << endl;
    }
};

#endif
