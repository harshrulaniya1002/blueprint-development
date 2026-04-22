/*
====================================================================
POLYMORPHISM & STRATEGY PATTERN WITH UNIQUE_PTR
====================================================================

EXTREMELY COMMON IN MACHINE CODING ROUNDS:
- Pricing strategies (Uber/Ola surge pricing)
- Matching strategies (Ride matching algorithms)
- Notification strategies
- Sorting/searching strategies
- Discount/promotion strategies

WHY UNIQUE_PTR:
- Store polymorphic objects
- Dynamic strategy switching
- Clean memory management
*/

#include <iostream>
#include <memory>
#include <string>
#include <cmath>
using namespace std;

// ========== Ride Pricing Strategy Example ==========
class PricingStrategy {
public:
    virtual double calculatePrice(double distance, int time) = 0;
    virtual string getName() = 0;
    virtual ~PricingStrategy() = default;
};

class NormalPricing : public PricingStrategy {
public:
    double calculatePrice(double distance, int time) override {
        return distance * 10.0; // Rs 10 per km
    }
    
    string getName() override { return "Normal"; }
};

class SurgePricing : public PricingStrategy {
    double surgeMultiplier;
    
public:
    SurgePricing(double multiplier) : surgeMultiplier(multiplier) {}
    
    double calculatePrice(double distance, int time) override {
        return distance * 10.0 * surgeMultiplier;
    }
    
    string getName() override { 
        return "Surge (x" + to_string(surgeMultiplier) + ")"; 
    }
};

class PeakHourPricing : public PricingStrategy {
public:
    double calculatePrice(double distance, int time) override {
        double basePrice = distance * 10.0;
        double timeCost = time * 2.0; // Rs 2 per minute
        return basePrice + timeCost;
    }
    
    string getName() override { return "Peak Hour"; }
};

class RideService {
    unique_ptr<PricingStrategy> currentStrategy;
    
public:
    RideService() {
        currentStrategy = make_unique<NormalPricing>(); // Default strategy
    }
    
    void setPricingStrategy(unique_ptr<PricingStrategy> strategy) {
        currentStrategy = move(strategy); // Switch strategy
    }
    
    void bookRide(double distance, int estimatedTime) {
        double price = currentStrategy->calculatePrice(distance, estimatedTime);
        cout << "Ride booked | Strategy: " << currentStrategy->getName() 
             << " | Distance: " << distance << "km | Time: " << estimatedTime 
             << "min | Price: Rs." << price << "\n";
    }
};

// ========== Notification Strategy Example ==========
class NotificationStrategy {
public:
    virtual void notify(const string& message) = 0;
    virtual ~NotificationStrategy() = default;
};

class EmailNotification : public NotificationStrategy {
public:
    void notify(const string& message) override {
        cout << "[EMAIL] " << message << "\n";
    }
};

class SMSNotification : public NotificationStrategy {
public:
    void notify(const string& message) override {
        cout << "[SMS] " << message << "\n";
    }
};

class PushNotification : public NotificationStrategy {
public:
    void notify(const string& message) override {
        cout << "[PUSH] " << message << "\n";
    }
};

class NotificationService {
    unique_ptr<NotificationStrategy> strategy;
    
public:
    void setStrategy(unique_ptr<NotificationStrategy> newStrategy) {
        strategy = move(newStrategy);
    }
    
    void sendNotification(const string& message) {
        if(strategy) {
            strategy->notify(message);
        } else {
            cout << "[ERROR] No notification strategy set!\n";
        }
    }
};

// ========== Discount Strategy Example (E-commerce) ==========
class DiscountStrategy {
public:
    virtual double applyDiscount(double price) = 0;
    virtual string getDescription() = 0;
    virtual ~DiscountStrategy() = default;
};

class NoDiscount : public DiscountStrategy {
public:
    double applyDiscount(double price) override { return price; }
    string getDescription() override { return "No Discount"; }
};

class PercentageDiscount : public DiscountStrategy {
    double percentage;
public:
    PercentageDiscount(double p) : percentage(p) {}
    
    double applyDiscount(double price) override {
        return price * (1.0 - percentage / 100.0);
    }
    
    string getDescription() override {
        return to_string((int)percentage) + "% OFF";
    }
};

class FlatDiscount : public DiscountStrategy {
    double amount;
public:
    FlatDiscount(double a) : amount(a) {}
    
    double applyDiscount(double price) override {
        return max(0.0, price - amount);
    }
    
    string getDescription() override {
        return "Flat Rs." + to_string((int)amount) + " OFF";
    }
};

class ShoppingCart {
    double totalPrice;
    unique_ptr<DiscountStrategy> discountStrategy;
    
public:
    ShoppingCart() : totalPrice(0) {
        discountStrategy = make_unique<NoDiscount>();
    }
    
    void addItem(double price) {
        totalPrice += price;
    }
    
    void applyDiscount(unique_ptr<DiscountStrategy> strategy) {
        discountStrategy = move(strategy);
    }
    
    void checkout() {
        double finalPrice = discountStrategy->applyDiscount(totalPrice);
        cout << "\n=== Checkout ===\n";
        cout << "Original Price: Rs." << totalPrice << "\n";
        cout << "Discount: " << discountStrategy->getDescription() << "\n";
        cout << "Final Price: Rs." << finalPrice << "\n";
    }
};

int main() {
    cout << "=== RIDE PRICING DEMO ===\n";
    RideService rideService;
    
    // Normal pricing
    rideService.bookRide(10.0, 20);
    
    // Switch to surge pricing
    rideService.setPricingStrategy(make_unique<SurgePricing>(1.5));
    rideService.bookRide(10.0, 20);
    
    // Switch to peak hour pricing
    rideService.setPricingStrategy(make_unique<PeakHourPricing>());
    rideService.bookRide(10.0, 20);
    
    cout << "\n=== NOTIFICATION DEMO ===\n";
    NotificationService notifier;
    
    notifier.setStrategy(make_unique<EmailNotification>());
    notifier.sendNotification("Your order has been shipped!");
    
    notifier.setStrategy(make_unique<SMSNotification>());
    notifier.sendNotification("Your OTP is 123456");
    
    notifier.setStrategy(make_unique<PushNotification>());
    notifier.sendNotification("New message received");
    
    cout << "\n=== SHOPPING CART DEMO ===\n";
    ShoppingCart cart;
    cart.addItem(500);
    cart.addItem(300);
    cart.addItem(200);
    
    // No discount
    cart.checkout();
    
    // Apply percentage discount
    cart.applyDiscount(make_unique<PercentageDiscount>(20));
    cart.checkout();
    
    // Apply flat discount
    cart.applyDiscount(make_unique<FlatDiscount>(150));
    cart.checkout();
    
    return 0;
}
