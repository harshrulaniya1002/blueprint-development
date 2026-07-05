#ifndef parkinglot_hpp
#define parkinglot_hpp
#include "floor.hpp"
#include "ticket.hpp"
#include "strategies/pricingStrategy.hpp"

class parkingLot{
    private:
    map<int,Floor> floors;
    map<string,Ticket> activeTickets;
    parkingLot(){};


    public:

    static parkingLot& getInstance(){
        static parkingLot instance;
        return instance;
    }

    Ticket parkVehicle(Vehicle* vehicle,time_t entry_time){

        Spot* bestSpot = nullptr;
        for(auto& it: floors){
            Floor& floor = it.second;
            Spot* spot = floor.findAvailableSlot(vehicle->vehicleType());
            if(spot != nullptr) {
                bestSpot = spot;
                break;
            }
        }

        
    }

    void unParkVehicle(Ticket ticket,time_t exitTime, PAYMENT_MODE mode){

    }

    void addFloor(){

    }

    parkingLot(const parkingLot&) = delete;
    parkingLot& operator=(const parkingLot&) = delete;
};

#endif