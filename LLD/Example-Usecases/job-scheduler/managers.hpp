#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include "strategies.hpp"
#include <unordered_map>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <fstream>
#include <atomic>
using namespace std;

// ============================================================
// LOCKING: 2-level pessimistic
// Level 1: schedulerMtx (JobScheduler)
//   -> pendingJobs, allJobs, deadLetterQueue, observers, strategy
//   -> lock_guard for simple reads/writes
//   -> unique_lock + condition_variable for worker wait/notify
//
// Level 2: jobMtx (per-Job)
//   -> job status, retry count, last error
//   -> lock_guard inside Job methods (atomic status updates)
//
// WORKER POOL:
//   Workers block on condition_variable when no jobs pending.
//   submitJob signals one worker. Workers loop until shutdown.
//
// PERSISTENCE:
//   persistJobs() writes all jobs to a file.
//   loadJobs() reads them back. Tasks must be re-registered
//   (functions can't be serialized).
//
// EXPONENTIAL BACKOFF:
//   On failure: delay = baseDelay * 2^attempt, then re-queue.
//   After maxRetries: move to dead letter queue.
//
// SMART POINTERS:
//   shared_ptr<Job>         -> scheduler + workers + caller all hold refs
//   unique_ptr<Strategy>    -> scheduler sole owner
//   shared_ptr<JobObserver> -> scheduler vector + caller
// ============================================================

class JobScheduler {
    static mutex singletonMtx;
    mutex schedulerMtx;
    condition_variable cv;

    vector<shared_ptr<Job>> pendingJobs;
    unordered_map<string, shared_ptr<Job>> allJobs;
    vector<shared_ptr<Job>> deadLetterQueue;
    vector<shared_ptr<JobObserver>> observers;
    unique_ptr<SchedulingStrategy> strategy;

    vector<thread> workers;
    atomic<bool> running{false};
    string persistFile = "jobs.dat";

    JobScheduler() {
        strategy = make_unique<PriorityFirstStrategy>();
    }

    // Snapshot pattern for notifications
    void notifyCompleted(shared_ptr<Job> job) {
        vector<shared_ptr<JobObserver>> snap;
        { lock_guard<mutex> lock(schedulerMtx); snap = observers; }
        for (auto& obs : snap) obs->onJobCompleted(job);
    }

    void notifyFailed(shared_ptr<Job> job) {
        vector<shared_ptr<JobObserver>> snap;
        { lock_guard<mutex> lock(schedulerMtx); snap = observers; }
        for (auto& obs : snap) obs->onJobFailed(job);
    }

    void notifyDeadLettered(shared_ptr<Job> job) {
        vector<shared_ptr<JobObserver>> snap;
        { lock_guard<mutex> lock(schedulerMtx); snap = observers; }
        for (auto& obs : snap) obs->onJobDeadLettered(job);
    }

    // Worker thread function
    void workerLoop(int workerId) {
        while (true) {
            shared_ptr<Job> job;

            // Wait for a job (unique_lock for condition_variable)
            {
                unique_lock<mutex> lock(schedulerMtx);
                cv.wait(lock, [this] {
                    return !pendingJobs.empty() || !running;
                });

                if (!running && pendingJobs.empty()) return;

                job = strategy->selectNext(pendingJobs);
                if (!job) continue;
            }
            // schedulerMtx released — worker executes without holding it

            job->setStatus(JobStatus::RUNNING);
            cout << "  [Worker " << workerId << "] Executing "
                 << job->getId() << " (" << job->getName() << ")" << endl;

            bool success = job->execute();

            if (success) {
                job->setStatus(JobStatus::COMPLETED);
                notifyCompleted(job);
            } else {
                job->incrementRetry();
                int retries = job->getRetryCount();
                int maxRetries = job->getRetryPolicy().getMaxRetries();
                job->setLastError("execution failed");

                if (retries >= maxRetries) {
                    // Dead letter queue
                    job->setStatus(JobStatus::DEAD_LETTERED);
                    {
                        lock_guard<mutex> lock(schedulerMtx);
                        deadLetterQueue.push_back(job);
                    }
                    notifyDeadLettered(job);
                } else {
                    // Exponential backoff: sleep then re-queue
                    int delayMs = job->getRetryPolicy().getDelayMs(retries - 1);
                    cout << "  [Worker " << workerId << "] "
                         << job->getId() << " failed, retry " << retries
                         << "/" << maxRetries << " in " << delayMs << "ms" << endl;

                    notifyFailed(job);

                    this_thread::sleep_for(chrono::milliseconds(delayMs));

                    job->setStatus(JobStatus::PENDING);
                    {
                        lock_guard<mutex> lock(schedulerMtx);
                        pendingJobs.push_back(job);
                    }
                    cv.notify_one();
                }
            }
        }
    }

public:
    static JobScheduler* getInstance() {
        lock_guard<mutex> lock(singletonMtx);
        static JobScheduler instance;
        return &instance;
    }

