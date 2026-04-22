#ifndef MODELS_HPP
#define MODELS_HPP
#include<iostream> 
#include<vector>
#include<mutex>
#include<algorithm>
#include<string>
using namespace std;


class Interval {
    public:
    int startTime;
    int endTime;

    Interval(int start, int end) : startTime(start), endTime(end) {
        if(start >= end){
            throw invalid_argument("Start time must be before end Time");
        }
    }

    bool overlaps(const Interval& other) const {
        return this->startTime < other.endTime && other.startTime < this->endTime;
    }
};

class Meeting {
    public:
    string id;
    Interval interval;
    string userId;

    Meeting(const string& id, const Interval& interval, const string& userId) 
       : id(id), interval(interval), userId(userId) {}

};


//handles its own concurrency
class Room{
    string id;
    int capacity;
    vector<Meeting> calender;
    mutable mutex roomMtx;

    public:

    Room(const string& id, const int& capacity)
       : id(id), capacity(capacity) {}

    string getId() const {return id;}   
    int getCapacity() const {return capacity;}

    bool book(const string& meetingId, const Interval& interval, const string& userId){
        // 1. Lock this specific room
        lock_guard<mutex> lock(roomMtx);

        // 2. Critical Section: Check for overlaps
        for(const auto& meeting : calender){
            if(meeting.interval.overlaps(interval)){
                return false; //conflict found
            }
        }

        // 3. No conflict: Add meeting
        calender.emplace_back(meetingId, interval, userId);
        return true;
        // 4. Lock automatically releases here
    }
    
    bool cancelBooking(const string& meetingId) {
        lock_guard<mutex> lock(roomMtx);
        
        auto it = find_if(calender.begin(), calender.end(),
            [&meetingId](const Meeting& m) { return m.id == meetingId; });
        
        if (it != calender.end()) {
            calender.erase(it);
            return true;
        }
        return false;
    }
    
    bool isAvailable(int start, int end) const {
        lock_guard<mutex> lock(roomMtx);
        Interval requestedInterval(start, end);
        
        for (const auto& meeting : calender) {
            if (meeting.interval.overlaps(requestedInterval)) {
                return false;
            }
        }
        return true;
    }
    
    void displayBookings() const {
        lock_guard<mutex> lock(roomMtx);
        cout << "Room " << id << " (Capacity: " << capacity << ") Bookings:\n";
        
        if (calender.empty()) {
            cout << "  No bookings.\n";
        } else {
            for (const auto& meeting : calender) {
                cout << "  [" << meeting.interval.startTime << "-" 
                     << meeting.interval.endTime << "] " 
                     << meeting.userId << " (ID: " << meeting.id << ")\n";
            }
        }
    }
    
    int getBookingCount() const {
        lock_guard<mutex> lock(roomMtx);
        return calender.size();
    }
};


// ========== STRATEGY PATTERN ==========
// Defines different booking policies

class BookingPolicy {
public:
    virtual ~BookingPolicy() = default;
    virtual bool canBook(const Room* room, const Interval& interval, const string& userId) const = 0;
    virtual string getPolicyName() const = 0;
};

class StandardBookingPolicy : public BookingPolicy {
public:
    bool canBook(const Room* room, const Interval& interval, const string& userId) const override {
        return true;  // Anyone can book
    }
    
    string getPolicyName() const override {
        return "Standard (No restrictions)";
    }
};

class MaxDurationPolicy : public BookingPolicy {
    int maxDuration;
public:
    MaxDurationPolicy(int maxHours) : maxDuration(maxHours) {}
    
    bool canBook(const Room* room, const Interval& interval, const string& userId) const override {
        int duration = interval.endTime - interval.startTime;
        if (duration > maxDuration) {
            cout << "Policy Violation: Max booking duration is " << maxDuration << " hours.\n";
            return false;
        }
        return true;
    }
    
    string getPolicyName() const override {
        return "Max Duration (" + to_string(maxDuration) + " hours)";
    }
};

class AdvanceBookingPolicy : public BookingPolicy {
    int minAdvanceHours;
public:
    AdvanceBookingPolicy(int hours) : minAdvanceHours(hours) {}
    
    bool canBook(const Room* room, const Interval& interval, const string& userId) const override {
        // In real system, would compare with current time
        // Simplified: check if booking is not immediate
        if (interval.startTime < minAdvanceHours) {
            cout << "Policy Violation: Must book at least " << minAdvanceHours << " hours in advance.\n";
            return false;
        }
        return true;
    }
    
    string getPolicyName() const override {
        return "Advance Booking (" + to_string(minAdvanceHours) + " hours notice)";
    }
};


// ========== OBSERVER PATTERN ==========
// Notification system for booking events

class NotificationObserver {
public:
    virtual ~NotificationObserver() = default;
    virtual void update(const string& message) = 0;
    virtual string getObserverName() const = 0;
};

class EmailNotifier : public NotificationObserver {
    string email;
public:
    EmailNotifier(const string& email) : email(email) {}
    
    void update(const string& message) override {
        cout << "[EMAIL to " << email << "]: " << message << endl;
    }
    
    string getObserverName() const override {
        return "EmailNotifier(" + email + ")";
    }
};

class SlackNotifier : public NotificationObserver {
    string channel;
public:
    SlackNotifier(const string& channel) : channel(channel) {}
    
    void update(const string& message) override {
        cout << "[SLACK #" << channel << "]: " << message << endl;
    }
    
    string getObserverName() const override {
        return "SlackNotifier(#" + channel + ")";
    }
};

class SMSNotifier : public NotificationObserver {
    string phoneNumber;
public:
    SMSNotifier(const string& phone) : phoneNumber(phone) {}
    
    void update(const string& message) override {
        cout << "[SMS to " << phoneNumber << "]: " << message << endl;
    }
    
    string getObserverName() const override {
        return "SMSNotifier(" + phoneNumber + ")";
    }
};


#endif // MODELS_HPP

