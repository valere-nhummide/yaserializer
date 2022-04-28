#pragma once

#include <concepts>
#include <iterator>

namespace yaserializer::concepts::iterators
{

    template <class I>
    concept ByteIterator = std::input_or_output_iterator<I> && std::contiguous_iterator<I> && sizeof(typename std::iterator_traits<I>::value_type) == 1;

    template <class I>
    concept InputByteIterator = ByteIterator<I> && std::input_iterator<I>;

    template <class I>
    concept OutputByteIterator = ByteIterator<I> && std::output_iterator<I, typename std::iterator_traits<I>::value_type>;

} // namespace yaserializer::concepts::iterators
