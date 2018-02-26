/// @file   Color.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of the Color type.

#ifndef M8_COLOR_HPP_
#define M8_COLOR_HPP_

#include <cstdint>
#include <cassert>

namespace m8
{
    /// Type that represent the color of the pieces
    typedef std::uint8_t Color;

    const Color kWhite = 0;
    const Color kBlack = 1;

    const Color kMinColor = kWhite;
    const Color kMaxColor = kBlack;

    /// Indicate if a value is a valid color
    ///
    /// @param color The value to evaluate
    /// @return True if the value is a valid color (0 or 1)
    inline bool IsColor(Color color) { return color <= kBlack; }

    /// Return the opposite color than the one it is passed in parameter.
    ///
    /// @param color Color for wich we want the opposite.
    /// @return kWhite if kBlack was passed in parameterm otherwise kBlack.
    inline Color OpposColor(Color color)
    {
        // A : color is valid
        assert(IsColor(color));

        return color ^ UINT8_C(1);
    }
}

#endif // M8_COLOR_HPP_