/// @file   Eval.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the evaluation function

#ifndef M8_EVAL_HPP_
#define M8_EVAL_HPP_

#include <cstdint>
#include <limits>

#include "../Board.hpp"
#include "../Types.hpp"

namespace m8::eval
{   
    const EvalType kMinEval  = std::numeric_limits<EvalType>::min() + 100;
    const EvalType kMaxEval  = std::numeric_limits<EvalType>::max() - 100;

    const EvalType kEvalDraw = 0;
    const EvalType kEvalMat  = 30000;
    const EvalType kMaxMat   = 1000;

    extern std::array<EvalType, kMaxPieceType + 1> kPieceTypeValues;

    /// Initialize evaluation globales variables.
    void InitializeEval();

    /// Evaluate a position.
    inline EvalType Evaluate(const Board& board)
    {
        return (1 - 2 * static_cast<int>(board.side_to_move())) * board.material_value();
    }

    /// Remve the specified distance to the evaluation if the evaluation represents a mate
    /// in N. This can be used before storing evaluations in the transposition table.
    inline EvalType RemoveDistanceFromMate(EvalType eval, DepthType distance)
    {
        if (eval > kEvalMat - kMaxMat)
        {
            return eval + distance;
        }

        if (eval < -kEvalMat + kMaxMat)
        {
            return eval - distance;
        }
        
        return eval;
    }

    /// Add the specified distance to the evaluation if the evaluation represents a mate
    /// in N. This can be used before retrieving evaluations from the transposition table.
    inline EvalType AddDistanceToMate(EvalType eval, DepthType distance)
    {
        if (eval > kEvalMat - kMaxMat)
        {
            return eval - distance;
        }

        if (eval < -kEvalMat + kMaxMat)
        {
            return eval + distance;
        }
        
        return eval;
    }
    
    /// Return the value of a mate position given the distance to the root of the search.
    inline EvalType GetMateValue(DepthType distance)
    {
        return -kEvalMat + distance;
    }

    inline double GetEvaluationInPawns(EvalType value)
    {
        return value / 100.0;
    }
}

#endif // M8_EVAL_HPP_