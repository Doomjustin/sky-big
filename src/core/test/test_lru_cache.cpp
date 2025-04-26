#include <catch2/catch_test_macros.hpp>

#include "sky_big/cache.h"

using namespace sb;

TEST_CASE("any type Value")
{

    SECTION("test any type value")
    {
        Value<int> value{ 1 };
        REQUIRE(value.get_value<int>() == 1);
    }
}


TEST_CASE("LRU Cache")
{
    LRUCache cache{ 2 };

    SECTION("LRU cache base api")
    {
        cache.put("key1", 1);
        REQUIRE(cache.get<int>("key1"));
        REQUIRE(*cache.get<int>("key1") == 1);
    }

    SECTION("return not exist if key exists but type is not match")
    {
        cache.put("key1", 1);
        REQUIRE_FALSE(cache.get<float>("key1"));
    }

    SECTION("update exists key when put twice")
    {
        cache.put("key1", 1);
        REQUIRE(*cache.get<int>("key1") == 1);

        cache.put("key1", 312);
        REQUIRE(*cache.get<int>("key1") == 312);
    }

    SECTION("LRU cache will erase last value when capacity is not enough")
    {
        cache.put("key1", 1);
        cache.put("key2", "value2");
        cache.put("key3", 312.f);

        REQUIRE_FALSE(cache.get<int>("key1"));

        REQUIRE(cache.get<std::string>("key2"));
        REQUIRE(*cache.get<std::string>("key2") == "value2");

        REQUIRE(cache.get<float>("key3"));
        REQUIRE(*cache.get<float>("key3") == 312.f);
    }

    SECTION("if we update a value, it will become the newest value")
    {
        cache.put("key1", 1);
        cache.put("key2", "value2");
        // 更新key1之后，key1就不是最老的值了
        cache.put("key1", 123);
        cache.put("key3", 312.f);

        REQUIRE(cache.get<int>("key1"));
        REQUIRE(*cache.get<int>("key1") == 123);

        REQUIRE_FALSE(cache.get<std::string>("key2"));

        REQUIRE(cache.get<float>("key3"));
        REQUIRE(*cache.get<float>("key3") == 312.f);
    }
}