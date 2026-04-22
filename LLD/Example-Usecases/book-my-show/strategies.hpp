#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include "models.hpp"
#include <memory>
using namespace std;

// ============================================================
// STRATEGY PATTERN: Pricing
// Different pricing based on seat type, time, etc.
// unique_ptr<PricingStrategy>: BookingService is sole owner.
// ============================================================

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double calculatePrice(SeatType type) = 0;
    virtual string getName() const = 0;
};

class StandardPricing : public PricingStrategy {
public:
    double calculatePrice(SeatType type) override {
        switch (type) {
            case SeatType::SILVER:   return 150.0;
            case SeatType::GOLD:     return 250.0;
            case SeatType::PLATINUM: return 400.0;
            default: return 150.0;
        }
    }
    string getName() const override { return "Standard"; }
};

class PeakHourPricing : public PricingStrategy {
    double multiplier;
public:
    PeakHourPricing(double mult = 1.5) : multiplier(mult) {}

    double calculatePrice(SeatType type) override {
        double base;
        switch (type) {
            case SeatType::SILVER:   base = 150.0; break;
            case SeatType::GOLD:     base = 250.0; break;
            case SeatType::PLATINUM: base = 400.0; break;
            default: base = 150.0;
        }
        return base * multiplier;
    }
    string getName() const override { return "PeakHour"; }
};

class DiscountPricing : public PricingStrategy {
    double discount;  // e.g., 0.8 = 20% off
public:
    DiscountPricing(double disc = 0.8) : discount(disc) {}

    double calculatePrice(SeatType type) override {
        double base;
        switch (type) {
            case SeatType::SILVER:   base = 150.0; break;
            case SeatType::GOLD:     base = 250.0; break;
            case SeatType::PLATINUM: base = 400.0; break;
            default: base = 150.0;
        }
        return base * discount;
    }
    string getName() const override { return "Discount"; }
};

// ============================================================
// STRATEGY PATTERN: Seat Selection
// How to auto-pick seats when user doesn't specify exact seats.
// unique_ptr<SeatSelectionStrategy>: BookingService sole owner.
// ============================================================

class SeatSelectionStrategy {
public:
    virtual ~SeatSelectionStrategy() = default;
    virtual vector<string> selectSeats(Show* show, int count, SeatType preferred) = 0;
    virtual string getName() const = 0;
};

// Pick first N available of preferred type
class FirstAvailableSelection : public SeatSelectionStrategy {
public:
    vector<string> selectSeats(Show* show, int count, SeatType preferred) override {
        auto avail = show->getAvailableSeatsByType(preferred);
        if ((int)avail.size() < count) {
            // Fall back to any available
            avail = show->getAvailableSeats();
        }
        vector<string> result;
        for (int i = 0; i < count && i < (int)avail.size(); i++) {
            result.push_back(avail[i]);
        }
        return result;
    }
    string getName() const override { return "FirstAvailable"; }
};

// ============================================================
// OBSERVER PATTERN: Booking Notifications
// shared_ptr<BookingObserver>: service + caller shared ownership.
// ============================================================

class BookingObserver {
public:
    virtual ~BookingObserver() = default;
    virtual void onBookingConfirmed(const Booking& b) = 0;
    virtual void onBookingCancelled(const Booking& b) = 0;
    virtual string getName() const = 0;
};

class EmailNotifier : public BookingObserver {
public:
    void onBookingConfirmed(const Booking& b) override {
        cout << "    [Email] Booking " << b.getId() << " confirmed for "
             << b.getUserId() << " ($" << b.getAmount() << ")" << endl;
    }
    void onBookingCancelled(const Booking& b) override {
        cout << "    [Email] Booking " << b.getId() << " cancelled, refund $"
             << b.getAmount() << endl;
    }
    string getName() const override { return "Email"; }
};

class SMSNotifier : public BookingObserver {
public:
    void onBookingConfirmed(const Booking& b) override {
        cout << "    [SMS] Booking " << b.getId() << " confirmed" << endl;
    }
    void onBookingCancelled(const Booking& b) override {
        cout << "    [SMS] Booking " << b.getId() << " cancelled" << endl;
    }
    string getName() const override { return "SMS"; }
};

#endif
