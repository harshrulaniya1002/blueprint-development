#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>
#include <chrono>
#include <functional>
#include <fstream>
#include <sstream>
#include <atomic>
using namespace std;

// ============================================================
// ENUMS
// ============================================================
enum class JobStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    DEAD_LETTERED
};

enum class JobPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    CRITICAL = 3
};

inline string statusToString(JobStatus s) {
    switch (s) {
        case JobStatus::PENDING:       return "PENDING";
        case JobStatus::RUNNING:       return "RUNNING";
        case JobStatus::COMPLETED:     return "COMPLETED";
        case JobStatus::FAILED:        return "FAILED";
        case JobStatus::DEAD_LETTERED: return "DEAD_LETTERED";
        default: return "UNKNOWN";
    }
}

inline string priorityToString(JobPriority p) {
    switch (p) {
        case JobPriority::LOW:      return "LOW";
        case JobPriority::MEDIUM:   return "MEDIUM";
        case JobPriority::HIGH:     return "HIGH";
        case JobPriority::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

// ============================================================
// RETRY POLICY
// Defines max retries and base delay for exponential backoff.
// Immutable after creation — no mutex needed.
// ============================================================
class RetryPolicy {
    int maxRetries;
    int baseDelayMs;   // base delay in milliseconds

public:
    RetryPolicy(int maxRetries = 3, int baseDelayMs = 100)
        : maxRetries(maxRetries), baseDelayMs(baseDelayMs) {}

    int getMaxRetries() const { return maxRetries; }
    int getBaseDelayMs() const { return baseDelayMs; }

    // Exponential backoff: delay = base * 2^attempt
    int getDelayMs(int attempt) const {
        return baseDelayMs * (1 << attempt);
    }
};

// ============================================================
// JOB
// Has its own mutex for thread-safe status updates.
// Workers lock the job while executing it.
// PESSIMISTIC: lock job before any state change.
//
// Persist format: "id|name|priority|status|retryCount|maxRetries|baseDelay"
// ============================================================
class Job {
    string jobId;
    string name;
    JobPriority priority;
    JobStatus status;
    int retryCount;
    RetryPolicy retryPolicy;
    function<bool()> task;  // the actual work to execute
    mutable mutex jobMtx;
    chrono::steady_clock::time_point scheduledTime;
    string lastError;

public:
    Job(const string& id, const string& name, JobPriority priority,
        RetryPolicy policy, function<bool()> task)
        : jobId(id), name(name), priority(priority),
          status(JobStatus::PENDING), retryCount(0),
          retryPolicy(policy), task(task),
          scheduledTime(chrono::steady_clock::now()) {}

    // For deserialization (no task — needs re-registration)
    Job(const string& id, const string& name, JobPriority priority,
        RetryPolicy policy, JobStatus status, int retryCount)
        : jobId(id), name(name), priority(priority),
          status(status), retryCount(retryCount),
          retryPolicy(policy), task(nullptr),
          scheduledTime(chrono::steady_clock::now()) {}

    string getId() const { return jobId; }
    string getName() const { return name; }
    JobPriority getPriority() const { return priority; }

    JobStatus getStatus() {
        lock_guard<mutex> lock(jobMtx);
        return status;
    }

    void setStatus(JobStatus s) {
        lock_guard<mutex> lock(jobMtx);
        status = s;
    }

    int getRetryCount() {
        lock_guard<mutex> lock(jobMtx);
        return retryCount;
    }

    void incrementRetry() {
        lock_guard<mutex> lock(jobMtx);
        retryCount++;
    }

    const RetryPolicy& getRetryPolicy() const { return retryPolicy; }

    void setLastError(const string& err) {
        lock_guard<mutex> lock(jobMtx);
        lastError = err;
    }

    string getLastError() {
        lock_guard<mutex> lock(jobMtx);
        return lastError;
    }

    bool hasTask() const { return task != nullptr; }

    void setTask(function<bool()> t) { task = t; }

    // Execute the task. Returns true on success, false on failure.
    // Called by worker threads — job mutex NOT held during execution
    // (to avoid blocking other operations on this job).
    bool execute() {
        if (!task) return false;
        return task();
    }

    // Serialize for persistence
    string serialize() const {
        ostringstream oss;
        oss << jobId << "|" << name << "|"
            << (int)priority << "|" << (int)status << "|"
            << retryCount << "|"
            << retryPolicy.getMaxRetries() << "|"
            << retryPolicy.getBaseDelayMs();
        return oss.str();
    }

    // Deserialize
    static shared_ptr<Job> deserialize(const string& line) {
        istringstream iss(line);
        string id, name;
        int pri, stat, retry, maxRetry, baseDelay;
        char delim;
        getline(iss, id, '|');
        getline(iss, name, '|');
        iss >> pri >> delim >> stat >> delim >> retry >> delim
            >> maxRetry >> delim >> baseDelay;
        return make_shared<Job>(id, name, (JobPriority)pri,
                                 RetryPolicy(maxRetry, baseDelay),
                                 (JobStatus)stat, retry);
    }

    void display() {
        lock_guard<mutex> lock(jobMtx);
        cout << "  Job[" << jobId << "] " << name
             << " | " << priorityToString(priority)
             << " | " << statusToString(status)
             << " | retries: " << retryCount << "/" << retryPolicy.getMaxRetries();
        if (!lastError.empty()) cout << " | err: " << lastError;
        cout << endl;
    }
};

#endif
