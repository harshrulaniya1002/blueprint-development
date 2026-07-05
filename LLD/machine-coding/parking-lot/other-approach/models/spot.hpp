#ifndef spot_h
#define spot_h
#include "utils/utils.hpp"
#include<iostream>
using namespace std;
class Spot{
    private:
    string id;
    VehicleType allowedType;
    bool occupied;
    
    public:
    Spot(string id, VehicleType spottyp): id(id), allowedType(spottyp){
        cout<<"Spot Created Successfullt for "<<id<<"\n";
    }

    bool tryOccupy(){
        if(occupied) return false;
        occupied=true;
        return true;
    }

    VehicleType SpotType(){
        return allowedType;
    }


    void vacate(){
        occupied=false;
    }

    bool isOccupied(){
        return occupied;
    }

    
};

#endif