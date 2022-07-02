// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#include "platform.h"
#include <curses.h>

namespace
{
    void draw_game(const gameplay::Game& game) noexcept;

    namespace Color
    {
        enum Color : short
        {
            Grass = 1,
            Rock,
            Player,
            Coin,
            Torch,
            Stairs,
            Black
        };
    }

    namespace Char
    {
        enum Char : char
        {
            Grass  = '.',
            Rock   = '#',
            Player = '@',
            Coin   = 'o',
            Torch  = '+',
            Stairs = '<'
        };
    }

    constexpr char KEY_ESCAPE = 27;
}


namespace platform
{
    void startup() noexcept
    {
        initscr();
        noecho();
        cbreak();
        timeout(-1);
        curs_set(FALSE);

        start_color();
        init_pair(Color::Grass, COLOR_BLUE, COLOR_BLACK);
        init_pair(Color::Rock, COLOR_CYAN, COLOR_BLACK);
        init_pair(Color::Player, COLOR_WHITE, COLOR_BLACK);
        init_pair(Color::Coin, COLOR_YELLOW, COLOR_BLACK);
        init_pair(Color::Torch, COLOR_RED, COLOR_BLACK);
        init_pair(Color::Stairs, COLOR_GREEN, COLOR_BLACK);
        init_pair(Color::Black, COLOR_BLACK, COLOR_BLACK);

        clear();
    }

    void update(gameplay::Game& game) noexcept
    {
        draw_game(game);
        const auto key = getch();
        if (key == KEY_ESCAPE)
        {
            game.OnPlayerAction(gameplay::InputEvent::GameOver);
            return;
        }
        switch (key)
        {
            case 'a': game.OnPlayerAction(gameplay::InputEvent::MoveLeft); break;
            case 'd': game.OnPlayerAction(gameplay::InputEvent::MoveRight); break;
            case 'w': game.OnPlayerAction(gameplay::InputEvent::MoveUp); break;
            case 's': game.OnPlayerAction(gameplay::InputEvent::MoveDown); break;
            case 'n': game.OnPlayerAction(gameplay::InputEvent::UseNextPlugin); break;
        }
    }

    void shutdown() noexcept
    {
        clear();
        endwin();
    }

    void notify_message(const char* message) noexcept
    {
        const int     row    = gameplay::Game::MAP_SIZE + 1;
        constexpr int column = 0;
        move(row, column);
        clrtobot();
        mvprintw(row, column, "%s", message);
        refresh();
    }
}

namespace
{
    void draw_game(const gameplay::Game& game) noexcept
    {
        using namespace gameplay;
        const int   view_range = std::min(Game::MAX_VIEW_RANGE, game.TorchCount() / 2);
        const auto& world      = game.TheWorld();

        const int player_x      = static_cast<int>(game.PlayerX());
        const int player_y      = static_cast<int>(game.PlayerY());
        auto      world_row_itr = world.begin();
        for (int row = 0; row < Game::MAP_SIZE; ++row, ++world_row_itr)
        {
            move(row, 0);
            auto       world_column_itr = world_row_itr->begin();
            const auto y_distance       = abs(player_y - row);
            for (int column = 0; column < Game::MAP_SIZE; ++column, ++world_column_itr)
            {
                if (y_distance + abs(player_x - column) > view_range)
                {
                    addch(' ' | COLOR_PAIR(Color::Black));
                }
                else
                {
                    switch (const auto tile = *world_column_itr; tile)
                    {
                        case Tiles::FLOOR: addch(Char::Grass | COLOR_PAIR(Color::Grass)); break;
                        case Tiles::WALL: addch(Char::Rock | COLOR_PAIR(Color::Rock)); break;
                        case Tiles::COIN: addch(Char::Coin | COLOR_PAIR(Color::Coin)); break;
                        case Tiles::STAIRS_DOWN: addch(Char::Stairs | COLOR_PAIR(Color::Stairs)); break;
                        case Tiles::TORCH: addch(Char::Torch | COLOR_PAIR(Color::Torch)); break;
                        default: addch(' ' | COLOR_PAIR(Color::Black)); break;
                    }
                }
            }
        }
        mvaddch(player_y, player_x, Char::Player | COLOR_PAIR(Color::Player));

        int row = 0;
        mvprintw(row++, Game::MAP_SIZE + 1, "Coins: %d", game.CoinsCount());
        mvprintw(row++, Game::MAP_SIZE + 1, "Torch: %d", game.TorchCount());
        mvprintw(row++, Game::MAP_SIZE + 1, "Moves: %d", game.MovesCount());
        mvprintw(row, Game::MAP_SIZE + 1, "Level: %d", game.CurrentLevel());

        refresh();
    }
}
