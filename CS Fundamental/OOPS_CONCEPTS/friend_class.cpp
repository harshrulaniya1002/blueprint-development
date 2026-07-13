#include <iostream>

// Forward declaration needed for the friend declaration
class Accessor; 

class Data {
private:
    int value;

public:
    Data(int v) : value(v) {}

    // Declare Accessor as a friend class
    friend class Accessor; 
};

class Accessor {
public:
    // This method belongs to Accessor, but can access Data's private members
    void display_value(const Data& d) const {
        // Direct access to d.value is allowed because Accessor is a friend
        std::cout << "Friend Accessor read Data value: " << d.value << std::endl;
        // We could also modify it if 'd' were not const
    }
};

int main() {
    Data private_data(1234);
    Accessor friend_accessor;

    // Accessor object uses its method to read Data's private member
    friend_accessor.display_value(private_data);
    
    // std::cout << private_data.value; // Compiler Error (Private)

    return 0;
}