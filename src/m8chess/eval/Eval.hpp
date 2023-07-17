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
	const EvalType kMinEval = std::numeric_limits<EvalType>::min() + 100;
	const EvalType kMaxEval    = std::numeric_limits<EvalType>::max() - 100;

	const EvalType kEvalNull   = 0;
	const EvalType kEvalMat    = 100000;
	const EvalType kMaxMat     = 1000;

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

		if (value < -kEvalMat + kMaxMat)
		{
			return value + 1;
		}
		
		return value;
	}

	inline double GetEvaluationInPawns(EvalType value)
	{
		return value / 100.0;
	}
}

#endif // M8_EVAL_HPP_