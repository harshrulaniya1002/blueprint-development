#include<iostream>
#include<vector>
#include <thread>
#include<ctime>
#include<chrono>
#include<cmath>
#include<iomanip>
#include<sstream>

using namespace std;

enum class CarType{
    SMALL,
    MEDIUM,
    LARGE
};

enum class SlotSize{
    SMALL,
    MEDIUM,
    LARGE
};

enum class SlotStatus{
    EMPTY,
    FILLED
};

enum class BookingStatus{
    IN_PROGESS,
    COMPLETED
};

string getCurrentTime(){
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&currentTime);

    stringstream ss;
    ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

string carTypeToString(CarType type) {
    switch (type) {
        case CarType::SMALL:  return "SMALL";
        case CarType::MEDIUM: return "MEDIUM";
        case CarType::LARGE:  return "LARGE";
    }
    return "UNKNOWN";
}

string slotTypeToString(SlotSize type) {
    switch (type) {
        case SlotSize::SMALL:  return "SMALL";
        case SlotSize::MEDIUM: return "MEDIUM";
        case SlotSize::LARGE:  return "LARGE";
    }
    return "UNKNOWN";
}

string bookingStatusToString(BookingStatus type) {
    switch (type) {
        case BookingStatus::IN_PROGESS:  return "IN_PROGRESS";
        case BookingStatus::COMPLETED: return "COMPLETED";
    }
    return "UNKNOWN";
}

string slotStatusToString(SlotStatus status){
    switch(status){
        case SlotStatus::EMPTY:
            return "EMPTY";
        case SlotStatus::FILLED:
            return "FILLED";    
    }
    return "UNKNOWN";
}

// class Slots{};

class Vehicle{
    static int nextvehicleId;
    int vehicleId;
    string name;
    string color;
    string vehicleNo;
    CarType vehicleSizeType;

    public:
    Vehicle(string name, string color, string vehicleNo, CarType vehicleSize) : name(name), color(color), vehicleNo(vehicleNo), vehicleSizeType(vehicleSize){
        vehicleId = ++nextvehicleId;
    };

    CarType VehicleType(){
        return vehicleSizeType;
    }

    void getCarDetails(){
        cout<<"********** CAR DETAILS For "<<name<<" **********"<<"\n";
        cout<<"VehicleId = "<<vehicleId<<"\n"<<"Color: "<<color<<"\n"<<"vehicleNumber: "<<vehicleNo<<"\n"<<"vehicleSizeType: "<<carTypeToString(vehicleSizeType)<<"\n\n";
    }
};
int Vehicle::nextvehicleId = 0;

class Slot{
    static int nextslotId;
    int slotId;
    SlotStatus SlotBookingStatus;
    SlotSize SlotVehicleSize;

    public:
    Slot(SlotSize slotsize) : SlotVehicleSize(slotsize){
        slotId = ++nextslotId;
        SlotBookingStatus = SlotStatus::EMPTY;
    }

    SlotStatus slot_status(){
        return SlotBookingStatus;
    }

    void set_slot_status(SlotStatus status){
        SlotBookingStatus = status;
    }

    SlotSize slot_vehicle_size(){
        return SlotVehicleSize;
    }

    int get_slot_id(){
        return slotId;
    }
};

int Slot::nextslotId = 0;

class Booking{
    public:
    static int nextbookingId;
    int bookingId;
    string startTime;
    string EndTime;
    Vehicle* vehicle;
    Slot* slot;
    BookingStatus bookingStatus;
    int amount;

    Booking(Slot* s , Vehicle* v) : slot(s), vehicle(v){
        bookingId = ++nextbookingId;
        startTime = getCurrentTime();
        amount = 0;
        bookingStatus = BookingStatus::IN_PROGESS;
        EndTime= "";
    };

    void setCheckoutDetails(){
        EndTime = getCurrentTime();
        bookingStatus = BookingStatus::COMPLETED;
    }

    void booking_details(){
        cout<<"Booking Id = "<<bookingId<<"\n";

        cout<<"Booking StartTime = "<<startTime<<"\n";
        cout<<"Booking EndTime = "<<EndTime<<"\n";
        cout<<"VehicleParkedDetails => "<<"\n";
        vehicle->getCarDetails();

        cout<<"Slot Booked ID = "<<slot->get_slot_id()<<"\n";
        cout<<"Booking Status "<<bookingStatusToString(bookingStatus)<<"\n";
    }

