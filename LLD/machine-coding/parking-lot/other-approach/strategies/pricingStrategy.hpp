#ifndef pricing_strategy_hpp
#define pricing_strategy_hpp
#include<iostream>
#include "utils/utils.hpp"
using namespace std;

class PricingStrategy{
    public:
    virtual double calculateFare(VehicleType type, time_t entryTime, time_t exitTime) =0;
};


class EventBased: public PricingStrategy{
    public:
    double calculateFare(VehicleType type, time_t entryTime, time_t exitTime) =0;
};
class TimeBased: public PricingStrategy{
    public:
    double calculateFare(VehicleType type, time_t entryTime, time_t exitTime) =0;
};


#endif