#include <catch2/catch.hpp>

#include <yaserializer/ranges.hpp>
#include <tests/ranges/details/test-utils.hpp>

#include <array>
#include <bit>
#include <cstddef>
#include <numeric>
#include <ranges>

using namespace yaserializer;
using namespace yaserializer::tests::ranges::details;

// TEMPLATE_TEST_CASE regex type parser does not handle commas
using Array = std::array<uint64_t, values_count>;

TEMPLATE_TEST_CASE("Fixed-size containers of integrals can be serialized and deserialized", "[containers]", Array)
{
    using Integral = typename TestType::value_type;
    TestType container{};
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
        serialize(iterator, std::move(container));

        CHECK(iterator == serialized.end());

        Bytes expected = generate_expected_serialized<std::endian::big>(container);

        CHECK(std::ranges::equal(serialized, expected));
    }

    SECTION("Deserializing from default (big) endian")
    {
        serialize(iterator, std::move(container));
        iterator = serialized.begin();
        TestType deserialized = deserialize<TestType>(iterator);

        CHECK(iterator == serialized.end());

        CHECK(std::ranges::equal(deserialized, container));
    }

    SECTION("Serializing to little endian")
    {
        serialize<std::endian::little>(iterator, std::move(container));

        CHECK(iterator == serialized.end());

        Bytes expected = generate_expected_serialized<std::endian::little>(container);

        CHECK(std::ranges::equal(serialized, expected));
    }

    SECTION("Deserializing from little endian")
    {
        serialize<std::endian::little>(iterator, std::move(container));
        iterator = serialized.begin();
        TestType deserialized = deserialize<TestType, std::endian::little>(iterator);

        CHECK(iterator == serialized.end());

        CHECK(std::ranges::equal(deserialized, container));
    }
}
