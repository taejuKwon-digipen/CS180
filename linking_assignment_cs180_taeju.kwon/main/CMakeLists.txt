add_executable(FindFire main.cpp PluginInstance.h dll_extension.h platform.h platform_curses.cpp)

if(WIN32)
    target_sources(FindFire PRIVATE dll_extension_win32.cpp PRIVATE PluginInstance_win32.cpp)
    target_compile_definitions(FindFire PRIVATE UNICODE _UNICODE)
else()
    target_sources(FindFire PRIVATE dll_extension_posix.cpp PRIVATE PluginInstance_posix.cpp)
endif ()

target_link_libraries(FindFire PRIVATE core project_warnings ${CMAKE_DL_LIBS} $<$<PLATFORM_ID:Linux>:ncurses>)

set_property(TARGET FindFire PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/)
