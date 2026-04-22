#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "managers.hpp"
using namespace std;

int main() {
    auto* sched = JobScheduler::getInstance();

    cout << "=== Job Scheduler ===" << endl << endl;

    // Observers
    sched->addObserver(make_shared<LoggingObserver>());
    sched->addObserver(make_shared<AlertObserver>());

    // Start worker pool (3 workers)
    sched->start(3);

    // ========== 1. Submit jobs with different priorities ==========
    cout << "\n--- Submitting Jobs ---" << endl;

    // Job that always succeeds
    auto job1 = make_shared<Job>("JOB_1", "SendEmail", JobPriority::LOW,
        RetryPolicy(3, 50), []() {
            this_thread::sleep_for(chrono::milliseconds(50));
            return true;
        });

    // Job that always succeeds (high priority — runs first)
    auto job2 = make_shared<Job>("JOB_2", "ProcessPayment", JobPriority::CRITICAL,
        RetryPolicy(3, 100), []() {
            this_thread::sleep_for(chrono::milliseconds(30));
            return true;
        });

    // Job that fails twice then succeeds (tests exponential backoff)
    static atomic<int> attempt3{0};
    auto job3 = make_shared<Job>("JOB_3", "SyncDatabase", JobPriority::HIGH,
        RetryPolicy(3, 100), []() {
            int a = ++attempt3;
            this_thread::sleep_for(chrono::milliseconds(20));
            if (a <= 2) return false;  // fail first 2 attempts
            return true;               // succeed on 3rd
        });

    // Job that always fails (tests dead letter queue)
    auto job4 = make_shared<Job>("JOB_4", "FlakyAPI", JobPriority::MEDIUM,
        RetryPolicy(2, 50), []() {
            this_thread::sleep_for(chrono::milliseconds(20));
            return false;  // always fails
        });

    // Job with high priority (tests priority ordering)
    auto job5 = make_shared<Job>("JOB_5", "GenerateReport", JobPriority::HIGH,
        RetryPolicy(1, 100), []() {
            this_thread::sleep_for(chrono::milliseconds(40));
            return true;
        });

    sched->submitJob(job1);
    sched->submitJob(job2);
    sched->submitJob(job3);
    sched->submitJob(job4);
    sched->submitJob(job5);

    // Wait for all jobs to process (including retries with backoff)
    this_thread::sleep_for(chrono::milliseconds(3000));

    // ========== 2. Persistence ==========
    cout << "\n--- Persisting Jobs ---" << endl;
    sched->persistJobs();

    // ========== 3. Display results ==========
    sched->displayAllJobs();
    sched->displayDeadLetterQueue();

    // ========== 4. Shutdown ==========
    cout << "\n--- Shutdown ---" << endl;
    sched->shutdown();

    // ========== 5. Simulate restart: load persisted jobs ==========
    // In a real system this would be a new process.
    // Here we just demonstrate the API.
    cout << "\n--- Simulating Restart (load from file) ---" << endl;
    // Note: loaded jobs need tasks re-registered since functions
    // can't be serialized. Jobs without tasks will fail execution.

    cout << "\n=== Done ===" << endl;
    return 0;
}
