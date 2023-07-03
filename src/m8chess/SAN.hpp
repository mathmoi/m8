/// @file   SAN.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2017
/// @brief  Contains methods to parse or generate moves in Standard Algebraic Notation.

#ifndef M8_SAN_HPP_
#define M8_SAN_HPP_

#include "Move.hpp"
#include "Board.hpp"
#include "InvalidMoveNotationException.hpp"

namespace m8
{
    /// Parse a SAN string into a move.
    ///
    /// @param input  SAN move to parse.
    /// @param board  Board object that provide the current position needed to parse the
    ///               SAN move.
    Move ParseSAN(std::string input, const Board& board);

    /// Render a move in SAN notation.
    ///
    /// @param move   Move to render. 
    /// @param board  Board object that provide the current position needed to render the
    ///               move in SAN notation.
    std::string RenderSAN(Move move, const Board& board);
}

#endif // M8_SAN_HPP_