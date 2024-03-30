/// @file PiecesValues.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the definition of a struct containing the values of each 
///        pieces.

#ifndef M8_OPTIONS_PIECES_VALUES_HPP_
#define M8_OPTIONS_PIECES_VALUES_HPP_

#include <cstdint>

namespace m8::options
{
    /// Structure containing the values of each pieces.
    struct PiecesValues
    {
        /// The value of a pawn.
        std::int16_t pawn;

        /// The value of a knight.
        std::int16_t knight;

        /// The value of a bishop.
        std::int16_t bishop;

        /// The value of a rook.
        std::int16_t rook;

        /// The value of a queen.
        std::int16_t queen;

        /// The value of a king.
        std::int16_t king;
    };
}

#endif // M8_OPTIONS_PIECES_VALUES_HPP_