/*
====================================================================
FACTORY PATTERN WITH UNIQUE_PTR
====================================================================

COMMON IN MACHINE CODING ROUNDS:
- Payment systems (different payment methods)
- Vehicle rental systems (Car, Bike, Truck)
- Notification systems (Email, SMS, Push)
- Shape drawing systems

WHY UNIQUE_PTR HERE:
- Factory creates and transfers ownership to caller
- Caller is responsible for object lifetime
- Clear ownership semantics
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// ========== Payment System Example ==========
class Payment {
public:
    virtual void processPayment(double amount) = 0;
    virtual string getType() = 0;
    virtual ~Payment() {
        cout << "Payment destroyed\n";
    }
};

class CreditCardPayment : public Payment {
    string cardNumber;
public:
    CreditCardPayment(const string& card) : cardNumber(card) {}
    
    void processPayment(double amount) override {
        cout << "Processing $" << amount << " via Credit Card " << cardNumber << "\n";
    }
    
    string getType() override { return "CreditCard"; }
};

class UPIPayment : public Payment {
    string upiId;
public:
    UPIPayment(const string& id) : upiId(id) {}
    
    void processPayment(double amount) override {
        cout << "Processing Rs." << amount << " via UPI " << upiId << "\n";
    }
    
    string getType() override { return "UPI"; }
};

class WalletPayment : public Payment {
    string walletName;
public:
    WalletPayment(const string& name) : walletName(name) {}
    
    void processPayment(double amount) override {
        cout << "Processing $" << amount << " via " << walletName << " Wallet\n";
    }
    
    string getType() override { return "Wallet"; }
};

// ========== Factory Class ==========
class PaymentFactory {
public:
    // Returns unique_ptr - transfers ownership to caller
    static unique_ptr<Payment> createPayment(const string& type, const string& details) {
        if(type == "CREDIT_CARD") {
            return make_unique<CreditCardPayment>(details);
        }
        else if(type == "UPI") {
            return make_unique<UPIPayment>(details);
        }
        else if(type == "WALLET") {
            return make_unique<WalletPayment>(details);
        }
        return nullptr; // Or throw exception
    }
};

// ========== Payment Processor ==========
class PaymentProcessor {
    vector<unique_ptr<Payment>> paymentHistory;
    
public:
    void processTransaction(const string& paymentType, const string& details, double amount) {
        // Create payment method using factory
        unique_ptr<Payment> payment = PaymentFactory::createPayment(paymentType, details);
        
        if(payment) {
            payment->processPayment(amount);
            paymentHistory.push_back(move(payment)); // Store for history
        } else {
            cout << "Invalid payment type!\n";
        }
    }
    
    void showHistory() {
        cout << "\n=== Payment History ===\n";
        for(const auto& payment : paymentHistory) {
            cout << "- " << payment->getType() << "\n";
        }
    }
};

int main() {
    cout << "=== PAYMENT SYSTEM DEMO ===\n\n";
    
    PaymentProcessor processor;
    
    // Process different types of payments
    processor.processTransaction("CREDIT_CARD", "4532-1234-5678-9010", 500.0);
    processor.processTransaction("UPI", "user@upi", 1500.0);
    processor.processTransaction("WALLET", "PayTM", 250.0);
    
    processor.showHistory();
    
    cout << "\n=== PROGRAM ENDING (all payments auto-cleaned) ===\n";
    return 0;
}

/*
OUTPUT:
=== PAYMENT SYSTEM DEMO ===

Processing $500 via Credit Card 4532-1234-5678-9010
Processing Rs.1500 via UPI user@upi
Processing $250 via PayTM Wallet

=== Payment History ===
- CreditCard
- UPI
- Wallet

=== PROGRAM ENDING (all payments auto-cleaned) ===
Payment destroyed
Payment destroyed
Payment destroyed
*/
