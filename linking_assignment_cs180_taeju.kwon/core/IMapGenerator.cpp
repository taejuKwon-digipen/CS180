// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#include "IMapGenerator.h"

namespace gameplay
{
    IMapGenerator::IMapGenerator()                               = default;
    IMapGenerator::IMapGenerator(const IMapGenerator& other)     = default;
    IMapGenerator::IMapGenerator(IMapGenerator&& other) noexcept = default;
    IMapGenerator& IMapGenerator::operator=(const IMapGenerator& other) = default;
    IMapGenerator& IMapGenerator::operator=(IMapGenerator&& other) noexcept = default;
    IMapGenerator::~IMapGenerator()                                         = default;
}
