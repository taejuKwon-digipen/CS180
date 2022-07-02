/*
 * Taeju Kwon
 * threads jobsystem assignment
 * CS225
 * Fall 2021
 */
#include "JobSystem.h"


using namespace std;

void JobSystem::DoJob(std::function<void()> job)
{
    ++numberOfJobs;

    jobs.push(job);

    wakeUpEvent.notify_one();
}

void JobSystem::WaitUntilDone()
{
    Job job;

    while (numberOfJobs > 0)
    {
        if (TryGetJob(job))
        {
            job();

            --numberOfJobs;
        }
    }

    // do your thang main thread
}

JobSystem::JobSystem()
{
    // get the number of HW cores to decide how many thread to make
    // create x amount of threads and store them in our

    const int num_hw_threads = static_cast<int>(thread::hardware_concurrency());
    const int num_threads    = (num_hw_threads > 0) ? num_hw_threads - 1 : 1;

    for (int i = 0; i < num_threads; ++i)
    {
        threadWorkers.emplace_back(
            [this]()
            {
                Job job;

                while (!isDone)
                {
                    if (TryGetJob(job))
                    {
                        job();

                        --numberOfJobs;
                    }
                    else
                    {
                        unique_lock lock(wakeUpMutex);
                        wakeUpEvent.wait(lock);
                    }
                }
            });
    }
}

JobSystem::~JobSystem()
{
    isDone = true;
    wakeUpEvent.notify_all();

    for (auto& t : threadWorkers)
    {
        if (t.joinable())
            t.join();
    }
}

bool JobSystem::TryGetJob(JobSystem::Job& job) { return jobs.try_pop(job); }

void JobSystem::DoJobs(int how_many, std::function<void(int)> job)
{
    int num_chunks = static_cast<int>(threadWorkers.size());
    int chunk_size = how_many / num_chunks;
    int remainder  = how_many % num_chunks;

    for (int i = 0; i < num_chunks; i++)
    {
        DoJob(
            [this, chunk_size, job, i, remainder]()
            {
                for (int k = chunk_size * i; k < chunk_size * (i + 1); k++)
                {
                    job(k);
                }
            }

        );
    }
    DoJob(
        [how_many, remainder, job]()
        {
            for (int r = (how_many - remainder); r < how_many; r++)
            {
                job(r);
            }
        });
}