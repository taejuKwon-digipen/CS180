// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

#include "plugin_visibility.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace gameplay
{
    struct IMapGenerator;
}

class PluginFactory
{
public:
    using CreateMapGeneratorInstance  = gameplay::IMapGenerator* (*)();
    using ReleaseMapGeneratorInstance = void (*)(gameplay::IMapGenerator*);
    using IMapGeneratorPtr             = std::unique_ptr<gameplay::IMapGenerator, ReleaseMapGeneratorInstance>;

    CORE_API void RegisterMapGeneratorPlugin(std::string unique_name, CreateMapGeneratorInstance create_map_generator,
                                             ReleaseMapGeneratorInstance release_map_generator);

    CORE_API IMapGeneratorPtr CreateMapGenerator(const std::string& unique_name) const;

    CORE_API std::vector<std::string> GetMapGeneratorNames() const;

private:
    struct AllocPair
    {
        CreateMapGeneratorInstance  create{nullptr};
        ReleaseMapGeneratorInstance release{nullptr};
    };
    std::unordered_map<std::string, AllocPair> mapGenMakers{};
};
