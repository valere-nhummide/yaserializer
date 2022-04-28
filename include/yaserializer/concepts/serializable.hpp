#pragma once

#include <yaserializer/concepts/integrals.hpp>
#include <yaserializer/concepts/iterators.hpp>
#include <yaserializer/concepts/ranges.hpp>

#include <concepts>

namespace yaserializer::concepts
{
    template <typename T, typename Out>
    concept Serializable =
        integrals::Serializable<T> ||
        ranges::Serializable<T> ||
        (OutputByteIterator<Out> &&requires(T t, Out &out) {
            // clang-format off
            { serialize_size(t) } -> std::convertible_to<size_t>;
            { serialize(out, t) };
            // clang-format on
        });
} // namespace yaserializer::concepts
