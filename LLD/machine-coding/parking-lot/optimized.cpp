#include<iostream>
#include<vector>
#include <thread>
#include<ctime>
#include<chrono>
#include<cmath>
#include<iomanip>
#include<sstream>
#include <memory>
#include <unordered_map>
#include <mutex>
#include<atomic>

using namespace std;

enum class VehicleSize{
    SMALL,
    MEDIUM,
    LARGE
};


class Vehicle{
    protected:
    static atomic<int> nextId;
    int vehicleId;
    string vehicleNo;

    public:
    Vehicle(string number): vehicleNo(number){
        vehicleId= ++nextId;
    }
    virtual VehicleSize getSize() const = 0;
    virtual string getType() const = 0;

    virtual ~Vehicle() = default;
};

atomic<int> Vehicle::nextId{0};

class Car: public Vehicle{
    public:
    Car(string number) : Vehicle(number){}

    VehicleSize getSize() const override {
        return VehicleSize::MEDIUM;
    }

    string getType() const override {
        return "Car";
    }
};


class Bike: public Vehicle{
    public:
    Bike(string number) : Vehicle(number){}

    VehicleSize getSize() const override {
        return VehicleSize::SMALL;
    }

    string getType() const override {
        return "Bike";
    }
};


class Truck: public Vehicle{
    public:
    Truck(string number) : Vehicle(number){}

    VehicleSize getSize() const override {
        return VehicleSize::LARGE;
    }

    string getType() const override {
        return "Truck";
    }
};


class Slot{
    static atomic<int> nextSlotId;
    int slotId;
    VehicleSize size;
    bool occupied=false;
    mutable mutex mtx;

    public:
    Slot(VehicleSize size):size(size){
        slotId=++nextSlotId;
    }

    bool isAvailable() const{
        lock_guard<mutex> lock(mtx);
        return !occupied;
    }

    bool occupy(){
        lock_guard<mutex> lock(mtx);
        if(occupied) return false;

        occupied=true;
        return true;
    }

    void release(){
        lock_guard<mutex> lock(mtx);
        occupied=false;
    }

    VehicleSize getSize() const{
        return size;
    }

    int getId() const {
        return slotId;
    }
};

atomic<int> Slot::nextSlotId{0};

class Floor{
    private:
    int floorNo;
    vector<unique_ptr<Slot>>slots;

    public:
    Floor(int floorNo):floorNo(floorNo){}

    void addSlot(VehicleSize size){
        slots.push_back(make_unique<Slot>(size));
    }

    vector<unique_ptr<Slot>>& getSlots(){
        return slots;
    }

    int getFloorNo() const {
        return floorNo;
    }
};


class Booking{
    private:
    static atomic<int> nextBookingId;
    int bookingId;
    time_t startTime;
    time_t endTime;
   
    //aggregation relationship
    Vehicle* vehicle;
    Slot* slot;

    public:

    Booking(Slot* slot,Vehicle* vehicle) :slot(slot),vehicle(vehicle){
        bookingId=++nextBookingId;
        startTime=time(nullptr);
        endTime=0;
    }

    int getBookingId(){
        return bookingId;
    }

    Slot* getSlot(){
        return slot;
    }

    time_t getStartTime(){
        return startTime;
    }

    time_t getEndTime() {
        return endTime;
    }

    void close(){
        endTime=time(nullptr);
    }
};

atomic<int> Booking::nextBookingId{0};

class Payment{
    private:
       int amount;
       bool paid;
    
    public:
    Payment(int amount=0,bool paid=true):amount(amount),paid(paid){}
    int getAmount(){return amount;}
};


class ParkingLot{
    private:
    vector<unique_ptr<Floor>> floors;

    public:

    void addFloor(int floorNo)
    {
        floors.push_back(make_unique<Floor>(floorNo));
    }

    vector<unique_ptr<Floor>>& getFloors()
    {
        return floors;
    }
};


class PaymentService{
    public:
    Payment calculateFee(Booking& booking){
        double seconds=difftime(booking.getEndTime(),booking.getStartTime());
        double hours=seconds/3600.0;
        int amount=max(20,(int)ceil(hours)*20);

        return Payment(amount,true);
    }
};

class ReceiptPrinter{
    public:
      void PrintReceipt(Booking& booking,Payment& payment){
        cout<<"\n========== RECEIPT ==========\n";
        cout<<"Booking Id : "<<booking.getBookingId()<<"\n";
        cout<<"Amount Paid : "<<payment.getAmount()<<"\n";
        cout<<"=============================\n";
    }
};


class NotificationService{
    public:
    void SendSMS(){

        cout<<"SMS sent\n";

    }

