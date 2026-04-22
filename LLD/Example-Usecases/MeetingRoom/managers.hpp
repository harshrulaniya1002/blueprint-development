#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include<iostream>
#include<mutex>
#include<unordered_map>
#include<memory>
#include<thread>
#include "models.hpp"
using namespace std;

// ============================================================
// LOCKING STRATEGY: PESSIMISTIC vs OPTIMISTIC
// ----------------------------------------------------------
// PESSIMISTIC LOCKING (what we use here):
//   Lock BEFORE reading/writing shared state. Assumes conflicts
//   are likely. Simple, safe, but can reduce throughput.
//   e.g., lock_guard on the rooms map before looking up a room.
//
// OPTIMISTIC LOCKING:
//   Read without lock, do work, then check-and-commit atomically.
//   Uses version numbers or CAS (compare-and-swap).
//   Better throughput when conflicts are rare.
//   e.g., Room could have a version field: read version, prepare
//   booking, then CAS(old_version, new_version) — retry if changed.
//
// WHY PESSIMISTIC HERE?
//   Meeting rooms have high contention (many users booking same rooms).
//   Optimistic would cause lots of retries. Pessimistic is simpler
//   and correct for this use case.
// ============================================================

// ============================================================
// SMART POINTER CHOICES:
// ----------------------------------------------------------
// shared_ptr<Room>     -> Room is in the map AND may be held by
//                         threads doing bookings. If room is removed
//                         mid-booking, shared_ptr keeps it alive.
// unique_ptr<Policy>   -> Manager is the SOLE owner of the policy.
//                         Nobody else holds a reference. Transferred
//                         via move(). Lightweight, no ref counting.
// shared_ptr<Observer> -> Observers are shared: caller keeps a
//                         reference (to remove later), manager's
//                         vector also holds one. Shared ownership.
// raw ptr              -> Not used here. Could be used for non-owning
//                         observer references if we didn't need
//                         shared lifetime management.
// ============================================================

// ============================================================
// LOCK CHOICES:
// ----------------------------------------------------------
// lock_guard    -> Used for simple scoped locks: addRoom, removeRoom
//                  lookup, setPolicy, add/removeObserver.
//                  Holds lock for entire scope. No early release.
//
// unique_lock   -> Used in bookRoom: we lock to find the room and
//                  read the policy, then UNLOCK before the actual
//                  room->book() call (which has its own per-room lock).
//                  This avoids holding the manager lock during the
//                  booking operation (fine-grained locking).
//                  Also used when we need to release lock before
//                  notifying observers (avoids deadlock).
// ============================================================

class MeetingMgr{
    private:
    static mutex singletonMtx;              // only for singleton creation

    // operationMtx: guards rooms map, bookingPolicy, observers, etc.
    // Separate from singletonMtx to avoid contention on getInstance()
    mutable mutex operationMtx;

    // shared_ptr<Room>: room lives in map AND may be held by booking
    // threads concurrently. Shared ownership keeps it alive.
    unordered_map<string, shared_ptr<Room>> rooms;
    
    // unique_ptr<BookingPolicy>: sole ownership by manager.
    // Transferred via move(). Only one policy active at a time.
    unique_ptr<BookingPolicy> bookingPolicy;
    
    // shared_ptr<Observer>: shared between manager's vector and
    // the caller (who needs a reference to removeObserver later).
    vector<shared_ptr<NotificationObserver>> observers;

    MeetingMgr();

    // Notify MUST be called WITHOUT operationMtx held (avoids deadlock
    // if an observer callback triggers another manager operation)
    void notifyObservers(const string& message);

    public:
    static shared_ptr<MeetingMgr> getInstance();
    
    // Room Management
    void addRoom(const string& roomId, int capacity);
    bool removeRoom(const string& roomId);
    
    // Booking Operations
    bool bookRoom(const string& roomId, int start, int end, const string& userId);
    bool cancelBooking(const string& roomId, const string& meetingId);
    
    // Query Operations
    void viewRoomBookings(const string& roomId) const;
    void listAllRooms() const;
    vector<string> findAvailableRooms(int start, int end) const;
    
    // Strategy Pattern Methods
    void setBookingPolicy(unique_ptr<BookingPolicy> policy);
    
    // Observer Pattern Methods
    void addObserver(shared_ptr<NotificationObserver> observer);
    void removeObserver(shared_ptr<NotificationObserver> observer);

    MeetingMgr(const MeetingMgr&) = delete;
    void operator=(const MeetingMgr&) = delete;
};


#endif