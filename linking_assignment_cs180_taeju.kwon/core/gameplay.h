// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

#include "plugin_visibility.h"
#include <array>
#include <functional>

namespace gameplay
{
    struct IMapGenerator;

    enum class InputEvent
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        GameOver,
        UseNextPlugin
    };

    enum class Tiles
    {
        FLOOR,
        WALL,
        COIN,
        STAIRS_DOWN,
        TORCH
    };

    class Game
    {
    public:
        static constexpr int MAP_SIZE       = 23;
        static constexpr int MAX_VIEW_RANGE = 13;
        static constexpr int TORCH_AMOUNT   = 25;
        using World                         = std::array<std::array<Tiles, MAP_SIZE>, MAP_SIZE>;

    private:
        World                               theWorld{};
        unsigned                            playerX{0}, playerY{0};
        int                                 coinsCount{0}, movesCount{0}, torchCount{30};
        bool                                isDone{false};
        unsigned                            currentLevel{0};
        std::function<IMapGenerator*(void)> getNextMapGenerator{};
        IMapGenerator*                      currentMapGenerator{nullptr};

    public:
        Game(std::function<IMapGenerator*(void)> get_next_map_generator)
        {
            getNextMapGenerator = get_next_map_generator;
            currentMapGenerator = getNextMapGenerator();
        }

        CORE_API void OnPlayerAction(InputEvent event) noexcept;
        CORE_API void GenerateAndStartNextLevel() noexcept;

        CORE_API [[nodiscard]] constexpr bool         IsDone() const noexcept { return isDone; }
        CORE_API [[nodiscard]] constexpr int          CoinsCount() const noexcept { return coinsCount; }
        CORE_API [[nodiscard]] constexpr int          MovesCount() const noexcept { return movesCount; }
        CORE_API [[nodiscard]] constexpr int          TorchCount() const noexcept { return torchCount; }
        CORE_API [[nodiscard]] constexpr unsigned     PlayerX() const noexcept { return playerX; }
        CORE_API [[nodiscard]] constexpr unsigned     PlayerY() const noexcept { return playerY; }
        CORE_API [[nodiscard]] constexpr unsigned     CurrentLevel() const noexcept { return currentLevel; }
        CORE_API [[nodiscard]] constexpr const World& TheWorld() const noexcept { return theWorld; }
    };
}
