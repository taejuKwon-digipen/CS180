// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021

#include "dll_extension.h"
#include "gameplay.h"
#include "platform.h"

#include "PluginInstance.h"
#include <PluginFactory.h>
#include <iostream>
#include <vector>

namespace
{
    std::vector<PluginInstance> GetAllPlugins();
    constexpr auto              PluginsBasePath = "plugins";
}

int main(void)
try
{
    const auto plugins = GetAllPlugins();
    {
        PluginFactory factory{};

        std::cout << "Found " << plugins.size() << " plugins\n";
        for (auto& plugin : plugins)
        {
            plugin.Load(factory);
            std::cout << "Loading : " << plugin.GetDisplayName() << " from " << plugin.GetFilePath() << '\n';            
        }
        std::cout << "\n\nW,A,S,D to move around\nN to load the next plugin\nEsc to quit\n\n";
        std::cout << "Enter P to play : " << std::endl;
        std::cin.ignore(), std::cin.get();
        const auto map_generator_names = factory.GetMapGeneratorNames();
        if (map_generator_names.empty())
            throw std::runtime_error{"Unable to find any map generators, so we cannot run the game..."};
        std::string::size_type name_index{map_generator_names.size() - 1};
        auto                   current_map_generator = factory.CreateMapGenerator(map_generator_names[name_index]);


        platform::startup();

        gameplay::Game game{[&]()
                            {
                                name_index            = (name_index + 1) % map_generator_names.size();
                                const auto& name      = map_generator_names[name_index];
                                current_map_generator = factory.CreateMapGenerator(name);
                                platform::notify_message(name.c_str());
                                return current_map_generator.get();
                            }};


        game.GenerateAndStartNextLevel();
        while (!game.IsDone())
        {
            platform::update(game);
        }
        platform::shutdown();
    }

    for (auto& plugin : plugins)
    {
        plugin.Unload();
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return -1;
}

namespace
{
    std::vector<PluginInstance> GetAllPlugins()
    {
        using namespace std::filesystem;
        std::vector<PluginInstance> plugins;
        for (const directory_entry& dir_entry : directory_iterator(PluginsBasePath))
        {
            if (!dir_entry.is_regular_file())
                continue;
            const auto& file_path = dir_entry.path();
            if (!file_path.has_extension() || file_path.extension() != util::platform_dll_extension())
                continue;
            plugins.emplace_back(file_path);
        }
        return plugins;
    }
}
