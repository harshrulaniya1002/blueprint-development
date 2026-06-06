#include<iostream>
#include<vector>
#include<unordered_set>
#include<cmath>
#include<climits>
using namespace std;


enum class RequestType{
    PICKUP,
    PICKDOWN,
    DESTINATION
};

enum class Direction{
    UP,
    DOWN,
    IDLE
};

class Request{
   int floor;
   RequestType type;
   public:
   Request(int floor, RequestType type): floor(floor), type(type) {}
   int getFloor() const { return floor; }
   RequestType getType() const { return type; }

   // Overload the equality operator for Request
   // This allows us to compare two Request objects directly using the == operator.
   bool operator==(const Request& other) const {
       return floor == other.floor && type == other.type;
   }
};

struct RequestHash {
        size_t operator()(const Request& req) const {
            return hash<int>()(req.getFloor()) ^ (hash<int>()(static_cast<int>(req.getType())) << 1);
        }
};

class Elevator{
    private:
    int id;
    int currentFloor;
    Direction direction;
    unordered_set<Request, RequestHash> requests; // Set of pending requests for this elevator

    public:
    Elevator(int id): id(id), currentFloor(0), direction(Direction::IDLE) {}
    bool addRequest(const Request& request) {
       if(request.getFloor() <0 || request.getFloor() > 9) {
           return false; // Invalid floor
       }

       if(currentFloor == request.getFloor()) {
           return false; // Elevator is already at the requested floor
       }

       if(requests.find(request) != requests.end()) {
           return false; // Request already exists in the queue
       }

       requests.insert(request);
       return true;
    }

    void move(){
        if(requests.empty()) {
            direction = Direction::IDLE;
            return; // No pending requests, elevator remains idle
        }

        if(direction == Direction::IDLE) {
            Request* nearest = nullptr;
            int minDistance = INT_MAX;

            for(const Request& request : requests) {
                int distance = abs(currentFloor - request.getFloor());
                if (distance < minDistance ||
                    (distance == minDistance && (nearest == nullptr || request.getFloor() < nearest->getFloor()))) {
                    minDistance = distance;
                    // If two requests are at the same distance, prioritize the one with the lower floor number
                    nearest = const_cast<Request*>(&request);
                }
            }

            if (nearest != nullptr) {
                direction = (nearest->getFloor() > currentFloor) ? Direction::UP : Direction::DOWN;
            }
        }


        RequestType pickupType = (direction == Direction::UP) ? RequestType::PICKUP : RequestType::PICKDOWN;
        Request pickupRequest(currentFloor, pickupType);
        Request destinationRequest(currentFloor, RequestType::DESTINATION);

        if (requests.find(pickupRequest) != requests.end() || requests.find(destinationRequest) != requests.end()) {
            requests.erase(pickupRequest);
            requests.erase(destinationRequest);

            if (requests.empty()) {
                direction = Direction::IDLE;
            }
            return;
        }

        if (!hasRequestsAhead(direction)) {
            direction = (direction == Direction::UP) ? Direction::DOWN : Direction::UP;
            return;
        }

        if (direction == Direction::UP) {
            currentFloor++;
        } else if (direction == Direction::DOWN) {
            currentFloor--;
        }
    }

    bool hasRequestsAhead(Direction dir) const {
        for (const auto& request : requests) {
            if (dir == Direction::UP && request.getFloor() > currentFloor) {
                return true;
            }
            if (dir == Direction::DOWN && request.getFloor() < currentFloor) {
                return true;
            }
        }
        return false;
    }

    int getCurrentFloor() const { return currentFloor; }
    Direction getDirection() const { return direction; }
    int getId() const { return id; }
    int getRequestCount() const { return requests.size(); }

    void displayStatus() const {
        cout << "  Elevator " << id << " | Floor: " << currentFloor << " | Direction: ";
        if (direction == Direction::UP) {
            cout << "UP";
        } else if (direction == Direction::DOWN) {
            cout << "DOWN";
        } else {
            cout << "IDLE";
        }
        cout << " | Pending Requests: " << requests.size() << "\n";
    }

    bool hasRequestAtorBeyond(int floor, Direction dir) const {
        for (const auto& request : requests) {
            if (dir == Direction::UP && request.getFloor() >= floor) {
                if (request.getType() == RequestType::PICKUP || request.getType() == RequestType::DESTINATION) {
                    return true;
                }
            }
            if (dir == Direction::DOWN && request.getFloor() <= floor) {
                if (request.getType() == RequestType::PICKDOWN || request.getType() == RequestType::DESTINATION) {
                    return true;
                }
            }
        }
        return false;
    }
};



