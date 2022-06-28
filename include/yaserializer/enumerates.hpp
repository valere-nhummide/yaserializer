#pragma once

#include <yaserializer/concepts/iterators.hpp>
#include <yaserializer/concepts/enumerates.hpp>
#include <yaserializer/integrals.hpp>

#include <algorithm>
#include <bit>
#include <concepts>
#include <iterator>
#include <span>

namespace yaserializer
{
    template <concepts::enumerates::Serializable T>
    consteval size_t serialized_size()
    {
        return sizeof(std::underlying_type_t<T>);
    }

    template <concepts::enumerates::Serializable T>
    constexpr size_t serialized_size(const T &)
    {
        return serialized_size<T>();
    }

    template <std::endian Endian = std::endian::big>
    void serialize(concepts::iterators::OutputByteIterator auto &iter, concepts::enumerates::Serializable auto t)
    {
        using T = std::decay_t<std::remove_cv_t<decltype(t)>>;
        serialize(iter, static_cast<std::underlying_type_t<T>>(t));
    }

    template <concepts::enumerates::Deserializable T, std::endian Endian = std::endian::big>
    T deserialize(concepts::iterators::InputByteIterator auto &iter)
    {
        return static_cast<T>(deserialize<std::underlying_type_t<T>>(iter));
    }
} // namespace yaserializer