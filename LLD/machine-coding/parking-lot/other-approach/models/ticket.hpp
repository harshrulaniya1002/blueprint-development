#ifndef ticket_h
#define ticket_h
#include<iostream>
#include<map>
#include "vehicle.hpp"
using namespace std;

class Ticket{
    private:
    string ticketId;
    time_t localentrytime;
    int floorId;
    string spotId;
    Vehicle vehicle;
    PaymentStatus payStatus;

    public:
    Ticket(string tid, time_t time, int fid, string spotid, Vehicle v): ticketId(tid), localentrytime(time), floorId(fid), spotId(spotid), vehicle(v){
        payStatus= PaymentStatus::IN_PROGRESS;
    }
};


#endif