#pragma once

#include <bit>
#include <cstdint>
#include <ranges>
#include <limits>
#include <numeric>
#include <vector>

namespace yaserializer::tests::ranges::details
{
    using Bytes = std::vector<uint8_t>;
    using Byte = Bytes::value_type;
    constexpr size_t values_count = std::numeric_limits<Byte>::max() + 1u;

    template <std::endian Endian, typename Container>
    Bytes generate_expected_serialized(Container container)
    {
        Bytes expected(container.size() * sizeof(typename Container::value_type), 0u);
        using Integral = typename Container::value_type;
        Byte lsbyte_increment = Endian == std::endian::little ? 0u : std::numeric_limits<Byte>::max();

        if constexpr (Endian == std::endian::little)
        {
            auto begin = std::ranges::begin(expected);
            for (auto iter = begin; iter != std::ranges::end(expected); iter++)
            {
                if (std::ranges::distance(begin, iter) % sizeof(Integral) == 0)
                {
                    *iter = lsbyte_increment++;
                }
            }
        }
        else
        {
            auto begin = std::ranges::rbegin(expected);
            for (auto iter = begin; iter != std::ranges::rend(expected); iter++)
            {
                if (std::ranges::distance(begin, iter) % sizeof(Integral) == 0)
                {
                    *iter = lsbyte_increment--;
                }
            }
        }

        return expected;
    }

} // namespace yaserializer::tests::ranges::details