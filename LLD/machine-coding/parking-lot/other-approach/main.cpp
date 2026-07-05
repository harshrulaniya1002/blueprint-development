#include<iostream>
using namespace std;
#include "factory/vehicleFactory.hpp"
#include "managers/parkingLot_singleton.hpp"
#include "models/gate.hpp"
#include "utils/utils.hpp"

int main(){

    cout<<"Welcome to ParkingLot System"<<"\n\n";

    //VehicleCreation;
    VehicleFactory vf;
    Vehicle* v1 = vf.Create("RJ_CA",VehicleType::BIKE);
    Vehicle* v2 = vf.Create("KA_CA",VehicleType::CAR);
    Vehicle* v3 = vf.Create("BH_CA",VehicleType::TRUCK);
    Vehicle* v4 = vf.Create("MH_CA",VehicleType::BIKE);
    Vehicle* v5 = vf.Create("GJ_CA",VehicleType::CAR);

    //ParkingLot Instance Creation
    parkingLot& parkingMgr = parkingLot::getInstance();

    //add Floors
    parkingMgr.addFloor();
    parkingMgr.addFloor();

    //GateCreation
    Gate* entryGate = new EntryGate(1);
    Gate* exitGate = new ExitGate(2);

    //park vehicle
    time_t localtime;
    entryGate->parkVehicle(v1,localtime);
    




}