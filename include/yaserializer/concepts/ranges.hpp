#pragma once

#include <yaserializer/concepts/integrals.hpp>

#include <concepts>
#include <cstddef>
#include <map>
#include <unordered_map>
#include <ranges>

namespace yaserializer::concepts::ranges
{
    namespace details
    {
        template <typename T>
        concept IsMap =
            std::same_as<T, std::map<typename T::key_type, typename T::mapped_type, typename T::key_compare, typename T::allocator_type>> ||
            std::same_as<T, std::unordered_map<typename T::key_type, typename T::mapped_type, typename T::hasher, typename T::key_equal, typename T::allocator_type>>;

        template <typename T>
        concept HasTupleSize = requires(T t)
        {
            // clang-format off
            { std::tuple_size<T>::value } -> std::convertible_to<std::ranges::range_size_t<T>>;
            // clang-format on
        };

        template <typename T>
        concept HasResizeFunction = requires(T t, std::ranges::range_size_t<T> new_size)
        {
            // clang-format off
            { t.resize(new_size) };
            // clang-format on
        };

        template <typename T>
        concept HasDataFunction = requires(T t)
        {
            // clang-format off
            { t.data() } -> std::convertible_to<typename T::const_pointer>;
            // clang-format on
        };

    } // namespace details

    template <typename T>
    concept Serializable = std::ranges::sized_range<T> && !details::IsMap<T>;

    template <typename T, std::endian Endian = std::endian::big>
    concept MoveSerializable =
        Serializable<T> &&
        details::HasDataFunction<T> &&
        std::ranges::contiguous_range<T> &&
        integrals::Serializable<typename std::ranges::range_value_t<T>> &&
        (sizeof(typename std::ranges::range_value_t<T>) == 1 || Endian == std::endian::native);

    template <typename T>
    concept Deserializable = Serializable<T>;

    template <typename T>
    concept FixedSizeDeserializable = Deserializable<T> && details::HasTupleSize<T> && std::default_initializable<T>;

    template <typename T>
    concept ResizableDeserializable = Deserializable<T> && details::HasResizeFunction<T>;

} // namespace yaserializer::concepts
