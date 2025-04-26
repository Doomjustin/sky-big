#include <catch2/catch_test_macros.hpp>

#include "sky_big/generator.h"

#include <vector>


TEST_CASE("range")
{
    using namespace sb;
    using std::vector;

    SECTION("range(end) generate as [0: end)")
    {
        int expected = 0;
        for (auto i: range(5))
            REQUIRE(i == expected++);
    }

    SECTION("range(sequence) generate from a sequence")
    {
        vector<float> vec{ 1.0, 31.1, 312.5 };
        int index = 0;
        for (auto i: range(vec))
            REQUIRE(i == vec[index++]);
    }
}