/*
 * Taeju Kwon
 * threads jobsystem assignment
 * CS225
 * Fall 2021
 */
#pragma once
#include "ThreadSafeQueue.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class JobSystem
{
public:
    using Job = std::function<void(void)>;

    void DoJob(Job job);
    void WaitUntilDone();
    void DoJobs(int how_many, std::function<void(int)> job);

public:
    JobSystem();
    ~JobSystem(); // RAII

public:
    JobSystem(const JobSystem&) = delete;
    JobSystem(JobSystem&&)      = delete;
    JobSystem& operator=(const JobSystem&) = delete;
    JobSystem& operator=(JobSystem&&) = delete;

private:
    bool TryGetJob(Job& job);

private:
    std::vector<std::thread> threadWorkers;
    threadsafe_queue<Job>    jobs;
    std::mutex               queueMutex;
    std::condition_variable  wakeUpEvent;
    std::mutex               wakeUpMutex;
    std::atomic<bool>        isDone{false};

    std::atomic<int> numberOfJobs;
};
