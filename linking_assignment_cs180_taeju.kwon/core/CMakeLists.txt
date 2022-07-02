add_library(core SHARED 
    gameplay.cpp gameplay.h
    IMapGenerator.cpp IMapGenerator.h
    plugin_api.h
    plugin_visibility.h
    PluginFactory.cpp PluginFactory.h
)

target_compile_definitions(core PRIVATE CORE_DLL_EXPORTS)
target_link_libraries(core PRIVATE project_warnings)
