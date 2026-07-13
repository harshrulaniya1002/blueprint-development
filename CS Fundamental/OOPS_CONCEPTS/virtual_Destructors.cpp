#include <iostream>

// Base Class
class Base {
public:
    // Change to 'virtual ~Base() {}' to fix the leak
    virtual ~Base() { std::cout << "1. Base destructor called." << std::endl; }
};
// Derived Class
class Derived : public Base {
private:
    int* resource;
public:
    Derived() {
        resource = new int(100);
        std::cout << "2. Derived constructor: Resource allocated." << std::endl;
    }

    ~Derived() {
        delete resource; // Cleanup step
        std::cout << "3. Derived destructor called: Resource DEALLOCATED." << std::endl;
    }
};

void run_test() {
    Base* ptr = new Derived(); // Base pointer pointing to Derived object
    std::cout << "\nDeleting Derived object via Base pointer..." << std::endl;
    delete ptr; 
}

int main() {
    run_test();
    
    return 0;
}