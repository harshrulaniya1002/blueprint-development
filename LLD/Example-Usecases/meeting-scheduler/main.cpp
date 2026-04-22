#include <iostream>
#include <thread>
#include <vector>
#include "managers.hpp"
using namespace std;

int main() {
    auto* sched = MeetingScheduler::getInstance();

    cout << "=== Meeting Scheduler ===" << endl << endl;

    // Observers
    sched->addObserver(make_shared<EmailNotifier>());
    sched->addObserver(make_shared<CalendarSyncObserver>());

    // Rooms (unique_ptr<MeetingRoom>: scheduler sole owner)
    sched->addRoom("R1", 5);
    sched->addRoom("R2", 10);
    sched->addRoom("R3", 3);

    // Participants (unique_ptr<Participant>: scheduler sole owner)
    sched->addParticipant("P1", "Alice");
    sched->addParticipant("P2", "Bob");
    sched->addParticipant("P3", "Charlie");
    sched->addParticipant("P4", "Diana");

    // ========== Sequential meetings ==========
    cout << "\n--- Sequential Meetings ---" << endl;
    sched->scheduleMeeting("P1", {"P1", "P2"}, 9, 10);         // R3 (best fit, cap 3 >= 2)
    sched->scheduleMeeting("P2", {"P2", "P3", "P4"}, 10, 12);  // R1 (best fit, cap 5 >= 3)
    sched->scheduleMeeting("P1", {"P1", "P2"}, 9, 11);         // FAIL: P1 & P2 busy 9-10

    // ========== Strategy switch ==========
    cout << "\n--- Switch to LargestRoom strategy ---" << endl;
    sched->setRoomStrategy(make_unique<LargestRoomStrategy>());
    sched->scheduleMeeting("P3", {"P3", "P4"}, 9, 10);  // Gets R2 (largest)

    // ========== Concurrent meetings ==========
    // 3 threads try to book overlapping slots
    // Per-participant locks detect conflicts atomically
    // Per-room locks prevent double-booking
    cout << "\n--- Concurrent Meetings (3 threads race for 14-15) ---" << endl;
    sched->setRoomStrategy(make_unique<FirstFitRoomStrategy>());

    vector<shared_ptr<Meeting>> results(3);
    vector<thread> threads;

    auto bookFn = [&](int idx, const string& org, vector<string> parts, int s, int e) {
        results[idx] = sched->scheduleMeeting(org, parts, s, e);
    };

    threads.emplace_back(bookFn, 0, "P1", vector<string>{"P1", "P2"}, 14, 15);
    threads.emplace_back(bookFn, 1, "P3", vector<string>{"P3", "P2"}, 14, 15); // P2 conflict
    threads.emplace_back(bookFn, 2, "P4", vector<string>{"P4"}, 14, 15);       // Different people, OK

    for (auto& t : threads) t.join();

    // ========== Cancellation ==========
    cout << "\n--- Cancellation ---" << endl;
    sched->cancelMeeting("MTG_1");
    // Now P1 and P2 are free 9-10, R3 is free 9-10
    cout << "\n--- Rebooking after cancellation ---" << endl;
    sched->scheduleMeeting("P3", {"P1", "P2"}, 9, 10);  // Should succeed now

    // ========== Display ==========
    sched->displayMeetings();
    sched->displayRooms();
    sched->displayParticipants();

    cout << "\n=== Done ===" << endl;
    return 0;
}
