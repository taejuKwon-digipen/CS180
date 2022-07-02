// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021

#include "random.h"
#include <IMapGenerator.h>
#include <PluginFactory.h>
#include <gameplay.h>
#include <plugin_api.h>

namespace
{
    class RandomMapGenerator final : public gameplay::IMapGenerator
    {
    public:
        void         GenerateMap(gameplay::Game::World& world, unsigned int current_level) override;
        unsigned int GetStartingPlayerX() const override;
        unsigned int GetStartingPlayerY() const override;

    private:
        unsigned int playerX{0};
        unsigned int playerY{0};
        util::Random random{};
    };

    gameplay::IMapGenerator* mapGeneratorAllocate() { return new RandomMapGenerator{}; }

    void mapGeneratorFree(gameplay::IMapGenerator* ptr_imapgen) { delete ptr_imapgen; }
}

bool PluginInit(PluginFactory& plugin_factory)
{
    plugin_factory.RegisterMapGeneratorPlugin("K&R Rand", mapGeneratorAllocate, mapGeneratorFree);
    return true;
}

const char* PluginDisplayName() { return "Random Map Generation Implementation"; }

namespace
{
    void RandomMapGenerator::GenerateMap(gameplay::Game::World& world, unsigned int current_level)
    {
        using namespace gameplay;
        constexpr unsigned int START_SEED  = 9973;
        constexpr unsigned int SCALE_LEVEL = 811;

        const auto level_seed = current_level * SCALE_LEVEL + START_SEED;
        random.set_seed(level_seed);
        for (unsigned j = 0; j < Game::MAP_SIZE; j++)
        {
            for (unsigned i = 0; i < Game::MAP_SIZE; i++)
            {
                if (i == 0 || i == Game::MAP_SIZE - 1 || j == 0 || j == Game::MAP_SIZE - 1 ||
                    random.get_random_uint(10) == 0)
                    world[i][j] = Tiles::WALL;
                else if (random.get_random_uint(20) == 0)
                    world[i][j] = Tiles::COIN;
                else if (random.get_random_uint(100) == 0)
                    world[i][j] = Tiles::TORCH;
                else
                    world[i][j] = Tiles::FLOOR;
            }
        }

        const auto random_coordinate     = [&] { return 1 + random.get_random_uint(Game::MAP_SIZE - 2); };
        playerX                          = random_coordinate();
        playerY                          = random_coordinate();
        const unsigned stairs_row        = random_coordinate();
        const unsigned stairs_column     = random_coordinate();
        world[stairs_row][stairs_column] = gameplay::Tiles::STAIRS_DOWN;
    }

    unsigned int RandomMapGenerator::GetStartingPlayerX() const { return playerX; }

    unsigned int RandomMapGenerator::GetStartingPlayerY() const { return playerY; }
}
