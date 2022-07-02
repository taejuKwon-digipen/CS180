add_library(taejus_three_maps MODULE taejus_three_maps.cpp)
target_link_libraries(taejus_three_maps PRIVATE core project_warnings)

add_custom_command(TARGET taejus_three_maps POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:taejus_three_maps> ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/plugins/
)
