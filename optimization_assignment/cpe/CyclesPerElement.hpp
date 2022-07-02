/*
 * Rudy Castan
 * CS180
 */
#include "CPUCyclesTimer.hpp"

#include <array>
#include <functional>
#include <vector>

namespace CS180
{
    class CyclesPerElement
    {
    public:
        using TestFunction = std::function<void(int how_many)>;

        double FindCPE(TestFunction func_to_test, int max_element_count);

    private:
        static inline constexpr int N_BEST = 3;
        using Cycles                       = double;

        CPUCyclesTimer                        timer{};
        std::array<Cycles, N_BEST>            topRecordedDurations{};
        decltype(topRecordedDurations.size()) sampleCount{ 0 };

    private:
        [[nodiscard]] Cycles measureDuration(TestFunction& func_to_test, int element_count);
        [[nodiscard]] bool   measurementsHasConverged() noexcept;
        void                 addSample(Cycles duration) noexcept;
    };


}
