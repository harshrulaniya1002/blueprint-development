/*
====================================================================
PRACTICAL DECISION GUIDE: WHICH POINTER TO USE?
====================================================================

This is a HANDS-ON guide with real interview scenarios
*/

#include <iostream>
#include <memory>
#include <vector>
#include <map>
using namespace std;

/*
====================================================================
SCENARIO 1: RIDE-SHARING APP (Ola/Uber)
====================================================================
*/

class Driver {
public:
    int id;
    Driver(int i) : id(i) {}
};

class Rider {
public:
    int id;
    Rider(int i) : id(i) {}
};

class Trip {
    int tripId;
    Driver* driver;  // ✅ Trip doesn't OWN driver, just references
    Rider* rider;    // ✅ Trip doesn't OWN rider, just references
    
public:
    Trip(int id, Driver* d, Rider* r) : tripId(id), driver(d), rider(r) {}
    
    void start() {
        cout << "Trip " << tripId << " started with Driver " << driver->id 
             << " and Rider " << rider->id << "\n";
    }
};

class RideService {
    vector<unique_ptr<Driver>> drivers;  // ✅ Service OWNS drivers
    vector<unique_ptr<Rider>> riders;    // ✅ Service OWNS riders
    vector<unique_ptr<Trip>> trips;      // ✅ Service OWNS trips
    
public:
    void addDriver(int id) {
        drivers.push_back(make_unique<Driver>(id));
    }
    
    void addRider(int id) {
        riders.push_back(make_unique<Rider>(id));
    }
    
    void createTrip(int tripId, int driverId, int riderId) {
        // Find driver and rider (they're owned by service)
        Driver* driver = drivers[driverId - 1].get();  // ✅ get() for observation
        Rider* rider = riders[riderId - 1].get();      // ✅ get() for observation
        
        // Service OWNS the trip
        trips.push_back(make_unique<Trip>(tripId, driver, rider));
    }
};

void rideServiceExample() {
    cout << "\n=== RIDE SERVICE EXAMPLE ===\n";
    cout << "DECISION LOGIC:\n";
    cout << "- RideService manages lifetime of drivers/riders → unique_ptr\n";
    cout << "- Trip just references driver/rider → raw pointer\n";
    cout << "- RideService owns trips → unique_ptr\n\n";
    
    RideService service;
    service.addDriver(1);
    service.addRider(1);
    service.createTrip(101, 1, 1);
}

/*
====================================================================
SCENARIO 2: CACHE SYSTEM (Recent Items Shared Across Code)
====================================================================
*/

class CachedData {
public:
    string data;
    CachedData(string d) : data(d) {
        cout << "CachedData '" << data << "' created\n";
    }
    ~CachedData() {
        cout << "CachedData '" << data << "' destroyed\n";
    }
};

class Cache {
    map<string, shared_ptr<CachedData>> cache;  // ✅ shared_ptr because data shared
    
public:
    shared_ptr<CachedData> get(const string& key) {
        if(cache.find(key) != cache.end()) {
            return cache[key];  // ✅ Return shared ownership
        }
        
        // Create and cache
        auto data = make_shared<CachedData>(key);
        cache[key] = data;
        return data;
    }
    
    void showCounts() {
        cout << "\n=== Cache Reference Counts ===\n";
        for(auto& [key, value] : cache) {
            cout << "Key: " << key << " | RefCount: " << value.use_count() << "\n";
        }
    }
};

void cacheExample() {
    cout << "\n=== CACHE SYSTEM EXAMPLE ===\n";
    cout << "DECISION LOGIC:\n";
    cout << "- Data can be accessed by cache AND user code → shared_ptr\n";
    cout << "- Multiple owners (cache + user variables) → shared_ptr\n\n";
    
    Cache cache;
    
    {
        auto data1 = cache.get("user:123");
        auto data2 = cache.get("user:123");  // Same data, shared
        cache.showCounts();  // RefCount = 3 (cache + data1 + data2)
    }
    
    cache.showCounts();  // RefCount = 1 (only cache owns it now)
}

/*
====================================================================
SCENARIO 3: NOTIFICATION SYSTEM
====================================================================
*/

class Notification {
    string message;
public:
    Notification(string msg) : message(msg) {}
    void send() { cout << "Sending: " << message << "\n"; }
};

