add_library(random_maps MODULE random_maps.cpp random.h)
target_link_libraries(random_maps PRIVATE core project_warnings)

add_custom_command(TARGET random_maps POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:random_maps> ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/plugins/
)
