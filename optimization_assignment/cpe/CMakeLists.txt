add_library(CPUCycles 
    CPUCyclesTimer.hpp CPUSpeed.hpp 
    CyclesPerElement.hpp CyclesPerElement.cpp)

if(WIN32)
    target_sources(CPUCycles PRIVATE platform/CPUSpeed_win32.cpp)
    target_compile_definitions(CPUCycles PRIVATE UNICODE _UNICODE)
elseif(APPLE)
    target_sources(CPUCycles PRIVATE platform/CPUSpeed_macos.cpp)
else()
    target_sources(CPUCycles PRIVATE platform/CPUSpeed_posix.cpp)
endif ()

target_include_directories(CPUCycles PUBLIC .)

target_link_libraries(CPUCycles PRIVATE project_options project_warnings)
