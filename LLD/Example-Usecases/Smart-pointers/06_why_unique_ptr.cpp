/*
====================================================================
WHY UNIQUE_PTR? - Decision Making Guide
====================================================================

This file explains EXACTLY when to use unique_ptr vs other options
in the Factory Pattern context
*/

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Payment {
public:
    virtual void process() = 0;
    virtual ~Payment() { cout << "Payment destroyed\n"; }
};

class CreditCard : public Payment {
public:
    void process() override { cout << "CC Payment\n"; }
};

/*
====================================================================
OPTION 1: RAW POINTER (Manual Memory Management)
====================================================================
*/

class PaymentFactory_RawPointer {
public:
    static Payment* createPayment(string type) {
        return new CreditCard();  // ⚠️ PROBLEM: Who owns this?
    }
};

void rawPointerExample() {
    cout << "\n=== RAW POINTER ISSUES ===\n";
    
    // ❌ PROBLEM 1: Unclear ownership
    Payment* payment = PaymentFactory_RawPointer::createPayment("CC");
    payment->process();
    
    // ❌ PROBLEM 2: Easy to forget delete → MEMORY LEAK
    // delete payment;  // What if we forget this?
    
    // ❌ PROBLEM 3: Exception safety
    // If exception occurs before delete, memory leaks!
    
    // ❌ PROBLEM 4: Double delete bugs
    // What if someone else also calls delete?
}

/*
====================================================================
OPTION 2: SHARED_PTR (Shared Ownership)
====================================================================
*/

class PaymentFactory_SharedPtr {
public:
    static shared_ptr<Payment> createPayment(string type) {
        return make_shared<CreditCard>();  // ⚠️ OVERKILL for this use case
    }
};

void sharedPointerExample() {
    cout << "\n=== SHARED_PTR ANALYSIS ===\n";
    
    // ✅ Works but has OVERHEAD
    shared_ptr<Payment> payment = PaymentFactory_SharedPtr::createPayment("CC");
    payment->process();
    
    // ❌ PROBLEM 1: Reference counting overhead
    cout << "Reference count: " << payment.use_count() << "\n";  // Extra memory + CPU overhead
    
    // ❌ PROBLEM 2: Unclear ownership semantics
    // Does factory still own it? Does caller own it? Both? Confusing!
    
    // ❌ PROBLEM 3: Circular reference risk
    // If two objects share pointers to each other → memory leak
    
    // ❌ PROBLEM 4: Unnecessary complexity
    // We don't need multiple owners! Only ONE owner (caller) needed.
}

/*
====================================================================
OPTION 3: UNIQUE_PTR (Exclusive Ownership) ✅ BEST CHOICE
====================================================================
*/

class PaymentFactory_UniquePtr {
public:
    static unique_ptr<Payment> createPayment(string type) {
        return make_unique<CreditCard>();  // ✅ CLEAR: Caller owns this
    }
};

void uniquePointerExample() {
    cout << "\n=== UNIQUE_PTR BENEFITS ===\n";
    
    // ✅ BENEFIT 1: Clear ownership transfer
    unique_ptr<Payment> payment = PaymentFactory_UniquePtr::createPayment("CC");
    // Factory says: "I created this, NOW YOU own it"
    
    payment->process();
    
    // ✅ BENEFIT 2: Automatic cleanup
    // No need to call delete! Automatic when goes out of scope
    
    // ✅ BENEFIT 3: Zero overhead (same as raw pointer)
    cout << "Size of unique_ptr: " << sizeof(payment) << " bytes\n";
    cout << "Size of raw pointer: " << sizeof(Payment*) << " bytes\n";
    // SAME SIZE!
    
    // ✅ BENEFIT 4: Cannot accidentally copy (prevents bugs)
    // unique_ptr<Payment> payment2 = payment;  // ❌ Compiler ERROR
    unique_ptr<Payment> payment2 = move(payment);  // ✅ Must explicitly move
    
    // ✅ BENEFIT 5: Exception safe
    // Even if exception thrown, destructor called automatically
}

/*
====================================================================
DETAILED COMPARISON IN FACTORY PATTERN
====================================================================
*/

