/*
====================================================================
RAII WITH UNIQUE_PTR - Resource Management
====================================================================

COMMON IN MACHINE CODING ROUNDS:
- File handlers
- Database connections
- Thread pools
- Socket connections
- Parking lot slots
- Meeting rooms

KEY PRINCIPLE:
"Resource Acquisition Is Initialization"
- Acquire resource in constructor
- Release resource in destructor
- unique_ptr ensures automatic cleanup
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

// ========== Database Connection Example ==========
class DatabaseConnection {
    string connectionId;
    bool connected;
    
public:
    DatabaseConnection(const string& id) : connectionId(id), connected(true) {
        cout << "DB Connection " << connectionId << " OPENED\n";
    }
    
    void executeQuery(const string& query) {
        if(connected) {
            cout << "[" << connectionId << "] Executing: " << query << "\n";
        }
    }
    
    ~DatabaseConnection() {
        cout << "DB Connection " << connectionId << " CLOSED\n";
        connected = false;
    }
};

// ========== Connection Pool ==========
class ConnectionPool {
    vector<unique_ptr<DatabaseConnection>> connections;
    
public:
    unique_ptr<DatabaseConnection> acquireConnection(const string& id) {
        auto conn = make_unique<DatabaseConnection>(id);
        return conn; // Transfer ownership to caller
    }
    
    void executeWithAutoCleanup(const string& query) {
        unique_ptr<DatabaseConnection> conn = acquireConnection("AUTO-CONN");
        conn->executeQuery(query);
        // Connection automatically closed when function ends!
    }
};

// ========== Parking Lot Slot Example ==========
class ParkingSlot {
    int slotNumber;
    string vehicleNumber;
    bool occupied;
    
public:
    ParkingSlot(int num) : slotNumber(num), occupied(false) {
        cout << "Slot " << slotNumber << " created\n";
    }
    
    bool parkVehicle(const string& vehicle) {
        if(!occupied) {
            vehicleNumber = vehicle;
            occupied = true;
            cout << "Vehicle " << vehicle << " parked in slot " << slotNumber << "\n";
            return true;
        }
        return false;
    }
    
    void removeVehicle() {
        if(occupied) {
            cout << "Vehicle " << vehicleNumber << " removed from slot " << slotNumber << "\n";
            occupied = false;
            vehicleNumber = "";
        }
    }
    
    int getSlotNumber() const { return slotNumber; }
    bool isOccupied() const { return occupied; }
    
    ~ParkingSlot() {
        cout << "Slot " << slotNumber << " destroyed\n";
    }
};

class ParkingLot {
    vector<unique_ptr<ParkingSlot>> slots;
    
public:
    ParkingLot(int numSlots) {
        for(int i = 1; i <= numSlots; i++) {
            slots.push_back(make_unique<ParkingSlot>(i));
        }
    }
    
    bool parkVehicle(const string& vehicleNumber) {
        for(auto& slot : slots) {
            if(!slot->isOccupied()) {
                return slot->parkVehicle(vehicleNumber);
            }
        }
        cout << "No available slots!\n";
        return false;
    }
    
    void showStatus() {
        cout << "\n=== Parking Lot Status ===\n";
        for(const auto& slot : slots) {
            cout << "Slot " << slot->getSlotNumber() << ": " 
                 << (slot->isOccupied() ? "OCCUPIED" : "EMPTY") << "\n";
        }
    }
    
    // All slots automatically cleaned up when ParkingLot is destroyed!
};

// ========== Meeting Room Booking Example ==========
class MeetingRoom {
    int roomId;
    string currentBooking;
    
public:
    MeetingRoom(int id) : roomId(id) {
        cout << "Meeting Room " << roomId << " created\n";
    }
    
    void book(const string& meetingTitle) {
        currentBooking = meetingTitle;
        cout << "Room " << roomId << " booked for: " << meetingTitle << "\n";
    }
    
    void release() {
        cout << "Room " << roomId << " released\n";
        currentBooking = "";
    }
    
    ~MeetingRoom() {
        cout << "Meeting Room " << roomId << " destroyed\n";
    }
};

class MeetingRoomManager {
    vector<unique_ptr<MeetingRoom>> rooms;
    
public:
    void addRoom(int roomId) {
        rooms.push_back(make_unique<MeetingRoom>(roomId));
    }
    
    unique_ptr<MeetingRoom> bookRoom(const string& meetingTitle) {
        if(!rooms.empty()) {
            // Transfer ownership to caller - they manage the booking lifetime
            auto room = move(rooms.back());
            rooms.pop_back();
            room->book(meetingTitle);
            return room;
        }
        return nullptr;
    }
};

int main() {
    cout << "=== DATABASE CONNECTION DEMO ===\n";
    {
        ConnectionPool pool;
        pool.executeWithAutoCleanup("SELECT * FROM users");
        // Connection auto-closed!
    }
    
    cout << "\n=== PARKING LOT DEMO ===\n";
    {
        ParkingLot lot(3);
        lot.parkVehicle("KA-01-1234");
        lot.parkVehicle("DL-02-5678");
        lot.showStatus();
        
        cout << "\n--- Parking lot closing ---\n";
    }
    // All slots automatically cleaned up!
    
    cout << "\n=== MEETING ROOM DEMO ===\n";
    {
        MeetingRoomManager manager;
        manager.addRoom(101);
        manager.addRoom(102);
        
        auto room = manager.bookRoom("Team Standup");
        if(room) {
            cout << "Meeting in progress...\n";
            // Room automatically released when 'room' goes out of scope
        }
        
        cout << "\n--- Manager shutting down ---\n";
    }
    
    cout << "\n=== PROGRAM ENDED ===\n";
    return 0;
}
