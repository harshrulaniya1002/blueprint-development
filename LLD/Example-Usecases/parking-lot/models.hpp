#ifndef MODELS_HPP
#define MODELS_HPP

#include<bits/stdc++.h>
#include<string>
#include<chrono>
using namespace std;

enum class VehicleType{
    BIKE,
    CAR,
    TRUCK
};

enum class SpotType{
    SMALL,
    MEDIUM,
    LARGE
};

enum class TicketStatus{
    ACTIVE,
    PAID
};

class Vehicle{
  private:
    string licensePlate;
    VehicleType type;
  public:
    Vehicle() : licensePlate(""), type(VehicleType::CAR) {}
    Vehicle(string plate, VehicleType vtype): licensePlate(plate), type(vtype) {}

    VehicleType getType() const { return type; }
    string getLicensePlate() const { return licensePlate; }
};

class ParkingSpot{
    private:
        string spotId;
        bool isOccupied;
        SpotType type;
        Vehicle* currentVehicle;
     
    public:
        ParkingSpot() : spotId(""), isOccupied(false), type(SpotType::SMALL), currentVehicle(nullptr) {}
        ParkingSpot(string id, SpotType stype)
            : spotId(id), isOccupied(false), type(stype), currentVehicle(nullptr) {}

        bool canFit(VehicleType vtype){
            if(isOccupied) return false;
            if((vtype == VehicleType::BIKE && type == SpotType::SMALL) ||
               (vtype == VehicleType::CAR && type == SpotType::MEDIUM) ||
               (vtype == VehicleType::TRUCK && type == SpotType::LARGE)){
                return true;
            }
            return false;
        }

        void park(Vehicle* v){
            this->currentVehicle = v;
            isOccupied = true;
        }

        void unpark(){
            this->currentVehicle = nullptr;
            isOccupied = false;
        }

        string getSpotId() const { return spotId; }
        bool getIsOccupied() const { return isOccupied; }
        SpotType getType() const { return type; }
}; 


class ParkingFloor{
    private:
    int floorNumber;
    vector<ParkingSpot> spots;

    public:
    ParkingFloor() : floorNumber(0) {}
    ParkingFloor(int num, vector<ParkingSpot> s) : floorNumber(num), spots(s) {}

    ParkingSpot* findAvailableSpot(VehicleType vtype){
        for(auto& spot : spots){
            if(spot.canFit(vtype)){
                return &spot;
            }
        }
        return nullptr;
    }

    int getFloorNumber() const { return floorNumber; }
};

class Ticket{
    private:
    string ticketId;
    Vehicle vehicle;
    ParkingSpot* spot;
    chrono::steady_clock::time_point entryTime;
    chrono::steady_clock::time_point exitTime;
    TicketStatus status;
    double amount;

    public:
    Ticket() : spot(nullptr), status(TicketStatus::ACTIVE), amount(0.0) {}
    Ticket(string id, Vehicle v, ParkingSpot* s, chrono::steady_clock::time_point entry)
        : ticketId(id), vehicle(v), spot(s), entryTime(entry),
          exitTime(), status(TicketStatus::ACTIVE), amount(0.0) {}

    string getTicketId() const { return ticketId; }
    Vehicle getVehicle() const { return vehicle; }
    ParkingSpot* getSpot() const { return spot; }
    TicketStatus getStatus() const { return status; }
    double getAmount() const { return amount; }
    chrono::steady_clock::time_point getEntryTime() const { return entryTime; }
    chrono::steady_clock::time_point getExitTime() const { return exitTime; }

    long long getDurationMinutes() const {
        auto duration = chrono::duration_cast<chrono::minutes>(exitTime - entryTime);
        return max(1LL, duration.count());
    }

    void setExitTime(chrono::steady_clock::time_point t) { exitTime = t; }
    void setStatus(TicketStatus s) { status = s; }
    void setAmount(double a) { amount = a; }
};

#endif