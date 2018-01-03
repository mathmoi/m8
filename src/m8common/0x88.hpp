/// @file   0x88.hpp
/// @author Mathieu Pagé
/// @date   Janurary 2018
/// @brief  Contains method related to the 0x88 indexing scheme.

#ifndef M8_0X88_HPP_
#define M8_0X88_HPP_

#include <cstdint>

/// Return the 0x88 index of a Mailbox square.
///
/// @param sq Index of the square in a mailbox board representation.
/// @return Index of the square in a 0x88 board representation.
inline std::uint8_t Get0x88Index(std::uint8_t index)
{
    return index + (index & ~7);
}

/// Return the 0x88 difference between two square. This diffrence has the property of
/// being unique for each combination of squares in regards to direction and distance.
///
/// @param from First square.
/// @param to Second square.
/// @param The difference between the squares. Should be in the range [-119, 119];
inline std::int8_t CalculateOx88diff(std::uint8_t from, std::uint8_t to)
{
    return Get0x88Index(to) - Get0x88Index(from);
}

#endif // M8_0X88_HPP_