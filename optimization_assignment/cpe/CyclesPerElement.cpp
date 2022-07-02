/*
 * Rudy Castan
 * CS180
 */
#include "CyclesPerElement.hpp"

#include <algorithm>
#include <cmath>


namespace CS180
{
    double CyclesPerElement::FindCPE(TestFunction func_to_test, int max_element_count)
    {
        constexpr int NUM_SAMPLES = 1'000;
        const double  min_count   = std::floor(0.3 * static_cast<double>(max_element_count));
        struct least_square_fit_data
        {
            double sum_x{ 0 };
            double sum_y{ 0 };
            double sum_xx{ 0 };
            double sum_xy{ 0 };
        } stats;
        for (int i = 0; i < NUM_SAMPLES; ++i)
        {
            // Let's feed a uniform distribution of element counts to the function
            const double t                 = (static_cast<double>(i) / (NUM_SAMPLES - 1));
            const double calculated_count  = (1.0 - t) * min_count + t * max_element_count;
            const int    how_many_elements = static_cast<int>(calculated_count);
            const double cycle_duration    = measureDuration(func_to_test, how_many_elements);

            // x-axis measures how many elements processed
            const double x = how_many_elements;
            // y-axis measures how many cycles it took to do
            const double y = cycle_duration;
            stats.sum_x += x;
            stats.sum_y += y;
            stats.sum_xx += x * x;
            stats.sum_xy += x * y;
        }
        // Calculate the slope of the line generated from the least square method
        // The slope will give us the rate of change of the line. In other words how much does the y-axis change when we
        // move 1 unit along the x-acis? Since the y-axis is cycles and the x-axis is number of elements this gets us
        // how many cycles we spend per one element
        const double cpe = (NUM_SAMPLES * stats.sum_xy - stats.sum_x * stats.sum_y) /
                           (NUM_SAMPLES * stats.sum_xx - stats.sum_x * stats.sum_x);
        return cpe;
    }

    CyclesPerElement::Cycles CyclesPerElement::measureDuration(CyclesPerElement::TestFunction& func_to_test,
                                                               int                             element_count)
    {
        // Run the function X amount of times
        // keep the top best recordings
        // also try to get top best recordings to not be drastically different from each other
        constexpr int MIN_DURATION_SAMPLES = 20;
        std::fill_n(std::begin(topRecordedDurations), topRecordedDurations.size(), 0.0);
        sampleCount = 0;
        do
        {
            timer.ResetTimeStamp();
            func_to_test(element_count);
            const Cycles duration = timer.GetElapsedCycles();
            if (duration > 0.0)
            {
                addSample(duration);
                ++sampleCount;
            }
        } while (!measurementsHasConverged() && sampleCount < MIN_DURATION_SAMPLES);
        const auto best_result = topRecordedDurations.front();
        return best_result;
    }

    bool CyclesPerElement::measurementsHasConverged() noexcept
    {
        constexpr double EPSILON = 0.01;
        if (sampleCount < N_BEST)
            return false;
        return (1.0 + EPSILON) * topRecordedDurations.front() >= topRecordedDurations.back();
    }

    void CyclesPerElement::addSample(Cycles duration) noexcept
    {
        if (sampleCount < topRecordedDurations.size())
        {
            topRecordedDurations[sampleCount] = duration;
            const auto start                  = std::begin(topRecordedDurations);
            std::sort(start, start + static_cast<ptrdiff_t>(sampleCount + 1));
        }
        else if (duration < topRecordedDurations.back())
        {
            topRecordedDurations.back() = duration;
            const auto start            = std::begin(topRecordedDurations);
            const auto last             = std::end(topRecordedDurations);
            std::sort(start, last);
        }
    }
}
