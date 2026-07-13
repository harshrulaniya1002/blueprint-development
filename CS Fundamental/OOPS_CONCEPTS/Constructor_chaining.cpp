#include <iostream>

// Level 1: Base Class
class Vehicle {
private:
    int year;
public:
    // 1. Vehicle Constructor
    Vehicle(int y) : year(y) {
        std::cout << "--> Vehicle constructor called with year " << year << std::endl;
    }
    int get_year() const { return year; }
};

// Level 2: Derived from Vehicle
class Car : public Vehicle {
private:
    int number_of_doors;
public:
    // 2. Car Constructor calls Vehicle constructor in the initialization list
    Car(int y, int doors) 
        : Vehicle(y), // Calls Vehicle's constructor
          number_of_doors(doors) 
    {
        std::cout << "----> Car constructor called with " << doors << " doors" << std::endl;
    }
};

// Level 3: Derived from Car
class SportsCar : public Car {
private:
    int max_speed;
public:
    // 3. SportsCar Constructor calls Car constructor in the initialization list
    SportsCar(int y, int doors, int speed) 
        : Car(y, doors), // Calls Car's constructor
          max_speed(speed) 
    {
        std::cout << "------> SportsCar constructor called with speed " << speed << std::endl;
    }
};

int main() {
    std::cout << "Creating Ferrari..." << std::endl;
    SportsCar ferrari(2024, 2, 220);
    std::cout << "Ferrari created, year: " << ferrari.get_year() << std::endl;

    return 0;
}