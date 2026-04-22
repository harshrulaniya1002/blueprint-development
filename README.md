# LLD Interview Template — C++ Concurrency Edition

## 1. FILE STRUCTURE (Same for every problem)

```
project/
  models.hpp       — Data classes, enums, per-entity mutexes
  strategies.hpp   — Strategy pattern, Observer pattern, Chain of Responsibility
  managers.hpp     — Singleton manager(s), concurrency logic, static mutex init
  main.cpp         — Demo: sequential + concurrent + cancellation + strategy switch
```

**Why this structure?**
- 4 files, flat folder. No nested subdirectories.
- `models.hpp` = pure data. `strategies.hpp` = behaviors. `managers.hpp` = orchestration.
- Interviewer can see your entire design at a glance.
- Only `main.cpp` has `main()`. Everything else is headers (simpler compilation).

**Compilation:**
```bash
g++ -std=c++17 -pthread -o app.exe main.cpp
```

---

## 2. WHICH POINTER TO USE WHEN

| Pointer | Use When | Example |
|---------|----------|---------|
| `unique_ptr<T>` | **Sole owner.** One entity owns the object, nobody else. | Manager owns entities: `unique_ptr<Driver>`, `unique_ptr<Room>` |
| `shared_ptr<T>` | **Multiple owners.** Object stored in manager AND returned to caller. | `shared_ptr<Trip>` (in vector + returned), `shared_ptr<Booking>` |
| `shared_ptr<T>` | **Observer/Strategy shared with caller.** Caller may remove later. | `shared_ptr<Observer>` in vector + caller holds ref |
| `unique_ptr<T>` | **Strategy sole owner.** Manager owns it, swaps via `move()`. | `unique_ptr<PricingStrategy>`, `unique_ptr<RoomSelectionStrategy>` |
| `raw T*` | **Non-owning reference.** Borrowing from owner, not managing lifetime. | `Driver*` inside `Trip` (DriverMgr owns the driver) |

### Decision flowchart:
```
Who owns it?
├── Only one entity → unique_ptr
├── Multiple entities need it alive → shared_ptr
└── Just borrowing, someone else owns it → raw T*
```

### Quick rules:
- **Manager stores entities** → `unique_ptr` (map/vector owns them)
- **Manager returns to caller** → `shared_ptr` (both need it alive)
- **Strategy in manager** → `unique_ptr` (swap via move)
- **Observer in manager** → `shared_ptr` (caller may remove later)
- **Entity references another** → `raw ptr` (Trip -> Driver, Booking -> Show)

---

## 3. WHICH LOCK TO USE WHEN

| Lock | Use When | Example |
|------|----------|---------|
| `lock_guard<mutex>` | **Simple scope.** Lock at start, auto-unlock at end. | `addRoom()`, `addObserver()`, `displayAll()` |
| `unique_lock<mutex>` | **Need early release** or **condition_variable**. | `bookRoom()` (release before per-room lock), worker pool `cv.wait()` |
| Per-entity `mutex` | **Fine-grained.** Lock individual entity, not whole manager. | `roomMtx` in Room, `driverMtx` in Driver, `walletMtx` in Wallet |

### Lock levels (always acquire in order, never reverse):
```
Level 1: Manager/Service mutex → collections, strategies, observers
Level 2: Per-entity mutex → individual entity state
```

### PESSIMISTIC vs OPTIMISTIC:

**PESSIMISTIC (use for LLD interviews):**
- Lock BEFORE reading/writing shared state
- Simple, correct, no retry logic
- Use when: high contention, money involved, can't afford lost updates
- All our projects use this

