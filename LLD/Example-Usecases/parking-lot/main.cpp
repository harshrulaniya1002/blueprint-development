#include<iostream>
#include<thread>
#include<vector>
#include "managers.hpp"
using namespace std;


int main()
{
    ParkingManager* manager = ParkingManager::getInstance();

    // Rate strategies
    HourlyRateStrategy hourlyRate(10.0);
    FlatRateStrategy flatRate(50.0);

    // Set initial rate strategy
    manager->setRateStrategy(&hourlyRate);

    // Create parking spots for floor 1
    vector<ParkingSpot> floor1Spots = {
        ParkingSpot("F1-S1", SpotType::SMALL),
        ParkingSpot("F1-S2", SpotType::MEDIUM),
        ParkingSpot("F1-S3", SpotType::MEDIUM),
        ParkingSpot("F1-S4", SpotType::LARGE)
    };
    ParkingFloor floor1(1, floor1Spots);
    manager->addFloor(floor1);

    // Create parking spots for floor 2
    vector<ParkingSpot> floor2Spots = {
        ParkingSpot("F2-S1", SpotType::SMALL),
        ParkingSpot("F2-S2", SpotType::SMALL),
        ParkingSpot("F2-S3", SpotType::MEDIUM),
        ParkingSpot("F2-S4", SpotType::MEDIUM),
        ParkingSpot("F2-S5", SpotType::LARGE)
    };
    ParkingFloor floor2(2, floor2Spots);
    manager->addFloor(floor2);

    // ============================================================
    // DEMO 1: Concurrent parking from multiple threads
    // Without the lock_guard in parkVehicle, two threads could
    // find the same spot free and both try to park there!
    // ============================================================
    cout << "=== DEMO 1: Concurrent Parking (lock_guard) ===" << endl;
    cout << "Multiple threads trying to park vehicles simultaneously.\n"
         << "lock_guard in parkVehicle ensures only one thread\n"
         << "can find+claim a spot at a time (no double-parking).\n" << endl;

    // Vehicles owned by main thread, passed as raw (non-owning) ptrs to parkVehicle
    Vehicle bike1("BIKE-001", VehicleType::BIKE);
    Vehicle bike2("BIKE-002", VehicleType::BIKE);
    Vehicle car1("CAR-001", VehicleType::CAR);
    Vehicle car2("CAR-002", VehicleType::CAR);
    Vehicle car3("CAR-003", VehicleType::CAR);
    Vehicle truck1("TRUCK-001", VehicleType::TRUCK);
    Vehicle truck2("TRUCK-002", VehicleType::TRUCK);

    // shared_ptr<Ticket>: returned from parkVehicle.
    // Why shared_ptr? The ticket is stored in the manager's map AND
    // we hold a reference here. Both owners -> shared ownership.
    vector<shared_ptr<Ticket>> tickets(7);

    // Spawn threads that concurrently try to park
    vector<thread> parkThreads;
    auto parkFn = [&](int idx, Vehicle* v){
        tickets[idx] = manager->parkVehicle(v);
    };

    parkThreads.emplace_back(parkFn, 0, &bike1);
    parkThreads.emplace_back(parkFn, 1, &bike2);
    parkThreads.emplace_back(parkFn, 2, &car1);
    parkThreads.emplace_back(parkFn, 3, &car2);
    parkThreads.emplace_back(parkFn, 4, &car3);
    parkThreads.emplace_back(parkFn, 5, &truck1);
    parkThreads.emplace_back(parkFn, 6, &truck2);

    for(auto& t : parkThreads) t.join();

    cout << "\nActive tickets after concurrent parking: "
         << manager->getActiveTicketCount() << endl;

    // ============================================================
    // DEMO 2: Concurrent unparking with unique_lock
    // unique_lock lets us RELEASE the lock before payment I/O
    // and RE-ACQUIRE it to erase from the map.
    // This means other threads aren't blocked during payment.
    // ============================================================
    cout << "\n=== DEMO 2: Concurrent Unparking (unique_lock) ===" << endl;
    cout << "unique_lock in unparkVehicle releases the lock before\n"
         << "payment processing, so other threads can work meanwhile.\n"
         << "Then re-acquires to safely erase from the map.\n" << endl;

    // Using HourlyRate for first batch
    CashPayment cashPayment;
    CardPayment cardPayment;

    vector<thread> unparkThreads;

    // Unpark bike1 & car1 with HourlyRate + Cash (threads run concurrently)
    for(int i : {0, 2}){
        if(tickets[i]){
            string tid = tickets[i]->getTicketId();
            unparkThreads.emplace_back([&manager, tid, &cashPayment](){
                manager->unparkVehicle(tid, &cashPayment);
            });
        }
    }

    // Switch to FlatRate mid-flight for the remaining vehicles
    manager->setRateStrategy(&flatRate);

    // Unpark car2 & truck1 with FlatRate + Card (threads run concurrently)
    for(int i : {3, 5}){
        if(tickets[i]){
            string tid = tickets[i]->getTicketId();
            unparkThreads.emplace_back([&manager, tid, &cardPayment](){
                manager->unparkVehicle(tid, &cardPayment);
            });
        }
    }

    for(auto& t : unparkThreads) t.join();

    cout << "\nActive tickets after concurrent unparking: "
         << manager->getActiveTicketCount() << endl;

    // ============================================================
    // SUMMARY OF LOCK CHOICES IN THIS CODEBASE:
    // ----------------------------------------------------------
    // getInstance()     -> lock_guard  (simple scope, create-and-done)
    // setRateStrategy() -> lock_guard  (simple one-liner write)
    // addFloor()        -> lock_guard  (simple one-liner write)
    // getActiveCount()  -> lock_guard  (simple one-liner read)
    // parkVehicle()     -> lock_guard  (must hold lock entire time:
    //                      find spot + park + create ticket is atomic)
    // unparkVehicle()   -> unique_lock (need to unlock before payment
    //                      I/O, then relock to erase from map)
    // ============================================================

    return 0;
}