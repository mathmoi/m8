/// @file CoordinateNotation.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// 
/// @date December 2022

#ifndef M8_CHESS_COORDINATE_NOTATION_HPP_
#define M8_CHESS_COORDINATE_NOTATION_HPP_

#include "Move.hpp"
#include "Board.hpp"
#include "InvalidMoveNotationException.hpp"

namespace m8
{
    /// Parse a coordinate algebraic notation string into a move.
    ///
    /// @param input  String move to parse.
    /// @param board  Board object that provide the current position needed to parse the
    ///               move.
    Move ParseCoordinateNotation(std::string input, const Board& board);

    /// Remder a move into a string usind the coordinate algebraic notation.
    ///
    /// @param move The move to render as a string
    /// @param chess960 Indicate if the castling moves need to be rendered as chess960
    ///                 moves.
    /// @return A std::string representing the move
    std::string RenderCoordinateNotation(Move move, const Board& board, bool chess960);
}

#endif // M8_CHESS_COORDINATE_NOTATION_HPP_