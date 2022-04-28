#pragma once

#include <yaserializer/concepts/iterators.hpp>
#include <yaserializer/concepts/integrals.hpp>

#include <algorithm>
#include <bit>
#include <concepts>
#include <iterator>
#include <span>

namespace yaserializer
{

    template <concepts::integrals::Serializable T>
    consteval size_t serialized_size()
    {
        return sizeof(T);
    }

    template <concepts::integrals::Serializable T>
    constexpr size_t serialized_size(const T &)
    {
        return serialized_size<T>();
    }

    template <std::endian Endian = std::endian::big>
    void serialize(concepts::iterators::OutputByteIterator auto &iter, concepts::integrals::Serializable auto t)
    {
        using T = std::decay_t<std::remove_cv_t<decltype(t)>>;

        if constexpr (serialized_size<T>() == 1)
        {
            using Iterator = std::decay_t<decltype(iter)>;
            *iter = static_cast<typename std::iterator_traits<Iterator>::value_type>(t);
        }
        else
        {
            using Byte = typename std::iterator_traits<std::decay_t<decltype(iter)>>::value_type;
            auto input_bytes = std::span(reinterpret_cast<const Byte *>(std::addressof(t)), serialized_size<T>());

            if constexpr (Endian == std::endian::native)
                std::ranges::copy(input_bytes, iter);
            else
                std::ranges::reverse_copy(input_bytes, iter);
        }
        std::advance(iter, serialized_size<T>());
    }

    template <concepts::integrals::Deserializable T, std::endian Endian = std::endian::big>
    T deserialize(concepts::iterators::InputByteIterator auto &iter)
    {
        T t{};
        if constexpr (serialized_size<T>() == 1)
        {
            t = static_cast<T>(*iter);
        }
        else
        {
            auto input_bytes = std::as_bytes(std::span(iter, serialized_size<T>()));
            auto output_bytes = std::span(reinterpret_cast<std::byte *>(std::addressof(t)), serialized_size<T>());

            if constexpr (Endian == std::endian::native)
                std::ranges::copy(input_bytes, output_bytes.begin());
            else
                std::ranges::reverse_copy(input_bytes, output_bytes.begin());
        }
        std::advance(iter, serialized_size<T>());
        return t;
    }
} // namespace yaserializer