void factoryPatternComparison() {
    cout << "\n=== FACTORY PATTERN OWNERSHIP SCENARIOS ===\n";
    
    // SCENARIO 1: Factory creates, caller owns (MOST COMMON)
    // ✅ Use unique_ptr
    {
        unique_ptr<Payment> p = PaymentFactory_UniquePtr::createPayment("CC");
        // Caller has FULL control, will auto-cleanup
    }
    
    // SCENARIO 2: Multiple parts of code need to share object
    // ✅ Use shared_ptr
    // Example: Logger used by multiple threads
    {
        shared_ptr<Payment> p1 = PaymentFactory_SharedPtr::createPayment("CC");
        shared_ptr<Payment> p2 = p1;  // Both can access
        // Deleted only when BOTH go out of scope
    }
    
    // SCENARIO 3: Just passing around, not owning
    // ✅ Use raw pointer or reference
    {
        unique_ptr<Payment> owner = PaymentFactory_UniquePtr::createPayment("CC");
        Payment* observer = owner.get();  // Just looking, not owning
        // observer->process();
    }
}

/*
====================================================================
DECISION TREE: HOW TO IDENTIFY WHICH POINTER TO USE
====================================================================

START HERE
    |
    ├─→ Q1: Do I need to allocate on heap?
    |   ├─ NO → Use stack allocation (Regular object)
    |   └─ YES → Continue
    |
    ├─→ Q2: Do multiple owners need this object?
    |   ├─ YES → Use shared_ptr
    |   └─ NO → Continue
    |
    ├─→ Q3: Do I own this object (responsible for deletion)?
    |   ├─ YES → Use unique_ptr ✅
    |   └─ NO → Use raw pointer/reference (observer)

*/

/*
====================================================================
CONCRETE EXAMPLES: WHEN TO USE WHAT
====================================================================
*/

// Example 1: Factory Pattern → unique_ptr ✅
class VehicleFactory {
public:
    static unique_ptr<Payment> create() {
        return make_unique<CreditCard>();
    }
    // WHY: Factory creates and TRANSFERS ownership to caller
};

// Example 2: Logger shared by multiple threads → shared_ptr ✅
class Logger {
    shared_ptr<Logger> instance;
    // WHY: Multiple threads need access, no single owner
};

// Example 3: Tree node owns children → unique_ptr ✅
struct TreeNode {
    unique_ptr<TreeNode> left;
    unique_ptr<TreeNode> right;
    // WHY: Parent owns children, clear hierarchy
};

// Example 4: Observer pattern (just observing) → raw pointer ✅
class Button {
    void onClick(Payment* handler) {  // Just uses it, doesn't own
        handler->process();
    }
    // WHY: Button doesn't own handler, just calls it
};

// Example 5: Storing polymorphic objects → unique_ptr ✅
class PaymentProcessor {
    vector<unique_ptr<Payment>> history;
    // WHY: Processor owns the payment objects in history
};

/*
====================================================================
VISUAL OWNERSHIP MODEL
====================================================================

FACTORY PATTERN WITH UNIQUE_PTR:
┌─────────────┐
│   Factory   │
└──────┬──────┘
       │ creates
       │ unique_ptr<Payment>
       │
       ↓ TRANSFERS ownership
┌─────────────┐
│   Caller    │  ← Now sole owner
└──────┬──────┘
       │ stores in
       ↓
┌─────────────┐
│   Vector    │  ← Owns collection
└─────────────┘

When vector goes out of scope → All payments deleted automatically

WITH RAW POINTER:
┌─────────────┐
│   Factory   │
└──────┬──────┘
       │ creates Payment*
       │
       ↓ ??? Who owns this ???
┌─────────────┐
│   Caller    │  ← Must remember to delete (easily forgotten)
└─────────────┘

Memory leak if delete forgotten!

WITH SHARED_PTR:
┌─────────────┐
│   Factory   │
└──────┬──────┘
       │ creates shared_ptr<Payment>
       │
       ↓ Shared ownership (overkill)
┌─────────────┐
│   Caller    │  ← Co-owns (unnecessary complexity)
└─────────────┘

Reference counting overhead + unclear semantics

*/

int main() {
    rawPointerExample();
    sharedPointerExample();
    uniquePointerExample();
    factoryPatternComparison();
    
    cout << "\n=== KEY INSIGHT ===\n";
    cout << "Factory Pattern: Factory CREATES → Caller OWNS → unique_ptr ✅\n";
    cout << "One owner, clear transfer, automatic cleanup = PERFECT FIT\n";
    
    return 0;
}
