#include <iostream>
#include <thread>
#include <vector>
#include "managers.hpp"
using namespace std;

int main() {
    srand(42);  // fixed seed for reproducible demo
    auto* svc = PaymentService::getInstance();

    cout << "=== Payment Processing System ===" << endl << endl;

    // ========== 1. OBSERVER SETUP ==========
    // shared_ptr<Observer>: shared between caller (to remove later)
    // and service's observer vector. Both owners -> shared_ptr.
    auto auditObs = make_shared<AuditObserver>();
    auto emailObs = make_shared<EmailObserver>("user@example.com");
    svc->addObserver(auditObs);
    svc->addObserver(emailObs);

    // ========== 2. WALLET SETUP ==========
    // shared_ptr<Wallet>: stored in service map AND passed to
    // WalletProcessor. Both need it alive -> shared_ptr.
    // Each Wallet has its own mutex for thread-safe debit/credit.
    cout << "\n--- Wallets ---" << endl;
    svc->createWallet("UserA", 500.0);
    svc->createWallet("UserB", 300.0);
    svc->createWallet("UserC", 150.0);
    svc->createWallet("SellerA", 0.0, true);
    svc->createWallet("SellerB", 0.0, true);

    // ========== 3. STRATEGY PATTERN: Different payment methods ==========
    cout << "\n--- Payment 1: Credit Card ---" << endl;
    {
        auto event = make_shared<PaymentEvent>("PAY_001", "UserA", PaymentMethod::CREDIT_CARD);
        event->addOrder(make_shared<PaymentOrder>("ORD_001", "SellerA", 100.0));
        event->addOrder(make_shared<PaymentOrder>("ORD_002", "SellerB", 50.0));
        auto proc = make_shared<CreditCardProcessor>("1234567890123456");
        svc->processPayment(event, proc);
    }

    cout << "\n--- Payment 2: UPI ---" << endl;
    {
        auto event = make_shared<PaymentEvent>("PAY_002", "UserB", PaymentMethod::UPI);
        event->addOrder(make_shared<PaymentOrder>("ORD_003", "SellerA", 75.0));
        auto proc = make_shared<UPIProcessor>("user@paytm");
        svc->processPayment(event, proc);
    }

    cout << "\n--- Payment 3: Wallet ---" << endl;
    {
        auto wallet = svc->getWallet("UserA");
        auto event = make_shared<PaymentEvent>("PAY_003", "UserA", PaymentMethod::WALLET);
        event->addOrder(make_shared<PaymentOrder>("ORD_004", "SellerB", 80.0));
        auto proc = make_shared<WalletProcessor>(wallet);
        svc->processPayment(event, proc);
    }

    // ========== 4. CHAIN OF RESPONSIBILITY: Validation failure ==========
    cout << "\n--- Payment 4: Amount too high (should fail validation) ---" << endl;
    {
        auto event = make_shared<PaymentEvent>("PAY_004", "UserC", PaymentMethod::CREDIT_CARD);
        event->addOrder(make_shared<PaymentOrder>("ORD_005", "SellerA", 15000.0));
        auto proc = make_shared<CreditCardProcessor>("9876543210987654");
        svc->processPayment(event, proc);
    }

    // ============================================================
    // 5. CONCURRENT PAYMENTS
    // ----------------------------------------------------------
    // PESSIMISTIC LOCKING IN ACTION:
    // - processPayment acquires serviceMtx briefly to store event
    // - Releases lock before PSP call (expensive external I/O)
    //   so other threads aren't blocked during network calls
    // - Wallet credits use per-wallet locks (Level 2)
    //   so payments to DIFFERENT sellers proceed in parallel
    // - Notifications use snapshot pattern (copy observers under
    //   lock, notify outside) to prevent deadlock
    // ============================================================
    cout << "\n\n--- Concurrent Payments (3 threads) ---" << endl;
    cout << "All three wallet payments hit at once." << endl;
    cout << "Per-wallet locks prevent double-debit." << endl;
    cout << "Seller wallet credits proceed in parallel." << endl << endl;

    vector<thread> threads;

    auto payFn = [&](const string& eventId, const string& userId,
                     const string& sellerId, double amount) {
        auto wallet = svc->getWallet(userId);
        if (!wallet) return;
        auto event = make_shared<PaymentEvent>(eventId, userId, PaymentMethod::WALLET);
        event->addOrder(make_shared<PaymentOrder>(eventId + "_ord", sellerId, amount));
        auto proc = make_shared<WalletProcessor>(wallet);
        svc->processPayment(event, proc);
    };

    threads.emplace_back(payFn, "CONC_001", "UserA", "SellerA", 50.0);
    threads.emplace_back(payFn, "CONC_002", "UserB", "SellerB", 75.0);
    threads.emplace_back(payFn, "CONC_003", "UserC", "SellerA", 100.0);

    for (auto& t : threads) t.join();

    // ========== 6. VIEW RESULTS ==========
    cout << "\n\n--- Final Wallet Balances ---" << endl;
    svc->viewWallet("UserA");
    svc->viewWallet("UserB");
    svc->viewWallet("UserC");
    cout << endl;
    svc->viewWallet("SellerA", true);
    svc->viewWallet("SellerB", true);

    svc->listAllEvents();

    // ============================================================
    // CONCURRENCY SUMMARY:
    // ----------------------------------------------------------
    // Level 1: serviceMtx (PaymentService)
    //   - lock_guard: wallet create, observer add, event store, queries
    //   - Released before PSP call and notifications
    //
    // Level 2: walletMtx (per-Wallet)
    //   - lock_guard: atomic balance check + modify
    //   - Different wallets lock independently (parallel credits)
    //
    // DEADLOCK PREVENTION:
    //   - Always Level 1 before Level 2 (never reverse)
    //   - Release Level 1 before notifications (snapshot pattern)
    //   - Validator chain is immutable after init (no lock needed)
    //
    // WHY PESSIMISTIC?
    //   Money is involved. Can't afford lost updates or double-charge.
    //   Lock first, validate, process, commit. Simple and correct.
    // ============================================================

    cout << "\n=== Done ===" << endl;
    return 0;
}