class ElevatorController{
    private:
    vector<Elevator> elevators;

    Elevator* findCommittedElevator(const Request& request) {
        int floor = request.getFloor();
        Direction direction = (request.getType() == RequestType::PICKUP) ? Direction::UP : Direction::DOWN;

        Elevator* best = nullptr;
        int minDistance = INT_MAX;

        for(Elevator& elevator : elevators) {
            if(elevator.getDirection() != direction) {
                continue; // Skip elevators moving in the opposite direction
            }

            if((direction == Direction::UP &&  elevator.getCurrentFloor() > floor) || (direction == Direction::DOWN && elevator.getCurrentFloor() < floor)) {
                continue; // Skip elevators that have already passed the requested floor
            }

            if(!elevator.hasRequestAtorBeyond(floor,direction)){
                continue; // Skip elevators that do not have any requests at or beyond the requested floor in the current direction
            }

            int distance = abs(elevator.getCurrentFloor() - floor);
            if(distance < minDistance) {
                minDistance = distance;
                best = &elevator;
            }
        }

        return best;
    }

    Elevator* findClosestIdleElevator(int floor) {
        Elevator* best = nullptr;
        int minDistance = INT_MAX;

        for(Elevator& elevator : elevators) {
            if(elevator.getDirection() != Direction::IDLE) {
                continue;
            }

            int distance = abs(elevator.getCurrentFloor() - floor);
            // When same distance, prefer elevator with fewer requests
            if(distance < minDistance || 
               (distance == minDistance && (best == nullptr || elevator.getRequestCount() < best->getRequestCount()))) {
                minDistance = distance;
                best = &elevator;
            }
        }

        return best;
    }

    Elevator* nearestElevator(int floor) {
        Elevator* best = nullptr;
        int minDistance = INT_MAX;
        for(Elevator& elevator : elevators) {
            int distance = abs(elevator.getCurrentFloor() - floor);
            if(distance < minDistance) {
                minDistance = distance;
                best = &elevator;
            }
        }
        return best;
    }

    Elevator* findBestElevator(const Request& request) {
        // First priority: Find an idle elevator (even if further away) to balance load
        Elevator* idle = findClosestIdleElevator(request.getFloor());
        if(idle != nullptr) {
            return idle;
        }

        // Second priority: Find a committed elevator moving in the same direction
        Elevator* best = findCommittedElevator(request);
        if(best != nullptr) {
            return best;
        }

        // Last resort: Find the nearest elevator (even if busy or moving opposite direction)
        return nearestElevator(request.getFloor());
    }

    public:
    ElevatorController(){
        elevators.push_back(Elevator(1));
        elevators.push_back(Elevator(2));
        elevators.push_back(Elevator(3));
    }

    bool requestElevator(int floor, RequestType type) {
        if(floor <0 || floor > 9){
            return false; // Invalid floor
        }

        if(type == RequestType::DESTINATION){
            return false; // Destination requests should be made from inside the elevator
        }

        Request request(floor, type);
        Elevator* elevator = findBestElevator(request);
        if(elevator == nullptr) {
            return false; // No available elevator
        }

        // Add the request to the selected elevator's queue
        bool success = elevator->addRequest(request);
        if(success) {
            cout << "✓ Request assigned to Elevator " << elevator->getId() << "\n";
        }
        return success;
    }

    void step() {
        for(Elevator& elevator : elevators) {
            elevator.move();
        }
    }

    void displayAllElevators() const {
        cout << "\n--- Elevator Status ---\n";
        for(const Elevator& elevator : elevators) {
            elevator.displayStatus();
        }
        cout << "\n";
    }
};



int main(){
    cout << "=== Elevator System Simulation ===\n\n";
    ElevatorController controller;
    
    cout << "📞 REQUEST 1: Someone at Floor 3 wants to go UP\n";
    controller.requestElevator(3, RequestType::PICKUP);
    controller.displayAllElevators();
    
    cout << "📞 REQUEST 2: Someone at Floor 5 wants to go UP\n";
    controller.requestElevator(5, RequestType::PICKUP);
    controller.displayAllElevators();
    
    cout << "📞 REQUEST 3: Someone at Floor 2 wants to go DOWN\n";
    controller.requestElevator(2, RequestType::PICKDOWN);
    controller.displayAllElevators();
    
    cout << "\n=== Simulation Running ===\n";
    for(int i=0; i<10; i++) {
        cout << "\n⏱️  STEP " << i+1 << ":\n";
        controller.step();
        controller.displayAllElevators();
    }
    
    cout << "=== Simulation Complete ===\n";
    return 0;
}