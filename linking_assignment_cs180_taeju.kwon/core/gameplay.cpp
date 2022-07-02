// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#include "gameplay.h"
#include "IMapGenerator.h"

namespace gameplay
{
    void Game::OnPlayerAction(InputEvent event) noexcept
    {
        unsigned new_x = playerX;
        unsigned new_y = playerY;

        switch (event)
        {
            case InputEvent::MoveLeft: --new_x; break;
            case InputEvent::MoveRight: ++new_x; break;
            case InputEvent::MoveUp: --new_y; break;
            case InputEvent::MoveDown: ++new_y; break;
            case InputEvent::GameOver: isDone = true; return;
            case InputEvent::UseNextPlugin:
                currentMapGenerator = getNextMapGenerator();
                GenerateAndStartNextLevel();
                return;
        }

        if (new_x >= MAP_SIZE || new_y >= MAP_SIZE)
            return;

        const auto new_tile_type = theWorld[new_y][new_x];
        if (new_tile_type == Tiles::WALL)
            return;

        ++movesCount;
        --torchCount;

        if (torchCount <= 0)
        {
            isDone = true;
            return;
        }

        playerX = new_x;
        playerY = new_y;

        switch (new_tile_type)
        {
            case Tiles::COIN:
                ++coinsCount;
                theWorld[new_y][new_x] = Tiles::FLOOR;
                break;
            case Tiles::STAIRS_DOWN: GenerateAndStartNextLevel(); break;
            case Tiles::TORCH:
                torchCount += TORCH_AMOUNT;
                theWorld[new_y][new_x] = Tiles::FLOOR;
                break;
            default: break;
        }
    }

    void Game::GenerateAndStartNextLevel() noexcept
    {
        IMapGenerator& map_generator = *currentMapGenerator;
        map_generator.GenerateMap(theWorld, currentLevel);
        playerX = map_generator.GetStartingPlayerX();
        playerY = map_generator.GetStartingPlayerY();
        ++currentLevel;
    }
}
