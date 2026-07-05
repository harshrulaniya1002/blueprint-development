#ifndef gate_h
#define gate_h
#include "utils/utils.hpp"
#include "managers/parkingLot_singleton.hpp"
#include<iostream>
#include <stdexcept>
using namespace std;

class Gate{
    private:
    int GateId;
    GateType gtype;
    public:
    Gate(int gno, GateType gtype) : GateId(gno), gtype(gtype) {}

    virtual Ticket parkVehicle(Vehicle*, time_t) {
        throw runtime_error("Not supported");
    }

    virtual void unparkVehicle(Ticket, time_t, PAYMENT_MODE mode) {
        throw runtime_error("Not supported");
    }

    virtual ~Gate() =default;
};

class EntryGate: public Gate {
    public:
    EntryGate(int gno) : Gate(gno,GateType::ENTRY){}

    Ticket parkVehicle(Vehicle* vehicle, time_t entryTime) override {
        return parkingLot::getInstance().parkVehicle(vehicle,entryTime);
    }
};

class ExitGate: public Gate {
    public:
    ExitGate(int gno) : Gate(gno,GateType::EXIT){}

    void unparkVehicle(Ticket ticket, time_t exitTime, PAYMENT_MODE mode) override{
        parkingLot::getInstance().unParkVehicle(ticket,exitTime,mode);
    }
};

#endif
