// Teaju Kwon
// Linking Assignment
// CS180
// Fall 2021

#include <IMapGenerator.h>
#include <PluginFactory.h>
#include <gameplay.h>
#include <plugin_api.h>
#include <iostream>

using namespace std;

char map1[23][23] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,
1,1,1,1,0,1,1,1,1,1,1,2,1,1,1,1,1,1,1,0,1,1,1,
1,1,1,1,0,1,1,1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,1,
1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,2,1,1,0,1,1,1,
1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,
1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,
1,1,1,1,0,1,1,1,1,1,1,2,1,1,1,1,0,0,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,2,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,2,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,2,1,1,1,1,1,1,1,
1,1,1,2,1,1,1,1,0,0,1,1,2,1,1,1,1,1,1,3,1,1,1,
1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,2,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

char map2[23][23] =
{
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,1,3,1,1,1,1,0,0,0,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,1,1,2,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,2,1,1,1,1,1,
1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,
1,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,2,1,3,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

char map3[23][23] =
{
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,
1,1,1,1,0,1,0,3,1,1,1,1,1,0,1,0,1,1,1,2,1,2,1,
1,1,1,1,0,1,0,1,1,2,1,1,1,0,1,0,1,1,1,1,1,1,1,
1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,
1,1,3,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,2,1,1,1,
1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,
1,1,1,1,0,1,0,1,0,2,0,0,1,0,1,0,1,1,1,1,1,1,1,
1,1,1,1,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,1,1,1,1,
1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0,1,1,1,1,
1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,
1,1,1,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,
1,1,1,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,
1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,
1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,3,1,1,
1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,
1,1,3,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,
1,1,1,1,1,0,2,0,1,1,2,1,1,1,0,2,2,1,1,1,1,1,1,
1,1,1,1,1,1,1,2,2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,
1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
}

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
        // 0 == floor, 1 == wall, 2 == coin, 3 == torch
        unsigned int i;
        unsigned int j;
        if(i == 0 || j == 0)
        {
            Tiles::FLOOR;
        }else if( i == 1 || j == 1)
        {
            Tiles::FLOOR;    
        }else if( i == 2 || j == 1)
        {
            Tiles::COIN;
        }else if(i == 3 || j == 3 )
        {
            Tiles::TORCH;
        }
        const auto level_seed = current_level * SCALE_LEVEL + START_SEED;
        //random.set_seed(level_seed);
        if(current_level == 0)
        {
           world = map1[i][j];
        } else if(current_level == 1)
        {
            world = map2[i][j];
        } else if (current_level == 2)
        {
            world = map3[i][j];
        }

        //const auto random_coordinate     = [&] { return 1 + Game::MAP_SIZE - 2; };
        playerX                          = 1;
        playerY                          = 1;
        const unsigned stairs_row        = {20};
        const unsigned stairs_column     = {20};
        world[stairs_row][stairs_column] = gameplay::Tiles::STAIRS_DOWN;
    }

    unsigned int RandomMapGenerator::GetStartingPlayerX() const { return playerX; }

    unsigned int RandomMapGenerator::GetStartingPlayerY() const { return playerY; }
}
