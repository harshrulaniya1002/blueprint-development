#include<iostream>
#include <string>
#include "managers.hpp"
#include "models.hpp"

using namespace std;

mutex MeetingMgr::singletonMtx;

MeetingMgr::MeetingMgr() {
    bookingPolicy = make_unique<StandardBookingPolicy>();
}

// Thread-safe singleton using function-local static
// The C++11 standard guarantees static local init is thread-safe
shared_ptr<MeetingMgr> MeetingMgr::getInstance(){
    lock_guard<mutex> lock(singletonMtx);
    static shared_ptr<MeetingMgr> instance(new MeetingMgr());
    return instance;
}

// ============================================================
// PESSIMISTIC LOCKING PATTERN used throughout:
// 1. Acquire lock (operationMtx)
// 2. Read/write shared state
// 3. Release lock BEFORE I/O (notifications, printing)
// This prevents deadlocks and reduces lock hold time.
// ============================================================

void MeetingMgr::addRoom(const string& roomId, int capacity){
    // lock_guard: simple scope, hold lock while modifying rooms map
    {
        lock_guard<mutex> lock(operationMtx);
        rooms[roomId] = make_shared<Room>(roomId, capacity);
    }
    // Lock released BEFORE I/O and notification (avoids deadlock)
    cout << "[Thread " << this_thread::get_id() << "] "
         << "Room " << roomId << " added (Capacity: " << capacity << ")" << endl;
    notifyObservers("Room " + roomId + " has been added to the system.");
}

bool MeetingMgr::removeRoom(const string& roomId) {
    shared_ptr<Room> room;
    
    {
        // lock_guard: simple lookup + erase, hold for entire block
        lock_guard<mutex> lock(operationMtx);
        auto it = rooms.find(roomId);
        
        if (it == rooms.end()) {
            cout << "Error: Room " << roomId << " does not exist." << endl;
            return false;
        }
        
        // shared_ptr ref count keeps room alive even after map erase.
        // If another thread is mid-booking on this room, the room
        // object stays alive until that thread's shared_ptr drops.
        room = it->second;  // ref count: map(1) + local(1) = 2
        rooms.erase(it);    // ref count: local(1) = 1
    }
    // Lock released BEFORE notification
    
    if (room.use_count() > 1) {
        cout << "[Thread " << this_thread::get_id() << "] "
             << "Room " << roomId << " removed but still in use by " 
             << (room.use_count() - 1) << " active operation(s). "
             << "Will be deleted when all operations complete." << endl;
    } else {
        cout << "[Thread " << this_thread::get_id() << "] "
             << "Room " << roomId << " successfully removed." << endl;
    }
    
    notifyObservers("Room " + roomId + " has been removed from the system.");
    return true;
}

