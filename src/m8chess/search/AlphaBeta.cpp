/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#include <chrono>

#include "../eval/Eval.hpp"

#include "../MoveGen.hpp"
#include "../Checkmate.hpp"

#include "Search.hpp"
#include "AlphaBeta.hpp"

namespace m8::search {

	AlphaBeta::AlphaBeta(std::shared_ptr<Search> search)
		: board_(search->board()),
		  continue_(true),
		  nodes_count_next_time_check_(kNodesBeforeFirstCheck),
		  search_(search)
	{}

	template<bool root, bool qsearch>
	EvalType AlphaBeta::AlphaBetaSearch(EvalType alpha, EvalType beta, DepthType depth, PV& pv)
	{
		PV local_pv;

		pv.Clear();

		qsearch ? stats_.qnodes++ : stats_.nodes++;

		// We check if we need to abort the search because of time constraint
		if (!qsearch && nodes_count_next_time_check_ <= stats_.nodes)
		{
			continue_ = !search_->is_aborted() && search_->time_manager().can_continue();
			if (!continue_)
			{
				return 0;
			}
			nodes_count_next_time_check_ = stats_.nodes + search_->time_manager().CalculateNodesBeforeNextCheck(stats_.nodes);
		}

		// If we are in the qsearch we must evaluate the stand path option.
		if (qsearch)
		{
			EvalType stand_path = eval::Evaluate(board_);
			if (stand_path >= beta)
			{
				return beta;
			}
			if (alpha < stand_path)
			{
				alpha = stand_path;
			}
		}

		// Generate all the moves
		MoveList moves; // TODO : MoveList should be a class. Test for performance.
		Move* first = moves.data();
		Move* last = qsearch ? GenerateAllCaptures(board_, moves.data())
			                 : GenerateAllMoves(board_, moves.data());
				
		// Evaluate all moves
		for (Move* next = first; next < last; ++next)
		{
			EvalType value;

			UnmakeInfo unmake_info = board_.Make(*next);

			// If the position is invalid because the side to move is in check we must 
			// undo the current move and continue with the next one.
			if (IsInvalidCheckPosition(board_))
			{
				board_.Unmake(*next, unmake_info);
			}
			else
			{
				// If we are at depth > 1 we need to make a recursive call to the search
				// function. If we are at depth 1 or in the qsearch we need to make a
				// recursive call to the qsearch function.
				if (!qsearch && depth > 1)
				{
					// Recursive call to the search function
					value = -AlphaBetaSearch<false, false>(-beta, -alpha, depth - 1, local_pv);
				}
				else
				{
					// Call to the qsearch
					value = -AlphaBetaSearch<false, true>(-beta, -alpha, 0, local_pv);
				}
				value = eval::AddDepthToMate(value);
				board_.Unmake(*next, unmake_info);

				// If we are aborting the search we need to leave immediately.
				if (!continue_)
				{
					return 0;
				}

				// If the value of the current move is better or equal to beta we can 
				// abort the search at this node.
				if (value >= beta)
				{
					return beta;
				}

				// If value is better than alpha we possibly have a new best move at this
				// node.
				if (value > alpha)
				{
					alpha = value;
					if (!qsearch)
					{
						pv.Replace(*next, local_pv);
					}

					// If it is a new best move we notify the user.
					if (root)
					{
						NotifyNewBestMove(pv, alpha, depth, 0, stats_.nodes);
					}
				}
			}
		}

		return alpha;
	}

	std::optional<SearchResult> AlphaBeta::Start(DepthType depth)
	{
		PV pv;

		NotifySearchStarted();

		auto value = AlphaBetaSearch<true, false>(eval::kMinEval, eval::kMaxEval, depth, pv);
		
		// If we are aborting the search we need to leave immediately.
		if (!continue_)
		{
			// TODO : If we make sure that we always search the previous best move first, we could still use the result of a partial search.
			return std::nullopt;
		}

		auto result = SearchResult(pv, value, stats_);

		NotifySearchCompleted(pv, 0, stats_);

		return result;
	}

}