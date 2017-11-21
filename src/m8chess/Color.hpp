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
    class Color
    {
    private:
        std::uint8_t value_;

    public:

        /// <summary>
        ///  Constructor.
        /// </summary>
        inline Color(std::uint8_t value) { value_ = value; };

        /// <summary>
        ///  White
        /// </summary>
        inline static Color White() { return Color(0); };

        /// <summary>
        ///  Black
        /// </summary>
        inline static Color Black() { return Color(1); };

        /// <summary>
        ///  Return the "first" color. In combination with next, this can be used to
        ///  iterate over all colors.
        /// </summary>
        inline static Color First() { return Color(0); };

        /// <summary>
        ///  Return the opposite color.
        /// </summary>
        inline Color opposite() const { return value_ ^ UINT8_C(1); };

        /// <summary>
        ///  Return the value of the color.
        /// </summary>
        inline std::uint8_t value() const { return value_; };

        /// <summary>
        ///  Check if this is a valid color.
        /// </summary>
        inline bool IsColor() const { return value_ <= 1; };

        /// <summary>
        ///  Return the "next" color. In combination with First, this can be used to
        ///  iterate over all colors.
        /// </summary>
        inline Color Next() const { return Color(value_ + 1); };

        friend bool operator==(Color, Color);
    };

    inline bool operator==(Color lhs, Color rhs) { return lhs.value_ == rhs.value_; }
    inline bool operator!=(Color lhs, Color rhs) { return !(lhs == rhs); }
}

#endif // M8_COLOR_HPP_