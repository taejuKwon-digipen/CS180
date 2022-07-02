add_library(jobsystem STATIC JobSystem.cpp JobSystem.h ThreadSafeQueue.h)
target_link_libraries(jobsystem PRIVATE project_warnings Threads::Threads)
