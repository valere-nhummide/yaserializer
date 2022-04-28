#include <catch2/catch.hpp>

#include <yaserializer/integrals.hpp>

#include <bit>
#include <cstddef>
#include <vector>

using namespace yaserializer;

TEMPLATE_TEST_CASE("Single-byte integrals can be serialized and deserialized to unsigned chars", "[integrals]", char, unsigned char, uint8_t, std::byte)
{
    constexpr TestType var = static_cast<TestType>(0xAB);

    SECTION("Establishing serialized size")
    {
        constexpr size_t size = serialized_size(var);

        STATIC_REQUIRE(size == 1);
    }

    std::vector<uint8_t> serialized;
    serialized.resize(serialized_size(var));
    auto iterator = serialized.begin();

    SECTION("Serializing the variable to the buffer")
    {
        serialize(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(*reinterpret_cast<TestType *>(std::addressof(serialized.front())) == var);
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

TEMPLATE_TEST_CASE("Single-byte integrals can be serialized and deserialized to standard bytes", "[integrals]", char, unsigned char, uint8_t, std::byte)
{
    constexpr TestType var = static_cast<TestType>(0xAB);

    SECTION("Establishing serialized size")
    {
        constexpr size_t size = serialized_size(var);

        STATIC_REQUIRE(size == 1);
    }

    std::vector<std::byte> serialized;
    serialized.resize(serialized_size(var));
    auto iterator = serialized.begin();

    SECTION("Serializing the variable to the buffer")
    {
        serialize(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(*reinterpret_cast<TestType *>(std::addressof(serialized.front())) == var);
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

TEMPLATE_TEST_CASE("Two-bytes integrals can be serialized and deserialized", "[integrals]", char16_t, int16_t, uint16_t)
{
    constexpr TestType var = static_cast<TestType>(0xABCD);

    SECTION("Establishing serialized size")
    {
        constexpr size_t size = serialized_size(var);

        STATIC_REQUIRE(size == 2);
    }

    std::vector<std::byte> serialized;
    serialized.resize(serialized_size(var));
    auto iterator = serialized.begin();

    SECTION("Serializing to default (big) endian")
    {
        serialize(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(serialized.at(0) == static_cast<std::byte>(0xAB));
        REQUIRE(serialized.at(1) == static_cast<std::byte>(0xCD));
    }
    SECTION("Deserializing from default (big) endian")
    {
        serialize(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
    SECTION("Serializing to little endian")
    {
        serialize<std::endian::little>(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(serialized.at(0) == static_cast<std::byte>(0xCD));
        REQUIRE(serialized.at(1) == static_cast<std::byte>(0xAB));
    }
    SECTION("Deserializing from little endian")
    {
        serialize<std::endian::little>(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType, std::endian::little>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
}

TEMPLATE_TEST_CASE("Four-bytes integrals can be serialized and deserialized", "[integrals]", char32_t, int32_t, uint32_t)
{
    constexpr TestType var = static_cast<TestType>(0x01234567);

    SECTION("Establishing serialized size")
    {
        constexpr size_t size = serialized_size(var);

        STATIC_REQUIRE(size == 4);
    }

    std::vector<std::byte> serialized;
    serialized.resize(serialized_size(var));
    auto iterator = serialized.begin();

    SECTION("Serializing to default (big) endian")
    {
        serialize(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(serialized.at(0) == static_cast<std::byte>(0x01));
        REQUIRE(serialized.at(1) == static_cast<std::byte>(0x23));
        REQUIRE(serialized.at(2) == static_cast<std::byte>(0x45));
        REQUIRE(serialized.at(3) == static_cast<std::byte>(0x67));
    }
    SECTION("Deserializing from default (big) endian")
    {
        serialize(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
    SECTION("Serializing to little endian")
    {
        serialize<std::endian::little>(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(serialized.at(0) == static_cast<std::byte>(0x67));
        REQUIRE(serialized.at(1) == static_cast<std::byte>(0x45));
        REQUIRE(serialized.at(2) == static_cast<std::byte>(0x23));
        REQUIRE(serialized.at(3) == static_cast<std::byte>(0x01));
    }
    SECTION("Deserializing from little endian")
    {
        serialize<std::endian::little>(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType, std::endian::little>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
}

TEMPLATE_TEST_CASE("Eight-bytes integrals can be serialized and deserialized", "[integrals]", int64_t, uint64_t)
{
    constexpr TestType var = static_cast<TestType>(0x0123456789ABCDEF);

    SECTION("Establishing serialized size")
    {
        constexpr size_t size = serialized_size(var);

        STATIC_REQUIRE(size == 8);
    }

    std::vector<std::byte> serialized;
    serialized.resize(serialized_size(var));
    auto iterator = serialized.begin();

    SECTION("Serializing to default (big) endian")
    {
        serialize(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(serialized.at(0) == static_cast<std::byte>(0x01));
        REQUIRE(serialized.at(1) == static_cast<std::byte>(0x23));
        REQUIRE(serialized.at(2) == static_cast<std::byte>(0x45));
        REQUIRE(serialized.at(3) == static_cast<std::byte>(0x67));
        REQUIRE(serialized.at(4) == static_cast<std::byte>(0x89));
        REQUIRE(serialized.at(5) == static_cast<std::byte>(0xAB));
        REQUIRE(serialized.at(6) == static_cast<std::byte>(0xCD));
        REQUIRE(serialized.at(7) == static_cast<std::byte>(0xEF));
    }
    SECTION("Deserializing from default (big) endian")
    {
        serialize(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
    SECTION("Serializing to little endian")
    {
        serialize<std::endian::little>(iterator, var);
        REQUIRE(iterator == serialized.end());

        REQUIRE(serialized.at(0) == static_cast<std::byte>(0xEF));
        REQUIRE(serialized.at(1) == static_cast<std::byte>(0xCD));
        REQUIRE(serialized.at(2) == static_cast<std::byte>(0xAB));
        REQUIRE(serialized.at(3) == static_cast<std::byte>(0x89));
        REQUIRE(serialized.at(4) == static_cast<std::byte>(0x67));
        REQUIRE(serialized.at(5) == static_cast<std::byte>(0x45));
        REQUIRE(serialized.at(6) == static_cast<std::byte>(0x23));
        REQUIRE(serialized.at(7) == static_cast<std::byte>(0x01));
    }
    SECTION("Deserializing from little endian")
    {
        serialize<std::endian::little>(iterator, var);
        iterator = serialized.begin();
        TestType deserialized_var = deserialize<TestType, std::endian::little>(iterator);

        REQUIRE(iterator == serialized.end());

        REQUIRE(deserialized_var == var);
    }
}