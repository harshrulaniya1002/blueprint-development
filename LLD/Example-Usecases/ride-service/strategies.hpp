#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include "models.hpp"
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// CONSTANTS
// ============================================================
namespace RideConstants {
    constexpr double BASE_FARE = 50.0;
    constexpr double PER_KM_RATE = 10.0;
    constexpr double SURGE_MULTIPLIER = 2.0;
    constexpr double RATING_DISCOUNT = 0.9;
    constexpr int MIN_RATING_FOR_DISCOUNT = 4;
}

// ============================================================
// PRICING STRATEGY (Strategy Pattern)
// unique_ptr<PricingStrategy>: Trip is sole owner of its pricing.
// Created by TripManager, transferred via move() to the caller.
// ============================================================

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double calculateFare(const TripMetaData* meta) = 0;
    virtual string getName() const = 0;
};

class DefaultPricing : public PricingStrategy {
public:
    double calculateFare(const TripMetaData* meta) override {
        return RideConstants::BASE_FARE + (meta->getDistance() * RideConstants::PER_KM_RATE);
    }
    string getName() const override { return "Default"; }
};

class SurgePricing : public PricingStrategy {
public:
    double calculateFare(const TripMetaData* meta) override {
        return (RideConstants::BASE_FARE + (meta->getDistance() * RideConstants::PER_KM_RATE))
               * RideConstants::SURGE_MULTIPLIER;
    }
    string getName() const override { return "Surge"; }
};

class RatingBasedPricing : public PricingStrategy {
public:
    double calculateFare(const TripMetaData* meta) override {
        double discount = (meta->getRiderRating() >= RideConstants::MIN_RATING_FOR_DISCOUNT)
                          ? RideConstants::RATING_DISCOUNT : 1.0;
        return (RideConstants::BASE_FARE + (meta->getDistance() * RideConstants::PER_KM_RATE)) * discount;
    }
    string getName() const override { return "RatingBased"; }
};

// ============================================================
// MATCHING STRATEGY (Strategy Pattern)
// These access the shared driver pool. Thread safety is handled
// at the Driver level (driver->tryAssign() is atomic).
// No global mutex needed here — each driver locks itself.
// ============================================================

class MatchingStrategy {
public:
    virtual ~MatchingStrategy() = default;
    virtual Driver* matchDriver(const vector<unique_ptr<Driver>>& drivers,
                                const TripMetaData* meta) = 0;
    virtual string getName() const = 0;
};

// Returns first available driver (fast, no preference)
class LeastTimeMatching : public MatchingStrategy {
public:
    Driver* matchDriver(const vector<unique_ptr<Driver>>& drivers,
                        const TripMetaData* meta) override {
        for (auto& d : drivers) {
            // tryAssign is atomic (lock_guard inside driver)
            // PESSIMISTIC: lock driver, check availability, claim if free
            if (d->tryAssign()) return d.get();
        }
        return nullptr;
    }
    string getName() const override { return "LeastTime"; }
};

// Returns highest-rated available driver
class HighRatingMatching : public MatchingStrategy {
public:
    Driver* matchDriver(const vector<unique_ptr<Driver>>& drivers,
                        const TripMetaData* meta) override {
        Driver* best = nullptr;
        int highestRating = 0;

        for (auto& d : drivers) {
            if (d->isAvailable() && d->getRating() > highestRating) {
                highestRating = d->getRating();
                best = d.get();
            }
        }
        // Now try to claim the best driver atomically
        if (best && best->tryAssign()) return best;
        // If someone else grabbed it, fall back to any available
        for (auto& d : drivers) {
            if (d->tryAssign()) return d.get();
        }
        return nullptr;
    }
    string getName() const override { return "HighRating"; }
};

#endif
