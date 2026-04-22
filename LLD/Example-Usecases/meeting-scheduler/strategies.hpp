#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include "models.hpp"
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// STRATEGY PATTERN: Room Selection
// Different policies for how to pick a room.
// unique_ptr<RoomSelectionStrategy>: MeetingScheduler is sole owner.
// ============================================================

class RoomSelectionStrategy {
public:
    virtual ~RoomSelectionStrategy() = default;
    virtual MeetingRoom* selectRoom(vector<unique_ptr<MeetingRoom>>& rooms,
                                     int requiredCapacity,
                                     const Interval& interval) = 0;
    virtual string getName() const = 0;
};

// Pick smallest room that fits (minimize waste)
class BestFitRoomStrategy : public RoomSelectionStrategy {
public:
    MeetingRoom* selectRoom(vector<unique_ptr<MeetingRoom>>& rooms,
                             int requiredCapacity,
                             const Interval& interval) override {
        MeetingRoom* best = nullptr;
        int bestCap = INT_MAX;
        for (auto& r : rooms) {
            if (r->getCapacity() >= requiredCapacity &&
                r->getCapacity() < bestCap &&
                r->isAvailable(interval)) {
                best = r.get();
                bestCap = r->getCapacity();
            }
        }
        return best;
    }
    string getName() const override { return "BestFit"; }
};

// Pick first available room that fits
class FirstFitRoomStrategy : public RoomSelectionStrategy {
public:
    MeetingRoom* selectRoom(vector<unique_ptr<MeetingRoom>>& rooms,
                             int requiredCapacity,
                             const Interval& interval) override {
        for (auto& r : rooms) {
            if (r->getCapacity() >= requiredCapacity &&
                r->isAvailable(interval)) {
                return r.get();
            }
        }
        return nullptr;
    }
    string getName() const override { return "FirstFit"; }
};

// Pick largest room available (luxury mode)
class LargestRoomStrategy : public RoomSelectionStrategy {
public:
    MeetingRoom* selectRoom(vector<unique_ptr<MeetingRoom>>& rooms,
                             int requiredCapacity,
                             const Interval& interval) override {
        MeetingRoom* best = nullptr;
        int bestCap = 0;
        for (auto& r : rooms) {
            if (r->getCapacity() >= requiredCapacity &&
                r->getCapacity() > bestCap &&
                r->isAvailable(interval)) {
                best = r.get();
                bestCap = r->getCapacity();
            }
        }
        return best;
    }
    string getName() const override { return "Largest"; }
};

// ============================================================
// OBSERVER PATTERN: Meeting Notifications
// shared_ptr<MeetingObserver>: shared between scheduler's vector
// and caller (to remove later). Shared ownership.
// ============================================================

class MeetingObserver {
public:
    virtual ~MeetingObserver() = default;
    virtual void onMeetingScheduled(const Meeting& m) = 0;
    virtual void onMeetingCancelled(const Meeting& m) = 0;
    virtual string getName() const = 0;
};

class EmailNotifier : public MeetingObserver {
public:
    void onMeetingScheduled(const Meeting& m) override {
        cout << "    [Email] Meeting " << m.getId() << " scheduled "
             << "[" << m.getInterval().getStart() << "-"
             << m.getInterval().getEnd() << "] Room: " << m.getRoomId() << endl;
    }
    void onMeetingCancelled(const Meeting& m) override {
        cout << "    [Email] Meeting " << m.getId() << " cancelled" << endl;
    }
    string getName() const override { return "Email"; }
};

class CalendarSyncObserver : public MeetingObserver {
public:
    void onMeetingScheduled(const Meeting& m) override {
        cout << "    [Calendar] Synced meeting " << m.getId() << endl;
    }
    void onMeetingCancelled(const Meeting& m) override {
        cout << "    [Calendar] Removed meeting " << m.getId() << endl;
    }
    string getName() const override { return "CalendarSync"; }
};

#endif
