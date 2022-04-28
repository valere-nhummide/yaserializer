#include <catch2/catch.hpp>

#include <yaserializer/ranges.hpp>
#include <tests/ranges/details/test-utils.hpp>

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <deque>
#include <list>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

using namespace yaserializer;
using namespace yaserializer::tests::ranges::details;

TEMPLATE_TEST_CASE("Resizable containers of integrals can be serialized and deserialized", "[containers]", std::vector<uint64_t>, std::list<uint64_t>, std::deque<uint64_t>, std::string, std::wstring)
{
    using Integral = typename TestType::value_type;
    TestType container{};
    container.resize(static_cast<size_t>(values_count));
    std::iota(std::ranges::next(std::ranges::begin(container)), std::ranges::end(container), 1);

    SECTION("Establishing serialized size")
    {
        REQUIRE(serialized_size(container) == container.size() * sizeof(Integral));
    }

    Bytes serialized;
    serialized.resize(serialized_size(container));
    auto iterator = serialized.begin();

    SECTION("Serializing to default (big) endian")
    {
        serialize(iterator, container);

        CHECK(iterator == serialized.end());

        Bytes expected = generate_expected_serialized<std::endian::big>(container);

        CHECK(std::ranges::equal(serialized, expected));
    }

    SECTION("Deserializing from default (big) endian")
    {
        serialize(iterator, container);
        iterator = serialized.begin();
        TestType deserialized = deserialize<TestType>(iterator, container.size());

        CHECK(iterator == serialized.end());

        CHECK(std::ranges::equal(deserialized, container));
    }

    SECTION("Serializing to little endian")
    {
        serialize<std::endian::little>(iterator, container);

        CHECK(iterator == serialized.end());

        Bytes expected = generate_expected_serialized<std::endian::little>(container);

        CHECK(std::ranges::equal(serialized, expected));
    }

    SECTION("Deserializing from little endian")
    {
        serialize<std::endian::little>(iterator, container);
        iterator = serialized.begin();
        TestType deserialized = deserialize<TestType, std::endian::little>(iterator, container.size());

        CHECK(iterator == serialized.end());

        CHECK(std::ranges::equal(deserialized, container));
    }
}
