/*
 * Rudy Castan
 * CS180
 */
#pragma once

namespace CS180
{
    void                 random_seed(unsigned int seed) noexcept;
    [[nodiscard]] double random(double min_inclusive, double max_exclusive) noexcept;
    [[nodiscard]] double random(double max_exclusive) noexcept;
    [[nodiscard]] double random() noexcept;
    [[nodiscard]] int    random(int min_inclusive, int max_exclusive) noexcept;
    [[nodiscard]] int    random(int max_exclusive) noexcept;
}
