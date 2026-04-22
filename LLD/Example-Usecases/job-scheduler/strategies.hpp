#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include "models.hpp"
#include <queue>
#include <memory>
using namespace std;

// ============================================================
// STRATEGY PATTERN: Job Scheduling Policy
// Determines the ORDER in which jobs are dispatched to workers.
// unique_ptr<SchedulingStrategy>: JobScheduler is sole owner.
// ============================================================

class SchedulingStrategy {
public:
    virtual ~SchedulingStrategy() = default;
    virtual shared_ptr<Job> selectNext(vector<shared_ptr<Job>>& pendingJobs) = 0;
    virtual string getName() const = 0;
};

// Highest priority first, then FIFO within same priority
class PriorityFirstStrategy : public SchedulingStrategy {
public:
    shared_ptr<Job> selectNext(vector<shared_ptr<Job>>& pendingJobs) override {
        if (pendingJobs.empty()) return nullptr;
        int bestIdx = 0;
        for (int i = 1; i < (int)pendingJobs.size(); i++) {
            if (pendingJobs[i]->getPriority() > pendingJobs[bestIdx]->getPriority()) {
                bestIdx = i;
            }
        }
        auto job = pendingJobs[bestIdx];
        pendingJobs.erase(pendingJobs.begin() + bestIdx);
        return job;
    }
    string getName() const override { return "PriorityFirst"; }
};

// Simple FIFO
class FIFOStrategy : public SchedulingStrategy {
public:
    shared_ptr<Job> selectNext(vector<shared_ptr<Job>>& pendingJobs) override {
        if (pendingJobs.empty()) return nullptr;
        auto job = pendingJobs.front();
        pendingJobs.erase(pendingJobs.begin());
        return job;
    }
    string getName() const override { return "FIFO"; }
};

// ============================================================
// OBSERVER PATTERN: Job Lifecycle Notifications
// shared_ptr<JobObserver>: shared between scheduler + caller.
// ============================================================

class JobObserver {
public:
    virtual ~JobObserver() = default;
    virtual void onJobCompleted(shared_ptr<Job> job) = 0;
    virtual void onJobFailed(shared_ptr<Job> job) = 0;
    virtual void onJobDeadLettered(shared_ptr<Job> job) = 0;
    virtual string getName() const = 0;
};

class LoggingObserver : public JobObserver {
public:
    void onJobCompleted(shared_ptr<Job> job) override {
        cout << "    [Log] Job " << job->getId() << " completed" << endl;
    }
    void onJobFailed(shared_ptr<Job> job) override {
        cout << "    [Log] Job " << job->getId() << " failed (retry "
             << job->getRetryCount() << "): " << job->getLastError() << endl;
    }
    void onJobDeadLettered(shared_ptr<Job> job) override {
        cout << "    [Log] Job " << job->getId() << " -> DEAD LETTER QUEUE" << endl;
    }
    string getName() const override { return "Logger"; }
};

class AlertObserver : public JobObserver {
public:
    void onJobCompleted(shared_ptr<Job> job) override {}
    void onJobFailed(shared_ptr<Job> job) override {}
    void onJobDeadLettered(shared_ptr<Job> job) override {
        cout << "    [ALERT] CRITICAL: Job " << job->getId()
             << " dead-lettered after " << job->getRetryCount()
             << " retries!" << endl;
    }
    string getName() const override { return "Alert"; }
};

#endif
