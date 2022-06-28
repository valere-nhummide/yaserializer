#pragma once

#include <yaserializer/concepts/integrals.hpp>

#include <concepts>
#include <type_traits>

namespace yaserializer::concepts::enumerates
{
    template <typename T>
    concept Serializable = std::is_enum_v<T> && integrals::Serializable<typename std::underlying_type_t<T>>;

    template <typename T>
    concept Deserializable = Serializable<T>;

} // namespace yaserializer::concepts
