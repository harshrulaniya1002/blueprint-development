#include <iostream>

// Level 1: Base Class
class Vehicle {
public:
    Vehicle(int y) : year(y) {}
    
    void start_transport() const {
        std::cout << "Vehicle (" << year << ") is moving." << std::endl;
    }
private:
    int year;
};

// Level 2: Derived from Vehicle
class Car : public Vehicle {
private:
    int number_of_doors;
public:
    Car(int y, int doors) : Vehicle(y), number_of_doors(doors) {} // Call base constructor

    void open_door() const {
        std::cout << "Car with " << number_of_doors << " doors opened." << std::endl;
    }
};

// Level 3: Derived from Car
class SportsCar : public Car {
private:
    int max_speed;
public:
    SportsCar(int y, int doors, int speed) 
        : Car(y, doors), max_speed(speed) {} // Call intermediate constructor

    void activate_turbo() const {
        std::cout << "Sports Car turbo activated! Max speed: " << max_speed << " mph." << std::endl;
    }
};

int main() {
    SportsCar ferrari(2023, 2, 210);
    
    // Access methods from all three levels
    ferrari.start_transport(); // From Vehicle
    ferrari.open_door();       // From Car
    ferrari.activate_turbo();  // From SportsCar

    return 0;
}