add_executable(benchmark 
    benchmark.cpp benchmark.h
    polynomial.c polynomial.h
    vec.c vec.h
    Random.cpp Random.hpp)

target_link_libraries(benchmark PRIVATE CPUCycles project_options project_warnings)
