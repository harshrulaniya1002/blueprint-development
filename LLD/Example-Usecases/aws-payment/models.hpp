#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <memory>
using namespace std;

// ============================================================
// ENUMS
// ============================================================

enum class PaymentStatus {
    PENDING,
    PROCESSING,
    SUCCESS,
    FAILED,
    REFUNDED
};

enum class PaymentMethod {
    CREDIT_CARD,
    DEBIT_CARD,
    UPI,
    WALLET,
    NET_BANKING
};

inline string statusToString(PaymentStatus s) {
    switch (s) {
        case PaymentStatus::PENDING:    return "PENDING";
        case PaymentStatus::PROCESSING: return "PROCESSING";
        case PaymentStatus::SUCCESS:    return "SUCCESS";
        case PaymentStatus::FAILED:     return "FAILED";
        case PaymentStatus::REFUNDED:   return "REFUNDED";
        default: return "UNKNOWN";
    }
}

inline string methodToString(PaymentMethod m) {
    switch (m) {
        case PaymentMethod::CREDIT_CARD:  return "CREDIT_CARD";
        case PaymentMethod::DEBIT_CARD:   return "DEBIT_CARD";
        case PaymentMethod::UPI:          return "UPI";
        case PaymentMethod::WALLET:       return "WALLET";
        case PaymentMethod::NET_BANKING:  return "NET_BANKING";
        default: return "UNKNOWN";
    }
}

// ============================================================
// WALLET
// Has its own mutex: multiple threads can debit/credit
// the same wallet concurrently (e.g., seller wallet receiving
// credits from different payment events simultaneously).
// PESSIMISTIC: lock before any balance check/modify.
// ============================================================

class Wallet {
    string ownerId;
    double balance;
    vector<string> history;
    mutable mutex walletMtx;

public:
    Wallet(const string& owner, double initial = 0.0)
        : ownerId(owner), balance(initial) {}

    string getOwnerId() const { return ownerId; }

    double getBalance() {
        lock_guard<mutex> lock(walletMtx);
        return balance;
    }

    bool credit(double amount, const string& txnId) {
        lock_guard<mutex> lock(walletMtx);
        if (amount <= 0) return false;
        balance += amount;
        history.push_back("CREDIT: $" + to_string(amount) + " [" + txnId + "]");
        return true;
    }

    bool debit(double amount, const string& txnId) {
        // PESSIMISTIC: lock, check balance, debit — all atomic
        lock_guard<mutex> lock(walletMtx);
        if (amount <= 0 || balance < amount) return false;
        balance -= amount;
        history.push_back("DEBIT: $" + to_string(amount) + " [" + txnId + "]");
        return true;
    }

    void display() {
        lock_guard<mutex> lock(walletMtx);
        cout << "  Wallet[" << ownerId << "] Balance: $" << balance << endl;
        int start = max(0, (int)history.size() - 5);
        for (int i = start; i < (int)history.size(); i++) {
            cout << "    " << history[i] << endl;
        }
    }
};

// ============================================================
// PAYMENT ORDER
// Represents a single order within a payment event.
// No mutex needed: owned by a single PaymentEvent, modified
// only under the service-level lock.
// ============================================================

class PaymentOrder {
    string orderId;
    string sellerId;
    double amount;
    PaymentStatus status;

public:
    PaymentOrder(const string& orderId, const string& sellerId, double amount)
        : orderId(orderId), sellerId(sellerId), amount(amount),
          status(PaymentStatus::PENDING) {}

    string getOrderId() const { return orderId; }
    string getSellerId() const { return sellerId; }
    double getAmount() const { return amount; }
    PaymentStatus getStatus() const { return status; }
    void setStatus(PaymentStatus s) { status = s; }

    void display() const {
        cout << "    Order[" << orderId << "] Seller: " << sellerId
             << " $" << amount << " " << statusToString(status) << endl;
    }
};

// ============================================================
// PAYMENT EVENT
// Groups multiple orders into one payment transaction.
// shared_ptr<PaymentOrder>: event owns orders, but orders are
// also passed to observers for notification. Shared ownership.
// ============================================================

class PaymentEvent {
    string eventId;
    string userId;
    vector<shared_ptr<PaymentOrder>> orders;
    PaymentMethod method;
    PaymentStatus status;

public:
    PaymentEvent(const string& id, const string& userId, PaymentMethod method)
        : eventId(id), userId(userId), method(method),
          status(PaymentStatus::PENDING) {}

    void addOrder(shared_ptr<PaymentOrder> order) { orders.push_back(order); }

    string getEventId() const { return eventId; }
    string getUserId() const { return userId; }
    PaymentMethod getMethod() const { return method; }
    PaymentStatus getStatus() const { return status; }
    void setStatus(PaymentStatus s) { status = s; }
    const vector<shared_ptr<PaymentOrder>>& getOrders() const { return orders; }

    double getTotalAmount() const {
        double total = 0;
        for (const auto& o : orders) total += o->getAmount();
        return total;
    }

    void display() const {
        cout << "  Event[" << eventId << "] User: " << userId
             << " Method: " << methodToString(method)
             << " Total: $" << getTotalAmount()
             << " Status: " << statusToString(status) << endl;
        for (const auto& o : orders) o->display();
    }
};

#endif
