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

	AlphaBeta::AlphaBeta(const Board& board,
		                 SearchObserver* observer)
		: board_(board),
		  continue_(true),
		  best_move_(kNullMove),
		  nodes_counter_(0),
		  observer_(observer)
	{}

	template<bool root, bool qsearch>
	EvalType AlphaBeta::Search(EvalType alpha, EvalType beta, DepthType depth, PV& pv)
	{
		PV local_pv;

		++nodes_counter_;

		if (!root && depth <= 0)
		{
			return eval::Evaluate(board_);
		}

		MoveList moves;
		Move* last = GenerateAllMoves(board_, moves.data());
		Move* first = moves.data();

		for (Move* next = first; next < last && continue_; ++next)
		{
			EvalType value;

			UnmakeInfo unmake_info = board_.Make(*next);

			if (IsInvalidCheckPosition(board_))
			{
				board_.Unmake(*next, unmake_info);
			}
			else
			{
				value = -Search<false, false>(-beta, -alpha, depth - 1, local_pv);
				value = eval::AddDepthToMate(value);
				board_.Unmake(*next, unmake_info);

				if (value >= beta)
				{
					return beta;
				}

				if (value > alpha)
				{
					alpha = value;
					pv.Replace(*next, local_pv);

					// If it is a new best move we notify the user.
					if (root && *next != best_move_)
					{
						best_move_ = *next;
						observer_->OnNewBestMove(pv, alpha, depth, 0, nodes_counter_);
					}
				}
			}
		}

		return alpha;
	}

	SearchResult AlphaBeta::Search(DepthType depth)
	{
		PV pv;

		observer_->OnBeginSearch();

		auto value = Search<true, false>(eval::kMinEval, eval::kMaxEval, depth, pv);
		auto result = SearchResult(pv, value, nodes_counter_);

		observer_->OnSearchCompleted(pv, 0);

		return result;
	}

	void AlphaBeta::Stop()
	{
		continue_ = false;
	}

}