    void payment_details(){
        //calculate amount to be payed;


        tm start_tm = {};
        tm end_tm = {};

        stringstream ss1(startTime);
        ss1 >> get_time(&start_tm, "%Y-%m-%d %H:%M:%S");

        stringstream ss2(EndTime);
        ss2 >> get_time(&end_tm, "%Y-%m-%d %H:%M:%S");
        
        time_t start = mktime(&start_tm);
        time_t end = mktime(&end_tm);

        double seconds = difftime(end, start);
        double minutes = seconds/60.0;
        double hours = seconds / 3600.0;

        amount = max(20, (int)ceil(hours)*20);;

        cout << "Parking Duration = " << seconds << " seconds\n";
        cout << "Parking Duration = " << hours << " hours\n";
        cout << "Amount to be paid = Rs. " << amount << "\n";
    }
};

int Booking::nextbookingId = 0;


// //single instant
class ParkingManager{
    vector<Slot> slots;
    vector<Booking* >bookings;
    public:

    void add_slots(Slot s){
        slots.push_back(s);
    }

    Booking* findBestAvailableSlot(Vehicle* v){

        for(auto& slot: slots){
            if((int)slot.slot_vehicle_size() == (int)v->VehicleType() && slot.slot_status() == SlotStatus::EMPTY){
                Booking *b = new Booking(&slot,v);
                bookings.push_back(b);
                slot.set_slot_status(SlotStatus::FILLED);
                return b;
            }
        }

        return nullptr;
    } 
    
    void checkout(Booking *b){
        for(auto booking: bookings){
            if(booking->bookingId == b->bookingId){
                b->setCheckoutDetails();
                b->slot->set_slot_status(SlotStatus::EMPTY);
                b->booking_details();
                b->payment_details();
            }
        }
    }

};

int main(){
    cout<<"Welcome to Parking Lot System, !!! Park Your Vehicle Tension Free"<<"\n\n";

    //vehicle creation
    Vehicle v1("verna","red", "RJ-CA-2001",CarType::MEDIUM);
    v1.getCarDetails();

    Vehicle v2("nano","white", "KA-CA-9991",CarType::SMALL);
    v2.getCarDetails();
    
    Vehicle v3("fortuner","whote", "BH-CA-2019",CarType::LARGE);
    v3.getCarDetails();



    //Entry Ticket creation
    ParkingManager pm;

    //add slots
    Slot s1(SlotSize::MEDIUM);
    pm.add_slots(s1);

    Slot s2(SlotSize::SMALL);
    pm.add_slots(s2);

    Slot s3(SlotSize::MEDIUM);
    pm.add_slots(s3);

    Slot s4(SlotSize::LARGE);
    pm.add_slots(s4);

    Slot s5(SlotSize::MEDIUM);
    pm.add_slots(s5);

    Slot s6(SlotSize::LARGE);
    pm.add_slots(s6);

    Slot s7(SlotSize::SMALL);
    pm.add_slots(s7);
    
    Slot s8(SlotSize::MEDIUM);
    pm.add_slots(s8);


    //Bookings start
    Booking * b1 = pm.findBestAvailableSlot(&v1);
    if(b1 == nullptr){
        cout<<"Currently Parking Lot is Completely Booked !!! Wait for Some time"<<"\n";
    }else{
        cout<<"*****. Booking SuccessFull *****"<<"\n";
        b1->booking_details();
        cout<<"\n";
    }

    Booking* b2 = pm.findBestAvailableSlot(&v2);
    if(b2 == nullptr){
        cout<<"Currently Parking Lot is Completely Booked !!! Wait for Some time"<<"\n";
    }else{
        cout<<"*****. Booking SuccessFull *****"<<"\n";
        b2->booking_details();
        cout<<"\n";
    }
    
    Booking* b3 = pm.findBestAvailableSlot(&v3);
    if(b3 == nullptr){
        cout<<"Currently Parking Lot is Completely Booked !!! Wait for Some time"<<"\n";
    }else{
        cout<<"*****. Booking SuccessFull *****"<<"\n";
        b3->booking_details();
        cout<<"\n";
    }

    this_thread::sleep_for(chrono::seconds(20));
    pm.checkout(b1);
    
    return 0;
}