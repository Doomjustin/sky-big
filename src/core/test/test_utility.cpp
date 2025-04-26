#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <sky_big/utility.h>

#include <string>

using namespace sb;

TEST_CASE("test case transform")
{
    const std::string lower_case = "0123456789 \t\n\v\r\f ;:<>.,?|\\'`~\"!@#$%^&*(){}[]/_+-= abcdefghijklmnopqrstuvwxyz";
    const std::string upper_case = "0123456789 \t\n\v\r\f ;:<>.,?|\\'`~\"!@#$%^&*(){}[]/_+-= ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    SECTION("to_upper only alpha transformed")
    {
        REQUIRE_THAT(to_upper(lower_case), Catch::Matchers::Equals(upper_case));
        REQUIRE_THAT(to_upper(upper_case), Catch::Matchers::Equals(upper_case));
    }

    SECTION("to_lower only alpha transformed")
    {
        REQUIRE_THAT(to_lower(upper_case), Catch::Matchers::Equals(lower_case));
        REQUIRE_THAT(to_lower(lower_case), Catch::Matchers::Equals(lower_case));
    }
}


TEST_CASE("test trim")
{
    const std::string base_str = "      123test f a s  ";

    SECTION("trim left")
    {
        REQUIRE_THAT(trim_left(base_str), Catch::Matchers::Equals("123test f a s  "));
    }

    SECTION("trim right")
    {
        REQUIRE_THAT(trim_right(base_str), Catch::Matchers::Equals("      123test f a s"));
    }

    SECTION("trim both")
    {
        REQUIRE_THAT(trim(base_str), Catch::Matchers::Equals("123test f a s"));
    }
}


static constexpr std::string trim_left_std(const std::string_view in)
{
    auto view = in | std::views::drop_while([] (const char c) { return std::isspace(c); });

    return { view.begin(), view.end() };
}

TEST_CASE("trim benchmark")
{
    const std::string lower_case = "0123456789 \t\n\v\r\f ;:<>.,?|\\'`~\"!@#$%^&*(){}[]/_+-= abcdefghijklmnopqrstuvwxyz";

    BENCHMARK("std::isspace")
    {
        return trim_left_std(lower_case);
    };

    BENCHMARK("constexpr is space")
    {
        return trim_left(lower_case);
    };
}