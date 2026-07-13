#include <iostream>
#include <stdexcept>

// Overloading the Subscript Operator ([]) allows a class object to be accessed using array notation, making user-defined containers behave like built-in arrays.
// Reference Return: Returning an int& allows the operator to be used on the left side of an assignment (e.g., arr[2] = 99;), which modifies the original element.
// Data Integrity: The implementation adds crucial boundary checking. This improves safety by catching common programming errors (accessing memory outside the array limits) and is a key feature that distinguishes a safe container class from a raw C-style array.

class ArrayWrapper {
private:
    int data[5];
    const int SIZE = 5;

public:
    ArrayWrapper() {
        for (int i = 0; i < SIZE; ++i) {
            data[i] = i * 10;
        }
    }

    // Overload the subscript operator for R/W access
    int& operator[](int index) {
        if (index < 0 || index >= SIZE) {
            throw std::out_of_range("Error: Index out of bounds (0-4).");
        }
        return data[index];
    }
    
    // Optional: Overload for read-only access on const objects
    const int& operator[](int index) const {
        if (index < 0 || index >= SIZE) {
             throw std::out_of_range("Error: Index out of bounds (0-4).");
        }
        return data[index];
    }
};

int main() {
    ArrayWrapper arr;
    
    std::cout << "Initial arr[2]: " << arr[2] << std::endl; // 20
    
    // Write access
    arr[2] = 99;
    std::cout << "New arr[2]: " << arr[2] << std::endl; // 99

    try {
        // Test boundary check
        std::cout <<"Boundary check for arr[5]"<< std::endl; // 99
        std::cout << arr[5] << std::endl; 
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl; 
    }

    return 0;
}