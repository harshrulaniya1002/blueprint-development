#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Product interface
class Vehicle {
public:
    virtual void drive() = 0;
    virtual ~Vehicle() {}
};

class Car : public Vehicle {
public:
    void drive() override {
        cout << "Driving a car" << endl;
    }
};

class Bike : public Vehicle {
public:
    void drive() override {
        cout << "Riding a bike" << endl;
    }
};

class Truck : public Vehicle {
public:
    void drive() override {
        cout << "Driving a truck" << endl;
    }
};

// Factory Method
class VehicleFactory {
public:
    virtual unique_ptr<Vehicle> createVehicle() = 0;
    virtual ~VehicleFactory() {}
};

class CarFactory : public VehicleFactory {
public:
    unique_ptr<Vehicle> createVehicle() override {
        return make_unique<Car>();
    }
};

class BikeFactory : public VehicleFactory {
public:
    unique_ptr<Vehicle> createVehicle() override {
        return make_unique<Bike>();
    }
};

class TruckFactory : public VehicleFactory {
public:
    unique_ptr<Vehicle> createVehicle() override {
        return make_unique<Truck>();
    }
};

int main() {
    unique_ptr<VehicleFactory> factory;
    factory = make_unique<CarFactory>();
    auto v1 = factory->createVehicle();
    v1->drive();
    factory = make_unique<BikeFactory>();
    auto v2 = factory->createVehicle();
    v2->drive();
    factory = make_unique<TruckFactory>();
    auto v3 = factory->createVehicle();
    v3->drive();
    return 0;
}
