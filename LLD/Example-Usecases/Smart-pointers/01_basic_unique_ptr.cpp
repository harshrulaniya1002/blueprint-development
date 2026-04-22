/*
====================================================================
UNIQUE_PTR BASICS - Why and When to Use It
====================================================================

KEY CONCEPTS:
1. AUTOMATIC MEMORY MANAGEMENT - No need for manual delete
2. EXCLUSIVE OWNERSHIP - Only one owner at a time
3. MOVE SEMANTICS - Transfer ownership using std::move()
4. NO COPY - Cannot be copied (prevents double-free bugs)

WHEN TO USE IN CODING ROUNDS:
- Managing dynamically allocated objects
- Preventing memory leaks
- Clear ownership semantics
- RAII (Resource Acquisition Is Initialization)
*/

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Resource {
public:
    int id;
    
    Resource(int id) : id(id) {
        cout << "Resource " << id << " created\n";
    }
    
    ~Resource() {
        cout << "Resource " << id << " destroyed\n";
    }
    
    void use() {
        cout << "Using resource " << id << "\n";
    }
};

// ========== BAD: Manual Memory Management ==========
void badExample() {
    cout << "\n=== BAD EXAMPLE (Manual Management) ===\n";
    Resource* res = new Resource(1);
    res->use();
    // PROBLEM: Easy to forget delete, causing memory leak!
    // If exception happens before delete, memory leaks!
    delete res;
}

// ========== GOOD: Using unique_ptr ==========
void goodExample() {
    cout << "\n=== GOOD EXAMPLE (unique_ptr) ===\n";
    unique_ptr<Resource> res = make_unique<Resource>(2);
    res->use();
    // Automatically deleted when res goes out of scope!
    // Exception-safe!
}

// ========== Demonstrating Move Semantics ==========
unique_ptr<Resource> createResource(int id) {
    return make_unique<Resource>(id); // Ownership transferred to caller
}

void transferOwnership() {
    cout << "\n=== TRANSFER OWNERSHIP ===\n";
    unique_ptr<Resource> res1 = createResource(3);
    
    // unique_ptr<Resource> res2 = res1; // ❌ ERROR: Cannot copy!
    unique_ptr<Resource> res2 = move(res1); // ✅ OK: Transfer ownership
    
    cout << "res1 is " << (res1 ? "valid" : "null") << "\n";
    cout << "res2 is " << (res2 ? "valid" : "null") << "\n";
}

// ========== Using with STL Containers ==========
void containerExample() {
    cout << "\n=== UNIQUE_PTR IN CONTAINERS ===\n";
    vector<unique_ptr<Resource>> resources;
    
    resources.push_back(make_unique<Resource>(10));
    resources.push_back(make_unique<Resource>(20));
    resources.push_back(make_unique<Resource>(30));
    
    for(auto& res : resources) {
        res->use();
    }
    // All resources automatically cleaned up!
}

int main() {
    badExample();
    goodExample();
    transferOwnership();
    containerExample();
    
    cout << "\n=== PROGRAM ENDING (watch destructor calls) ===\n";
    return 0;
}
