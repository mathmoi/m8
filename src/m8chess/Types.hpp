/// @file   Types.hpp
/// @author Mathieu Pag�
/// @date   January 2020
/// @brief  Contains differents typedef.

#ifndef M8_TYPES_HPP_
#define M8_TYPES_HPP_

#include <cstdint>

namespace m8
{
    /// Type for the search depth
    typedef std::uint16_t DepthType;

    /// Type to count nodes
    typedef std::uint64_t NodeCounterType;

    /// Type for evaluation values
    typedef std::int16_t EvalType;
}

#endif //M8_TYPES_HPP_