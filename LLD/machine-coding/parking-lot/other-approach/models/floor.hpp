#ifndef floor_h
#define floor_h
#include "spot.hpp"
#include "vehicle.hpp"
#include<iostream>
#include<map>
using namespace std;

class Floor{
    private:
    int id;
    map<int,Spot>FloorMap;
    
    public:
    void AddSpot(int id, Spot spot){
        if(FloorMap.find(id) != FloorMap.end()){
            return ;
        }
        FloorMap.insert({id,spot});
    }

    Spot* findAvailableSlot(VehicleType vtype){

        for(auto it: FloorMap){
            Spot& spot = it.second;

            if(!spot.isOccupied() && (spot.SpotType() == vtype)){
                spot.vacate();
                return &spot;
            }
        }

        return nullptr;
    }
};

#endif