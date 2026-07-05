#ifndef Vehicle_h
#define Vehicle_h
#include "utils/utils.hpp"
#include<iostream>
using namespace std;

class Vehicle{
    private:
    string VehicleNo;
    VehicleType vtype;
    public:
    Vehicle(string vno, VehicleType vtype) : VehicleNo(vno), vtype(vtype) {}
    VehicleType vehicleType(){
        return vtype;
    }
};

class Car: public Vehicle {
    public:
    Car(string VehicleNo): Vehicle(VehicleNo,VehicleType::CAR) {}
};

class Bike: public Vehicle {
    public:
    Bike(string VehicleNo): Vehicle(VehicleNo,VehicleType::BIKE) {}
};

class Truck: public Vehicle {
    public:
    Truck(string VehicleNo): Vehicle(VehicleNo,VehicleType::TRUCK) {}
};


#endif