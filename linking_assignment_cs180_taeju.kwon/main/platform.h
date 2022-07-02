// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

#include <gameplay.h>
#include <string_view>

namespace platform
{
    void startup() noexcept;

    void update(gameplay::Game& game) noexcept;

    void shutdown() noexcept;

    void notify_message(const char* message) noexcept;

}
