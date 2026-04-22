#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include<iostream>
using namespace std;


class PaymentStrategy{
    public:
    virtual bool pay(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class CashPayment: public PaymentStrategy {
   public:
   bool pay(double amount) override {
       cout << "Paid $" << amount << " in cash." << endl;
       return true;
   }
};

class CardPayment: public PaymentStrategy {
   public:
   bool pay(double amount) override {
       cout << "Paid $" << amount << " by card." << endl;
       return true;
   }
};

class RateStrategy{
    public:
    virtual double calculateRate(long long minutes) = 0;
    virtual ~RateStrategy() {}
};

class HourlyRateStrategy: public RateStrategy {
    double ratePerHour;
    public:
    HourlyRateStrategy(double rate = 10.0) : ratePerHour(rate) {}
    double calculateRate(long long minutes) override {
        double hours = (minutes + 59) / 60.0;
        double amount = hours * ratePerHour;
        cout << "Hourly rate: " << ratePerHour << "/hr x " << hours << " hrs = $" << amount << endl;
        return amount;
    }
};

class FlatRateStrategy: public RateStrategy {
    double flatRate;
    public:
    FlatRateStrategy(double rate = 50.0) : flatRate(rate) {}
    double calculateRate(long long minutes) override {
        cout << "Flat rate applied: $" << flatRate << " (duration: " << minutes << " min)" << endl;
        return flatRate;
    }
};

#endif