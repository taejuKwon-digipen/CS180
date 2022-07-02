/*
 * Rudy Castan
 * CS180
 */
#include <CPUSpeed.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

namespace CS180
{
    double FindCPUSpeedGHz()
    {
        static const double CPUCyclesPerNanoSecond = []
        {
            using namespace std;
            ifstream istream{ "/proc/cpuinfo" };
            if (!istream)
                throw runtime_error("Can't open /proc/cpuinfo file to get CPU Speed");
            istream.exceptions(istream.exceptions() | ios_base::badbit);

            string cpu{}, MHz{}, current_text{};
            char   seperator{};
            double speed_megahz{ 0 };
            while (istream >> current_text)
            {
                cpu = MHz;
                MHz = current_text;
                if (cpu == "cpu" && MHz == "MHz" && (istream >> seperator >> speed_megahz))
                {
                    return speed_megahz / 1000.0;
                }
            }
            throw runtime_error(R"(Unable to find "cpu MHz" in /proc/cpuinfo file to get CPU Speed)");
        }();
        return CPUCyclesPerNanoSecond;
    }
}
