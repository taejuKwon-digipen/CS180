/*
 * Rudy Castan
 * CS180
 */

#include "Random.hpp"
#include "polynomial.h"
#include <CPUCyclesTimer.hpp>
#include <CyclesPerElement.hpp>

#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;


struct PolynomialTest
{
    using size = int;

    struct TestCase
    {
        polynomial  functionToTest     = [](const vec_ptr, const data_t*, data_t*) {};
        polynomial  baseImplementation = [](const vec_ptr, const data_t*, data_t*) {};
        std::string description        = "Empty";
    };

    std::vector<TestCase>   cases{};
    CS180::CyclesPerElement cycleMeasure{};
    size                    dataSize{};
    vec_ptr                 coefficients{};
    data_t                  X{};


    PolynomialTest(size data_size = 9073)
    {
        CS180::random_seed(2021);
        dataSize     = data_size;
        coefficients = new_vec(data_size);
        for (size i = 0; i < data_size; i++)
        {
            if constexpr (std::is_floating_point_v<data_t>)
            {
                constexpr auto max_x = static_cast<data_t>(6.0);
                constexpr auto min_x = static_cast<data_t>(-6.0);
                set_vec_element(coefficients, i, (CS180::random(2) == 0) ? min_x : max_x);
            }
            else
            {
                constexpr auto max_x = static_cast<data_t>(6);
                constexpr auto min_x = static_cast<data_t>(-6);
                set_vec_element(coefficients, i, (CS180::random(2) == 0) ? min_x : max_x);
            }
        }
    }

    ~PolynomialTest() { free_vec(coefficients); }

    void Run()
    {
        std::cout << std::fixed << std::setprecision(3);
        for (const auto& test : cases)
        {
            data_t                                result{};
            const polynomial                      current_implementation = test.functionToTest;
            CS180::CyclesPerElement::TestFunction f                      = [&](int how_many)
            {
                set_vec_length(coefficients, how_many);

                if constexpr (std::is_floating_point_v<data_t>)
                {
                    constexpr auto max_x = static_cast<data_t>(1.0);
                    constexpr auto min_x = static_cast<data_t>(-1.0);
                    X                    = (CS180::random(2) == 0) ? min_x : max_x;
                }
                else
                {
                    constexpr auto max_x = static_cast<data_t>(1);
                    constexpr auto min_x = static_cast<data_t>(-1);
                    X                    = (CS180::random(2) == 0) ? min_x : max_x;
                }
                current_implementation(coefficients, &X, &result);
            };
            const auto cpe = cycleMeasure.FindCPE(f, dataSize);
            std::cout << std::setw(7) << cpe << " cycles/element for " << test.description << '\n';

            set_vec_length(coefficients, dataSize);
            data_t base_result;
            test.baseImplementation(coefficients, &X, &base_result);
            current_implementation(coefficients, &X, &result);

            const auto result_str      = std::to_string(result);
            const auto base_result_str = std::to_string(base_result);
            if (result_str != base_result_str)
            {
                std::cerr << "Function " << test.description << ", Should be " << base_result_str << ", Got "
                          << result_str << '\n';
            }
        }
    }

    PolynomialTest(const PolynomialTest&) = delete;
    PolynomialTest(PolynomialTest&&)      = delete;
    PolynomialTest& operator=(const PolynomialTest&) = delete;
    PolynomialTest& operator=(PolynomialTest&&) = delete;
} gTest;


int main()
{
    register_polynomial_functions();
    gTest.Run();
    return 0;
}


/* Add a new version of the inner product function to benchmark.
    Also needs the base version to test for correctness since there is no point in getting a faster version that
   produces wrong results. It also takes in a description of the new version. This will be printed with the stats,
   so we know who the stats belongs to.
 */
extern "C" void add_polynomial_function(polynomial new_version, polynomial base_version, char* new_version_description)
{
    gTest.cases.push_back({ new_version, base_version, new_version_description });
}
