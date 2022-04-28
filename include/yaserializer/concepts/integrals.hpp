#pragma once

#include <bit>
#include <concepts>
#include <cstddef>

namespace yaserializer::concepts::integrals
{

    template <typename T>
    concept Serializable = std::integral<T> && !std::same_as<std::remove_cv<T>, bool> || std::same_as<T, std::byte>;

    template <typename T>
    concept Deserializable = Serializable<T>;

} // namespace yaserializer::concepts
