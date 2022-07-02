/*
 * Rudy Castan
 * CS180
 */
#include "Random.hpp"

#include <cassert>
#include <chrono>
#include <random>
#include <thread>


namespace
{
    using namespace std;
    using namespace chrono;
    thread_local mt19937_64 Engine{ []
                                    {
                                        return static_cast<unsigned long long>(
                                                   system_clock::now().time_since_epoch().count()) +
                                               hash<thread::id>{}(this_thread::get_id());
                                    }() };
}

namespace CS180
{
    void random_seed(unsigned int seed) noexcept { Engine.seed(seed); }

    double random(double min_inclusive, double max_exclusive) noexcept
    {
        assert(min_inclusive < max_exclusive) ;
        return std::uniform_real_distribution<double>(min_inclusive, max_exclusive)(Engine);
    }

    double random(double max_exclusive) noexcept { return random(0.0, max_exclusive); }

    double random() noexcept { return random(1.0); }

    int random(int min_inclusive, int max_exclusive) noexcept
    {
        assert(min_inclusive < max_exclusive - 1);
        return std::uniform_int_distribution<int>(min_inclusive, max_exclusive - 1)(Engine);
    }

    int random(int max_exclusive) noexcept { return random(0, max_exclusive); }
}