// Factory creates notifications
class NotificationFactory {
public:
    static unique_ptr<Notification> create(string msg) {
        return make_unique<Notification>(msg);  // ✅ Caller owns
    }
};

// Service processes and may store notifications
class NotificationService {
    vector<unique_ptr<Notification>> sentNotifications;
    
public:
    void sendNotification(string msg) {
        unique_ptr<Notification> notif = NotificationFactory::create(msg);
        notif->send();
        sentNotifications.push_back(move(notif));  // ✅ Store for history
    }
};

void notificationExample() {
    cout << "\n=== NOTIFICATION SYSTEM EXAMPLE ===\n";
    cout << "DECISION LOGIC:\n";
    cout << "- Factory creates → Caller owns → unique_ptr\n";
    cout << "- Service stores in history → unique_ptr (exclusive ownership)\n\n";
    
    NotificationService service;
    service.sendNotification("Welcome!");
    service.sendNotification("Order shipped!");
}

/*
====================================================================
SCENARIO 4: GRAPH/TREE DATA STRUCTURES
====================================================================
*/

struct GraphNode {
    int value;
    vector<GraphNode*> neighbors;  // ✅ Just references, not ownership
    
    GraphNode(int v) : value(v) {}
};

class Graph {
    vector<unique_ptr<GraphNode>> nodes;  // ✅ Graph OWNS all nodes
    
public:
    GraphNode* addNode(int value) {
        auto node = make_unique<GraphNode>(value);
        GraphNode* ptr = node.get();
        nodes.push_back(move(node));
        return ptr;  // ✅ Return observer pointer
    }
    
    void addEdge(GraphNode* from, GraphNode* to) {
        from->neighbors.push_back(to);  // ✅ Just reference
    }
};

void graphExample() {
    cout << "\n=== GRAPH STRUCTURE EXAMPLE ===\n";
    cout << "DECISION LOGIC:\n";
    cout << "- Graph owns all nodes → unique_ptr\n";
    cout << "- Edges are just references → raw pointer\n\n";
    
    Graph g;
    auto* n1 = g.addNode(1);
    auto* n2 = g.addNode(2);
    g.addEdge(n1, n2);
}

/*
====================================================================
DECISION FLOWCHART
====================================================================

┌─────────────────────────────────────┐
│ Creating object on heap?            │
└────────┬────────────────────────────┘
         │ YES
         ↓
┌─────────────────────────────────────┐
│ Will multiple parts of code         │
│ need to keep this alive?            │
└────┬─────────────────────┬──────────┘
     │ YES                  │ NO
     ↓                      ↓
┌─────────────┐    ┌────────────────┐
│ shared_ptr  │    │ Do you OWN it? │
│             │    └───┬────────────┘
│ Examples:   │        │ YES     │ NO
│ - Cache     │        ↓         ↓
│ - Logger    │    ┌────────┐  ┌────────┐
│ - Config    │    │unique_ │  │raw ptr │
└─────────────┘    │ptr     │  │or ref  │
                   │        │  │        │
                   │Examples│  │Examples│
                   │-Factory│  │-Observer│
                   │-Tree   │  │-Callback│
                   │-Manager│  └────────┘
                   └────────┘

*/

/*
====================================================================
QUICK RULES OF THUMB
====================================================================

1. FACTORY PATTERN → unique_ptr ✅
   - Factory creates and transfers ownership
   - Caller is sole owner
   
2. CACHE/SHARED RESOURCE → shared_ptr ✅
   - Multiple users need access
   - No single owner
   
3. OBSERVER/CALLBACK → raw pointer ✅
   - Just using, not owning
   - Someone else manages lifetime
   
4. PARENT-CHILD HIERARCHY → unique_ptr ✅
   - Tree nodes, file systems
   - Clear ownership chain
   
5. COLLECTION OF OBJECTS → unique_ptr ✅
   - vector<unique_ptr<T>>
   - Container owns elements

*/

int main() {
    rideServiceExample();
    cacheExample();
    notificationExample();
    graphExample();
    
    cout << "\n=== KEY TAKEAWAY ===\n";
    cout << "ASK: 'Who OWNS this object?'\n";
    cout << "  - ONE owner → unique_ptr\n";
    cout << "  - MULTIPLE owners → shared_ptr\n";
    cout << "  - NOT owning, just using → raw pointer\n";
    
    return 0;
}
