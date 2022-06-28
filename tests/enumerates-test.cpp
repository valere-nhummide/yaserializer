#include <catch2/catch.hpp>

#include <yaserializer/enumerates.hpp>

#include <bit>
#include <cstddef>
#include <vector>

using namespace yaserializer;

template<typename T>
consteval size_t first_value() { return std::numeric_limits<T>::max() - static_cast<T>('f' - 'a'); }

// clang-format off
enum class CharEnumerate: uint8_t { a = first_value<uint8_t>(), b, c, d, e, f };
enum class ShortEnumerate: uint16_t { a = first_value<uint16_t>(), b, c, d, e, f };
enum class UnsignedEnumerate: uint32_t { a = first_value<uint32_t>(), b, c, d, e, f };
enum class LongEnumerate: uint64_t { a = first_value<uint64_t>(), b, c, d, e, f };
// clang-format off

TEMPLATE_TEST_CASE("Enum types can be serialized and deserialized", "[enumerates]", CharEnumerate, ShortEnumerate, UnsignedEnumerate, LongEnumerate)
{
    constexpr TestType var = TestType::f;

    SECTION("Establishing serialized size")
    {
        constexpr size_t size = serialized_size(var);

        STATIC_REQUIRE(size == sizeof(std::underlying_type_t<TestType>));
    }

    std::vector<uint8_t> serialized;
    serialized.resize(serialized_size(var));
    auto iterator = serialized.begin();

    SECTION("Serializing the variable to the buffer")
    {
        serialize(iterator, var);
        REQUIRE(iterator == serialized.end());

        using Integral = std::underlying_type_t<TestType>;
        REQUIRE(*reinterpret_cast<Integral *>(std::addressof(serialized.front())) == std::numeric_limits<Integral>::max());
    }
    SECTION("Deserializing previously deserialized data")
    {
        serialize(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
}