**OPTIMISTIC (mention in interview, don't implement):**
- Read without lock, validate before commit (version/CAS)
- Use when: low contention, reads >> writes
- Example: "If reads were 99% and writes 1%, we could use optimistic with version numbers"

### Deadlock prevention rules:
1. Always acquire Level 1 before Level 2
2. Release Level 1 before expensive I/O (PSP calls, notifications)
3. Notify observers OUTSIDE the lock (snapshot pattern)
4. Never call into another manager while holding your lock

---

## 4. DESIGN PATTERNS CHEAT SHEET

| Pattern | When to Use | How It Appears |
|---------|-------------|----------------|
| **Singleton** | One global manager instance | `static getInstance()`, private constructor, deleted copy/assign |
| **Strategy** | Swappable algorithm (pricing, matching, selection) | `unique_ptr<Strategy>` in manager, `setStrategy(move(s))` |
| **Observer** | Notify on events (booking, cancellation, failure) | `vector<shared_ptr<Observer>>`, snapshot notify outside lock |
| **Chain of Responsibility** | Sequential validation/filtering | `shared_ptr<Validator>` chain with `setNext()` |
| **Factory** | Create objects based on runtime conditions | `determinePricingStrategy()` returns `unique_ptr<Strategy>` |

### Pattern to lock mapping:
- **Singleton**: `singletonMtx` (static) — protects creation only
- **Strategy**: read under `operationMtx`, no lock during execution if stateless
- **Observer**: snapshot under lock → notify outside lock
- **Chain of Resp**: immutable after setup → no lock needed

---

## 5. CONCURRENCY TEMPLATE (Copy-paste skeleton)

```cpp
// managers.hpp skeleton
class MyService {
    static mutex singletonMtx;
    mutable mutex serviceMtx;           // Level 1: protects collections

    unordered_map<string, shared_ptr<Entity>> entities;
    unique_ptr<MyStrategy> strategy;
    vector<shared_ptr<MyObserver>> observers;

    MyService() { strategy = make_unique<DefaultStrategy>(); }

    // Snapshot pattern for notifications
    void notifyEvent(const Entity& e) {
        vector<shared_ptr<MyObserver>> snap;
        { lock_guard<mutex> lock(serviceMtx); snap = observers; }
        for (auto& obs : snap) obs->onEvent(e);
    }

public:
    static MyService* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static MyService instance;
        return &instance;
    }

    // Simple operation: lock_guard
    void addEntity(shared_ptr<Entity> e) {
        lock_guard<mutex> lock(serviceMtx);
        entities[e->getId()] = e;
    }

    // Complex operation: release lock before per-entity work
    shared_ptr<Result> doWork(const string& entityId, ...) {
        shared_ptr<Entity> entity;
        string resultId;
        {
            lock_guard<mutex> lock(serviceMtx);  // L1
            entity = entities[entityId];
            resultId = generateId();
        }
        // L1 released — other threads can proceed

        bool ok = entity->tryDoSomething();  // L2 (per-entity lock inside)

        if (ok) {
            auto result = make_shared<Result>(...);
            { lock_guard<mutex> lock(serviceMtx); results[resultId] = result; }
            notifyEvent(*result);  // outside lock
            return result;
        }
        return nullptr;
    }
};
mutex MyService::singletonMtx;
```

---

## 6. HOW TO HANDLE "ADD MORE REQUIREMENTS" IN INTERVIEW

The whole architecture is designed for this. Here's how to extend:

### "Add a new payment method / pricing / matching algorithm"
→ **Add a new Strategy class.** Zero changes to manager.
```cpp
class NewPricing : public PricingStrategy { ... };
svc->setStrategy(make_unique<NewPricing>());
```

### "Add notifications (email, SMS, Slack, webhook)"
→ **Add a new Observer class.** Zero changes to manager.
```cpp
class SlackObserver : public MyObserver { ... };
svc->addObserver(make_shared<SlackObserver>());
```

### "Add validation (fraud check, rate limit, amount cap)"
→ **Add a new Validator in the chain.** Zero changes to manager.
```cpp
auto rateLimit = make_shared<RateLimitValidator>();
existingChain->setNext(rateLimit);
```

### "Support multiple cities / regions"
→ **Add a field to the model.** Filter in manager.
```cpp
// models.hpp: add string city to Theatre
// managers.hpp: searchShows(movieId, city) filters by city
```

### "Add priority / scheduling"
→ **New Strategy for ordering.** Manager uses strategy to pick next.
```cpp
class PriorityFirstStrategy : public SchedulingStrategy { ... };
```

### "Add persistence / survive restarts"
→ **Add serialize/deserialize to Model.** Manager adds persist/load methods.
```cpp
// models.hpp: string serialize() / static deserialize()
// managers.hpp: persistToFile() / loadFromFile()
```

### "Add concurrency / thread safety"
→ Already built in! Just explain the 2-level lock architecture.

### "Scale to multiple servers"
→ Say: "Replace in-memory collections with Redis/DB. Replace mutex with distributed locks (Redlock). Replace Observer with message queue (Kafka/SQS)."

---

## 7. INTERVIEW FLOW (How to present)

**Step 1 (2 min):** Draw the 4-file structure on whiteboard. List entities.

**Step 2 (5 min):** Write `models.hpp` — data classes, enums, per-entity mutex.

**Step 3 (5 min):** Write `strategies.hpp` — identify which algorithms are swappable.

**Step 4 (10 min):** Write `managers.hpp` — singleton, lock levels, core operations.

**Step 5 (3 min):** Write `main.cpp` — demo sequential, concurrent, strategy switch.

**Key phrases to say:**
- "I'm using pessimistic locking because [resource] has high contention"
- "unique_ptr here because manager is sole owner, shared_ptr there because both caller and manager need it alive"
- "I release the manager lock before per-entity operations so different [entities] can be processed in parallel"
- "Observers are notified outside the lock using snapshot pattern to prevent deadlock"
- "If you want to add a new [X], just create a new Strategy/Observer class — zero changes to the manager"

---

## 8. QUICK REFERENCE: All Projects

| Project | L1 Lock | L2 Lock | Strategy | Observer | Extra Pattern |
|---------|---------|---------|----------|----------|---------------|
| Parking Lot | ParkingMgr.operationMtx | — | RateStrategy, PaymentStrategy | — | — |
| Meeting Room | MeetingMgr.operationMtx | Room.roomMtx | BookingPolicy | NotificationObserver | — |
| Ride Service | TripMgr.operationMtx | Driver.driverMtx | PricingStrategy, MatchingStrategy | — | Factory |
| AWS Payment | PaymentService.serviceMtx | Wallet.walletMtx | ProcessorStrategy | PaymentObserver | Chain of Resp |
| Meeting Scheduler | Scheduler.schedulerMtx | Room.roomMtx + Participant.participantMtx | RoomSelectionStrategy | MeetingObserver | — |
| Job Scheduler | JobScheduler.schedulerMtx | Job.jobMtx | SchedulingStrategy | JobObserver | condition_variable, persistence |
| Book My Show | BookingService.serviceMtx | Show.showMtx | PricingStrategy, SeatSelectionStrategy | BookingObserver | — |
