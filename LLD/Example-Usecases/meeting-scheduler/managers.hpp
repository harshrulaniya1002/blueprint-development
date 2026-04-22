#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include "strategies.hpp"
#include <unordered_map>
#include <mutex>
#include <thread>
using namespace std;

// ============================================================
// LOCKING: 3-level pessimistic
// Level 1: schedulerMtx (MeetingScheduler) -> meetings map, rooms, participants
// Level 2: roomMtx (per-MeetingRoom) -> room bookings
// Level 3: participantMtx (per-Participant) -> participant schedule
//
// LOCK ORDER: always L1 -> L2 -> L3 (never reverse)
// Release L1 before per-resource locks where possible.
//
// SMART POINTERS:
//   unique_ptr<MeetingRoom>     -> scheduler is sole owner of rooms
//   unique_ptr<Participant>     -> scheduler is sole owner of participants
//   shared_ptr<Meeting>         -> scheduler map + caller both hold ref
//   unique_ptr<RoomStrategy>    -> scheduler sole owner, swapped via move()
//   shared_ptr<MeetingObserver> -> scheduler vector + caller (to remove)
// ============================================================

class MeetingScheduler {
    static mutex singletonMtx;
    mutable mutex schedulerMtx;

    vector<unique_ptr<MeetingRoom>> rooms;
    unordered_map<string, unique_ptr<Participant>> participants;
    unordered_map<string, shared_ptr<Meeting>> meetings;
    unique_ptr<RoomSelectionStrategy> roomStrategy;
    vector<shared_ptr<MeetingObserver>> observers;
    int meetingCounter = 0;

    MeetingScheduler() {
        roomStrategy = make_unique<BestFitRoomStrategy>();
    }

    // Snapshot pattern: copy observers under lock, notify outside
    void notifyScheduled(const Meeting& m) {
        vector<shared_ptr<MeetingObserver>> snap;
        { lock_guard<mutex> lock(schedulerMtx); snap = observers; }
        for (auto& obs : snap) obs->onMeetingScheduled(m);
    }

