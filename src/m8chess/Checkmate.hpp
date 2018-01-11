/// @file   Checkmate.hpp
/// @author Mathieu Pagé
/// @date   Janurary 2018
/// @brief  Contains a method used to verify that a player is in check or mate.

#ifndef M8_CHECKMATE_HPP_
#define M8_CHECKMATE_HPP_

#include "MoveGen.hpp"

namespace m8
{
    /// Verify if a given color is in check.
    ///
    /// @param color Color of the side to for which to verify if the king is in check.
    inline bool IsInCheck(Color color, const Board& board, const MoveGen& generator)
    {
        Piece king = NewPiece(kKing, color);
        Bb bb_king = board.bb_piece(king);
        Sq king_position = GetLsb(bb_king);
        Bb attackers = generator.AttacksTo(king_position);
        Bb opponent_pieces = board.bb_color(OpposColor(color));
        return (attackers & opponent_pieces) != kEmptyBb;
    }

    /// This method check if the side to move is mat. For performance reasons no 
    /// verification is made to make sure that the king is currently attacked (in check)
    /// this must be done separately.
    inline bool IsMat(Board& board)
    {
        Color side_to_move = board.side_to_move();

        MoveGen generator(board);
        MoveList moves;
        Move* end = moves.data();
        end = generator.GenerateAllMoves(end);

        // We look for a move that doesn't leave the side to move in check.
        bool found = false;
        Move* next = moves.data();
        while (!found && next < end)
        {
            UnmakeInfo unmake_info = board.Make(*next);
            found = !IsInCheck(side_to_move, board, generator);
            board.Unmake(*next, unmake_info);

            ++next;
        }

        return !found;
    }
}

#endif // M8_CHECKMATE_HPP_