// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#include "PluginFactory.h"
#include <algorithm>

void PluginFactory::RegisterMapGeneratorPlugin(std::string unique_name, CreateMapGeneratorInstance create_map_generator,
                                               ReleaseMapGeneratorInstance release_map_generator)
{
    const auto key_value = std::make_pair(std::move(unique_name), AllocPair{create_map_generator, release_map_generator});
    mapGenMakers.insert(std::move(key_value));
}

PluginFactory::IMapGeneratorPtr PluginFactory::CreateMapGenerator(const std::string& unique_name) const
{
    IMapGeneratorPtr instance{nullptr, nullptr};
    const auto      itr_entry = mapGenMakers.find(unique_name);
    if (itr_entry != std::end(mapGenMakers))
    {
        const auto [create_now, release_for_later] = itr_entry->second;
        instance                                   = IMapGeneratorPtr{create_now(), release_for_later};
    }
    return instance;
}

std::vector<std::string> PluginFactory::GetMapGeneratorNames() const
{
    using namespace std;
    vector<string> names(mapGenMakers.size());
    transform(begin(mapGenMakers), end(mapGenMakers), begin(names), [](const auto& pair) { return pair.first; });
    return names;
}
