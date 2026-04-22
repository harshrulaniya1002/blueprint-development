#include<iostream>
#include<thread>
#include<chrono>
#include "managers.hpp"
using namespace std;


void userSimulation(string userId, string roomId, int start, int end) {
    auto mgr = MeetingMgr::getInstance();
    mgr->bookRoom(roomId, start, end, userId);
}

void demonstrateRemoveRoom() {
    auto mgr = MeetingMgr::getInstance();
    
    cout << "\n=== Demonstrating Room Removal with Active Usage ===" << endl;
    
    mgr->addRoom("R3", 6);
    
    // Thread tries to book R3 while main thread removes it
    // shared_ptr<Room> keeps the room alive even after map erase
    thread t1([&]() {
        cout << "Thread 1: Got reference to R3" << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        mgr->bookRoom("R3", 14, 15, "UserE");
    });
    
    this_thread::sleep_for(chrono::milliseconds(50));
    mgr->removeRoom("R3");
    
    t1.join();
}

int main(){
    auto mgr = MeetingMgr::getInstance();

    cout << "=== Meeting Room Booking System ===" << endl << endl;
    
    // ========== 1. OBSERVER PATTERN DEMO ==========
    cout << "--- Setting up Notification Observers ---" << endl;
    // shared_ptr<Observer>: shared between caller (to remove later)
    // and manager's observer vector. Both need ownership.
    auto emailNotifier = make_shared<EmailNotifier>("admin@company.com");
    auto slackNotifier = make_shared<SlackNotifier>("meeting-rooms");
    auto smsNotifier = make_shared<SMSNotifier>("+1-555-0123");
    
    mgr->addObserver(emailNotifier);
    mgr->addObserver(slackNotifier);
    mgr->addObserver(smsNotifier);
    
    
    // ========== 2. ROOM SETUP ==========
    cout << "\n--- Adding Rooms ---" << endl;
    mgr->addRoom("R1", 10);
    mgr->addRoom("R2", 4);
    
    mgr->listAllRooms();
    
    
    // ========== 3. STRATEGY PATTERN DEMO ==========
    cout << "\n--- Testing Booking Policies (Strategy Pattern) ---" << endl;
    
    // unique_ptr<BookingPolicy>: manager is sole owner. Transferred via move().
    // When we setBookingPolicy, old policy is auto-deleted, new one takes over.
    cout << "\nUsing Standard Policy:" << endl;
    mgr->bookRoom("R1", 9, 14, "Alice");
    
    cout << "\nChanging to Max Duration Policy (3 hours max):" << endl;
    mgr->setBookingPolicy(make_unique<MaxDurationPolicy>(3));
    mgr->bookRoom("R1", 15, 20, "Bob");     // 5 hours - should FAIL
    mgr->bookRoom("R1", 15, 17, "Bob");     // 2 hours - should work
    
    cout << "\nChanging to Advance Booking Policy (min 5 hours notice):" << endl;
    mgr->setBookingPolicy(make_unique<AdvanceBookingPolicy>(5));
    mgr->bookRoom("R2", 3, 4, "Charlie");     // 3hrs notice - should FAIL
    mgr->bookRoom("R2", 10, 11, "Charlie");   // 10hrs notice - should work
    
    
    // ========== 4. CONCURRENT BOOKING DEMO ==========
    // ============================================================
    // PESSIMISTIC LOCKING IN ACTION:
    // - bookRoom uses unique_lock on operationMtx to find room + check policy
    // - Then RELEASES operationMtx (fine-grained: other rooms can proceed)
    // - room->book() uses its own lock_guard on roomMtx (per-room lock)
    // - Two threads booking SAME room at SAME time: one wins, one gets conflict
    // - Two threads booking DIFFERENT rooms: proceed in parallel (no contention)
    // ============================================================
    cout << "\n--- Concurrent Booking (Pessimistic Locking) ---" << endl;
    cout << "UserA & UserB race for same slot on R1 (one will fail)." << endl;
    cout << "UserC books different time on R1 (no conflict)." << endl;
    cout << "UserD books R2 in parallel (different room, no contention)." << endl << endl;
    
    mgr->setBookingPolicy(make_unique<StandardBookingPolicy>());
    
    thread t1(userSimulation, "UserA", "R1", 20, 21);
    thread t2(userSimulation, "UserB", "R1", 20, 21); // Conflict!
    thread t3(userSimulation, "UserC", "R1", 21, 22); // Different time
    thread t4(userSimulation, "UserD", "R2", 20, 21); // Different room
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    
    // ========== 5. QUERY OPERATIONS ==========
    cout << "\n--- Viewing Room Bookings ---" << endl;
    mgr->viewRoomBookings("R1");
    mgr->viewRoomBookings("R2");
    
    cout << "\n--- Finding Available Rooms ---" << endl;
    auto available = mgr->findAvailableRooms(20, 21);
    cout << "Rooms available [20-21]: ";
    if (available.empty()) {
        cout << "None" << endl;
    } else {
        for (const auto& room : available) {
            cout << room << " ";
        }
        cout << endl;
    }
    
    
    // ========== 6. CANCEL BOOKING ==========
    cout << "\n--- Cancelling Booking ---" << endl;
    mgr->cancelBooking("R1", "Alice_9");
    mgr->viewRoomBookings("R1");
    
    
    // ========== 7. ROOM REMOVAL DEMO ==========
    // shared_ptr<Room> keeps room alive if another thread holds a ref
    cout << "\n--- Removing Room ---" << endl;
    mgr->removeRoom("R2");
    mgr->listAllRooms();
    
    mgr->bookRoom("R2", 25, 26, "UserX");  // Should fail - room removed
    
    
    // ========== 8. ADVANCED: Remove room while in use ==========
    demonstrateRemoveRoom();
    
    
    // ========== 9. OBSERVER MANAGEMENT ==========
    cout << "\n--- Removing Observer ---" << endl;
    mgr->removeObserver(smsNotifier);
    
    cout << "\n--- Final Booking (fewer notifications) ---" << endl;
    mgr->bookRoom("R1", 30, 31, "FinalUser");
    
    // ============================================================
    // SUMMARY: CONCURRENCY ARCHITECTURE
    // ----------------------------------------------------------
    // Level 1: operationMtx (manager) -> rooms map, policy, observers
    //   - lock_guard for simple reads/writes (addRoom, setPolicy, etc.)
    //   - unique_lock in bookRoom (release before room-level lock)
    //
    // Level 2: roomMtx (per-room) -> that room's calendar
    //   - lock_guard always (simple check+book is atomic)
    //
    // DEADLOCK PREVENTION:
    //   - Always acquire Level 1 before Level 2 (never reverse)
    //   - Release Level 1 before calling notifyObservers
    //   - notifyObservers takes a snapshot under lock, notifies outside
    //
    // WHY PESSIMISTIC?
    //   Meeting rooms = high contention. Pessimistic is simpler
    //   and avoids retry storms that optimistic would cause.
    // ============================================================
    
    cout << "\n=== Simulation Complete ===" << endl;
    return 0;
}