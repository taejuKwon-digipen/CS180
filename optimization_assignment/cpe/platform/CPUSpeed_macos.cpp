/*
 * Rudy Castan
 * CS180
 */
#include <CPUSpeed.hpp>


#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

#include <sys/sysctl.h>
#include <sys/types.h>

namespace CS180
{
    double FindCPUSpeedGHz()
    {
        static const double CPUCyclesPerNanoSecond = []
        {
            uint64_t freq_hz = 0;
            size_t   size    = sizeof(freq_hz);

            if (const auto error_code = sysctlbyname("hw.cpufrequency", &freq_hz, &size, nullptr, 0); error_code < 0)
            {
                std::string message = "Failed to get CPU Information :\n";
                message += std::strerror(errno);
                throw std::runtime_error(message);
            }
            return static_cast<double>(freq_hz) / 1'000'000'000.0;
        }();
        return CPUCyclesPerNanoSecond;
    }
}
