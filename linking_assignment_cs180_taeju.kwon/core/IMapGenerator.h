// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

#include "gameplay.h"

#include "plugin_visibility.h"

namespace gameplay
{
    // Is the interface level class of every map plugin.
    struct CORE_API IMapGenerator
    {
        virtual void         GenerateMap(Game::World& world, unsigned int current_level) = 0;
        virtual unsigned int GetStartingPlayerX() const                                  = 0;
        virtual unsigned int GetStartingPlayerY() const                                  = 0;


        IMapGenerator();
        IMapGenerator(const IMapGenerator& other);
        IMapGenerator(IMapGenerator&& other) noexcept;
        IMapGenerator& operator=(const IMapGenerator& other);
        IMapGenerator& operator=(IMapGenerator&& other) noexcept;
        virtual ~IMapGenerator();
    };

}
