# Threads Assignment

For this assignment you will create a multi-threaded Job System

## Introduction

From class discussions, you should already have the Job System started that implements the two functions:

```c++
void DoJob(std::function<void(void)> job);
void WaitUntilDone();
```

You will be adding a new member function : 
```c++
void DoJobs(int how_many, const std::function<void(int)>& job);
```

You will also be simplifying the implementation by adding logic to use **atomic** variables and a **lock based queue**.



## Project Setup and Dependencies


### CMake to build the project

We are using [CMake](https://cmake.org) to define the project and build it for either Windows or Ubuntu. 


To build the project on the command line **`cd`** into the root of the project and invoke the following commands:
```sh
# create a folder called build and generate build files from this source directory
# Tell cmake to generate Release version of the project. This is needed when making makefiles
cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
# create the assignment exe using the build files
# --config Release isn't needed if we have makefiles but if we have XCode/Visual Studio then 
#   we need this flag to get a Release exe
cmake --build build --config Release
```

Note the executable will be in the **\build\dist\CONFIG** folder, where CONFIG could be Debug, Release, etc...


## Task 1 - Add DoJobs()

The `DoJobs()` member function will allow users to define a job that gets broken up into chunks. The amount of chunks will be determined by the JobSystem. Users will say how many total elements there are and provide a function object that will take an index to let them do some work for an element identified by the index.

Example :
```c++
vector<double> values(300'000);
// Each element will have a value of it's index
job_system.DoJobs(300'000, [&](int index){
    values[index] = 1.0 * index;
});
```

Based off of the number of threads in the pool, the 300,000 range will be divided up into chunks. Then for each chunk it will increment an index and invoke the function object.

```js
// non-threaded pseudo code version
function DoJobs(how_many, job)
{
    var num_chunks = calculate_chunk_count(how_many);
    var chunk_size = how_many / num_chunks;
    // TODO Parallelize the iterations of this loop
    // TODO Handle edge cases where how_many is not an
    //      exact multiple of # of worker threads
    for(let i = 0; i < num_chunks; i+=chunk_size)
    {
        // Inner Loop - run on a single thread
        for(let current_index = i; current_index < i+chunk_size; ++current_index)
        {
            job(current_index);
        }
    }
}
```

`DoJobs()` needs to run the inner loops across multiple threads.


### Motivation

The goal of the `DoJobs()` function is to have an efficient way to run a small computation many, many times.


## Task 2 - Utilize atomic variables

Use atomic variables to replace using mutexes for synchronizing access to the `isDone` and `numberOfJobs` fields.

This should simplify the code, be slightly more efficient and still keep their state synchronized across all threads.

## Task 3 - Create and utilize a lock based Queue

Based off the study material create a thread safe queue using mutex locking to replace the current std::queue being used.

This should simplify the code of the Job System and still keep it's state synchronized across all threads.

## Create a ReadMe

Create a **`ReadMe.md`** markdown file that defines the following.

1. Your name, assignment name/number, course name, term
2. Section describing anything incomplete about the assignment
3. Section describing something you're proud of about the assignment

# Submission

Submit your `JobSystem.cpp`, `JobSystem.h` and `ThreadSafeQueue.h` on the course site. 


## Grading Rubric

- [ ] [**core**] Implemented the `DoJobs()` member function so that users can do many small computations across the threads in the thread pool.
- [ ] [**core**] Simplified the JobSystem code by using atomic variables
- [ ] [**core**] Simplified the JobSystem code by creating and using a thread safe queue
- [ ] Code builds and runs on Windows and Ubuntu
- [ ] Create a properly filled out `ReadMe.md` file.
- [ ] All source files have a proper header comment with your name, assignment name/number, course name, term.
- [ ] All source files compile without warnings and without errors on both platforms
- [ ] Followed the appropriate Code Quality Guidelines and Code Style Guidelines.

Scores for Assignments will be given as the following:

Score        | Assessment
------------ | ----------
Zero         | Nothing turned in at all
Failing      | Close to meeting core requirements
Rudimentary  | Meets all of the core requirements
Satisfactory | Close to meeting all requirements
Good         | Clearly meets all requirements 
Excellent    | High quality, well beyond the requirements

### Ideas to get an Excellent score

Add more features for users or make the job system more efficient.

Checkout the following to get some ideas:

[Building a load-balanced task scheduler](https://blog.molecular-matters.com/2012/04/05/building-a-load-balanced-task-scheduler-part-1-basics/)

[Job System 2.0: Lock-Free Work Stealing](https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/)

