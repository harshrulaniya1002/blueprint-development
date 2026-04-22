#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include "strategies.hpp"
#include <map>
#include <mutex>
#include <thread>
using namespace std;

// ============================================================
// LOCKING STRATEGY: PESSIMISTIC (local locks, single server)
// ----------------------------------------------------------
// Level 1: serviceMtx (PaymentService)
//   -> protects paymentEvents map, wallet maps, observers vector
//   -> lock_guard for simple reads/writes
//   -> unique_lock in processPayment (release before PSP call
//      and notification — both are expensive external I/O)
//
// Level 2: walletMtx (per-Wallet)
//   -> protects individual wallet balance
//   -> lock_guard inside debit/credit (atomic check-and-modify)
//   -> Multiple concurrent payments can credit DIFFERENT seller
//      wallets in parallel (no contention)
//
// WHY PESSIMISTIC?
//   Payments involve money — we can't afford lost updates or
//   double-charges. Lock first, validate, process, commit.
//   Optimistic locking could cause a charge to succeed at PSP
//   but fail the local commit (version mismatch) — messy refund.
//
// DEADLOCK PREVENTION:
//   - Always Level 1 before Level 2
//   - Release Level 1 before PSP call and notification
//   - notifyObservers takes snapshot under lock, notifies outside
//
// SMART POINTERS:
//   shared_ptr<Wallet>        -> service map + WalletProcessor both hold ref
//   shared_ptr<PaymentEvent>  -> service map + caller both hold ref
//   shared_ptr<PaymentObserver> -> service vector + caller (to remove later)
//   shared_ptr<PaymentValidator> -> chain of responsibility links
//   shared_ptr<PaymentProcessorStrategy> -> caller creates, passes to service
// ============================================================

class PaymentService {
    static mutex singletonMtx;
    mutable mutex serviceMtx;

    map<string, shared_ptr<PaymentEvent>> events;
    map<string, shared_ptr<Wallet>> userWallets;
    map<string, shared_ptr<Wallet>> sellerWallets;
    vector<shared_ptr<PaymentObserver>> observers;
    shared_ptr<PaymentValidator> validatorChain;

    PaymentService() {
        auto amountVal = make_shared<AmountValidator>(10000.0);
        auto fraudVal = make_shared<FraudValidator>();
        amountVal->setNext(fraudVal);
        validatorChain = amountVal;
    }

    // Notify OUTSIDE serviceMtx (snapshot pattern, avoids deadlock)
    void notifySuccess(shared_ptr<PaymentEvent> event) {
        vector<shared_ptr<PaymentObserver>> snap;
        { lock_guard<mutex> lock(serviceMtx); snap = observers; }
        for (auto& obs : snap) obs->onSuccess(event);
    }

    void notifyFailure(shared_ptr<PaymentEvent> event, const string& reason) {
        vector<shared_ptr<PaymentObserver>> snap;
        { lock_guard<mutex> lock(serviceMtx); snap = observers; }
        for (auto& obs : snap) obs->onFailure(event, reason);
    }

public:
    static PaymentService* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static PaymentService instance;
        return &instance;
    }

    // --- Observer management (lock_guard: simple scope) ---
    void addObserver(shared_ptr<PaymentObserver> obs) {
        lock_guard<mutex> lock(serviceMtx);
        observers.push_back(obs);
        cout << "[PaymentService] Observer added: " << obs->getName() << endl;
    }

    // --- Wallet management (lock_guard: simple scope) ---
    void createWallet(const string& owner, double balance, bool isSeller = false) {
        lock_guard<mutex> lock(serviceMtx);
        auto w = make_shared<Wallet>(owner, balance);
        (isSeller ? sellerWallets : userWallets)[owner] = w;
        cout << "[PaymentService] " << (isSeller ? "Seller" : "User")
             << " wallet created: " << owner << " ($" << balance << ")" << endl;
    }

    shared_ptr<Wallet> getWallet(const string& owner, bool isSeller = false) {
        lock_guard<mutex> lock(serviceMtx);
        auto& m = isSeller ? sellerWallets : userWallets;
        auto it = m.find(owner);
        return (it != m.end()) ? it->second : nullptr;
    }

    // --- Core: Process Payment ---
    // unique_lock: we release serviceMtx before the PSP call (external I/O)
    // and before notifications. This lets other threads process payments
    // to different sellers in parallel.
    bool processPayment(shared_ptr<PaymentEvent> event,
                        shared_ptr<PaymentProcessorStrategy> processor) {

        cout << "\n=== Processing " << event->getEventId() << " ===" << endl;
        event->display();

        // Step 1: Store event under lock
        {
            lock_guard<mutex> lock(serviceMtx);
            events[event->getEventId()] = event;
            event->setStatus(PaymentStatus::PROCESSING);
        }

        // Step 2: Validate (validator chain is immutable after setup, no lock needed)
        cout << "  [Validation]" << endl;
        if (!validatorChain->validate(event)) {
            event->setStatus(PaymentStatus::FAILED);
            notifyFailure(event, "Validation failed");
            return false;
        }

        // Step 3: Call external PSP — NO lock held (expensive I/O)
        // This is WHY we use unique_lock pattern: release before I/O
        // so other threads aren't blocked during network calls.
        cout << "  [PSP Call]" << endl;
        double total = event->getTotalAmount();
        bool pspOk = processor->processPayment(event, total);

        if (!pspOk) {
            event->setStatus(PaymentStatus::FAILED);
            for (auto& o : event->getOrders()) o->setStatus(PaymentStatus::FAILED);
            notifyFailure(event, "PSP declined");
            return false;
        }

        // Step 4: Credit seller wallets
        // Each wallet has its own lock (Level 2). No serviceMtx needed.
        // Concurrent payments to DIFFERENT sellers proceed in parallel.
        cout << "  [Wallet Credits]" << endl;
        for (auto& order : event->getOrders()) {
            auto sellerWallet = getWallet(order->getSellerId(), true);
            if (sellerWallet) {
                sellerWallet->credit(order->getAmount(), order->getOrderId());
                order->setStatus(PaymentStatus::SUCCESS);
                cout << "    Credited $" << order->getAmount()
                     << " to " << order->getSellerId() << endl;
            } else {
                order->setStatus(PaymentStatus::FAILED);
                cout << "    Seller wallet not found: " << order->getSellerId() << endl;
            }
        }

        event->setStatus(PaymentStatus::SUCCESS);

        // Step 5: Notify outside lock (snapshot pattern)
        cout << "  [Notifications]" << endl;
        notifySuccess(event);

        cout << "=== " << event->getEventId() << " Complete ===" << endl;
        return true;
    }

    // --- Query operations ---
    void viewEvent(const string& id) {
        lock_guard<mutex> lock(serviceMtx);
        auto it = events.find(id);
        if (it != events.end()) it->second->display();
        else cout << "Event not found: " << id << endl;
    }

    void viewWallet(const string& owner, bool isSeller = false) {
        auto w = getWallet(owner, isSeller);
        if (w) w->display();
        else cout << "Wallet not found: " << owner << endl;
    }

    void listAllEvents() {
        lock_guard<mutex> lock(serviceMtx);
        cout << "\n=== All Payment Events ===" << endl;
        for (auto& [id, ev] : events) {
            ev->display();
            cout << "  ---" << endl;
        }
    }

    PaymentService(const PaymentService&) = delete;
    PaymentService& operator=(const PaymentService&) = delete;
};

mutex PaymentService::singletonMtx;

#endif
