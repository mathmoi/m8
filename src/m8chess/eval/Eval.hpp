/// @file   Eval.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the evaluation function

#ifndef M8_EVAL_HPP_
#define M8_EVAL_HPP_

#include "../Board.hpp"

namespace m8 { namespace eval
{
    /// Evaluate a position.
    inline int Evaluate(const Board& board)
    {
        return (1 - 2 * static_cast<int>(board.side_to_move())) * board.material_value();
    }
}}

#endif // eval                                         