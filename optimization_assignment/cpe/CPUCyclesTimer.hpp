/*
 * Rudy Castan
 * CS180
 */
#pragma once

#include "CPUSpeed.hpp"
#include <chrono>

namespace CS180
{
    class [[nodiscard]] CPUCyclesTimer
    {
    private:
        using clock_t       = std::chrono::steady_clock;
        using nano_second_t = std::chrono::duration<double, std::ratio<1, 1'000'000'000>>;

        std::chrono::time_point<clock_t> timeStamp;

    public:
        CPUCyclesTimer() noexcept : timeStamp(clock_t::now()) {}

        void ResetTimeStamp() noexcept { timeStamp = clock_t::now(); }

        [[nodiscard]] double GetElapsedCycles() const
        {
            const double elapsed_nanoseconds =
                std::chrono::duration_cast<nano_second_t>(clock_t::now() - timeStamp).count();
            const double cycles_per_nanosecond = FindCPUSpeedGHz();
            return elapsed_nanoseconds * cycles_per_nanosecond;
        }
    };
}