bool MeetingMgr::bookRoom(const string& roomId, int start, int end, const string& userId){
    shared_ptr<Room> room;
    
    // ============================================================
    // FINE-GRAINED LOCKING (2-level pessimistic):
    // Level 1: operationMtx (manager-level) -> protects rooms map + policy
    // Level 2: roomMtx (per-room) -> protects that room's calendar
    //
    // unique_lock here because we RELEASE operationMtx after
    // looking up the room and checking the policy. Then room->book()
    // only takes the per-room lock. This way, bookings to DIFFERENT
    // rooms can proceed in parallel.
    // ============================================================
    unique_lock<mutex> lock(operationMtx);
    
    auto it = rooms.find(roomId);
    if (it == rooms.end()) {
        cout << "Error: Room " << roomId << " does not exist." << endl;
        return false;
    }
    room = it->second;  // shared_ptr: room stays alive even if removed from map

    try {
        Interval interval(start, end);
        
        // Check policy while we still hold operationMtx
        // (policy could be swapped by setBookingPolicy on another thread)
        if (!bookingPolicy->canBook(room.get(), interval, userId)) {
            cout << "POLICY REJECTION: " << userId << " cannot book Room " << roomId 
                 << " [" << start << "-" << end << "]" << endl;
            return false;
        }
        
        // Release manager lock BEFORE room-level operation
        // This is the KEY benefit of unique_lock over lock_guard:
        // Other threads can now book DIFFERENT rooms in parallel
        lock.unlock();
        
        string meetingId = userId + "_" + to_string(start);
        
        // room->book() acquires its own per-room roomMtx (Level 2 lock)
        // This is pessimistic locking at the room level
        bool success = room->book(meetingId, interval, userId);
        
        if (success) {
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "SUCCESS: " << userId << " booked Room " << roomId 
                 << " [" << start << "-" << end << "]" << endl;
            // Notify OUTSIDE all locks (avoids deadlock)
            notifyObservers(userId + " booked Room " + roomId + " [" + 
                          to_string(start) + "-" + to_string(end) + "]");
            return true;
        } else {
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "FAIL: " << userId << " could not book Room " << roomId 
                 << " [" << start << "-" << end << "] (Conflict)" << endl;
            return false;
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return false;
    }
}

bool MeetingMgr::cancelBooking(const string& roomId, const string& meetingId) {
    shared_ptr<Room> room;
    
    {
        // lock_guard: simple lookup, hold for short duration
        lock_guard<mutex> lock(operationMtx);
        auto it = rooms.find(roomId);
        if (it == rooms.end()) {
            cout << "Error: Room " << roomId << " does not exist." << endl;
            return false;
        }
        room = it->second;
    }
    // Manager lock released. room->cancelBooking uses its own roomMtx.
    
    bool success = room->cancelBooking(meetingId);
    
    if (success) {
        cout << "[Thread " << this_thread::get_id() << "] "
             << "Booking " << meetingId << " cancelled in Room " << roomId << endl;
        notifyObservers("Booking " + meetingId + " cancelled in Room " + roomId);
        return true;
    } else {
        cout << "Booking " << meetingId << " not found in Room " << roomId << endl;
        return false;
    }
}

void MeetingMgr::viewRoomBookings(const string& roomId) const {
    shared_ptr<Room> room;
    
    {
        lock_guard<mutex> lock(operationMtx);
        auto it = rooms.find(roomId);
        if (it == rooms.end()) {
            cout << "Error: Room " << roomId << " does not exist." << endl;
            return;
        }
        room = it->second;
    }
    // Manager lock released. room->displayBookings uses its own roomMtx.
    room->displayBookings();
}

void MeetingMgr::listAllRooms() const {
    lock_guard<mutex> lock(operationMtx);
    
    cout << "\n=== All Meeting Rooms ===" << endl;
    if (rooms.empty()) {
        cout << "No rooms available." << endl;
        return;
    }
    
    for (const auto& [roomId, room] : rooms) {
        cout << "  " << roomId << " (Capacity: " << room->getCapacity() 
             << ", Bookings: " << room->getBookingCount() << ")" << endl;
    }
}

vector<string> MeetingMgr::findAvailableRooms(int start, int end) const {
    lock_guard<mutex> lock(operationMtx);
    vector<string> availableRooms;
    
    for (const auto& [roomId, room] : rooms) {
        if (room->isAvailable(start, end)) {
            availableRooms.push_back(roomId);
        }
    }
    
    return availableRooms;
}

void MeetingMgr::setBookingPolicy(unique_ptr<BookingPolicy> policy) {
    // lock_guard: simple one-liner write to shared state
    lock_guard<mutex> lock(operationMtx);
    bookingPolicy = move(policy);
    cout << "Policy changed to: " << bookingPolicy->getPolicyName() << endl;
}

void MeetingMgr::addObserver(shared_ptr<NotificationObserver> observer) {
    lock_guard<mutex> lock(operationMtx);
    observers.push_back(observer);
    cout << "Observer added: " << observer->getObserverName() << endl;
}

void MeetingMgr::removeObserver(shared_ptr<NotificationObserver> observer) {
    lock_guard<mutex> lock(operationMtx);
    auto it = find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        cout << "Observer removed: " << (*it)->getObserverName() << endl;
        observers.erase(it);
    }
}

// CRITICAL: This is called WITHOUT operationMtx held.
// We take a snapshot of observers under lock, then notify outside lock.
// This prevents deadlock if an observer callback triggers another
// manager operation (which would try to acquire operationMtx again).
void MeetingMgr::notifyObservers(const string& message) {
    vector<shared_ptr<NotificationObserver>> snapshot;
    {
        lock_guard<mutex> lock(operationMtx);
        snapshot = observers;  // copy the vector (shared_ptrs are cheap to copy)
    }
    // Notify outside the lock — safe because we hold shared_ptrs
    for (const auto& observer : snapshot) {
        observer->update(message);
    }
}
