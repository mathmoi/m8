/// @file   Mat.hpp
/// @author Mathieu Pagé
/// @date   Janurary 2018
/// @brief  Contains a method used to check if a postion is a mat.

#ifndef M8_MAT_HPP_
#define M8_MAT_HPP_

#include "MoveGen.hpp"

namespace m8
{
    /// This method check if the side to move is mat. For performance reasons no 
    /// verification is made to make sure that the king is currently attacked (in check)
    /// this must be done separately.
    bool IsMat(Board& board)
    {
        Color side_to_move = board.side_to_move();

        MoveGen generator(board);
        MoveList moves;
        Move* end = moves.data();
        end = generator.GenerateAllMoves(side_to_move, end);

        // We look for a move that doesn't leave the side to move in check.
        bool found = false;
        Move* next = moves.data();
        while (!found && next < end)
        {
            UnmakeInfo unmake_info = board.Make(*next);
            found = !generator.IsInCheck(side_to_move);
            board.Unmake(*next, unmake_info);

            ++next;
        }

        return !found;
    }
}

#endif // M8_MAT_HPP_