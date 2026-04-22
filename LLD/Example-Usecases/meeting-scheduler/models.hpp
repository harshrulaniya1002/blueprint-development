#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>
#include <algorithm>
using namespace std;

// ============================================================
// INTERVAL
// ============================================================
class Interval {
    int start, end;
public:
    Interval(int s, int e) : start(s), end(e) {
        if (s >= e) throw invalid_argument("start must be < end");
    }
    int getStart() const { return start; }
    int getEnd() const { return end; }
    bool overlaps(const Interval& o) const {
        return start < o.end && o.start < end;
    }
};

// ============================================================
// PARTICIPANT
// ============================================================
class Participant {
    string id;
    string name;
    // Per-participant calendar with its own mutex
    // PESSIMISTIC: lock before checking/modifying schedule
    vector<Interval> schedule;
    mutable mutex participantMtx;

public:
    Participant(const string& id, const string& name)
        : id(id), name(name) {}

    string getId() const { return id; }
    string getName() const { return name; }

    // Check if participant is free during interval
    bool isFree(const Interval& interval) const {
        lock_guard<mutex> lock(participantMtx);
        for (const auto& s : schedule) {
            if (s.overlaps(interval)) return false;
        }
        return true;
    }

    // Atomically check and book slot — returns false if conflict
    bool tryBook(const Interval& interval) {
        lock_guard<mutex> lock(participantMtx);
        for (const auto& s : schedule) {
            if (s.overlaps(interval)) return false;
        }
        schedule.push_back(interval);
        return true;
    }

    // Remove a booked slot (for cancellation)
    void removeSlot(const Interval& interval) {
        lock_guard<mutex> lock(participantMtx);
        schedule.erase(
            remove_if(schedule.begin(), schedule.end(),
                [&](const Interval& s) {
                    return s.getStart() == interval.getStart() &&
                           s.getEnd() == interval.getEnd();
                }),
            schedule.end());
    }

    void displaySchedule() const {
        lock_guard<mutex> lock(participantMtx);
        cout << "    " << name << ": ";
        if (schedule.empty()) { cout << "free" << endl; return; }
        for (const auto& s : schedule)
            cout << "[" << s.getStart() << "-" << s.getEnd() << "] ";
        cout << endl;
    }
};

// ============================================================
// MEETING ROOM
// Has its own mutex for per-room locking.
// Multiple rooms can be booked in parallel (no contention).
// ============================================================
class MeetingRoom {
    string id;
    int capacity;
    vector<Interval> bookings;
    mutable mutex roomMtx;

public:
    MeetingRoom(const string& id, int cap)
        : id(id), capacity(cap) {}

    string getId() const { return id; }
    int getCapacity() const { return capacity; }

    bool isAvailable(const Interval& interval) const {
        lock_guard<mutex> lock(roomMtx);
        for (const auto& b : bookings) {
            if (b.overlaps(interval)) return false;
        }
        return true;
    }

    // Atomic check-and-book
    bool tryBook(const Interval& interval) {
        lock_guard<mutex> lock(roomMtx);
        for (const auto& b : bookings) {
            if (b.overlaps(interval)) return false;
        }
        bookings.push_back(interval);
        return true;
    }

    void cancelSlot(const Interval& interval) {
        lock_guard<mutex> lock(roomMtx);
        bookings.erase(
            remove_if(bookings.begin(), bookings.end(),
                [&](const Interval& b) {
                    return b.getStart() == interval.getStart() &&
                           b.getEnd() == interval.getEnd();
                }),
            bookings.end());
    }

    void display() const {
        lock_guard<mutex> lock(roomMtx);
        cout << "  Room[" << id << "] cap=" << capacity << " bookings: ";
        if (bookings.empty()) { cout << "none" << endl; return; }
        for (const auto& b : bookings)
            cout << "[" << b.getStart() << "-" << b.getEnd() << "] ";
        cout << endl;
    }
};

// ============================================================
// MEETING
// Stores meeting info. No mutex needed — immutable after creation,
// only modified under MeetingScheduler's lock for cancellation.
// ============================================================
enum class MeetingStatus { SCHEDULED, CANCELLED };

class Meeting {
    string meetingId;
    string organizerId;
    Interval interval;
    vector<string> participantIds;
    string roomId;
    MeetingStatus status;

public:
    Meeting(const string& id, const string& organizer, const Interval& interval,
            const vector<string>& participants, const string& room)
        : meetingId(id), organizerId(organizer), interval(interval),
          participantIds(participants), roomId(room),
          status(MeetingStatus::SCHEDULED) {}

    string getId() const { return meetingId; }
    string getOrganizerId() const { return organizerId; }
    const Interval& getInterval() const { return interval; }
    const vector<string>& getParticipantIds() const { return participantIds; }
    string getRoomId() const { return roomId; }
    MeetingStatus getStatus() const { return status; }
    void setStatus(MeetingStatus s) { status = s; }

    void display() const {
        cout << "  Meeting[" << meetingId << "] "
             << "[" << interval.getStart() << "-" << interval.getEnd() << "] "
             << "Room: " << roomId << " Organizer: " << organizerId
             << " Participants: ";
        for (const auto& p : participantIds) cout << p << " ";
        cout << "| " << (status == MeetingStatus::SCHEDULED ? "SCHEDULED" : "CANCELLED")
             << endl;
    }
};

#endif
