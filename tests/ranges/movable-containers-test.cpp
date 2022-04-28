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

TEMPLATE_TEST_CASE("Movable containers of integrals can be serialized by moving data", "[containers]", std::vector<uint64_t>, std::string, std::wstring)
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

    SECTION("Serializing to native endian")
    {
        Bytes expected = generate_expected_serialized<std::endian::native>(container);
        serialize<std::endian::native>(iterator, std::move(container));

        CHECK(iterator == serialized.end());
        CHECK(std::ranges::equal(serialized, expected));
        CHECK(container.empty());
    }

    SECTION("Serializing to non-native endian")
    {
        constexpr auto NonNativeEndian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;
        Bytes expected = generate_expected_serialized<NonNativeEndian>(container);
        serialize<NonNativeEndian>(iterator, std::move(container));

        CHECK(iterator == serialized.end());
        CHECK(std::ranges::equal(serialized, expected));
        CHECK(container.empty());
    }
}
