/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#include <chrono>

#include "../eval/Eval.hpp"
#include "../MoveGen.hpp"
#include "../Checkmate.hpp"

#include "AlphaBeta.hpp"

namespace m8::search {

	AlphaBeta::AlphaBeta(const Board& board)
		: board_(board),
		  continue_(true),
		  best_move_(kNullMove)
	{}

	template<bool root, bool qsearch>
	eval::EvalType AlphaBeta::Search(eval::EvalType alpha, eval::EvalType beta, std::int_fast16_t depth)
	{
		if (!root && depth <= 0)
		{
			return eval::Evaluate(board_);
		}

		MoveList moves;
		Move* last = GenerateAllMoves(board_, moves.data());

		for (Move* next = moves.data(); next < last && continue_; ++next)
		{
			eval::EvalType value;

			UnmakeInfo unmake_info = board_.Make(*next);

			if (IsInvalidCheckPosition(board_))
			{
				board_.Unmake(*next, unmake_info);
			}
			else
			{
				value = -Search<false, false>(-beta, -alpha, depth - 1);
				value = eval::AddDepthToMate(value);
				board_.Unmake(*next, unmake_info);

				if (value >= beta)
				{
					return beta;
				}

				if (value > alpha)
				{
					alpha = value;
					if (root)
					{
						best_move_ = *next;
					}
				}
			}
		}

		return alpha;
	}

	SearchResult AlphaBeta::Search(std::uint32_t depth)
	{
		auto value = Search<true, false>(-eval::kMinEval, eval::kMaxEval, depth);
		return SearchResult(value, best_move_);
	}

	void AlphaBeta::Stop()
	{
		continue_ = false;
	}

}