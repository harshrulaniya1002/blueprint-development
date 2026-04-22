#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include<bits/stdc++.h>
#include<mutex>
#include<memory>
#include<algorithm>
#include<thread>
#include "models.hpp"
#include "strategies.hpp"
using namespace std;

// ============================================================
// SMART POINTER CHEAT SHEET:
// ----------------------------------------------------------
// unique_ptr  -> sole ownership, non-copyable, lightweight
//   Use when: one owner (e.g., singleton instance, owned children)
// shared_ptr  -> shared ownership, ref-counted, heavier
//   Use when: multiple owners need to keep object alive
//   (e.g., Ticket returned to caller but also stored in map)
// raw ptr     -> non-owning reference (observer)
//   Use when: you don't own the object (e.g., rateStrategy set from outside)
// ============================================================

// ============================================================
// LOCK CHEAT SHEET:
// ----------------------------------------------------------
// lock_guard<mutex>   -> simple RAII lock, locks in ctor, unlocks in dtor
//   Use when: you hold the lock for the ENTIRE scope, no early unlock needed
//   (e.g., singleton creation, simple read/write to shared state)
//
// unique_lock<mutex>  -> flexible RAII lock, supports:
//   - early unlock/relock via .unlock()/.lock()
//   - deferred locking (defer_lock)
//   - try_lock (non-blocking attempt)
//   - condition_variable (requires unique_lock, not lock_guard)
//   Use when: you need to unlock mid-scope (e.g., release lock
//   before doing expensive I/O like payment processing)
// ============================================================

class ParkingManager{
    private:
    static ParkingManager* instance;
    static mutex singletonMtx;       // only for singleton creation

    // operationMtx: guards shared mutable state (floors, activeTickets, ticketCounter)
    // WHY mutex here? Multiple threads can call parkVehicle/unparkVehicle concurrently.
    // Without this, two threads could find the same spot available and double-park.
    mutable mutex operationMtx;

    vector<ParkingFloor> floors;
    // shared_ptr<Ticket>: ticket is owned by the map AND returned to caller.
    // Both may need it alive -> shared ownership -> shared_ptr
    map<string, shared_ptr<Ticket>> activeTickets;

    // raw ptr: ParkingManager does NOT own the strategy.
    // Caller owns it and can swap it anytime. Non-owning observer.
    RateStrategy* rateStrategy;
    int ticketCounter;

    ParkingManager() : rateStrategy(nullptr), ticketCounter(0) {}

    public:
    ParkingManager(const ParkingManager&) = delete;
    ParkingManager& operator=(const ParkingManager&) = delete;

    static ParkingManager* getInstance(){
        // Double-checked locking for singleton
        // lock_guard is enough here: we hold the lock for the entire if-block
        if(instance == nullptr){
            lock_guard<mutex> lock(singletonMtx);
            if(instance == nullptr){
                instance = new ParkingManager();
            }
        }
        return instance;
    }

    void setRateStrategy(RateStrategy* strategy){
        lock_guard<mutex> lock(operationMtx);
        rateStrategy = strategy;
    }

    void addFloor(ParkingFloor floor){
        lock_guard<mutex> lock(operationMtx);
        floors.push_back(floor);
    }

    shared_ptr<Ticket> parkVehicle(Vehicle* vehicle){
        // lock_guard: we need the lock for the entire duration of finding a spot
        // and creating a ticket. If we released early, another thread could
        // grab the same spot between our find and park calls (race condition).
        lock_guard<mutex> lock(operationMtx);

        for(auto& floor : floors){
            ParkingSpot* spot = floor.findAvailableSpot(vehicle->getType());
            if(spot != nullptr){
                spot->park(vehicle);
                string ticketId = "TKT-" + to_string(++ticketCounter);
                auto ticket = make_shared<Ticket>(ticketId, *vehicle, spot, chrono::steady_clock::now());
                activeTickets[ticketId] = ticket;
                cout << "[Thread " << this_thread::get_id() << "] "
                     << "Vehicle " << vehicle->getLicensePlate()
                     << " parked at spot " << spot->getSpotId()
                     << " on floor " << floor.getFloorNumber()
                     << " | Ticket: " << ticketId << endl;
                return ticket;
            }
        }
        cout << "[Thread " << this_thread::get_id() << "] "
             << "No available spot for vehicle " << vehicle->getLicensePlate() << endl;
        return nullptr;
    }

    void unparkVehicle(string ticketId, PaymentStrategy* paymentStrategy){
        // unique_lock here instead of lock_guard because:
        // 1. We need the lock to find and update the ticket (shared state)
        // 2. But we can RELEASE it before payment processing (expensive I/O)
        //    so other threads aren't blocked waiting during payment
        // 3. Then re-acquire to erase from map
        unique_lock<mutex> lock(operationMtx);

        auto it = activeTickets.find(ticketId);
        if(it == activeTickets.end()){
            cout << "[Thread " << this_thread::get_id() << "] "
                 << "Ticket " << ticketId << " not found!" << endl;
            return;
        }

        shared_ptr<Ticket> ticket = it->second;
        ticket->setExitTime(chrono::steady_clock::now());
        long long minutes = ticket->getDurationMinutes();
        double amount = rateStrategy ? rateStrategy->calculateRate(minutes) : 0.0;
        ticket->setAmount(amount);
        ticket->getSpot()->unpark();
        ticket->setStatus(TicketStatus::PAID);

        // Release lock BEFORE payment (I/O-heavy, don't block other threads)
        lock.unlock();

        // Payment happens outside the lock - other threads can park/unpark meanwhile
        paymentStrategy->pay(amount);

        // Re-acquire lock to modify shared state (erase from map)
        lock.lock();
        activeTickets.erase(ticketId);

        cout << "[Thread " << this_thread::get_id() << "] "
             << "Vehicle unparked. Ticket " << ticketId << " closed. Amount: $" << amount << endl;
    }

    int getActiveTicketCount(){
        lock_guard<mutex> lock(operationMtx);
        return activeTickets.size();
    }
};

ParkingManager* ParkingManager::instance = nullptr;
mutex ParkingManager::singletonMtx;

#endif
