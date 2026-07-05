#ifndef vehicle_factory.hpp
#define vehicle_factory.hpp
#include "vehicle.hpp"
#include "utils/utils.hpp"

class VehicleFactory{
    public:
    Vehicle* Create(string number, VehicleType type){
        switch(type){
            case VehicleType::BIKE:
               return new Bike(number);
            case VehicleType::CAR:
               return new Car(number);
            case VehicleType::TRUCK:
               return new Truck(number);  
            default:
               return nullptr;   
        }
    }
};

#endif