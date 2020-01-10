/// @file   Eval.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the evaluation function

#ifndef M8_EVAL_HPP_
#define M8_EVAL_HPP_

#include <cstdint>
#include <limits>

#include "../Board.hpp"

namespace m8::eval
{   
	typedef std::int32_t EvalType;

	const EvalType kMinEval    = std::numeric_limits<EvalType>::min();
	const EvalType kMaxEval    = std::numeric_limits<EvalType>::max();

	const EvalType kEvalNull   = 0;
	const EvalType kEvalMat    = 1000000;
	const EvalType kMaxMat     = 10000;

    /// Evaluate a position.
    inline EvalType Evaluate(const Board& board)
    {
        return (1 - 2 * static_cast<int>(board.side_to_move())) * board.material_value();
    }

	/// Add a 1 depth to the score if the value is a mate score.
	inline EvalType AddDepthToMate(EvalType value)
	{
		if (value > kEvalMat - kMaxMat)
		{
			return value - 1;
		}
		else if (value < -kEvalMat + kMaxMat)
		{
			return value + 1;
		}
		else
		{
			return value;
		}
	}

	inline double GetEvaluationInPawns(EvalType value)
	{
		return value / 100.0;
	}
}

#endif // M8_EVAL_HPP_