    void setStrategy(unique_ptr<SchedulingStrategy> s) {
        lock_guard<mutex> lock(schedulerMtx);
        strategy = move(s);
        cout << "[Scheduler] Strategy: " << strategy->getName() << endl;
    }

    void addObserver(shared_ptr<JobObserver> obs) {
        lock_guard<mutex> lock(schedulerMtx);
        observers.push_back(obs);
    }

    // Start worker pool
    void start(int numWorkers) {
        running = true;
        for (int i = 0; i < numWorkers; i++) {
            workers.emplace_back(&JobScheduler::workerLoop, this, i);
        }
        cout << "[Scheduler] Started " << numWorkers << " workers" << endl;
    }

    // Graceful shutdown: finish pending, then stop
    void shutdown() {
        {
            lock_guard<mutex> lock(schedulerMtx);
            running = false;
        }
        cv.notify_all();
        for (auto& w : workers) w.join();
        workers.clear();
        cout << "[Scheduler] Shutdown complete" << endl;
    }

    // Submit a job
    void submitJob(shared_ptr<Job> job) {
        {
            lock_guard<mutex> lock(schedulerMtx);
            allJobs[job->getId()] = job;
            pendingJobs.push_back(job);
        }
        cv.notify_one();
        cout << "[Scheduler] Submitted: " << job->getId()
             << " (" << job->getName() << ") "
             << priorityToString(job->getPriority()) << endl;
    }

    // Persist all jobs to file (survive restarts)
    void persistJobs() {
        lock_guard<mutex> lock(schedulerMtx);
        ofstream out(persistFile);
        for (auto& [id, job] : allJobs) {
            out << job->serialize() << "\n";
        }
        cout << "[Scheduler] Persisted " << allJobs.size() << " jobs to "
             << persistFile << endl;
    }

    // Load jobs from file. Tasks must be re-registered via registerTask().
    void loadJobs() {
        lock_guard<mutex> lock(schedulerMtx);
        ifstream in(persistFile);
        if (!in.is_open()) {
            cout << "[Scheduler] No persist file found" << endl;
            return;
        }
        string line;
        int count = 0;
        while (getline(in, line)) {
            if (line.empty()) continue;
            auto job = Job::deserialize(line);
            allJobs[job->getId()] = job;
            // Re-queue pending/failed jobs
            if (job->getStatus() == JobStatus::PENDING ||
                job->getStatus() == JobStatus::FAILED) {
                job->setStatus(JobStatus::PENDING);
                pendingJobs.push_back(job);
            }
            count++;
        }
        cout << "[Scheduler] Loaded " << count << " jobs from " << persistFile << endl;
    }

    // Register a task function for a loaded job
    void registerTask(const string& jobId, function<bool()> task) {
        lock_guard<mutex> lock(schedulerMtx);
        auto it = allJobs.find(jobId);
        if (it != allJobs.end()) {
            it->second->setTask(task);
        }
    }

    // Display
    void displayAllJobs() {
        lock_guard<mutex> lock(schedulerMtx);
        cout << "\n=== All Jobs ===" << endl;
        for (auto& [id, job] : allJobs) job->display();
    }

    void displayDeadLetterQueue() {
        lock_guard<mutex> lock(schedulerMtx);
        cout << "\n=== Dead Letter Queue ===" << endl;
        if (deadLetterQueue.empty()) {
            cout << "  (empty)" << endl;
            return;
        }
        for (auto& job : deadLetterQueue) job->display();
    }

    int getPendingCount() {
        lock_guard<mutex> lock(schedulerMtx);
        return pendingJobs.size();
    }

    JobScheduler(const JobScheduler&) = delete;
    JobScheduler& operator=(const JobScheduler&) = delete;
};

mutex JobScheduler::singletonMtx;

#endif
