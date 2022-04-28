#pragma once

#include <yaserializer/concepts/integrals.hpp>
#include <yaserializer/concepts/ranges.hpp>

#include <concepts>

namespace yaserializer::concepts
{
    template <typename T, typename In>
    concept Deserializable =
        integrals::Deserializable<T> ||
        ranges::Deserializable<T> ||
        (InputByteIterator<In> &&requires(T t, In &in) {
            // clang-format off
            { deserialize(in) } -> std::same_as<T>;
            // clang-format on
        });
} // namespace yaserializer::concepts
