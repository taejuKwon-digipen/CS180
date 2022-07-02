// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

namespace util
{
    // based off of C Programming Language,	by Brian W.Kernighan, Dennis M.Ritchie page 46
    class Random
    {
        unsigned long long int seed = {1};

    public:
        constexpr void                   set_seed(unsigned long long int new_seed) noexcept { seed = new_seed; }
        [[nodiscard]] constexpr unsigned get_random_uint(unsigned max_exclusive = 32768) noexcept
        {
            seed   = seed * 1103515245 + 12345;
            auto i = static_cast<unsigned>((seed / 65536) % max_exclusive);
            return i;
        }
    };
}