    void notifyCancelled(const Meeting& m) {
        vector<shared_ptr<MeetingObserver>> snap;
        { lock_guard<mutex> lock(schedulerMtx); snap = observers; }
        for (auto& obs : snap) obs->onMeetingCancelled(m);
    }

public:
    static MeetingScheduler* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static MeetingScheduler instance;
        return &instance;
    }

    void setRoomStrategy(unique_ptr<RoomSelectionStrategy> strategy) {
        lock_guard<mutex> lock(schedulerMtx);
        roomStrategy = move(strategy);
        cout << "[Scheduler] Room strategy: " << roomStrategy->getName() << endl;
    }

    void addRoom(const string& id, int capacity) {
        lock_guard<mutex> lock(schedulerMtx);
        rooms.push_back(make_unique<MeetingRoom>(id, capacity));
        cout << "[Scheduler] Room added: " << id << " (cap " << capacity << ")" << endl;
    }

    void addParticipant(const string& id, const string& name) {
        lock_guard<mutex> lock(schedulerMtx);
        participants[id] = make_unique<Participant>(id, name);
        cout << "[Scheduler] Participant added: " << name << endl;
    }

    void addObserver(shared_ptr<MeetingObserver> obs) {
        lock_guard<mutex> lock(schedulerMtx);
        observers.push_back(obs);
    }

    // ============================================================
    // SCHEDULE MEETING — core concurrent operation
    // 1. Lock scheduler to generate ID, look up participants, select room
    // 2. Release scheduler lock
    // 3. Lock each participant (Level 3) to check/book their schedule
    // 4. Lock room (Level 2) to book the room
    // 5. Lock scheduler again to store meeting
    // ============================================================
    shared_ptr<Meeting> scheduleMeeting(const string& organizerId,
                                         const vector<string>& participantIds,
                                         int start, int end) {
        Interval interval(start, end);
        string meetingId;
        vector<Participant*> participantPtrs;
        MeetingRoom* room = nullptr;

        // Step 1: Under scheduler lock — ID gen, lookups, room selection
        {
            lock_guard<mutex> lock(schedulerMtx);
            meetingId = "MTG_" + to_string(++meetingCounter);

            // Verify all participants exist
            for (const auto& pid : participantIds) {
                auto it = participants.find(pid);
                if (it == participants.end()) {
                    cout << "[Thread " << this_thread::get_id() << "] "
                         << "Participant not found: " << pid << endl;
                    return nullptr;
                }
                participantPtrs.push_back(it->second.get());
            }

            // Select room (strategy reads room availability — room locks itself)
            int requiredCap = participantIds.size();
            room = roomStrategy->selectRoom(rooms, requiredCap, interval);
            if (!room) {
                cout << "[Thread " << this_thread::get_id() << "] "
                     << meetingId << " FAILED: no room available" << endl;
                return nullptr;
            }
        }
        // Scheduler lock released

        // Step 2: Check & book each participant (per-participant lock)
        vector<Participant*> booked;
        bool allFree = true;
        for (auto* p : participantPtrs) {
            if (p->tryBook(interval)) {
                booked.push_back(p);
            } else {
                allFree = false;
                cout << "[Thread " << this_thread::get_id() << "] "
                     << meetingId << " CONFLICT: " << p->getName()
                     << " busy [" << start << "-" << end << "]" << endl;
                break;
            }
        }

        // Rollback if any participant had a conflict
        if (!allFree) {
            for (auto* p : booked) p->removeSlot(interval);
            return nullptr;
        }

        // Step 3: Book the room (per-room lock)
        if (!room->tryBook(interval)) {
            // Room got taken between strategy check and booking (race)
            for (auto* p : booked) p->removeSlot(interval);
            cout << "[Thread " << this_thread::get_id() << "] "
                 << meetingId << " FAILED: room " << room->getId()
                 << " taken (race)" << endl;
            return nullptr;
        }

        // Step 4: Store meeting under scheduler lock
        auto meeting = make_shared<Meeting>(meetingId, organizerId, interval,
                                             participantIds, room->getId());
        {
            lock_guard<mutex> lock(schedulerMtx);
            meetings[meetingId] = meeting;
        }

        cout << "[Thread " << this_thread::get_id() << "] "
             << meetingId << " scheduled [" << start << "-" << end << "] "
             << "Room: " << room->getId() << endl;

        notifyScheduled(*meeting);
        return meeting;
    }

    // ============================================================
    // CANCEL MEETING
    // Release room slot, release participant slots, mark cancelled
    // ============================================================
    bool cancelMeeting(const string& meetingId) {
        shared_ptr<Meeting> meeting;
        {
            lock_guard<mutex> lock(schedulerMtx);
            auto it = meetings.find(meetingId);
            if (it == meetings.end()) {
                cout << "Meeting not found: " << meetingId << endl;
                return false;
            }
            meeting = it->second;
        }

        if (meeting->getStatus() == MeetingStatus::CANCELLED) {
            cout << "Meeting already cancelled: " << meetingId << endl;
            return false;
        }

        // Release room
        {
            lock_guard<mutex> lock(schedulerMtx);
            for (auto& r : rooms) {
                if (r->getId() == meeting->getRoomId()) {
                    r->cancelSlot(meeting->getInterval());
                    break;
                }
            }
        }

        // Release participants
        {
            lock_guard<mutex> lock(schedulerMtx);
            for (const auto& pid : meeting->getParticipantIds()) {
                auto it = participants.find(pid);
                if (it != participants.end()) {
                    it->second->removeSlot(meeting->getInterval());
                }
            }
        }

        meeting->setStatus(MeetingStatus::CANCELLED);
        cout << "[Thread " << this_thread::get_id() << "] "
             << meetingId << " cancelled" << endl;

        notifyCancelled(*meeting);
        return true;
    }

    void displayMeetings() {
        lock_guard<mutex> lock(schedulerMtx);
        cout << "\n=== All Meetings ===" << endl;
        for (auto& [id, m] : meetings) m->display();
    }

    void displayRooms() {
        lock_guard<mutex> lock(schedulerMtx);
        cout << "\n=== Rooms ===" << endl;
        for (auto& r : rooms) r->display();
    }

    void displayParticipants() {
        lock_guard<mutex> lock(schedulerMtx);
        cout << "\n=== Participants ===" << endl;
        for (auto& [id, p] : participants) p->displaySchedule();
    }

    MeetingScheduler(const MeetingScheduler&) = delete;
    MeetingScheduler& operator=(const MeetingScheduler&) = delete;
};

mutex MeetingScheduler::singletonMtx;

#endif
