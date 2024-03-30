/// @file PhasePieceSquareTable.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// Contains the definition of a struct containing the piece-square table
/// informations for multiple game phases.

#ifndef M8_OPTIONS_PHASE_PIECE_SQUARE_TABLE_HPP_
#define M8_OPTIONS_PHASE_PIECE_SQUARE_TABLE_HPP_

#include <array>
#include <cstdint>

#include "PieceSquareTable.hpp"

namespace m8::options
{
    /// Contains the square tables for differents phases of the game
    struct PhasePieceSquareTable
    {
        /// The values for each squares in the middle game
        PieceSquareTable middle_game;

        /// The values for each squares in the end game
        PieceSquareTable end_game;
    };
}

#endif // M8_OPTIONS_PHASE_PIECE_SQUARE_TABLE_HPP_