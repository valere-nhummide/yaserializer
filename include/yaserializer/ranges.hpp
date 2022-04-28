#pragma once

#include <yaserializer/concepts/iterators.hpp>
#include <yaserializer/concepts/ranges.hpp>
#include <yaserializer/integrals.hpp>

#include <bit>
#include <iterator>
#include <ranges>

namespace yaserializer
{
    template <concepts::ranges::Serializable Range>
    constexpr size_t serialized_size(Range range)
    {
        if constexpr (concepts::integrals::Serializable<typename std::ranges::range_value_t<Range>>)
            return std::ranges::size(range) * serialized_size<typename std::ranges::range_value_t<Range>>();
        else
        {
            size_t cumulated_size = 0u;
            for (auto &value : range)
            {
                cumulated_size += serialized_size(value);
            }
            return cumulated_size;
        }
    }

    template <std::endian Endian = std::endian::big>
    void serialize(concepts::iterators::OutputByteIterator auto &iter, concepts::ranges::Serializable auto range)
    {
        if constexpr (concepts::ranges::MoveSerializable<decltype(range), Endian>)
        {
            using Byte = typename std::iterator_traits<std::decay_t<decltype(iter)>>::value_type;
            auto input_bytes = std::span(reinterpret_cast<const Byte *>(range.data()), serialized_size(range));
            std::ranges::copy(input_bytes, iter);
            std::advance(iter, input_bytes.size());
        }
        else
        {
            for (auto val : range)
            {
                serialize<Endian>(iter, std::forward<decltype(val)>(val));
            }
        }
    }

    template <concepts::ranges::FixedSizeDeserializable Range, std::endian Endian = std::endian::big>
    Range deserialize(concepts::iterators::InputByteIterator auto &iter)
    {
        Range range{};

        for (auto &value : range)
        {
            value = deserialize<std::ranges::range_value_t<Range>, Endian>(iter);
        }

        return range;
    }

    template <concepts::ranges::ResizableDeserializable Range, std::endian Endian = std::endian::big>
    Range deserialize(concepts::iterators::InputByteIterator auto &iter, std::ranges::range_size_t<Range> values_count)
    {
        Range range;
        range.resize(values_count);

        for (auto &value : range)
        {
            value = deserialize<std::ranges::range_value_t<Range>, Endian>(iter);
        }

        return range;
    }
} // namespace yaserializer