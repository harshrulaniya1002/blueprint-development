# 🎯 UNIQUE_PTR Complete Guide for Machine Coding Rounds

## 📚 What is unique_ptr?

`unique_ptr` is a smart pointer that **owns and manages** an object through a pointer and **deletes** it when the `unique_ptr` goes out of scope.

### Key Features:
- ✅ **Exclusive Ownership** - Only one owner at a time
- ✅ **Automatic Cleanup** - No manual `delete` needed
- ✅ **Move-Only** - Cannot be copied (prevents bugs)
- ✅ **Zero Overhead** - Same performance as raw pointer
- ✅ **Exception Safe** - RAII guarantees cleanup

---

## 🎓 When to Use unique_ptr

### ✅ USE unique_ptr when:

| Scenario | Reason |
|----------|--------|
| **Factory Pattern** | Return newly created objects with clear ownership |
| **Tree/Graph Structures** | Parent owns children, automatic recursive cleanup |
| **Resource Management** | Files, connections, locks - RAII pattern |
| **Polymorphic Objects** | Store base class pointers to derived objects |
| **STL Containers** | Store dynamically allocated objects safely |
| **Strategy Pattern** | Dynamically switch implementations |

### ❌ DON'T USE unique_ptr when:

| Scenario | Use Instead |
|----------|-------------|
| Multiple owners needed | `shared_ptr` |
| Just observing (not owning) | Raw pointer or reference |
| Stack allocation works | Regular object on stack |
| Passing to function temporarily | Pass by reference |

---

## 📖 Core Syntax

```cpp
#include <memory>

// Create unique_ptr
unique_ptr<int> ptr1 = make_unique<int>(42);  // ✅ Preferred (C++14+)
unique_ptr<int> ptr2(new int(42));            // ⚠️ Works but less safe

// Access value
cout << *ptr1;           // Dereference
cout << ptr1.get();      // Get raw pointer

// Transfer ownership
unique_ptr<int> ptr3 = move(ptr1);  // ptr1 becomes null

// Check if valid
if(ptr1) { /* ... */ }

// Release ownership (returns raw pointer, doesn't delete)
int* raw = ptr1.release();

// Reset (deletes old, takes new)
ptr1.reset(new int(100));
```

---

## 🚀 Machine Coding Round Examples

### 1️⃣ Factory Pattern (Payment System)

```cpp
class Payment { virtual void process() = 0; };
class UPI : public Payment { void process() { /*...*/ } };

class PaymentFactory {
public:
    static unique_ptr<Payment> create(string type) {
        if(type == "UPI") return make_unique<UPI>();
        // ...
    }
};

// Usage
unique_ptr<Payment> payment = PaymentFactory::create("UPI");
payment->process();
```

### 2️⃣ Binary Tree (Automatic Cleanup)

```cpp
struct TreeNode {
    int val;
    unique_ptr<TreeNode> left;
    unique_ptr<TreeNode> right;
    
    TreeNode(int v) : val(v) {}
};

class BinaryTree {
    unique_ptr<TreeNode> root;
public:
    void insert(int val) {
        root = insertHelper(move(root), val);
    }
};
// Entire tree automatically deleted when BinaryTree is destroyed!
```

### 3️⃣ Strategy Pattern (Pricing)

```cpp
class PricingStrategy {
    virtual double calculate(double dist) = 0;
};

class RideService {
    unique_ptr<PricingStrategy> strategy;
public:
    void setStrategy(unique_ptr<PricingStrategy> s) {
        strategy = move(s);  // Switch strategy dynamically
    }
};
```

### 4️⃣ RAII (Resource Management)

```cpp
class DatabaseConnection {
public:
    DatabaseConnection() { /* connect */ }
    ~DatabaseConnection() { /* disconnect */ }
};

void query() {
    unique_ptr<DatabaseConnection> conn = 
        make_unique<DatabaseConnection>();
    // Use conn...
    // Automatically disconnected when function ends!
}
```

---

## 🎯 Common Patterns in Coding Rounds

### Pattern 1: Factory Returns unique_ptr
```cpp
unique_ptr<Vehicle> VehicleFactory::create(string type);
```
**When:** Creating objects of different types

### Pattern 2: Container of unique_ptr
```cpp
vector<unique_ptr<Task>> tasks;
tasks.push_back(make_unique<Task>());
```
**When:** Managing collection of objects

### Pattern 3: Ownership Transfer
```cpp
unique_ptr<Order> processOrder(unique_ptr<Order> order) {
    // Process...
    return order;  // Transfer back
}
```
**When:** Functions need to take ownership

### Pattern 4: Parent Owns Children
```cpp
struct Node {
    vector<unique_ptr<Node>> children;
};
```
**When:** Tree structures, file systems

---

## ⚠️ Common Mistakes

### ❌ WRONG: Trying to Copy
```cpp
unique_ptr<int> p1 = make_unique<int>(42);
unique_ptr<int> p2 = p1;  // ❌ ERROR: Cannot copy!
```

### ✅ CORRECT: Use Move
```cpp
unique_ptr<int> p1 = make_unique<int>(42);
unique_ptr<int> p2 = move(p1);  // ✅ OK: Transfer ownership
```

### ❌ WRONG: Double Delete
```cpp
int* raw = new int(42);
unique_ptr<int> p1(raw);
unique_ptr<int> p2(raw);  // ❌ ERROR: Both will try to delete!
```

### ✅ CORRECT: One Owner
```cpp
unique_ptr<int> p1 = make_unique<int>(42);
int* raw = p1.get();  // ✅ OK: Just observe, don't own
```

---

## 🏆 Interview Tips

1. **Always use `make_unique`** instead of `new` (safer, exception-safe)
2. **Store in containers** for collections: `vector<unique_ptr<T>>`
3. **Return from factory** methods to transfer ownership
4. **Use for polymorphism** when storing base class pointers
5. **Check for nullptr** before dereferencing: `if(ptr) { ... }`

---

## 📝 Quick Cheat Sheet

| Operation | Syntax |
|-----------|--------|
| Create | `make_unique<T>(args)` |
| Access | `*ptr` or `ptr->member` |
| Get raw pointer | `ptr.get()` |
| Check validity | `if(ptr)` or `if(ptr != nullptr)` |
| Transfer ownership | `move(ptr)` |
| Release ownership | `ptr.release()` |
| Delete and replace | `ptr.reset(new_ptr)` |
| Delete | `ptr.reset()` or let go out of scope |

---

## 🎪 Demo Files

1. **01_basic_unique_ptr.cpp** - Fundamentals and move semantics
2. **02_factory_pattern.cpp** - Payment system with factory
3. **03_tree_graph_structures.cpp** - Trees, file systems, graphs
4. **04_raii_resource_management.cpp** - DB connections, parking lots
5. **05_polymorphism_strategy.cpp** - Strategy pattern examples

---

## 🚀 Compile and Run

```bash
# Compile any example
g++ -std=c++14 01_basic_unique_ptr.cpp -o basic
./basic

# Compile with optimizations
g++ -std=c++17 -O2 02_factory_pattern.cpp -o factory
./factory
```

---

## 🎓 Summary

**unique_ptr = Exclusive Ownership + Automatic Cleanup + Zero Overhead**

Perfect for:
- ✅ Factory patterns
- ✅ Tree/Graph data structures  
- ✅ Polymorphic objects
- ✅ Resource management (RAII)
- ✅ Strategy patterns

**Remember:** If you need ONE clear owner → use `unique_ptr` 🎯
