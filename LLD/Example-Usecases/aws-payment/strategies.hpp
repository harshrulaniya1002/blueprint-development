#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include "models.hpp"
#include <memory>
#include <iostream>
using namespace std;

// ============================================================
// STRATEGY PATTERN: Payment Processor
// unique_ptr<PaymentProcessorStrategy> when only one owner,
// shared_ptr when passed to processPayment (service + caller).
// ============================================================

class PaymentProcessorStrategy {
public:
    virtual ~PaymentProcessorStrategy() = default;
    virtual bool processPayment(shared_ptr<PaymentEvent> event, double amount) = 0;
    virtual string getName() const = 0;
};

class CreditCardProcessor : public PaymentProcessorStrategy {
    string cardNumber;
public:
    CreditCardProcessor(const string& card) : cardNumber(card) {}

    bool processPayment(shared_ptr<PaymentEvent> event, double amount) override {
        cout << "    [CreditCard] Processing $" << amount
             << " on card ****" << cardNumber.substr(cardNumber.length() - 4) << endl;
        // Simulate external PSP call (90% success)
        bool success = (rand() % 10) < 9;
        cout << "    [CreditCard] " << (success ? "OK" : "DECLINED") << endl;
        return success;
    }
    string getName() const override { return "CreditCard"; }
};

class UPIProcessor : public PaymentProcessorStrategy {
    string upiId;
public:
    UPIProcessor(const string& upi) : upiId(upi) {}

    bool processPayment(shared_ptr<PaymentEvent> event, double amount) override {
        cout << "    [UPI] Processing $" << amount << " via " << upiId << endl;
        bool success = (rand() % 10) < 9;
        cout << "    [UPI] " << (success ? "OK" : "TIMEOUT") << endl;
        return success;
    }
    string getName() const override { return "UPI"; }
};

class WalletProcessor : public PaymentProcessorStrategy {
    shared_ptr<Wallet> wallet;  // shared_ptr: wallet also lives in service's map
public:
    WalletProcessor(shared_ptr<Wallet> w) : wallet(w) {}

    bool processPayment(shared_ptr<PaymentEvent> event, double amount) override {
        cout << "    [Wallet] Processing $" << amount
             << " (Balance: $" << wallet->getBalance() << ")" << endl;
        // Wallet::debit is thread-safe (has its own mutex)
        bool success = wallet->debit(amount, event->getEventId());
        cout << "    [Wallet] " << (success ? "OK" : "INSUFFICIENT BALANCE") << endl;
        return success;
    }
    string getName() const override { return "Wallet"; }
};

// ============================================================
// CHAIN OF RESPONSIBILITY: Payment Validation
// shared_ptr<PaymentValidator>: validators are chained via
// shared_ptr links. Service holds the head of the chain.
// ============================================================

class PaymentValidator {
protected:
    shared_ptr<PaymentValidator> next;
public:
    virtual ~PaymentValidator() = default;
    void setNext(shared_ptr<PaymentValidator> n) { next = n; }

    virtual bool validate(shared_ptr<PaymentEvent> event) {
        if (next) return next->validate(event);
        return true;
    }
};

class AmountValidator : public PaymentValidator {
    double maxAmount;
public:
    AmountValidator(double max) : maxAmount(max) {}

    bool validate(shared_ptr<PaymentEvent> event) override {
        double total = event->getTotalAmount();
        if (total <= 0 || total > maxAmount) {
            cout << "    [Validate] FAIL: amount $" << total
                 << " (max $" << maxAmount << ")" << endl;
            return false;
        }
        cout << "    [Validate] OK: amount" << endl;
        return PaymentValidator::validate(event);
    }
};

class FraudValidator : public PaymentValidator {
public:
    bool validate(shared_ptr<PaymentEvent> event) override {
        bool fraud = (rand() % 100) < 5;  // 5% fraud rate
        if (fraud) {
            cout << "    [Validate] FAIL: fraud detected" << endl;
            return false;
        }
        cout << "    [Validate] OK: fraud check" << endl;
        return PaymentValidator::validate(event);
    }
};

// ============================================================
// OBSERVER PATTERN: Payment Notifications
// shared_ptr<PaymentObserver>: shared between service's vector
// and caller (who may want to remove it later).
// ============================================================

class PaymentObserver {
public:
    virtual ~PaymentObserver() = default;
    virtual void onSuccess(shared_ptr<PaymentEvent> event) = 0;
    virtual void onFailure(shared_ptr<PaymentEvent> event, const string& reason) = 0;
    virtual string getName() const = 0;
};

class AuditObserver : public PaymentObserver {
public:
    void onSuccess(shared_ptr<PaymentEvent> event) override {
        cout << "    [Audit] SUCCESS: " << event->getEventId()
             << " $" << event->getTotalAmount() << endl;
    }
    void onFailure(shared_ptr<PaymentEvent> event, const string& reason) override {
        cout << "    [Audit] FAILED: " << event->getEventId()
             << " reason: " << reason << endl;
    }
    string getName() const override { return "Audit"; }
};

class EmailObserver : public PaymentObserver {
    string email;
public:
    EmailObserver(const string& e) : email(e) {}

    void onSuccess(shared_ptr<PaymentEvent> event) override {
        cout << "    [Email -> " << email << "] Payment "
             << event->getEventId() << " succeeded ($"
             << event->getTotalAmount() << ")" << endl;
    }
    void onFailure(shared_ptr<PaymentEvent> event, const string& reason) override {
        cout << "    [Email -> " << email << "] Payment "
             << event->getEventId() << " failed: " << reason << endl;
    }
    string getName() const override { return "Email(" + email + ")"; }
};

#endif