    void SendMail(){

        cout<<"Mail sent\n";

    }
};

class DisplayBoard{
    public:

    void update(){
      cout<<"Display board updated\n";
    }
};

class ParkingStrategy{
    public:

    virtual Slot* findSlot(ParkingLot& lot, Vehicle* vehicle) = 0;
    virtual ~ParkingStrategy()=default;
};

class ExactFitStrategy : public ParkingStrategy{
    public:
    Slot* findSlot(ParkingLot& lot,Vehicle* vehicle) override{
        for(auto& floor : lot.getFloors())
        {
            for(auto& slot : floor->getSlots())
            {
                if(slot->isAvailable() && slot->getSize()==vehicle->getSize())
                {
                    return slot.get();
                }
            }
        }

        return nullptr;
    }
};

// class NearestSlotStrategy : public ParkingStrategy{

//    public:
//     Slot* findSlot(ParkingLot& lot,Vehicle* vehicle) override{

//     }
// };

class BookingManager{

    private:
    unordered_map<int,unique_ptr<Booking>> bookings;
    mutex mtx;

    public:
        Booking* createBooking(Slot* slot, Vehicle* vehicle){
            lock_guard<mutex> lock(mtx);
            auto booking = make_unique<Booking>(slot, vehicle);
            int id = booking->getBookingId();
            bookings[id] = std::move(booking);
            return bookings[id].get();
        }

        void closeBooking(int bookingId){
            lock_guard<mutex> lock(mtx);
            if(bookings.count(bookingId)){
                bookings[bookingId]->close();
            }
        }

};


class ParkingManager{
   private:
   mutex parkingMutex;
   ParkingLot lot;
   BookingManager bookingManager;
   PaymentService paymentService;
   ReceiptPrinter recieptPrinter;
   DisplayBoard displayBoard;
   NotificationService notificationService;
   unique_ptr<ParkingStrategy>strategy;
   ParkingManager(){
    strategy = make_unique<ExactFitStrategy>();
   }

   public:
   static ParkingManager& getInstance(){
    static ParkingManager instance;
    return instance;
   }

   void addFloor(int floorNo){
    lot.addFloor(floorNo);
   };

   void addSlot(int floorNo, VehicleSize size){
        auto& floors=lot.getFloors();

        for(auto& floor:floors){
            if(floor->getFloorNo()==floorNo){
                floor->addSlot(size);
                return;
            }
        }
   }

   Booking* parkVehicle(Vehicle* vehicle){
     lock_guard<mutex> lock(parkingMutex);
     Slot* slot = strategy->findSlot(lot,vehicle);
     if(slot == nullptr)
     {
            cout<<"No slot available\n";
            return nullptr;
     }

     if(!slot->occupy()){
        return nullptr;
     };

     Booking* booking = bookingManager.createBooking(slot,vehicle);

     displayBoard.update();
     notificationService.SendSMS();
     return booking;
   }

   void checkout(Booking* booking){

        if(booking == nullptr) return;

        lock_guard<mutex> lock(parkingMutex);
        bookingManager.closeBooking(booking->getBookingId());
        booking->getSlot()->release();

        Payment payment = paymentService.calculateFee(*booking);

        recieptPrinter.PrintReceipt(*booking,payment);
        displayBoard.update();
        notificationService.SendSMS();
    }
};

int main()
{
    cout<<"========== PARKING LOT SYSTEM ==========\n\n";

    // Create vehicles
    unique_ptr<Vehicle> car1 = make_unique<Car>("KA-01-AA-1111");
    unique_ptr<Vehicle> bike1 = make_unique<Bike>("KA-02-BB-2222");
    unique_ptr<Vehicle> truck1 = make_unique<Truck>("KA-03-CC-3333");


    // Singleton manager
    ParkingManager& manager = ParkingManager::getInstance();


    // Build parking lot
    manager.addFloor(1);

    manager.addSlot(1, VehicleSize::SMALL);
    manager.addSlot(1, VehicleSize::SMALL);
    manager.addSlot(1, VehicleSize::MEDIUM);
    manager.addSlot(1, VehicleSize::MEDIUM);
    manager.addSlot(1, VehicleSize::LARGE);


    cout<<"\n======= VEHICLE ENTRY =======\n\n";

    Booking* booking1 = manager.parkVehicle(car1.get());
    Booking* booking2 = manager.parkVehicle(bike1.get());
    Booking* booking3 = manager.parkVehicle(truck1.get());


    this_thread::sleep_for(chrono::seconds(10));


    cout<<"\n======= VEHICLE EXIT =======\n\n";

    manager.checkout(booking1);
    manager.checkout(booking2);
    manager.checkout(booking3);

    return 0;
}